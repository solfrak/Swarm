#include <swarm_internal.h>

#if defined(__linux__)
#include <wayland-client.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_wayland.h>

#include "vkinstance.h"
#include "vksurface.h"

namespace swarm
{
    SurfaceHandle CreateSurfaceWayland(InstanceHandle instance, const SurfaceCreateInfo& surfaceCreateInfo)
    {
        //TODO fetch wayland surface and display

        VkWaylandSurfaceCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR;
        createInfo.surface = static_cast<wl_surface *>(surfaceCreateInfo.surfaceReference.waylandSurface);
        createInfo.display = static_cast<wl_display *>(surfaceCreateInfo.displayReference.waylandDisplay);

        VkSurfaceKHR surfaceHandle{VK_NULL_HANDLE};
        if (vkCreateWaylandSurfaceKHR(instance->instance, &createInfo, nullptr, &surfaceHandle) != VK_SUCCESS)
        {
            return nullptr;
        }

        SurfaceHandle surface = SWARM_NEW<Surface_T>();
        surface->surface = surfaceHandle;

        return surface;
    }
}
#else
namespace swarm
{
    SurfaceHandle CreateSurfaceWayland(InstanceHandle instance, const SurfaceCreateInfo& surfaceCreateInfo)
    {
        return nullptr; //Platform not supported
    }
}
#endif
