#include "vkshader.h"
#include "vkdevice.h"

#include <cassert>
#include <filesystem>
#include <fstream>

#include <iostream>

namespace swarm
{
    constexpr const char *vkShaderFileExtension = ".spv";

    ShaderHandle CreateShader(DeviceHandle device, const ShaderCreateInfo &shaderCreateInfo)
    {
        assert(g_SwarmLibrary.isInitialized);
        assert(device);

        std::filesystem::path path(shaderCreateInfo.path);
        if (path.extension() != vkShaderFileExtension)
            return nullptr;

        std::fstream file(path, std::ios::in | std::ios::binary);

        if (!file.is_open())
            return nullptr;


        const unsigned int fileSize = std::filesystem::file_size(path);
        std::vector<char> buffer;
        buffer.resize(fileSize);
        file.read(buffer.data(), fileSize);
        file.close();

        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = fileSize;
        createInfo.pCode = reinterpret_cast<unsigned int *>(buffer.data());

        VkShaderModule shader{VK_NULL_HANDLE};
        if (vkCreateShaderModule(device->device, &createInfo, nullptr, &shader) != VK_SUCCESS)
        {
            return nullptr;
        }

        ShaderHandle handle = SWARM_NEW<Shader_T>();
        handle->module = shader;
        handle->stage = shaderCreateInfo.stage;

        return handle;
    }

    void DestroyShader(DeviceHandle device, ShaderHandle &handle)
    {
        assert(g_SwarmLibrary.isInitialized);
        assert(device);
        assert(handle);

        vkDestroyShaderModule(device->device, handle->module, nullptr);

        SWARM_DELETE(handle);
    }
}
