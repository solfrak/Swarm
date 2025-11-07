#include <swarm/swarm.h>
#include <swarm_internal.h>

#include "vkdevice.h"
#include "vksynchronisation.h"

#include <vulkan/vulkan.h>
namespace swarm
{
    void CmdBeginFrame(CmdBeginFrameInfo& info)
    {
        vkWaitForFences(info.device->device, 1, &info.inFlightFence->fence, VK_TRUE, UINT64_MAX);
        unsigned int imageIndex = 0;
        vkAcquireNextImageKHR(info.device->device, )
    }

    void CmdEndFrame(CmdEndFrameInfo& info)
    {

    }

    void CmdSubmitFrame(CmdSubmitInfo& info)
    {

    }
}