#pragma once

#include "engine/graphics/AerialPerspective.hpp"
#include "engine/graphics/Subpass.hpp"
#include "engine/graphics/Sun.hpp"
#include "engine/graphics/Atmosphere.hpp"
#include "engine/graphics/vulkan/Swapchain.hpp"
#include <engine/graphics/Common.hpp>
#include <engine/graphics/vulkan/Shader.hpp>
#include <engine/graphics/vulkan/CommandPool.hpp>
#include <engine/objects/Model.hpp>
#include <engine/graphics/Camera.hpp>
#include <vulkan/vulkan_core.h>
#include <engine/graphics/Precomputer.hpp>

namespace en
{
	class AerialPerspectiveRenderer : public Subpass
	{
	public:
		AerialPerspectiveRenderer(
			size_t max_concurrent,
			uint32_t width,
			uint32_t height,
			const Camera* camera,
			const Sun &sun,
			vk::Swapchain &swapchain,
			Atmosphere &atmosphere,
			Precomputer &precomp,
			AerialPerspective &aerial );

		void Render(VkQueue queue, size_t imageIndx) const;
		void Precompute() const;
		void Destroy();

		void ResizeFrame(uint32_t width, uint32_t height);

		void CreatePipeline(size_t subpass, VkRenderPass renderpass) override;
		void RecordFrameCommandBuffer(VkCommandBuffer buf, size_t frame_indx) override;

		void AllocateResources(
			std::vector<VkImageView> &colorImageViews,
			std::vector<VkImageView> &depthImageViews,
			std::vector<VkImageView> &swapchainImageViews,
			std::vector<VkFramebuffer> &framebuffers,
			VkRenderPass renderpass) override;

	std::pair<VkSubpassDescription, VkSubpassContents> GetSubpass(
		size_t subpass_indx,
		const VkAttachmentReference &color_attachment,
		const VkAttachmentReference &depth_attachment,
		const VkAttachmentReference &swapchain_attachment) override;

	private:
		uint32_t m_FrameWidth;
		uint32_t m_FrameHeight;

		size_t m_MaxConcurrent;

		const Camera* m_Camera;
		const Sun &m_Sun;

		vk::Shader m_VertShader;
		vk::Shader m_FragShader;

		std::array<VkAttachmentReference, 2> m_InputAttachmentRefs;
		VkDescriptorPool m_DescriptorPool;
		VkDescriptorSetLayout m_InputAttachmentDSL;
		std::vector<VkDescriptorSet> m_ColorInputAttachmentDSs;
		std::vector<VkDescriptorSet> m_DepthInputAttachmentDSs;

		VkPipelineLayout m_PipelineLayout;
		VkPipeline m_GraphicsPipeline;

		vk::CommandPool m_GraphicsCommandPool;
		std::vector<VkCommandBuffer> m_GraphicsCommandBuffers;

		Atmosphere &m_Atmosphere;
		Precomputer &m_Precomp;
		AerialPerspective &m_Aerial;

		void CreatePipelineLayout(VkDevice device);
		void CreateCommandBuffers();
	};
}
