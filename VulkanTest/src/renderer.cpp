#include "renderer.h"
#include "Assets/Model.h"
#include "Assets/Scene.h"
#include "Assets/Texture.h"
#include "Assets/UniformBuffer.h"
#include "Utilities/Glm.h"
#include "Vulkan/Buffer.h"
#include "Vulkan/BufferUtil.h"
#include "Vulkan/Image.h"
#include "Vulkan/ImageMemoryBarrier.h"
#include "Vulkan/ImageView.h"
#include "Vulkan/PipelineLayout.h"
#include "Vulkan/SingleTimeCommands.h"
#include "Vulkan/SwapChain.h"
#include <chrono>
#include <iostream>
#include <numeric>


Renderer::Renderer(const vk::WindowConfig& windowConfig, const VkPresentModeKHR presentMode, const bool enableValidationLayers) :
	vk::Application(windowConfig, presentMode, enableValidationLayers)
{
}

Renderer::~Renderer()
{
	Application::DeleteSwapChain();
	scene_.reset();
}

void Renderer::OnDeviceSet()
{
	Application::OnDeviceSet();

	LoadScene();
}


Assets::UniformBufferObject Renderer::GetUniformBufferObject(const VkExtent2D extent) const
{
	Assets::UniformBufferObject ubo = {};
	ubo.model = glm::mat4(1.0f);
	ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.proj = glm::perspective(glm::radians(45.0f), extent.width / (float)extent.height, 0.1f, 10.0f);
	ubo.proj[1][1] *= -1;       // flip the y-axis

	return ubo;
}

void Renderer::LoadScene()
{
	Assets::Model model = Assets::Model::LoadModel("./models/viking_room.obj");
	std::vector<Assets::Model> models{ model };
	std::vector<Assets::Texture> textures;
	textures.push_back(Assets::Texture::LoadTexture("./textures/viking_room.png", vk::SamplerConfig()));

	scene_.reset(new Assets::Scene(CommandPool(), std::move(models), std::move(textures)));
}

