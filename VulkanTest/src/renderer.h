#pragma once
#include "Application.h"

class Renderer final: public vk::Application
{
public:

	VULKAN_NON_COPIABLE(Renderer);

	Renderer(const vk::WindowConfig& windowConfig, VkPresentModeKHR presentMode, bool enableValidationLayers);
	~Renderer();

protected:
	void OnDeviceSet() override;
	const Assets::Scene& GetScene() const override { return *scene_; }
	Assets::UniformBufferObject GetUniformBufferObject(VkExtent2D extent) const override;

private:
	void LoadScene();

	std::unique_ptr<const Assets::Scene> scene_;
};
