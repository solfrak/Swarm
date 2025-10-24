#pragma once


namespace swarm
{
#define SWARM_HANDLE(object) \
struct object##_T; \
using object##Handle = object##_T*;

    SWARM_HANDLE(Instance);
    SWARM_HANDLE(PhysicalDevice);
    SWARM_HANDLE(Device);

    using SwarmAllocFn = void* (*)(unsigned int size);
    using SwarmFreeFn = void (*)(void* ptr);

    bool InitSwarm(SwarmAllocFn allocFn = nullptr, SwarmFreeFn freeFn = nullptr);
    void ShutdownSwarm();


    struct InstanceCreateInfo
    {
        const char* applicationName{nullptr};
        unsigned int applicationVersion{0};
        bool isDebug{false};
    };
    InstanceHandle CreateInstance(InstanceCreateInfo& instanceCreateInfo);
    void DestroyInstance(InstanceHandle handle);


}