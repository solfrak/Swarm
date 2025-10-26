#include "vkdescriptorsetlayout.h"
#include "vkdevice.h"
#include "swarm_internal.h"


#include <cassert>
#include <vulkan/vulkan_core.h>

namespace swarm
{

    VkDescriptorType GetDescriptorType(BindingType type)
    {
        switch (type)
        {
            case BindingType::UBO:
                return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            case BindingType::IMAGE_SAMPLER:
                return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        }
    }

    VkShaderStageFlags GetShaderStageFlags(ShaderStage stage)
    {
        switch (stage)
        {
            case ShaderStage::VERTEX:
                return VK_SHADER_STAGE_VERTEX_BIT;
            case ShaderStage::FRAGMENT:
                return VK_SHADER_STAGE_FRAGMENT_BIT;
        }
    }


    DescriptorSetlayoutHandle CreateDescriptorSetlayout(DeviceHandle device, DescriptorSetLayoutBinding *bindings,
        unsigned int bindingCount)
    {
        assert(g_SwarmLibrary.isInitialized);
        assert(device);

        std::vector<VkDescriptorSetLayoutBinding> vkBindings(bindingCount);
        for (unsigned int i = 0; i < bindingCount; i++)
        {
            vkBindings[i].binding = bindings[i].position;
            vkBindings[i].descriptorType = GetDescriptorType(bindings[i].type);
            vkBindings[i].descriptorCount = 1;
            vkBindings[i].stageFlags = GetShaderStageFlags(bindings[i].stage);
        }

        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = static_cast<uint32_t>(vkBindings.size());
        layoutInfo.pBindings = vkBindings.data();

        VkDescriptorSetLayout setLayout;
        if (vkCreateDescriptorSetLayout(device->device, &layoutInfo, nullptr, &setLayout) != VK_SUCCESS)
        {
            return nullptr;
        }

        DescriptorSetlayoutHandle handle = SWARM_NEW<DescriptorSetlayout_T>();
        handle->setLayout = setLayout;
        return handle;
    }

    void DestroyDescriptorSetlayout(DeviceHandle device, DescriptorSetlayoutHandle &handle)
    {
        assert(g_SwarmLibrary.isInitialized);
        assert(device);
        assert(handle);

        vkDestroyDescriptorSetLayout(device->device, handle->setLayout, nullptr);

        SWARM_DELETE(handle);
    }
}
