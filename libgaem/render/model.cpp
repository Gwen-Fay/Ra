#include <libgaem/core.hpp>
#include <libgaem/render/model.hpp>

#include <cassert>
#include <cstring>

namespace gaem {

GaemModel::GaemModel(GaemDevice &gaemDevice,
                     const std::vector<Vertex> &vertices)
    : gaemDevice{gaemDevice} {

  createVertexBuffers(vertices);
}

GaemModel::~GaemModel() {
  vkDestroyBuffer(gaemDevice.device(), vertexBuffer, nullptr);
  vkFreeMemory(gaemDevice.device(), vertexBufferMemory, nullptr);
}

void GaemModel::createVertexBuffers(const std::vector<Vertex> &vertices) {
  vertexCount = static_cast<uint32_t>(vertices.size());
  assert(vertexCount >= 3 && "vertex count must be atleast 3");
  VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;
  gaemDevice.createBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                          VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                              VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                          vertexBuffer, vertexBufferMemory);

  void *data;
  vkMapMemory(gaemDevice.device(), vertexBufferMemory, 0, bufferSize, 0, &data);
  memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
  vkUnmapMemory(gaemDevice.device(), vertexBufferMemory);
}

void GaemModel::draw(VkCommandBuffer commandBuffer) {
  vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
}

void GaemModel::bind(VkCommandBuffer commandBuffer) {
  VkBuffer buffers[] = {vertexBuffer};
  VkDeviceSize offsets[] = {0};
  vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
}

std::vector<VkVertexInputBindingDescription>
GaemModel::Vertex::getBindingDescriptions() {
  std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
  bindingDescriptions[0].binding = 0;
  bindingDescriptions[0].stride = sizeof(Vertex);
  bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
  return bindingDescriptions;
}

std::vector<VkVertexInputAttributeDescription>
GaemModel::Vertex::getAtributeDescriptions() {
  std::vector<VkVertexInputAttributeDescription> attributeDescriptions(1);
  attributeDescriptions[0].binding = 0;
  attributeDescriptions[0].location = 0;
  attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
  attributeDescriptions[0].offset = 0;
  return attributeDescriptions;
}

} // namespace gaem
