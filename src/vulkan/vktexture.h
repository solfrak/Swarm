#pragma once
#include "swarm_internal.h"
#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>
namespace swarm
{
    struct Texture_T
    {
        VkImage image;
        VkImageView imageView;
        VmaAllocation imageAllocation;
    };
}