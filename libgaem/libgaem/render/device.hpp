/**
 *  GaemDevice class abstracts all vulkan code relating to the graphics card.
 */

#pragma once

#include <libgaem/render/window.hpp>

#include <string>
#include <vector>

// turn off debugMessenger in release
#ifdef DEBUG
#define SETUP_DEBUG_MESSENGER(...) setupDebugMessenger(__VA_ARGS__)
#else
#define SETUP_DEBUG_MESSENGER(...)
#endif

namespace gaem {

struct SwapChainSupportDetails {
  VkSurfaceCapabilitiesKHR capabilities;
  std::vector<VkSurfaceFormatKHR> formats;
  std::vector<VkPresentModeKHR> presentModes;
};

struct QueueFamilyIndices {
  uint32_t graphicsFamily;
  uint32_t presentFamily;
  uint32_t computeFamily;
  bool graphicsFamilyHasValue = false;
  bool presentFamilyHasValue = false;
  bool computeFamilyHasValue = false;
  bool isComplete() { return graphicsFamilyHasValue && presentFamilyHasValue; }
};

class GaemDevice {
public:
#ifdef NDEBUG
  const bool enableValidationLayers = false;
#else
  const bool enableValidationLayers = true;
#endif

  GaemDevice(GaemWindow &gaemWindow);
  ~GaemDevice();

  // Not copyable or movable
  GaemDevice(const GaemDevice &) = delete;
  void operator=(const GaemDevice &) = delete;
  GaemDevice(GaemDevice &&) = delete;
  GaemDevice &operator=(GaemDevice &&) = delete;

  VkCommandPool getGraphicsCommandPool() { return graphicsCommandPool; }
  VkCommandPool getComputeCommandPool() { return computeCommandPool; }
  VkDevice device() { return device_; }
  VkSurfaceKHR surface() { return surface_; }
  VkQueue graphicsQueue() { return graphicsQueue_; }
  VkQueue presentQueue() { return presentQueue_; }
  VkQueue computeQueue() { return computeQueue_; }

  SwapChainSupportDetails getSwapChainSupport() {
    return querySwapChainSupport(physicalDevice);
  }
  uint32_t findMemoryType(uint32_t typeFilter,
                          VkMemoryPropertyFlags properties);
  QueueFamilyIndices findPhysicalQueueFamilies() {
    return findQueueFamilies(physicalDevice);
  }
  VkFormat findSupportedFormat(const std::vector<VkFormat> &candidates,
                               VkImageTiling tiling,
                               VkFormatFeatureFlags features);

  // Buffer Helper Functions
  void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
                    VkMemoryPropertyFlags properties, VkBuffer &buffer,
                    VkDeviceMemory &bufferMemory);
  VkCommandBuffer beginSingleTimeCommands(VkCommandPool &commandPool);
  void endSingleTimeCommands(VkCommandBuffer commandBuffer,
                             VkCommandPool &commandPool);
  void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size,
                  VkCommandPool &commandPool);
  void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width,
                         uint32_t height, uint32_t layerCount,
                         VkCommandPool &commandPool);

  void createImageWithInfo(const VkImageCreateInfo &imageInfo,
                           VkMemoryPropertyFlags properties, VkImage &image,
                           VkDeviceMemory &imageMemory);

  VkPhysicalDeviceProperties properties;

private:
  void createInstance();
  void setupDebugMessenger();
  void createSurface();
  void pickPhysicalDevice();
  void createLogicalDevice();
  void createCommandPools();

  // helper functions
  bool isDeviceSuitable(VkPhysicalDevice device);
  std::vector<const char *> getRequiredExtensions();
  bool checkValidationLayerSupport();
  QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
  void populateDebugMessengerCreateInfo(
      VkDebugUtilsMessengerCreateInfoEXT &createInfo);
  void hasGflwRequiredInstanceExtensions();
  bool checkDeviceExtensionSupport(VkPhysicalDevice device);
  SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

  VkInstance instance;
  VkDebugUtilsMessengerEXT debugMessenger;
  VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
  GaemWindow &gaemWindow;
  VkCommandPool graphicsCommandPool;
  VkCommandPool computeCommandPool;

  VkDevice device_;
  VkSurfaceKHR surface_;
  VkQueue graphicsQueue_;
  VkQueue presentQueue_;
  VkQueue computeQueue_;

  const std::vector<const char *> validationLayers = {
      "VK_LAYER_KHRONOS_validation"};
  const std::vector<const char *> deviceExtensions = {
      VK_KHR_SWAPCHAIN_EXTENSION_NAME};
};

} // namespace gaem
