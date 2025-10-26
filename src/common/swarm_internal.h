#pragma once
#include <utility>
#include <swarm/swarm.h>

namespace swarm
{
    struct SwarmLibrary
    {
        SwarmAllocFn allocFn{nullptr};
        SwarmFreeFn freeFn{nullptr};

        bool isInitialized{false};
    };

    extern SwarmLibrary g_SwarmLibrary;

    template<typename T, typename... Args>
    T *SWARM_NEW(Args &&... args)
    {
        T *ptr = static_cast<T *>(g_SwarmLibrary.allocFn(sizeof(T)));
        new(ptr) T(std::forward<Args>(args)...);
        return ptr;
    }

    template<typename T>
    void SWARM_DELETE(T *ptr)
    {
        ptr->~T(); // Explicitly call destructor
        g_SwarmLibrary.freeFn(ptr);
    }
}
