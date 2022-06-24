#ifndef LVE_SIMPLERENDERSYSTEM_HPP
#define LVE_SIMPLERENDERSYSTEM_HPP

#include "LveCamera.hpp"
#include "LveDevice.hpp"
#include "LveGameObject.hpp"
#include "LvePipeline.hpp"
#include <memory>
#include <vector>

namespace lve {

  class SimpleRenderSystem {
  public:
    SimpleRenderSystem(LveDevice &device, VkRenderPass renderPass);

    ~SimpleRenderSystem();

    SimpleRenderSystem(const SimpleRenderSystem &) = delete;

    SimpleRenderSystem &operator=(const SimpleRenderSystem &) = delete;

    void renderGameObjects(
        VkCommandBuffer commandBuffer, std::vector<LveGameObject> &gameObjects, const LveCamera &camera
    );

  private:
    void createPipelineLayout();

    void createPipeline(VkRenderPass renderPass);

    LveDevice &lveDevice;
    std::unique_ptr<LvePipeline> lvePipeline;
    VkPipelineLayout pipelineLayout;
  };

}// namespace lve

#endif// LVE_SIMPLERENDERSYSTEM_HPP