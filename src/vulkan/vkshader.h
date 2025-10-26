#pragma once
#include <swarm_internal.h>
#include <vulkan/vulkan.h>
namespace swarm
{
    struct Shader_T
    {
        VkShaderModule module;
        ShaderStage stage;
    };
}