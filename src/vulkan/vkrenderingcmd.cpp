#include <swarm/swarm.h>
#include <swarm_internal.h>

#include "vkdevice.h"
#include "vksynchronisation.h"
#include "vkswapchain.h"
#include "vkcommandbuffer.h"
#include "vkrenderpass.h"
#include "vkframebuffer.h"

#include <vulkan/vulkan.h>


namespace swarm
{
    unsigned int CmdBeginFrame(CmdBeginFrameInfo &info)
    {
        vkWaitForFences(info.device->device, 1, &info.inFlightFence->fence, VK_TRUE, UINT64_MAX);
        unsigned int imageIndex = 0;
        vkAcquireNextImageKHR(info.device->device, info.swapchain->swapchain, UINT64_MAX, info.imageAvailableSemaphore->semaphore, nullptr, &imageIndex);

        vkResetFences(info.device->device, 1, &info.inFlightFence->fence);
        vkResetCommandBuffer(info.commandBuffer->commandBuffer, 0);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(info.commandBuffer->commandBuffer, &beginInfo) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to begin recording command buffer!");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = info.renderpass->renderPass;
        renderPassInfo.framebuffer = info.framebuffer->framebuffers[imageIndex];
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = info.swapchain->swapchain.extent;

        renderPassInfo.clearValueCount = info.renderpass->clearValues.size();
        renderPassInfo.pClearValues = info.renderpass->clearValues.data();

        vkCmdBeginRenderPass(info.commandBuffer->commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        return imageIndex;
    }

    void CmdEndFrame(CmdEndFrameInfo& info)
    {
        vkCmdEndRenderPass(info.commandBuffer->commandBuffer);

        if (vkEndCommandBuffer(info.commandBuffer->commandBuffer) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to record command buffer!");
        }
    }

    void CmdSubmitFrame(CmdSubmitInfo& info)
    {
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = {info.imageAvailableSemaphore->semaphore};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &info.commandBuffer->commandBuffer;

        VkSemaphore signalSemaphores[] = {info.renderFinishedSemaphore[info.imageIndex]->semaphore};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if (vkQueueSubmit(info.device->device.get_queue(vkb::QueueType::graphics).value(), 1, &submitInfo,
                          info.inFlightFence->fence) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to submit draw command buffer!");
        }

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = {info.swapchain->swapchain};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;

        presentInfo.pImageIndices = &info.imageIndex;

        vkQueuePresentKHR(info.device->device.get_queue(vkb::QueueType::present).value(), &presentInfo);
    }
}
