#include "vkinstance.h"

namespace swarm
{
    InstanceHandle CreateInstance(const InstanceCreateInfo &instanceCreateInfo)
    {
        assert(g_SwarmLibrary.isInitialized);

        vkb::InstanceBuilder builder{};

        if (instanceCreateInfo.applicationName)
            builder.set_app_name(instanceCreateInfo.applicationName);

        if (instanceCreateInfo.applicationVersion)
            builder.set_app_version(instanceCreateInfo.applicationVersion);

        if (instanceCreateInfo.isDebug)
        {
            builder.request_validation_layers();
            builder.use_default_debug_messenger();
        }

        const auto builderResult = builder.build();

        if (!builderResult.has_value())
            return nullptr;

        Instance_T* instance = SWARM_NEW<Instance_T>();
        instance->instance = builderResult.value();
        return instance;
    }

    void DestroyInstance(InstanceHandle &handle)
    {
        assert(g_SwarmLibrary.isInitialized);
        assert(handle != nullptr);

        vkb::destroy_instance(handle->instance);

        SWARM_DELETE(handle);
    }
}