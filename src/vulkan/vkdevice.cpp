#include "vkdevice.h"

#include "vkinstance.h"
#include "vksurface.h"

#include <vk_mem_alloc.h>
namespace swarm
{
    DeviceHandle CreateDevice(InstanceHandle instance, SurfaceHandle surface, const DeviceCreateInfo &deviceCreateInfo)
    {
        assert(g_SwarmLibrary.isInitialized);
        assert(instance);

        vkb::PhysicalDeviceSelector deviceSelector{instance->instance};
        deviceSelector.set_surface(surface->surface);

        if (deviceCreateInfo.isDiscreteGPURequired)
            deviceSelector.prefer_gpu_device_type(vkb::PreferredDeviceType::discrete);


        const auto physicalDeviceResult = deviceSelector.select();
        if (!physicalDeviceResult.has_value())
            return nullptr;

        vkb::PhysicalDevice physicalDevice = physicalDeviceResult.value();

        VkPhysicalDeviceFeatures deviceFeatures{};
        deviceFeatures.samplerAnisotropy = VK_TRUE;
        physicalDevice.enable_features_if_present(deviceFeatures);

        vkb::DeviceBuilder deviceBuilder{physicalDevice};

        const auto deviceResult = deviceBuilder.build();

        if (!deviceResult.has_value())
            return nullptr;

        VmaAllocatorCreateInfo allocatorInfo = {};
        allocatorInfo.physicalDevice = physicalDevice.physical_device;
        allocatorInfo.device = deviceResult.value().device;
        allocatorInfo.instance = instance->instance;

        VmaAllocator allocator;
        if(vmaCreateAllocator(&allocatorInfo, &allocator) != VK_SUCCESS)
            return nullptr;


        DeviceHandle handle = SWARM_NEW<Device_T>();
        handle->device = deviceResult.value();
        handle->allocator = allocator;


        return handle;
    }

    void DestroyDevice(DeviceHandle &handle)
    {
        assert(g_SwarmLibrary.isInitialized);
        assert(handle);

        vmaDestroyAllocator(handle->allocator);
        vkb::destroy_device(handle->device);

        SWARM_DELETE(handle);
    }

    void WaitDeviceIdle(DeviceHandle device)
    {
        vkDeviceWaitIdle(device->device);
    }


}
