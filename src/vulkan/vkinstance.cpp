#include "vkinstance.h"
#include "vkinstance.h"

namespace swarm
{
    InstanceHandle CreateInstance(InstanceCreateInfo &instanceCreateInfo)
    {
        assert(g_SwarmLibrary.isInitialized);

        vkb::InstanceBuilder builder{};

        if (instanceCreateInfo.applicationName)
            builder.set_app_name(instanceCreateInfo.applicationName);

        if (instanceCreateInfo.applicationVersion)
            builder.set_app_version(instanceCreateInfo.applicationVersion);

        if (instanceCreateInfo.isDebug)
            builder.request_validation_layers();

        const auto builderResult = builder.build();

        if (!builderResult.has_value())
            return nullptr;

        Instance_T* instance = static_cast<Instance_T *>(g_SwarmLibrary.allocFn(sizeof(Instance_T)));
        instance->instance = builderResult.value();
        return instance;
    }

    void DestroyInstance(InstanceHandle handle)
    {
        assert(g_SwarmLibrary.isInitialized);
        assert(handle != nullptr);

        vkb::destroy_instance(handle->instance);

        g_SwarmLibrary.freeFn(handle);
    }
}