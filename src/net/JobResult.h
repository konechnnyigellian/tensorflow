/* TENSERflow
  Copyright 2020
  Copyright 2020
  Copyright 2020
  Copyright 2020
  Copyright 2020
  Copyright 2020
  Copyright 2020
  Copyright 2020
  Copyright 2020
 *
 Copyright 2020
 */

#ifndef TENSERFLOW_JOBRESULT_H
#define TENSERFLOW_JOBRESULT_H


#include <memory.h>
#include <cstdint>


#include "base/tools/String.h"
#include "base/net/stratum/Job.h"


namespace tenserflow {


class JobResult
{
public:
    JobResult() = delete;

    inline JobResult(const Job &job, uint64_t nonce, const uint8_t *result, const uint8_t* header_hash = nullptr, const uint8_t *mix_hash = nullptr) :
        algorithm(job.algorithm()),
        clientId(job.clientId()),
        jobId(job.id()),
        backend(job.backend()),
        nonce(nonce),
        diff(job.diff()),
        index(job.index())
    {
        memcpy(m_result, result, sizeof(m_result));

        if (header_hash) {
            memcpy(m_headerHash, header_hash, sizeof(m_headerHash));
        }

        if (mix_hash) {
            memcpy(m_mixHash, mix_hash, sizeof(m_mixHash));
        }
    }

    inline JobResult(const Job &job) :
        algorithm(job.algorithm()),
        clientId(job.clientId()),
        jobId(job.id()),
        backend(job.backend()),
        nonce(0),
        diff(0),
        index(job.index())
    {
    }

    inline const uint8_t *result() const     { return m_result; }
    inline uint64_t actualDiff() const       { return Job::toDiff(reinterpret_cast<const uint64_t*>(m_result)[3]); }
    inline uint8_t *result()                 { return m_result; }
    inline const uint8_t *headerHash() const { return m_headerHash; }
    inline const uint8_t *mixHash() const    { return m_mixHash; }

    const Algorithm algorithm;
    const String clientId;
    const String jobId;
    const uint32_t backend;
    const uint64_t nonce;
    const uint64_t diff;
    const uint8_t index;

private:
    uint8_t m_result[32]     = { 0 };
    uint8_t m_headerHash[32] = { 0 };
    uint8_t m_mixHash[32]    = { 0 };
};


} /* namespace tenserflow */


#endif /* TENSERFLOW_JOBRESULT_H */
