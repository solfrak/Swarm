#include <swarm_internal.h>

#include <cstdlib>

namespace swarm
{
    SwarmLibrary g_SwarmLibrary;

    bool InitSwarm(SwarmAllocFn allocFn, SwarmFreeFn freeFn)
    {
        if (g_SwarmLibrary.isInitialized)
            return true;


        g_SwarmLibrary.allocFn = allocFn;
        g_SwarmLibrary.freeFn = freeFn;

        if (!g_SwarmLibrary.allocFn || !g_SwarmLibrary.freeFn)
        {
            g_SwarmLibrary.allocFn = [](unsigned int size) -> void*
            {
                return std::malloc(size);
            };

            g_SwarmLibrary.freeFn = [](void* ptr) -> void
            {
                std::free(ptr);
            };
        }

        g_SwarmLibrary.isInitialized = true;
        return true;
    }

    void ShutdownSwarm()
    {
        g_SwarmLibrary.allocFn = nullptr;
        g_SwarmLibrary.freeFn = nullptr;
        g_SwarmLibrary.isInitialized = false;
    }


}
