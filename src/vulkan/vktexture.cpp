#include "vktexture.h"
#include "vkdevice.h"
#include "utils.h"

#include <cassert>

namespace swarm
{
    VkImageType ConvertImageType(TextureType type)
    {
        switch (type)
        {
            case TextureType::TEXTURE_2D:
                return VK_IMAGE_TYPE_2D;
            default:
                return VK_IMAGE_TYPE_MAX_ENUM;
        }
    }

    VkFormat ConvertTextureFormat(TextureFormat format)
    {
        switch (format)
        {
            case TextureFormat::RGBA8_UNORM:
                return VK_FORMAT_R8G8B8A8_UNORM;
            case TextureFormat::RGBA8_SRGB:
                return VK_FORMAT_R8G8B8A8_SRGB;
            case TextureFormat::RGBA16_SFLOAT:
                return VK_FORMAT_R16G16B16A16_SFLOAT;
            case TextureFormat::D32_SFLOAT:
                return VK_FORMAT_D32_SFLOAT;
            default:
                return VK_FORMAT_UNDEFINED;
        }
    }

    VkImageUsageFlags ConvertTextureUsage(TextureUsageFlags usage)
    {
        VkImageUsageFlags flags = 0;

        if (static_cast<uint32_t>(usage & TextureUsageFlags::TRANSFER_SRC))
            flags |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
        if (static_cast<uint32_t>(usage & TextureUsageFlags::TRANSFER_DST))
            flags |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
        if (static_cast<uint32_t>(usage & TextureUsageFlags::SAMPLED))
            flags |= VK_IMAGE_USAGE_SAMPLED_BIT;
        if (static_cast<uint32_t>(usage & TextureUsageFlags::COLOR_ATTACHMENT))
            flags |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        if (static_cast<uint32_t>(usage & TextureUsageFlags::DEPTH_STENCIL_ATTACHMENT))
            flags |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

        return flags;
    }

    VkImageAspectFlags GetImageAspect(TextureFormat format)
    {
        switch (format)
        {
            case TextureFormat::D32_SFLOAT:
                return VK_IMAGE_ASPECT_DEPTH_BIT;
            default:
                return VK_IMAGE_ASPECT_COLOR_BIT;
        }
    }

    TextureHandle CreateTexture(DeviceHandle device, const TextureCreateInfo &createInfo)
    {
        assert(g_SwarmLibrary.isInitialized);
        assert(device);

        // Create image
        VkImageCreateInfo imageInfo{};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = ConvertImageType(createInfo.type);
        imageInfo.extent.width = createInfo.width;
        imageInfo.extent.height = createInfo.height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = createInfo.mipLevels;
        imageInfo.arrayLayers = (createInfo.type == TextureType::TEXTURE_CUBE) ? 6 : 1;
        imageInfo.format = ConvertTextureFormat(createInfo.format);
        imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = ConvertTextureUsage(createInfo.usage);
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.flags = (createInfo.type == TextureType::TEXTURE_CUBE) ? VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT : 0;

        // Use VMA for allocation
        VmaAllocationCreateInfo allocInfo{};
        allocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

        VkImage image{VK_NULL_HANDLE};
        VmaAllocation imageAllocation{VK_NULL_HANDLE};

        if (vmaCreateImage(device->allocator, &imageInfo, &allocInfo,
                           &image, &imageAllocation, nullptr) != VK_SUCCESS)
        {
            return nullptr;
        }

        // Create image view
        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = image;
        viewInfo.viewType = (createInfo.type == TextureType::TEXTURE_CUBE)
                                ? VK_IMAGE_VIEW_TYPE_CUBE
                                : VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = ConvertTextureFormat(createInfo.format);
        viewInfo.subresourceRange.aspectMask = GetImageAspect(createInfo.format);
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = createInfo.mipLevels;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = (createInfo.type == TextureType::TEXTURE_CUBE) ? 6 : 1;

        VkImageView imageView{VK_NULL_HANDLE};
        if (vkCreateImageView(device->device, &viewInfo, nullptr, &imageView) != VK_SUCCESS)
        {
            vmaDestroyImage(device->allocator, image, imageAllocation);
            return nullptr;
        }


        TextureHandle handle = SWARM_NEW<Texture_T>();
        handle->image = image;
        handle->imageView = imageView;
        handle->imageAllocation = imageAllocation;
        return handle;
    }

    void DestroyTexture(DeviceHandle device, TextureHandle &handle)
    {
        assert(g_SwarmLibrary.isInitialized);
        assert(device);
        assert(handle);

        vmaDestroyImage(device->allocator, handle->image, handle->imageAllocation);
        vkDestroyImageView(device->device, handle->imageView, nullptr);

        SWARM_DELETE(handle);
    }
}
