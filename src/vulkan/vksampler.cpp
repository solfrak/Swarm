#include "vksampler.h"
#include "vkdevice.h"

#include <cassert>
namespace swarm
{
    VkFilter ConvertFilter(TextureFilter filter)
    {
        switch (filter)
        {
            case TextureFilter::NEAREST:
                return VK_FILTER_NEAREST;
            case TextureFilter::LINEAR:
                return VK_FILTER_LINEAR;
            default:
                return VK_FILTER_MAX_ENUM;
        }
    }

    VkSamplerAddressMode ConvertAddressMode(TextureAddressMode mode)
    {
        switch (mode)
        {
            case TextureAddressMode::REPEAT:
                return VK_SAMPLER_ADDRESS_MODE_REPEAT;
            case TextureAddressMode::CLAMP_TO_EDGE:
                return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
            default:
                return VK_SAMPLER_ADDRESS_MODE_MAX_ENUM;
        }
    }

    SamplerHandle CreateSampler(DeviceHandle device, const SamplerCreateInfo& createInfo)
    {
        assert(g_SwarmLibrary.isInitialized);
        assert(device);

        VkPhysicalDeviceProperties properties{};
        vkGetPhysicalDeviceProperties(device->device.physical_device, &properties);

        VkSamplerCreateInfo samplerInfo{};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = ConvertFilter(createInfo.magFilter);
        samplerInfo.minFilter = ConvertFilter(createInfo.minFilter);
        samplerInfo.addressModeU = ConvertAddressMode(createInfo.addressModeU);
        samplerInfo.addressModeV = ConvertAddressMode(createInfo.addressModeV);
        samplerInfo.addressModeW = ConvertAddressMode(createInfo.addressModeW);
        samplerInfo.anisotropyEnable = VK_FALSE;
        samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

        VkSampler sampler{VK_NULL_HANDLE};
        if (vkCreateSampler(device->device, &samplerInfo, nullptr, &sampler) != VK_SUCCESS) {
            return nullptr;
        }

        SamplerHandle handle = SWARM_NEW<Sampler_T>();
        handle->sampler = sampler;
        return handle;
    }

    void DestroySampler(DeviceHandle device, SamplerHandle& handle)
    {
        assert(g_SwarmLibrary.isInitialized);
        assert(device);
        assert(handle);

        vkDestroySampler(device->device, handle->sampler, nullptr);

        SWARM_DELETE(handle);
    }
}