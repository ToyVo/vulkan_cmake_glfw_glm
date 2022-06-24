#ifndef LVE_LIGHTPOINTSYSTEM_HPP
#define LVE_LIGHTPOINTSYSTEM_HPP

#include "LveCamera.hpp"
#include "LveDevice.hpp"
#include "LveFrameInfo.hpp"
#include "LveGameObject.hpp"
#include "LvePipeline.hpp"
#include <memory>
#include <vector>

namespace lve {

  class LightPointSystem {
  public:
    LightPointSystem(LveDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);

    ~LightPointSystem();

    LightPointSystem(const LightPointSystem &) = delete;

    LightPointSystem &operator=(const LightPointSystem &) = delete;

    void render(FrameInfo &frameInfo);

  private:
    void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);

    void createPipeline(VkRenderPass renderPass);

    LveDevice &lveDevice;
    std::unique_ptr<LvePipeline> lvePipeline;
    VkPipelineLayout pipelineLayout;
  };

}// namespace lve

#endif// LVE_LIGHTPOINTSYSTEM_HPP
