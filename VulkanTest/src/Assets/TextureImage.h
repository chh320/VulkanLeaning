#pragma once

#include <memory>

namespace vk
{
	class CommandPool;
	class DeviceMemory;
	class Image;
	class ImageView;
	class Sampler;
}

namespace Assets
{
	class Texture;

	class TextureImage final
	{
	public:

		TextureImage(const TextureImage&) = delete;
		TextureImage(TextureImage&&) = delete;
		TextureImage& operator = (const TextureImage&) = delete;
		TextureImage& operator = (TextureImage&&) = delete;

		TextureImage(vk::CommandPool& commandPool, const Texture& texture);
		~TextureImage();

		const vk::ImageView& ImageView() const { return *imageView_; }
		const vk::Sampler& Sampler() const { return *sampler_; }

	private:

		std::unique_ptr<vk::Image> image_;
		std::unique_ptr<vk::DeviceMemory> imageMemory_;
		std::unique_ptr<vk::ImageView> imageView_;
		std::unique_ptr<vk::Sampler> sampler_;
	};

}
