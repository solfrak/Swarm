#include "vkswapchain.h"
#include "vkdevice.h"

#include <vulkan/vulkan.h>
namespace swarm
{
    SwapchainHandle CreateSwapchain(DeviceHandle device, const SwapchainCreateInfo &swapchainCreateInfo)
    {
        assert(g_SwarmLibrary.isInitialized);
        assert(device);

        vkb::SwapchainBuilder swapchainBuilder{device->device};
        const auto swapchainResult = swapchainBuilder.build();

        if (!swapchainResult.has_value())
            return nullptr;

        SwapchainHandle handle = SWARM_NEW<Swapchain_T>();
        handle->swapchain = swapchainResult.value();
        handle->imageViews = handle->swapchain.get_image_views().value();

        return handle;
    }

    void DestroySwapchain(DeviceHandle device, SwapchainHandle &handle)
    {
        assert(g_SwarmLibrary.isInitialized);
        assert(device);
        assert(handle);

        for (const auto& imageView : handle->imageViews)
        {
            vkDestroyImageView(device->device, imageView, nullptr);
        }

        vkb::destroy_swapchain(handle->swapchain);

        SWARM_DELETE(handle);
    }
}
