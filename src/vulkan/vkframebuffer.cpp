#include "vkframebuffer.h"
#include "vkrenderpass.h"
#include "vkswapchain.h"
#include "vkdevice.h"
#include "vktexture.h"

#include <cassert>

namespace swarm
{
    FramebufferHandle CreateFramebuffer(DeviceHandle device, SwapchainHandle swapchain, RenderpassHandle renderpass, TextureHandle depthTexture)
    {
        assert(g_SwarmLibrary.isInitialized);
        assert(device);
        assert(swapchain);
        assert(renderpass);
        std::vector<VkFramebuffer> framebuffers(swapchain->imageViews.size());

        for (unsigned int i = 0; i < swapchain->imageViews.size(); i++)
        {
            VkImageView attachments[] = {
                swapchain->imageViews[i],
                depthTexture->imageView,
            };
            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = renderpass->renderPass;
            framebufferInfo.attachmentCount = 2;
            framebufferInfo.pAttachments = attachments;
            framebufferInfo.width = swapchain->swapchain.extent.width;
            framebufferInfo.height = swapchain->swapchain.extent.height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(device->device, &framebufferInfo, nullptr, &framebuffers[i]) != VK_SUCCESS)
            {
                return nullptr;
            }
        }

        FramebufferHandle handle = SWARM_NEW<Framebuffer_T>();
        handle->framebuffers = std::move(framebuffers);

        return handle;
    }

    void DestroyFramebuffer(DeviceHandle device, FramebufferHandle &handle)
    {
        assert(g_SwarmLibrary.isInitialized);
        assert(device);

        for (auto& framebuffer : handle->framebuffers)
        {
            vkDestroyFramebuffer(device->device, framebuffer, nullptr);
        }

        SWARM_DELETE(handle);
    }
}
