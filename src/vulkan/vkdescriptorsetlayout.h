#pragma once

#include <swarm_internal.h>

#include <vulkan/vulkan.h>
#include <vector>
namespace swarm
{
    struct DescriptorSetLayoutBuilder_T
    {
        std::vector<VkDescriptorSetLayoutBinding> bindings;
    };

    struct DescriptorSetlayout_T
    {
        VkDescriptorSetLayout setLayout;
    };
}
