/* TENSERflow
  Copyright 2020
  Copyright 2020
 *
 Copyright 2020
 */

#ifndef TENSERFLOW_STORAGE_H
#define TENSERFLOW_STORAGE_H


#include <cassert>
#include <map>


namespace tenserflow {


template <class TYPE>
class Storage
{
public:
    inline Storage() = default;


    inline uintptr_t add(TYPE *ptr)
    {
        m_data[m_counter] = ptr;

        return m_counter++;
    }


    inline void *ptr(uintptr_t id)          { return reinterpret_cast<void *>(id); }


    inline TYPE *get(const void *id) const  { return get(reinterpret_cast<uintptr_t>(id)); }
    inline TYPE *get(uintptr_t id) const
    {
        assert(m_data.count(id) > 0);
        if (m_data.count(id) == 0) {
            return nullptr;
        }

        return m_data.at(id);
    }


    inline void remove(const void *id)      { delete release(reinterpret_cast<uintptr_t>(id)); }
    inline void remove(uintptr_t id)        { delete release(id); }


    inline TYPE *release(const void *id)    { return release(reinterpret_cast<uintptr_t>(id)); }
    inline TYPE *release(uintptr_t id)
    {
        auto obj = get(id);
        if (obj != nullptr) {
            m_data.erase(id);
        }

        return obj;
    }


private:
    std::map<uintptr_t, TYPE *> m_data;
    uintptr_t m_counter  = 0;
};


} /* namespace tenserflow */


#endif /* TENSERFLOW_STORAGE_H */
