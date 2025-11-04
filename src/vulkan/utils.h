#pragma once

#include <vulkan/vulkan.h>
#include <swarm/swarm.h>
#include <vector>
namespace swarm
{
    inline VkFormat FindSupportedFormat(VkPhysicalDevice physDevice, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
    {
        for (VkFormat format : candidates) {
            VkFormatProperties props;
            vkGetPhysicalDeviceFormatProperties(physDevice, format, &props);

            if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
                return format;
            } else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
                return format;
            }
        }

        return VK_FORMAT_UNDEFINED;
    }

    inline VkFormat FindDepthFormat(VkPhysicalDevice physDevice)
    {
        return FindSupportedFormat(physDevice,
             {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
             VK_IMAGE_TILING_OPTIMAL,
             VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
         );
    }

    inline unsigned int FindMemoryType(VkPhysicalDevice physicalDevice, unsigned int typeFilter, VkMemoryPropertyFlags properties)
    {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

        for (unsigned int i = 0; i < memProperties.memoryTypeCount; i++)
        {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
            {
                return i;
            }
        }

        return -1;
    }

    inline VkFormat VertexAttributeTypeToVkFormat(VertexAttributeType type)
    {
        switch (type)
        {
            case VertexAttributeType::FLOAT: return VK_FORMAT_R32_SFLOAT;
            case VertexAttributeType::VEC2: return VK_FORMAT_R32G32_SFLOAT;
            case VertexAttributeType::VEC3: return VK_FORMAT_R32G32B32_SFLOAT;
            case VertexAttributeType::VEC4: return VK_FORMAT_R32G32B32A32_SFLOAT;
            case VertexAttributeType::INT: return VK_FORMAT_R32_SINT;
            case VertexAttributeType::IVEC2: return VK_FORMAT_R32G32_SINT;
            case VertexAttributeType::IVEC3: return VK_FORMAT_R32G32B32_SINT;
            case VertexAttributeType::IVEC4: return VK_FORMAT_R32G32B32A32_SINT;
            case VertexAttributeType::UINT: return VK_FORMAT_R32_UINT;
            case VertexAttributeType::UVEC2: return VK_FORMAT_R32G32_UINT;
            case VertexAttributeType::UVEC3: return VK_FORMAT_R32G32B32_UINT;
            case VertexAttributeType::UVEC4: return VK_FORMAT_R32G32B32A32_UINT;
        }
        return VK_FORMAT_UNDEFINED;
    }

    inline std::vector<VkVertexInputBindingDescription> BuildVertexInputBindings(const VertexSpecification& vertexSpec)
    {
        std::vector<VkVertexInputBindingDescription> bindings;
        bindings.reserve(vertexSpec.bindingCount);

        for (unsigned int i = 0; i < vertexSpec.bindingCount; ++i)
        {
            VkVertexInputBindingDescription bindingDesc{};
            bindingDesc.binding = vertexSpec.bindings[i].binding;
            bindingDesc.stride = vertexSpec.bindings[i].stride;
            bindingDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
            bindings.push_back(bindingDesc);
        }

        return bindings;
    }

    inline std::vector<VkVertexInputAttributeDescription> BuildVertexInputAttributes(const VertexSpecification& vertexSpec)
    {
        std::vector<VkVertexInputAttributeDescription> attributes;
        attributes.reserve(vertexSpec.attributeCount);

        for (unsigned int i = 0; i < vertexSpec.attributeCount; ++i)
        {
            VkVertexInputAttributeDescription attrDesc{};
            // For now, assume all attributes use binding 0, but this could be extended
            // by adding a binding field to VertexAttribute in the future
            attrDesc.binding = 0;
            attrDesc.location = vertexSpec.attributes[i].location;
            attrDesc.format = VertexAttributeTypeToVkFormat(vertexSpec.attributes[i].type);
            attrDesc.offset = vertexSpec.attributes[i].offset;
            attributes.push_back(attrDesc);
        }

        return attributes;
    }
}