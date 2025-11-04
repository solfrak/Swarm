#include "vkbuffer.h"
#include "vkdevice.h"
#include "vkcommandpool.h"
#include "swarm_internal.h"

#include <cassert>
#include <vulkan/vulkan_core.h>
#include <vk_mem_alloc.h>

namespace swarm
{
    VkBufferUsageFlags TranslateUsageFlags(BufferUsageFlags usage)
    {
        VkBufferUsageFlags flags = 0;

        if ((usage & BufferUsageFlags::VERTEX) != BufferUsageFlags::NONE)
            flags |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

        if ((usage & BufferUsageFlags::INDEX) != BufferUsageFlags::NONE)
            flags |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;

        if ((usage & BufferUsageFlags::UNIFORM) != BufferUsageFlags::NONE)
            flags |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

        if ((usage & BufferUsageFlags::STORAGE) != BufferUsageFlags::NONE)
            flags |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;

        if ((usage & BufferUsageFlags::TRANSFER_SRC) != BufferUsageFlags::NONE)
            flags |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

        if ((usage & BufferUsageFlags::TRANSFER_DST) != BufferUsageFlags::NONE)
            flags |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;

        if ((usage & BufferUsageFlags::INDIRECT) != BufferUsageFlags::NONE)
            flags |= VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;

        return flags;
    }

    VmaAllocationCreateInfo GetAllocationInfo(BufferMemoryType memoryType)
    {
        VmaAllocationCreateInfo allocInfo{};
        allocInfo.usage = VMA_MEMORY_USAGE_AUTO;

        switch (memoryType)
        {
            case BufferMemoryType::GPU_ONLY:
                // Device-local, best performance
                allocInfo.flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;
                allocInfo.priority = 1.0f; // High priority for GPU resources
                break;

            case BufferMemoryType::CPU_TO_GPU:
                // Host-visible, persistently mapped
                allocInfo.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT |
                                  VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;
                break;

            case BufferMemoryType::GPU_TO_CPU:
                // For readbacks
                allocInfo.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT |
                                  VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT;
                break;

            case BufferMemoryType::STAGING:
                // Temporary transfer buffer
                allocInfo.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT |
                                  VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;
                break;
        }

        return allocInfo;
    }

    BufferHandle CreateBuffer(DeviceHandle device, const BufferCreateInfo &bufferCreateInfo)
    {
        assert(g_SwarmLibrary.isInitialized);
        assert(device);

        VkBufferCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        createInfo.size = bufferCreateInfo.size;
        createInfo.usage = TranslateUsageFlags(bufferCreateInfo.usage);
        createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VmaAllocationCreateInfo allocInfo = GetAllocationInfo(bufferCreateInfo.memoryType);

        VkBuffer buffer{VK_NULL_HANDLE};
        VmaAllocation allocation{VK_NULL_HANDLE};
        VmaAllocationInfo allocationInfo{};
        if (vmaCreateBuffer(device->allocator, &createInfo, &allocInfo, &buffer, &allocation, &allocationInfo) !=
            VK_SUCCESS)
        {
            return nullptr;
        }

        BufferHandle handle = SWARM_NEW<Buffer_T>();
        handle->buffer = buffer;
        handle->size = bufferCreateInfo.size;
        handle->allocation = allocation;
        handle->mappedData = allocationInfo.pMappedData;

        return handle;
    }


    void DestroyBuffer(DeviceHandle device, BufferHandle &handle)
    {
        assert(g_SwarmLibrary.isInitialized);
        assert(device);
        assert(handle);

        vmaDestroyBuffer(device->allocator, handle->buffer, handle->allocation);

        SWARM_DELETE(handle);
        handle = nullptr;
    }

    void UpdateBuffer(DeviceHandle device, CommandPoolHandle commandPool, BufferHandle buffer, const void *data, unsigned int size)
    {
        assert(g_SwarmLibrary.isInitialized);
        assert(device);
        assert(buffer);
        assert(data);
        assert(size > 0 && size <= buffer->size);

        if (buffer->mappedData)
        {
            memcpy(buffer->mappedData, data, size);
        } else
        {
            // Create a temporary staging buffer
            BufferCreateInfo stagingInfo{};
            stagingInfo.usage = BufferUsageFlags::TRANSFER_SRC;
            stagingInfo.memoryType = BufferMemoryType::CPU_TO_GPU;
            stagingInfo.size = size;
            BufferHandle stagingBuffer = CreateBuffer(device, stagingInfo);

            // Copy data to staging buffer
            memcpy(stagingBuffer->mappedData, data, size);

            // Copy from staging to destination buffer using a command buffer
            CopyBuffer(device, commandPool, stagingBuffer, buffer, size);

            // Clean up staging buffer
            DestroyBuffer(device, stagingBuffer);
        }
    }

    void CopyBuffer(DeviceHandle device, CommandPoolHandle commandPool, BufferHandle srcBuffer, BufferHandle dstBuffer, unsigned int size)
    {
        assert(device);
        assert(commandPool);
        assert(srcBuffer);
        assert(dstBuffer);
        assert(size > 0);

        // Create a one-time command buffer
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = commandPool->commandPool;
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(device->device, &allocInfo, &commandBuffer);

        // Begin recording
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);

        // Record the copy command
        VkBufferCopy copyRegion{};
        copyRegion.srcOffset = 0;
        copyRegion.dstOffset = 0;
        copyRegion.size = size;
        vkCmdCopyBuffer(commandBuffer, srcBuffer->buffer, dstBuffer->buffer, 1, &copyRegion);

        // End recording
        vkEndCommandBuffer(commandBuffer);

        // Submit and execute
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        vkQueueSubmit(device->device.get_queue(vkb::QueueType::graphics).value(), 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(device->device.get_queue(vkb::QueueType::graphics).value());

        // Clean up
        vkFreeCommandBuffers(device->device, commandPool->commandPool, 1, &commandBuffer);
    }
}
