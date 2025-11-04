#pragma once
#include <swarm_internal.h>

#include <VkBootstrap.h>
#include <vk_mem_alloc.h>
namespace swarm
{
    struct Device_T
    {
        vkb::Device device;
        VmaAllocator allocator;
    };
}
