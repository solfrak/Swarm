#include "vkdevice.h"

#include "vkinstance.h"
#include "vksurface.h"

namespace swarm
{
    DeviceHandle CreateDevice(InstanceHandle instance, SurfaceHandle surface, const DeviceCreateInfo &deviceCreateInfo)
    {
        assert(g_SwarmLibrary.isInitialized);
        assert(instance);

        vkb::PhysicalDeviceSelector deviceSelector{instance->instance, surface->surface};
        if (deviceCreateInfo.isDiscreteGPURequired)
            deviceSelector.prefer_gpu_device_type(vkb::PreferredDeviceType::discrete);

        const auto physicalDeviceResult = deviceSelector.select();
        if (!physicalDeviceResult.has_value())
            return nullptr;

        vkb::PhysicalDevice physicalDevice = physicalDeviceResult.value();
        vkb::DeviceBuilder deviceBuilder{physicalDevice};
        const auto deviceResult = deviceBuilder.build();

        if (!deviceResult.has_value())
            return nullptr;

        DeviceHandle handle = SWARM_NEW<Device_T>();
        handle->device = deviceResult.value();

        return handle;
    }

    void DestroyDevice(DeviceHandle &handle)
    {
        assert(g_SwarmLibrary.isInitialized);
        assert(handle);

        vkb::destroy_device(handle->device);

        SWARM_DELETE(handle);
    }
}
