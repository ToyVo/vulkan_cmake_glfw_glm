#ifndef LVE_FIRSTAPP_HPP
#define LVE_FIRSTAPP_HPP

#include "LveDevice.hpp"
#include "LveModel.hpp"
#include "LvePipeline.hpp"
#include "LveSwapChain.hpp"
#include "LveWindow.hpp"
#include <memory>
#include <vector>

namespace lve {

  class FirstApp {
  public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    FirstApp();

    ~FirstApp();

    FirstApp(const FirstApp &) = delete;

    FirstApp &operator=(const FirstApp &) = delete;

    void run();

  private:
    void loadModels();

    void createPipelineLayout();

    void createPipeline();

    void createCommandBuffers();

    void freeCommandBuffers();

    void drawFrame();

    void recreateSwapChain();

    void recordCommandBuffer(int imageIndex);

    void siserpinski(
        std::vector<LveModel::Vertex> &vertices, int depth, glm::vec2 left, glm::vec2 right, glm::vec2 top
    );

    LveWindow lveWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
    LveDevice lveDevice{lveWindow};
    std::unique_ptr<LveSwapChain> lveSwapChain;
    std::unique_ptr<LvePipeline> lvePipeline;
    VkPipelineLayout pipelineLayout;
    std::vector<VkCommandBuffer> commandBuffers;
    std::unique_ptr<LveModel> lveModel;
  };

}// namespace lve

#endif// LVE_FIRSTAPP_HPP
