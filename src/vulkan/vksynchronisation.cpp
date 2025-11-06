#include "vksynchronisation.h"

#include <array>
#include <cassert>

#include "vkcommandbuffer.h"
#include "vkdevice.h"
#include "vkframebuffer.h"
#include "vkpipeline.h"
#include "vkrenderpass.h"
#include "vkswapchain.h"

namespace swarm
{
    SemaphoreHandle CreateSemaphore(DeviceHandle device)
    {
        assert(g_SwarmLibrary.isInitialized);
        assert(device);

        VkSemaphoreCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkSemaphore semaphore{VK_NULL_HANDLE};
        if (vkCreateSemaphore(device->device, &createInfo, nullptr, &semaphore) != VK_SUCCESS)
        {
            return nullptr;
        }

        SemaphoreHandle handle = SWARM_NEW<Semaphore_T>();
        handle->semaphore = semaphore;

        return handle;
    }

    FenceHandle CreateFence(DeviceHandle device)
    {
        assert(g_SwarmLibrary.isInitialized);
        assert(device);

        VkFenceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        createInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        VkFence fence{VK_NULL_HANDLE};
        if (vkCreateFence(device->device, &createInfo, nullptr, &fence) != VK_SUCCESS)
        {
            return nullptr;
        }

        FenceHandle handle = SWARM_NEW<Fence_T>();
        handle->fence = fence;

        return handle;
    }

    void DestroySemaphore(DeviceHandle device, SemaphoreHandle &handle)
    {
        assert(g_SwarmLibrary.isInitialized);
        assert(device);
        assert(handle);

        vkDestroySemaphore(device->device, handle->semaphore, nullptr);

        SWARM_DELETE(handle);
    }

    void DestroyFence(DeviceHandle device, FenceHandle &handle)
    {
        assert(g_SwarmLibrary.isInitialized);
        assert(device);
        assert(handle);

        vkDestroyFence(device->device, handle->fence, nullptr);

        SWARM_DELETE(handle);
    }

    void draw(DeviceHandle device, FenceHandle inFlightFence, SemaphoreHandle imageAvailableSemaphore, const std::vector<SemaphoreHandle> &renderFinishedSemaphore, SwapchainHandle swapchain, CommandBufferHandle commandBuffer, RenderpassHandle renderpass, PipelineHandle pipeline, FramebufferHandle framebuffer)
    {
        vkWaitForFences(device->device, 1, &inFlightFence->fence, VK_TRUE, UINT64_MAX);

        uint32_t imageIndex;
        vkAcquireNextImageKHR(device->device, swapchain->swapchain, UINT64_MAX, imageAvailableSemaphore->semaphore, VK_NULL_HANDLE, &imageIndex);

        vkResetFences(device->device, 1, &inFlightFence->fence);
        vkResetCommandBuffer(commandBuffer->commandBuffer, 0);
        {
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            if (vkBeginCommandBuffer(commandBuffer->commandBuffer, &beginInfo) != VK_SUCCESS) {
                throw std::runtime_error("failed to begin recording command buffer!");
            }

            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = renderpass->renderPass;
            renderPassInfo.framebuffer = framebuffer->framebuffers[imageIndex];
            renderPassInfo.renderArea.offset = {0, 0};
            renderPassInfo.renderArea.extent = swapchain->swapchain.extent;

            std::array<VkClearValue, 2> clearValues{};
            clearValues[0].color = {{1.0f, 0.0f, 0.0f, 1.0f}};
            clearValues[1].depthStencil = {1.0f, 0};

            renderPassInfo.clearValueCount = clearValues.size();
            renderPassInfo.pClearValues = clearValues.data();

            vkCmdBeginRenderPass(commandBuffer->commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
            //
            // vkCmdBindPipeline(commandBuffer->commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->pipeline);
            //
            // VkViewport viewport{};
            // viewport.x = 0.0f;
            // viewport.y = 0.0f;
            // viewport.width = static_cast<float>(swapchain->swapchain.extent.width);
            // viewport.height = static_cast<float>(swapchain->swapchain.extent.height);
            // viewport.minDepth = 0.0f;
            // viewport.maxDepth = 1.0f;
            // vkCmdSetViewport(commandBuffer->commandBuffer, 0, 1, &viewport);
            //
            // VkRect2D scissor{};
            // scissor.offset = {0, 0};
            // scissor.extent = swapchain->swapchain.extent;
            // vkCmdSetScissor(commandBuffer->commandBuffer, 0, 1, &scissor);
            //
            // // vkCmdDraw(commandBuffer, 3, 1, 0, 0);
            //
            vkCmdEndRenderPass(commandBuffer->commandBuffer);

            if (vkEndCommandBuffer(commandBuffer->commandBuffer) != VK_SUCCESS) {
                throw std::runtime_error("failed to record command buffer!");
            }
        }

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = {imageAvailableSemaphore->semaphore};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer->commandBuffer;

        VkSemaphore signalSemaphores[] = {renderFinishedSemaphore[imageIndex]->semaphore};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if (vkQueueSubmit(device->device.get_queue(vkb::QueueType::graphics).value(), 1, &submitInfo, inFlightFence->fence) != VK_SUCCESS) {
            throw std::runtime_error("failed to submit draw command buffer!");
        }

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = {swapchain->swapchain};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;

        presentInfo.pImageIndices = &imageIndex;

        vkQueuePresentKHR(device->device.get_queue(vkb::QueueType::present).value(), &presentInfo);
    }

}
