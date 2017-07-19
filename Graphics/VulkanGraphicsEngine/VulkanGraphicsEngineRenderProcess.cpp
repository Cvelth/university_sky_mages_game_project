#include "VulkanGraphicsEngine.hpp"
#include "Exceptions\VulkanExceptions.hpp"

void VulkanGraphicsEngine::initializeRenderProcess() {
	for (size_t i = 0; i < m_commandBuffers.size(); i++) {
		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
		beginInfo.pInheritanceInfo = nullptr; // Optional
		vkBeginCommandBuffer(m_commandBuffers.at(i), &beginInfo);

		VkRenderPassBeginInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_renderPass;
		renderPassInfo.framebuffer = m_framebuffers.at(i);
		renderPassInfo.renderArea.offset = {0, 0};
		renderPassInfo.renderArea.extent = m_swapChain.extent;

		VkClearValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		vkCmdBeginRenderPass(m_commandBuffers.at(i), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		vkCmdBindPipeline(m_commandBuffers.at(i), VK_PIPELINE_BIND_POINT_GRAPHICS, *m_pipeline);
		vkCmdDraw(m_commandBuffers.at(i), 3, 1, 0, 0);

		vkCmdEndRenderPass(m_commandBuffers[i]);
		auto error = vkEndCommandBuffer(m_commandBuffers[i]);
		if (error != VK_SUCCESS)
			throw Exceptions::RenderInitializationException(error);

		glfwPollEvents();
	}
}

void VulkanGraphicsEngine::renderProcess() {
	uint32_t imageIndex;
	vkAcquireNextImageKHR(m_device, *m_swapChain, std::numeric_limits<uint64_t>::max(), m_imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	VkSemaphore waitSemaphores[] = {m_imageAvailableSemaphore};
	VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &m_commandBuffers.at(imageIndex);

	VkSemaphore signalSemaphores[] = {m_renderFinishedSemaphore};
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	auto error = vkQueueSubmit(m_queue, 1, &submitInfo, VK_NULL_HANDLE);
	if (error != VK_SUCCESS)
		throw Exceptions::VulkanRenderException(error);

	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = {*m_swapChain};
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr; // Optional
	vkQueuePresentKHR(m_queue, &presentInfo);

	glfwPollEvents();
}

void VulkanGraphicsEngine::clearRenderProcess() {
	vkDeviceWaitIdle(m_device);
}