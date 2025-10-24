#pragma once
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
}