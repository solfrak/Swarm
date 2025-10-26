#include <swarm_internal.h>

#if defined(__linux__)
#include <vulkan/vulkan.h>
#include <X11/Xlib.h>
#include <vulkan/vulkan_xlib.h>
#include "vkinstance.h"
#include "vksurface.h"

namespace swarm
{
    SurfaceHandle CreateSurfaceX11(InstanceHandle instance, const SurfaceCreateInfo& surfaceCreateInfo)
    {
        VkXlibSurfaceCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
        createInfo.dpy = static_cast<Display *>(surfaceCreateInfo.displayReference.x11Dpy);
        createInfo.window = surfaceCreateInfo.surfaceReference.x11WindowId;

        VkSurfaceKHR surface{VK_NULL_HANDLE};
        if (vkCreateXlibSurfaceKHR(instance->instance, &createInfo, nullptr, &surface) != VK_SUCCESS)
        {
            return nullptr;
        }

        SurfaceHandle surfaceHandle = SWARM_NEW<Surface_T>();
        surfaceHandle->surface = surface;

        return surfaceHandle;
    }
}
#else
namespace swarm
{
    SurfaceHandle CreateSurfaceX11(InstanceHandle instance, const SurfaceCreateInfo& surfaceCreateInfo)
    {
        return nullptr; //Platform not supported
    }
}
#endif