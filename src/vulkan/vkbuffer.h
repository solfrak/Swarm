#pragma once
#include <swarm_internal.h>

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

namespace swarm
{
    struct Buffer_T
    {
        VkBuffer buffer;
        VmaAllocation allocation;
        void* mappedData{nullptr};

        VkDeviceSize size;
        VkBufferUsageFlags usage;
    };
}
