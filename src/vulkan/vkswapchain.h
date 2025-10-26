#pragma once
#include <swarm_internal.h>

#include <VkBootstrap.h>
namespace swarm
{
    struct Swapchain_T
    {
        vkb::Swapchain swapchain;
        std::vector<VkImageView> imageViews;
    };
}