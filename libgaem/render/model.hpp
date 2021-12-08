#pragma once

#include <libgaem/render/device.hpp>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vector>

namespace gaem {
class GaemModel {
public:
  struct Vertex {
    glm::vec2 position;
    static std::vector<VkVertexInputBindingDescription>
    getBindingDescriptions();
    static std::vector<VkVertexInputAttributeDescription>
    getAtributeDescriptions();
  };

  GaemModel(GaemDevice &gaemDevice, const std::vector<Vertex> &vertices);
  ~GaemModel();
  GaemModel(const GaemModel &) = delete;
  GaemModel &operator=(const GaemModel &) = delete;

  void bind(VkCommandBuffer commandBuffer);
  void draw(VkCommandBuffer commandBuffer);

private:
  GaemDevice &gaemDevice;
  VkBuffer vertexBuffer;
  VkDeviceMemory vertexBufferMemory;
  uint32_t vertexCount;
  void createVertexBuffers(const std::vector<Vertex> &vertices);
};
} // namespace gaem
