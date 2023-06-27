#include "Scene.h"
#include "Model.h"
#include "Texture.h"
#include "TextureImage.h"
#include "BufferUtil.h"
#include "ImageView.h"
#include "Sampler.h"
#include <stdexcept>
#include "SingleTimeCommands.h"


namespace Assets {

	Scene::Scene(vk::CommandPool& commandPool, std::vector<Model>&& models, std::vector<Texture>&& textures) :
		models_(std::move(models)),
		textures_(std::move(textures))
	{
		// Concatenate all the models
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		std::vector<glm::uvec2> offsets;

		for (const auto& model : models_)
		{
			// Remember the index, vertex offsets.
			const auto indexOffset = static_cast<uint32_t>(indices.size());
			const auto vertexOffset = static_cast<uint32_t>(vertices.size());

			offsets.emplace_back(indexOffset, vertexOffset);

			// Copy model data one after the other.
			vertices.insert(vertices.end(), model.Vertices().begin(), model.Vertices().end());
			indices.insert(indices.end(), model.Indices().begin(), model.Indices().end());

		}

		//constexpr auto flags = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT;

		vk::BufferUtil::CreateDeviceBuffer(commandPool, "Vertices", VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR , vertices, vertexBuffer_, vertexBufferMemory_);
		vk::BufferUtil::CreateDeviceBuffer(commandPool, "Indices", VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR , indices, indexBuffer_, indexBufferMemory_);
		//vk::BufferUtil::CreateDeviceBuffer(commandPool, "Offsets", flags, offsets, offsetBuffer_, offsetBufferMemory_);

		// Upload all textures
		textureImages_.reserve(textures_.size());
		textureImageViewHandles_.resize(textures_.size());
		textureSamplerHandles_.resize(textures_.size());

		for (size_t i = 0; i != textures_.size(); ++i)
		{
			textureImages_.emplace_back(new TextureImage(commandPool, textures_[i]));
			textureImageViewHandles_[i] = textureImages_[i]->ImageView().Handle();
			textureSamplerHandles_[i] = textureImages_[i]->Sampler().Handle();
		}
	}

	Scene::~Scene()
	{
		textureSamplerHandles_.clear();
		textureImageViewHandles_.clear();
		textureImages_.clear();
		offsetBuffer_.reset();
		offsetBufferMemory_.reset(); // release memory after bound buffer has been destroyed
		indexBuffer_.reset();
		indexBufferMemory_.reset(); // release memory after bound buffer has been destroyed
		vertexBuffer_.reset();
		vertexBufferMemory_.reset(); // release memory after bound buffer has been destroyed
	}

}
