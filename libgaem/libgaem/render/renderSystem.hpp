/**
 *
 * This is a simple example GaemRenderSystem which manages a pipeline
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

class GaemRenderSystem {
public:
  GaemRenderSystem(GaemDevice &device, VkRenderPass renderPass);
  virtual ~GaemRenderSystem();
  GaemRenderSystem(const GaemRenderSystem &) = delete;
  GaemRenderSystem &operator=(const GaemRenderSystem &) = delete;
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
