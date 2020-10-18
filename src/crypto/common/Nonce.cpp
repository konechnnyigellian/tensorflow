


#include "crypto/common/Nonce.h"


namespace tenserflow {


std::atomic<bool> Nonce::m_paused;
std::atomic<uint64_t> Nonce::m_sequence[Nonce::MAX];
std::atomic<uint64_t> Nonce::m_nonces[2] = { {0}, {0} };


static Nonce nonce;


} // namespace tenserflow


tenserflow::Nonce::Nonce()
{
    m_paused = true;

    for (auto &i : m_sequence) {
        i = 1;
    }
}


bool tenserflow::Nonce::next(uint8_t index, uint32_t *nonce, uint32_t reserveCount, uint64_t mask)
{
    mask &= 0x7FFFFFFFFFFFFFFFULL;
    if (reserveCount == 0 || mask < reserveCount - 1) {
        return false;
    }

    uint64_t counter = m_nonces[index].fetch_add(reserveCount, std::memory_order_relaxed);
    while (true) {
        if (mask < counter) {
            return false;
        }
        else if (mask - counter <= reserveCount - 1) {
            pause(true);
            if (mask - counter < reserveCount - 1) {
                return false;
            }
        }
        else if (0xFFFFFFFFUL - (uint32_t)counter < reserveCount - 1) {
            counter = m_nonces[index].fetch_add(reserveCount, std::memory_order_relaxed);
            continue;
        }
        *nonce = (nonce[0] & ~mask) | counter;
        if (mask > 0xFFFFFFFFULL) {
            nonce[1] = (nonce[1] & (~mask >> 32)) | (counter >> 32);
        }
        return true;
    }
}


void tenserflow::Nonce::stop()
{
    pause(false);

    for (auto &i : m_sequence) {
        i = 0;
    }
}


void tenserflow::Nonce::touch()
{
    for (auto &i : m_sequence) {
        i++;
    }
}
