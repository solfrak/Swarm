#pragma once

#include "math.h"
namespace swarm
{
#define SWARM_HANDLE(object) \
struct object##_T; \
using object##Handle = object##_T*;


    using SwarmAllocFn = void* (*)(unsigned int size);
    using SwarmFreeFn = void (*)(void *ptr);

    bool InitSwarm(SwarmAllocFn allocFn = nullptr, SwarmFreeFn freeFn = nullptr);
    void ShutdownSwarm();

    SWARM_HANDLE(Instance);
    SWARM_HANDLE(Surface);
    SWARM_HANDLE(PhysicalDevice);
    SWARM_HANDLE(Device);
    SWARM_HANDLE(Swapchain);
    SWARM_HANDLE(Renderpass);
    SWARM_HANDLE(Framebuffer);
    SWARM_HANDLE(Shader);
    SWARM_HANDLE(DescriptorSetlayout);
    SWARM_HANDLE(Pipeline);
    SWARM_HANDLE(CommandPool);
    SWARM_HANDLE(CommandBuffer);
    SWARM_HANDLE(Semaphore); //GPU
    SWARM_HANDLE(Fence); //CPU
    SWARM_HANDLE(Buffer);
    SWARM_HANDLE(Texture);
    SWARM_HANDLE(Sampler);

    //============================ Instance ============================

    struct InstanceCreateInfo
    {
        const char *applicationName{nullptr};
        unsigned int applicationVersion{0};
        bool isDebug{false};
    };
    InstanceHandle CreateInstance(const InstanceCreateInfo &instanceCreateInfo);
    void DestroyInstance(InstanceHandle &handle);


    //============================ Surface ============================

    enum class SessionType
    {
        WIN, X11, WAYLAND, INVALID
    };

    struct SurfaceCreateInfo
    {
        union
        {
            void *waylandSurface{nullptr};
            void* win32Hwnd;
            unsigned int x11WindowId;
        } surfaceReference;

        union
        {
            void* waylandDisplay{nullptr};
            void* x11Dpy;
            void* win32Hinstance;
        } displayReference;

        SessionType type{SessionType::INVALID};
    };

    SurfaceHandle CreateSurface(InstanceHandle instance, const SurfaceCreateInfo &surfaceCreateInfo);
    void DestroySurface(InstanceHandle instance, SurfaceHandle &handle);


    //============================ Device ============================

    struct DeviceCreateInfo
    {
        bool isDiscreteGPURequired{false};
    };

    DeviceHandle CreateDevice(InstanceHandle instance, SurfaceHandle surface, const DeviceCreateInfo& deviceCreateInfo);
    void DestroyDevice(DeviceHandle &handle);


    void WaitDeviceIdle(DeviceHandle handle);

    //============================ Swapchain ============================

    struct SwapchainCreateInfo
    {

    };

    SwapchainHandle CreateSwapchain(DeviceHandle device, const SwapchainCreateInfo &swapchainCreateInfo);
    void DestroySwapchain(DeviceHandle device, SwapchainHandle &handle);

    void GetSwapchainExtent(SwapchainHandle handle, unsigned int &width, unsigned int &height);
    unsigned int GetSwapchainImageCount(SwapchainHandle handle);

    //============================ Renderpass ============================

    struct RenderpassCreateInfo
    {

    };
    RenderpassHandle CreateRenderpass(DeviceHandle device, SwapchainHandle swapchain, const RenderpassCreateInfo &renderpassCreateInfo);
    void DestroyRenderpass(DeviceHandle device, RenderpassHandle &handle);


    //============================ Framebuffer ============================

    FramebufferHandle CreateFramebuffer(DeviceHandle device, SwapchainHandle swapchain, RenderpassHandle renderpass, TextureHandle depthTexture);
    void DestroyFramebuffer(DeviceHandle device, FramebufferHandle &handle);
    //============================ Shader ============================

    enum class ShaderStage
    {
        VERTEX, FRAGMENT
    };

    struct ShaderCreateInfo
    {
        const char* path;
        ShaderStage stage;
    };
    ShaderHandle CreateShader(DeviceHandle device, const ShaderCreateInfo &shaderCreateInfo);
    void DestroyShader(DeviceHandle device, ShaderHandle &handle);


    //============================ DescriptorSetLayout ============================
    enum class BindingType
    {
        UBO, IMAGE_SAMPLER
    };

    struct DescriptorSetLayoutBinding
    {
        unsigned int position;
        unsigned int count;
        BindingType type;
        ShaderStage stage;
    };


    DescriptorSetlayoutHandle CreateDescriptorSetlayout(DeviceHandle device, DescriptorSetLayoutBinding* bindings, unsigned int bindingCount);
    void DestroyDescriptorSetlayout(DeviceHandle device, DescriptorSetlayoutHandle &handle);


    //============================ Vertex Specification ============================
    // Flexible vertex layout system that allows users to define custom vertex formats
    //
    // Example usage:
    //     // Define a simple vertex with position and texture coordinates
    //     VertexBinding binding = {0, sizeof(float) * 5}; // binding 0, stride 5 floats
    //     VertexAttribute attrs[] = {
    //         {0, VertexAttributeType::VEC3, 0},                  // location 0: position at offset 0
    //         {1, VertexAttributeType::VEC2, sizeof(float) * 3}   // location 1: texcoord at offset 12
    //     };
    //     VertexSpecification spec = {&binding, 1, attrs, 2};
    //
    //     PipelineCreateInfo pipelineInfo = {};
    //     pipelineInfo.vertexSpec = spec;
    //     // ... set other pipeline info
    //     PipelineHandle pipeline = CreatePipeline(device, pipelineInfo);
    enum class VertexAttributeType
    {
        FLOAT, VEC2, VEC3, VEC4,
        INT, IVEC2, IVEC3, IVEC4,
        UINT, UVEC2, UVEC3, UVEC4
    };

    struct VertexAttribute
    {
        unsigned int location;
        VertexAttributeType type;
        unsigned int offset;
    };

    struct VertexBinding
    {
        unsigned int binding;
        unsigned int stride;
    };

    struct VertexSpecification
    {
        VertexBinding* bindings;
        unsigned int bindingCount;
        VertexAttribute* attributes;
        unsigned int attributeCount;
    };

    // Helper functions for creating common vertex specifications
    namespace VertexSpec
    {
        // Common vertex layouts for typical 3D rendering scenarios

        // Simple position-only vertex (12 bytes per vertex)
        // struct { Vec3 position; }
        inline VertexSpecification Position3D()
        {
            static VertexBinding binding = {0, sizeof(float) * 3};
            static VertexAttribute attribute = {0, VertexAttributeType::VEC3, 0};
            return {&binding, 1, &attribute, 1};
        }

        // Position + texture coordinates (20 bytes per vertex)
        // struct { Vec3 position; Vec2 texCoord; }
        inline VertexSpecification Position3DTexCoord()
        {
            static VertexBinding binding = {0, sizeof(float) * 5};
            static VertexAttribute attributes[] = {
                {0, VertexAttributeType::VEC3, 0},                    // position
                {1, VertexAttributeType::VEC2, sizeof(float) * 3}     // texcoord
            };
            return {&binding, 1, attributes, 2};
        }

        // Position + normal + texture coordinates (32 bytes per vertex)
        // struct { Vec3 position; Vec3 normal; Vec2 texCoord; }
        inline VertexSpecification Position3DNormal3DTexCoord()
        {
            static VertexBinding binding = {0, sizeof(float) * 8};
            static VertexAttribute attributes[] = {
                {0, VertexAttributeType::VEC3, 0},                    // position
                {1, VertexAttributeType::VEC3, sizeof(float) * 3},    // normal
                {2, VertexAttributeType::VEC2, sizeof(float) * 6}     // texcoord
            };
            return {&binding, 1, attributes, 3};
        }

        // Full vertex with position, normal, texture coords, and color (48 bytes per vertex)
        // struct { Vec3 position; Vec3 normal; Vec2 texCoord; Vec4 color; }
        inline VertexSpecification Position3DNormal3DTexCoordColor4()
        {
            static VertexBinding binding = {0, sizeof(float) * 12};
            static VertexAttribute attributes[] = {
                {0, VertexAttributeType::VEC3, 0},                     // position
                {1, VertexAttributeType::VEC3, sizeof(float) * 3},     // normal
                {2, VertexAttributeType::VEC2, sizeof(float) * 6},     // texcoord
                {3, VertexAttributeType::VEC4, sizeof(float) * 8}      // color
            };
            return {&binding, 1, attributes, 4};
        }
    }


    //============================ Pipeline ============================

    struct PipelineCreateInfo
    {
        ShaderHandle vertexShader;
        ShaderHandle fragmentShader;
        RenderpassHandle renderpass;
        DescriptorSetlayoutHandle descriptoSetLayout;
        VertexSpecification vertexSpec;
    };
    PipelineHandle CreatePipeline(DeviceHandle device, const PipelineCreateInfo &pipelineCreateInfo);
    void DestroyPipeline(DeviceHandle device, PipelineHandle &handle);




    //============================ CommandPool ============================

    CommandPoolHandle CreateCommandPool(DeviceHandle device);
    void DestroyCommandPool(DeviceHandle device, CommandPoolHandle &handle);

    //============================ CommandBuffer ============================


    CommandBufferHandle CreateCommandBuffer(DeviceHandle device, CommandPoolHandle commandPool);
    void DestroyCommandBuffer(DeviceHandle device, CommandPoolHandle commandPool, CommandBufferHandle &handle);

    //============================ Synchronisation ============================

    SemaphoreHandle CreateSemaphore(DeviceHandle device);
    FenceHandle CreateFence(DeviceHandle device);

    void DestroySemaphore(DeviceHandle device, SemaphoreHandle &handle);
    void DestroyFence(DeviceHandle device, FenceHandle &handle);

    //============================ Buffer ============================

    enum class BufferMemoryType
    {
        GPU_ONLY,
        CPU_TO_GPU, //Host visible for frequent update
        GPU_TO_CPU, //For readback
        STAGING, //Temporary transfer buffer
    };

    enum class BufferUsageFlags : uint32_t
    {
        NONE = 0,
        VERTEX = 1 << 0,
        INDEX = 1 << 1,
        UNIFORM = 1 << 2,
        STORAGE = 1 << 3,
        TRANSFER_SRC = 1 << 4,
        TRANSFER_DST = 1 << 5,
        INDIRECT = 1 << 6,
    };

    inline BufferUsageFlags operator|(BufferUsageFlags a, BufferUsageFlags b) {
        return static_cast<BufferUsageFlags>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
    }

    inline BufferUsageFlags operator&(BufferUsageFlags a, BufferUsageFlags b) {
        return static_cast<BufferUsageFlags>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
    }

    struct BufferCreateInfo
    {
        unsigned int size{0};
        BufferMemoryType memoryType{BufferMemoryType::GPU_ONLY};
        BufferUsageFlags usage{BufferUsageFlags::NONE};
    };

    BufferHandle CreateBuffer(DeviceHandle device, const BufferCreateInfo &bufferCreateInfo);
    void DestroyBuffer(DeviceHandle device, BufferHandle &handle);
    void UpdateBuffer(DeviceHandle device, CommandPoolHandle commandPool, BufferHandle buffer, const void* data, unsigned int size);
    void CopyBuffer(DeviceHandle device, CommandPoolHandle commandPool, BufferHandle srcBuffer, BufferHandle dstBuffer, unsigned int size);

    //============================ Texture ============================
    enum class TextureType
    {
        TEXTURE_2D,
        TEXTURE_CUBE,
    };

    enum class TextureFormat
    {
        // Color formats
        RGBA8_UNORM,
        RGBA8_SRGB,

        // HDR formats
        RGBA16_SFLOAT,

        // Depth formats
        D32_SFLOAT,
    };

    enum class TextureUsageFlags : uint32_t
    {
        NONE = 0,
        TRANSFER_SRC = 1 << 0,
        TRANSFER_DST = 1 << 1,
        SAMPLED = 1 << 2,
        COLOR_ATTACHMENT = 1 << 3,
        DEPTH_STENCIL_ATTACHMENT = 1 << 4,
    };

    inline TextureUsageFlags operator|(TextureUsageFlags a, TextureUsageFlags b) {
        return static_cast<TextureUsageFlags>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
    }

    inline TextureUsageFlags operator&(TextureUsageFlags a, TextureUsageFlags b) {
        return static_cast<TextureUsageFlags>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
    }

    struct TextureCreateInfo
    {
        TextureType type{TextureType::TEXTURE_2D};
        TextureFormat format{TextureFormat::RGBA8_UNORM};
        TextureUsageFlags usage{TextureUsageFlags::NONE};

        unsigned int width{1};
        unsigned int height{1};
        unsigned int mipLevels{1};
    };

    TextureHandle CreateTexture(DeviceHandle device, const TextureCreateInfo& createInfo);
    void DestroyTexture(DeviceHandle device, TextureHandle &handle);
    // void UpdateTexture(DeviceHandle device, CommandPoolHandle commandPool, TextureHandle texture, const void* data, unsigned int size);

    //============================ Sampler ============================
    enum class TextureFilter
    {
        NEAREST,
        LINEAR,
    };

    enum class TextureAddressMode
    {
        REPEAT,
        CLAMP_TO_EDGE,
    };

    struct SamplerCreateInfo
    {
        TextureFilter minFilter{TextureFilter::LINEAR};
        TextureFilter magFilter{TextureFilter::LINEAR};
        TextureFilter mipFilter{TextureFilter::LINEAR};

        TextureAddressMode addressModeU{TextureAddressMode::REPEAT};
        TextureAddressMode addressModeV{TextureAddressMode::REPEAT};
        TextureAddressMode addressModeW{TextureAddressMode::REPEAT};

        float maxAnisotropy{1.0f};
    };

    SamplerHandle CreateSampler(DeviceHandle device, const SamplerCreateInfo& createInfo);
    void DestroySampler(DeviceHandle device, SamplerHandle& handle);

    //============================ Rendering cmd ============================
    struct CmdBeginFrameInfo
    {
        DeviceHandle device;
        FenceHandle inFlightFence;

        Swapchainchain
    };
    void CmdBeginFrame(CmdBeginFrameInfo& info);

    struct CmdEndFrameInfo
    {

    };
    void CmdEndFrame(CmdEndFrameInfo& info);

    struct CmdSubmitInfo
    {

    };
    void CmdSubmitFrame(CmdSubmitInfo& info);
}
