#pragma once


namespace swarm
{
#define SWARM_HANDLE(object) \
struct object##_T; \
using object##Handle = object##_T*;


    using SwarmAllocFn = void* (*)(unsigned int size);
    using SwarmFreeFn = void (*)(void *ptr);

    bool InitSwarm(SwarmAllocFn allocFn = nullptr, SwarmFreeFn freeFn = nullptr);
    void ShutdownSwarm();


    //============================ Instance ============================
    SWARM_HANDLE(Instance);
    struct InstanceCreateInfo
    {
        const char *applicationName{nullptr};
        unsigned int applicationVersion{0};
        bool isDebug{false};
    };
    InstanceHandle CreateInstance(const InstanceCreateInfo &instanceCreateInfo);
    void DestroyInstance(InstanceHandle &handle);


    //============================ Surface ============================
    SWARM_HANDLE(Surface);
    enum class SessionType
    {
        WIN, X11, WAYLAND, INVALID
    };

    struct SurfaceCreateInfo
    {
        union
        {
            void *waylandSurface{nullptr};
            unsigned int x11WindowId;
        } surfaceReference;

        union
        {
            void* waylandDisplay{nullptr};
            void* x11Dpy;
        } displayReference;

        SessionType type{SessionType::INVALID};
    };

    SurfaceHandle CreateSurface(InstanceHandle instance, const SurfaceCreateInfo &surfaceCreateInfo);
    void DestroySurface(InstanceHandle instance, SurfaceHandle &handle);


    //============================ Device ============================
    SWARM_HANDLE(PhysicalDevice);
    SWARM_HANDLE(Device);
    struct DeviceCreateInfo
    {
        bool isDiscreteGPURequired{false};
    };

    DeviceHandle CreateDevice(InstanceHandle instance, SurfaceHandle surface, const DeviceCreateInfo& deviceCreateInfo);
    void DestroyDevice(DeviceHandle &handle);


    //============================ Swapchain ============================
    SWARM_HANDLE(Swapchain);
    struct SwapchainCreateInfo
    {

    };

    SwapchainHandle CreateSwapchain(DeviceHandle device, const SwapchainCreateInfo &swapchainCreateInfo);
    void DestroySwapchain(DeviceHandle device, SwapchainHandle &handle);


    //============================ Renderpass ============================
    SWARM_HANDLE(Renderpass);
    struct RenderpassCreateInfo
    {

    };
    RenderpassHandle CreateRenderpass(DeviceHandle device, SwapchainHandle swapchain, const RenderpassCreateInfo &renderpassCreateInfo);
    void DestroyRenderpass(DeviceHandle device, RenderpassHandle &handle);


    //============================ Shader ============================
    SWARM_HANDLE(Shader);
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


    SWARM_HANDLE(DescriptorSetlayout);
    DescriptorSetlayoutHandle CreateDescriptorSetlayout(DeviceHandle device, DescriptorSetLayoutBinding* bindings, unsigned int bindingCount);
    void DestroyDescriptorSetlayout(DeviceHandle device, DescriptorSetlayoutHandle &handle);


    //============================ Pipeline ============================
    SWARM_HANDLE(Pipeline);

    struct PipelineCreateInfo
    {
        ShaderHandle vertexShader;
        ShaderHandle fragmentShader;
        RenderpassHandle renderpass;
        DescriptorSetlayoutHandle descriptoSetLayout;
    };
    PipelineHandle CreatePipeline(DeviceHandle device, const PipelineCreateInfo &pipelineCreateInfo);
    void DestroyPipeline(DeviceHandle device, PipelineHandle &handle);
}
