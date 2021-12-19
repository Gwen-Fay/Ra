/**
 *
 * This is a simple example RenderSystem which manages a pipeline
 *
 */

#pragma once

#include <libgaem/core.hpp>
#include <libgaem/render/device.hpp>
#include <libgaem/render/model.hpp>
#include <libgaem/render/pipeline.hpp>

#include <memory>
#include <vector>

namespace gaem {

class RenderSystem {
public:
  RenderSystem(GaemDevice &device, VkRenderPass renderPass);
  virtual ~RenderSystem();
  RenderSystem(const RenderSystem &) = delete;
  RenderSystem &operator=(const RenderSystem &) = delete;
  void renderModels(VkCommandBuffer commandBuffer,
                    std::unique_ptr<GaemModel> &model);

private:
  void createPipelineLayout();
  void createPipeline(VkRenderPass renderPass);

  GaemDevice &device;

  std::unique_ptr<GaemPipeline> pipeline;
  VkPipelineLayout pipelineLayout;
};

} // namespace gaem
