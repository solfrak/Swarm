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
        swapchainBuilder.set_desired_extent(900, 720);

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

    void GetSwapchainExtent(SwapchainHandle handle, unsigned int &width, unsigned int &height)
    {
        width = handle->swapchain.extent.width;
        height = handle->swapchain.extent.height;
    }


    unsigned int GetSwapchainImageCount(SwapchainHandle handle)
    {
        return handle->imageViews.size();
    }
}
