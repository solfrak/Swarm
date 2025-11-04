#include "vkcommandpool.h"

#include <cassert>

#include "vkdevice.h"

namespace swarm
{
    CommandPoolHandle CreateCommandPool(DeviceHandle device)
    {
        assert(g_SwarmLibrary.isInitialized);
        assert(device);

        VkCommandPoolCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        createInfo.queueFamilyIndex = device->device.get_queue_index(vkb::QueueType::graphics).value();

        VkCommandPool commandPool {VK_NULL_HANDLE};
        if (vkCreateCommandPool(device->device, &createInfo, nullptr, &commandPool) != VK_SUCCESS)
        {
            return nullptr;
        }

        CommandPoolHandle handle = SWARM_NEW<CommandPool_T>();
        handle->commandPool = commandPool;

        return handle;
    }


    void DestroyCommandPool(DeviceHandle device, CommandPoolHandle &handle)
    {
        assert(g_SwarmLibrary.isInitialized);
        assert(device);
        assert(handle);

        vkDestroyCommandPool(device->device, handle->commandPool, nullptr);

        SWARM_DELETE(handle);
    }
}
