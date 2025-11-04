#pragma once
#include <swarm_internal.h>

#include <vulkan/vulkan.h>
namespace swarm
{
    struct Fence_T
    {
        VkFence fence;
    };

    struct Semaphore_T
    {
        VkSemaphore semaphore;
    };
}