#include <swarm_internal.h>


#if defined(WIN32)
#include "vkinstance.h"
#include "vksurface.h"

#include <Windows.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_win32.h>
namespace swarm
{
    SurfaceHandle CreateSurfaceWin(InstanceHandle instance, const SurfaceCreateInfo& surfaceCreateInfo)
    {
        VkWin32SurfaceCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        createInfo.hinstance = static_cast<HINSTANCE>(surfaceCreateInfo.displayReference.win32Hinstance);
        createInfo.hwnd = static_cast<HWND>(surfaceCreateInfo.surfaceReference.win32Hwnd);

        VkSurfaceKHR surface{VK_NULL_HANDLE};
        if (vkCreateWin32SurfaceKHR(instance->instance, &createInfo, nullptr, &surface) != VK_SUCCESS)
        {
            return nullptr;
        }

        SurfaceHandle handle = SWARM_NEW<Surface_T>();
        handle->surface = surface;

        return handle;
    }
}
#else
namespace swarm
{
    SurfaceHandle CreateSurfaceWin(InstanceHandle instance, const SurfaceCreateInfo& surfaceCreateInfo)
    {
        return nullptr; //Platform not supported
    }
}
#endif
