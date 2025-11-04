#include "vkcommandbuffer.h"
#include "vkcommandpool.h"
#include "vkdevice.h"

#include <cassert>

#include <vk_mem_alloc.h>

#include "vkswapchain.h"


namespace swarm
{
    CommandBufferHandle CreateCommandBuffer(DeviceHandle device, CommandPoolHandle commandPool)
    {
        assert(g_SwarmLibrary.isInitialized);
        assert(device);
        assert(commandPool);

        VkCommandBufferAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = commandPool->commandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1; //TODO: maybe return a vector and receive a number of command buffer to create


        VkCommandBuffer commandBuffer{VK_NULL_HANDLE};
        if (vkAllocateCommandBuffers(device->device, &allocInfo, &commandBuffer) != VK_SUCCESS)
        {
            return nullptr;
        }

        CommandBufferHandle handle = SWARM_NEW<CommandBuffer_T>();
        handle->commandBuffer = commandBuffer;

        return handle;
    }

    void DestroyCommandBuffer(DeviceHandle device, CommandPoolHandle commandPool, CommandBufferHandle &handle)
    {
        assert(g_SwarmLibrary.isInitialized);
        assert(device);
        assert(handle);

        vkFreeCommandBuffers(device->device, commandPool->commandPool, 1, &handle->commandBuffer); //TODO: maybe receive a vector of command buffers

        SWARM_DELETE(handle);
    }

}
