#pragma once

#include <swarm_internal.h>
#include <vulkan/vulkan.h>
namespace swarm
{
    struct Framebuffer_T
    {
        std::vector<VkFramebuffer> framebuffers;
    };
}