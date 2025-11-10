#pragma once

#include <vector>
#include <vulkan/vulkan.h>
namespace swarm
{
    struct Renderpass_T
    {
        VkRenderPass renderPass{VK_NULL_HANDLE};
        std::vector<VkClearValue> clearValues;
    };
}