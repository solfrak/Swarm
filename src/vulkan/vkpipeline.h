#pragma once
#include <swarm_internal.h>
#include <vulkan/vulkan.h>
namespace swarm
{
    struct Pipeline_T
    {
        VkPipeline pipeline{VK_NULL_HANDLE};
        VkPipelineLayout pipelineLayout{VK_NULL_HANDLE};
        VkDescriptorSetLayout descriptorSetLayout{VK_NULL_HANDLE};
    };
}
