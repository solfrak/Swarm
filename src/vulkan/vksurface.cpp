#include "vksurface.h"

#include <VkBootstrap.h>

#include "vkinstance.h"

namespace swarm
{
    SurfaceHandle CreateSurfaceWin(InstanceHandle instance, const SurfaceCreateInfo& surfaceCreateInfo);
    SurfaceHandle CreateSurfaceX11(InstanceHandle instance, const SurfaceCreateInfo& surfaceCreateInfo);
    SurfaceHandle CreateSurfaceWayland(InstanceHandle instance, const SurfaceCreateInfo& surfaceCreateInfo);

    SurfaceHandle CreateSurface(InstanceHandle instance, const SurfaceCreateInfo& surfaceCreateInfo)
    {
        assert(g_SwarmLibrary.isInitialized);
        assert(instance);

        switch (surfaceCreateInfo.type)
        {
            case SessionType::WIN:
                return CreateSurfaceWin(instance, surfaceCreateInfo);
            case SessionType::X11:
                return CreateSurfaceX11(instance, surfaceCreateInfo);
            case SessionType::WAYLAND:
                return CreateSurfaceWayland(instance, surfaceCreateInfo);
            case SessionType::INVALID:
            default:
                assert(false); //We should never end up here
                return nullptr;
        }
    }

    void DestroySurface(InstanceHandle instance, SurfaceHandle &handle)
    {
        assert(g_SwarmLibrary.isInitialized);
        assert(instance != nullptr);
        assert(handle != nullptr);

        vkb::destroy_surface(instance->instance, handle->surface);

        SWARM_DELETE(handle);
    }
}
