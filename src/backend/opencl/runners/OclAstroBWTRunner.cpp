


#include "backend/opencl/runners/OclAstroBWTRunner.h"
#include "backend/opencl/kernels/astrobwt/AstroBWT_FilterKernel.h"
#include "backend/opencl/kernels/astrobwt/AstroBWT_FindSharesKernel.h"
#include "backend/opencl/kernels/astrobwt/AstroBWT_MainKernel.h"
#include "backend/opencl/kernels/astrobwt/AstroBWT_PrepareBatch2Kernel.h"
#include "backend/opencl/kernels/astrobwt/AstroBWT_Salsa20Kernel.h"
#include "backend/opencl/kernels/astrobwt/AstroBWT_SHA3InitialKernel.h"
#include "backend/opencl/kernels/astrobwt/AstroBWT_SHA3Kernel.h"
#include "backend/opencl/OclLaunchData.h"
#include "backend/opencl/wrappers/OclLib.h"
#include "base/io/log/Log.h"
#include "base/net/stratum/Job.h"


namespace tenserflow {


constexpr int STAGE1_SIZE = 147253;
constexpr uint32_t STAGE1_DATA_STRIDE = (STAGE1_SIZE + 256 + 255) & ~255U;
constexpr uint32_t OclAstroBWTRunner::BWT_DATA_STRIDE;


} // namespace tenserflow


tenserflow::OclAstroBWTRunner::OclAstroBWTRunner(size_t index, const OclLaunchData &data) : OclBaseRunner(index, data)
{
    switch (data.device.type())
    {
    case OclDevice::Baffin:
    case OclDevice::Polaris:
    case OclDevice::Lexa:
    case OclDevice::Vega_10:
    case OclDevice::Vega_20:
    case OclDevice::Raven:
        m_workgroup_size = 64;
        break;

    default:
        m_workgroup_size = 32;
        break;
    }

    m_options += " -DSALSA20_GROUP_SIZE=" + std::to_string(m_workgroup_size);
    m_options += " -DBWT_GROUP_SIZE="     + std::to_string(m_workgroup_size);

    m_bwt_allocation_size = static_cast<uint64_t>(m_intensity) * BWT_DATA_STRIDE;
    m_batch_size1 = static_cast<uint32_t>(m_bwt_allocation_size / STAGE1_DATA_STRIDE + 255U) & ~255U;

    m_bwt_data_sizes_host = new uint32_t[m_batch_size1];
}


tenserflow::OclAstroBWTRunner::~OclAstroBWTRunner()
{
    delete m_sha3_initial_kernel;
    delete m_sha3_kernel;
    delete m_salsa20_kernel;
    delete m_bwt_kernel;
    delete m_filter_kernel;
    delete m_prepare_batch2_kernel;
    delete m_find_shares_kernel;

    OclLib::release(m_salsa20_keys);
    OclLib::release(m_bwt_data);
    OclLib::release(m_bwt_data_sizes);
    OclLib::release(m_indices);
    OclLib::release(m_tmp_indices);
    OclLib::release(m_filtered_hashes);

    delete m_bwt_data_sizes_host;
}


size_t tenserflow::OclAstroBWTRunner::bufferSize() const
{
    return OclBaseRunner::bufferSize() +
        align(m_batch_size1 * 32) +                    // m_salsa20_keys
        align(m_bwt_allocation_size) +                 // m_bwt_data
        align(m_batch_size1 * 4) +                     // m_bwt_data_sizes
        align(m_bwt_allocation_size * 8) +             // m_indices
        align(m_bwt_allocation_size * 8) +             // m_tmp_indices
        align((m_batch_size1 + m_intensity) * 36 + 4); // m_filtered_hashes
}


void tenserflow::OclAstroBWTRunner::run(uint32_t nonce, uint32_t *hashOutput)
{
    m_sha3_initial_kernel->setArg(2, sizeof(nonce), &nonce);
    m_salsa20_kernel->setArg(3, sizeof(STAGE1_DATA_STRIDE), &STAGE1_DATA_STRIDE);
    m_bwt_kernel->setArg(2, sizeof(STAGE1_DATA_STRIDE), &STAGE1_DATA_STRIDE);

    const uint32_t t = STAGE1_DATA_STRIDE * 8;
    m_sha3_kernel->setArg(2, sizeof(t), &t);
    m_filter_kernel->setArg(0, sizeof(nonce), &nonce);

    const uint32_t zero = 0;
    enqueueWriteBuffer(m_output, CL_FALSE, sizeof(cl_uint) * 0xFF, sizeof(uint32_t), &zero);

    m_sha3_initial_kernel->enqueue(m_queue, m_batch_size1);

    for (uint32_t i = 0; i < m_batch_size1; ++i)
        m_bwt_data_sizes_host[i] = STAGE1_SIZE;

    enqueueWriteBuffer(m_bwt_data_sizes, CL_FALSE, 0, m_batch_size1 * sizeof(uint32_t), m_bwt_data_sizes_host);

    m_salsa20_kernel->enqueue(m_queue, m_batch_size1, m_workgroup_size);
    m_bwt_kernel->enqueue(m_queue, m_batch_size1, m_workgroup_size);
    m_sha3_kernel->enqueue(m_queue, m_batch_size1);
    m_filter_kernel->enqueue(m_queue, m_batch_size1, m_workgroup_size);

    uint32_t num_filtered_hashes = 0;
    enqueueReadBuffer(m_filtered_hashes, CL_TRUE, 0, sizeof(num_filtered_hashes), &num_filtered_hashes);

    m_processedHashes = 0;
    while (num_filtered_hashes >= m_intensity)
    {
        num_filtered_hashes -= m_intensity;
        m_processedHashes += m_intensity;

        m_salsa20_kernel->setArg(3, sizeof(BWT_DATA_STRIDE), &BWT_DATA_STRIDE);
        m_bwt_kernel->setArg(2, sizeof(BWT_DATA_STRIDE), &BWT_DATA_STRIDE);

        const uint32_t t = BWT_DATA_STRIDE * 8;
        m_sha3_kernel->setArg(2, sizeof(t), &t);

        m_prepare_batch2_kernel->enqueue(m_queue, m_intensity, m_workgroup_size);
        m_salsa20_kernel->enqueue(m_queue, m_intensity, m_workgroup_size);
        m_bwt_kernel->enqueue(m_queue, m_intensity, m_workgroup_size);
        m_sha3_kernel->enqueue(m_queue, m_intensity);

        m_find_shares_kernel->enqueue(m_queue, m_intensity, m_workgroup_size);

        finalize(hashOutput);

        OclLib::finish(m_queue);
    }
}


void tenserflow::OclAstroBWTRunner::set(const Job &job, uint8_t *blob)
{
    if (job.size() > (Job::kMaxBlobSize - 4)) {
        throw std::length_error("job size too big");
    }

    if (job.size() < Job::kMaxBlobSize) {
        memset(blob + job.size(), 0, Job::kMaxBlobSize - job.size());
    }

    enqueueWriteBuffer(m_input, CL_TRUE, 0, Job::kMaxBlobSize, blob);

    m_sha3_initial_kernel->setArgs(m_input, static_cast<uint32_t>(job.size()), *job.nonce(), m_salsa20_keys);
    m_salsa20_kernel->setArgs(m_salsa20_keys, m_bwt_data, m_bwt_data_sizes, STAGE1_DATA_STRIDE);
    m_bwt_kernel->setArgs(m_bwt_data, m_bwt_data_sizes, STAGE1_DATA_STRIDE, m_indices, m_tmp_indices);
    m_sha3_kernel->setArgs(m_tmp_indices, m_bwt_data_sizes, STAGE1_DATA_STRIDE * 8, m_salsa20_keys);
    m_filter_kernel->setArgs(*job.nonce(), BWT_DATA_MAX_SIZE, m_salsa20_keys, m_filtered_hashes);
    m_prepare_batch2_kernel->setArgs(m_salsa20_keys, m_filtered_hashes, m_bwt_data_sizes);
    m_find_shares_kernel->setArgs(m_salsa20_keys, m_filtered_hashes, m_output);
    m_find_shares_kernel->setTarget(job.target());

    const uint32_t zero = 0;
    enqueueWriteBuffer(m_filtered_hashes, CL_TRUE, 0, sizeof(uint32_t), &zero);
}


void tenserflow::OclAstroBWTRunner::build()
{
    OclBaseRunner::build();

    m_sha3_initial_kernel   = new AstroBWT_SHA3InitialKernel(m_program);
    m_sha3_kernel           = new AstroBWT_SHA3Kernel(m_program);
    m_salsa20_kernel        = new AstroBWT_Salsa20Kernel(m_program);
    m_bwt_kernel            = new AstroBWT_MainKernel(m_program);
    m_filter_kernel         = new AstroBWT_FilterKernel(m_program);
    m_prepare_batch2_kernel = new AstroBWT_PrepareBatch2Kernel(m_program);
    m_find_shares_kernel    = new AstroBWT_FindSharesKernel(m_program);
}


void tenserflow::OclAstroBWTRunner::init()
{
    OclBaseRunner::init();

    const cl_mem_flags f = CL_MEM_READ_WRITE | CL_MEM_HOST_NO_ACCESS;

    m_salsa20_keys    = createSubBuffer(f, m_batch_size1 * 32);
    m_bwt_data        = createSubBuffer(f, m_bwt_allocation_size);
    m_bwt_data_sizes  = createSubBuffer(CL_MEM_READ_WRITE | CL_MEM_HOST_WRITE_ONLY, m_batch_size1 * 4);
    m_indices         = createSubBuffer(f, m_bwt_allocation_size * 8);
    m_tmp_indices     = createSubBuffer(f, m_bwt_allocation_size * 8);
    m_filtered_hashes = createSubBuffer(CL_MEM_READ_WRITE, (m_batch_size1 + m_intensity) * 36 + 4);
}
