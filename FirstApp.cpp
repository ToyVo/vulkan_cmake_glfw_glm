#include "FirstApp.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <array>
#include <stdexcept>

namespace lve {

  struct SimplePushConstantData {
    glm::mat2 transform{1.0f};
    glm::vec2 offset;
    alignas(16) glm::vec3 color;
  };

  FirstApp::FirstApp() {
    loadGameObjects();
    createPipelineLayout();
    recreateSwapChain();
    createCommandBuffers();
  }

  FirstApp::~FirstApp() {
    vkDestroyPipelineLayout(lveDevice.device(), pipelineLayout, nullptr);
  }

  void FirstApp::run() {
    while (!lveWindow.shouldClose()) {
      glfwPollEvents();
      drawFrame();
    }

    vkDeviceWaitIdle(lveDevice.device());
  }

  void FirstApp::loadGameObjects() {
    std::vector<LveModel::Vertex> vertices{};
    siserpinski(vertices, 1, {-0.5f, 0.5f}, {0.5f, 0.5f}, {0.0f, -0.5f});
    auto lveModel = std::make_shared<LveModel>(lveDevice, vertices);

    std::vector<glm::vec3> colors{{204.f / 255.f, 36.f / 255.f,  29.f / 255.f},
                                  {184.f / 255.f, 187.f / 255.f, 38.f / 255.f},
                                  {250.f / 255.f, 189.f / 255.f, 47.f / 255.f},
                                  {131.f / 255.f, 165.f / 255.f, 152.f / 255.f},
                                  {211.f / 255.f, 134.f / 255.f, 155.f / 255.f},
                                  {142.f / 255.f, 192.f / 255.f, 124.f / 255.f},
                                  {254.f / 255.f, 128.f / 255.f, 25.f / 255.f}};

    float num = colors.size() * colors.size();
    for (int i = 0; i <= num; i++) {
      auto triangle = LveGameObject::createGameObject();
      triangle.model = lveModel;
      triangle.color = colors[i % colors.size()];
      triangle.transform2d.scale = glm::vec2(.5f) + i * 1.f / num;
      triangle.transform2d.rotation = i * glm::pi<float>() * 1.f / num;

      gameObjects.push_back(std::move(triangle));
    }
  }

  void FirstApp::createPipelineLayout() {

    VkPushConstantRange pushConstantRange;
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(SimplePushConstantData);

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
    if (vkCreatePipelineLayout(
        lveDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout
    ) != VK_SUCCESS) {
      throw std::runtime_error("failed to create pipeline layout!");
    }
  }

  void FirstApp::createPipeline() {
    assert(lveSwapChain != nullptr && "Cannot create pipeline before swap chain");
    assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

    PipelineConfigInfo pipelineConfig{};
    LvePipeline::defaultPipelineConfigInfo(pipelineConfig);
    pipelineConfig.renderPass = lveSwapChain->getRenderPass();
    pipelineConfig.pipelineLayout = pipelineLayout;
    lvePipeline = std::make_unique<LvePipeline>(
        lveDevice, "shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv", pipelineConfig
    );
  }

  void FirstApp::recreateSwapChain() {
    auto extent = lveWindow.getExtent();
    while (extent.width == 0 || extent.height == 0) {
      extent = lveWindow.getExtent();
      glfwWaitEvents();
    }

    vkDeviceWaitIdle(lveDevice.device());

    if (lveSwapChain == nullptr) {
      lveSwapChain = std::make_unique<LveSwapChain>(lveDevice, extent);
    } else {
      lveSwapChain = std::make_unique<LveSwapChain>(
          lveDevice, extent, std::move(lveSwapChain));
      if (lveSwapChain->imageCount() != commandBuffers.size()) {
        freeCommandBuffers();
        createCommandBuffers();
      }
    }

    // if render pass compatible do nothing else
    createPipeline();
  }

  void FirstApp::createCommandBuffers() {
    commandBuffers.resize(lveSwapChain->imageCount());

    VkCommandBufferAllocateInfo allocateInfo{};
    allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocateInfo.commandPool = lveDevice.getCommandPool();
    allocateInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

    if (vkAllocateCommandBuffers(
        lveDevice.device(), &allocateInfo, commandBuffers.data()) != VK_SUCCESS) {
      throw std::runtime_error("failed to allocate command buffers!");
    }
  }

  void FirstApp::freeCommandBuffers() {
    vkFreeCommandBuffers(
        lveDevice.device(),
        lveDevice.getCommandPool(),
        static_cast<uint32_t>(commandBuffers.size()),
        commandBuffers.data());
    commandBuffers.clear();
  }

  void FirstApp::recordCommandBuffer(int imageIndex) {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS) {
      throw std::runtime_error("failed to begin recording command buffer!");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = lveSwapChain->getRenderPass();
    renderPassInfo.framebuffer = lveSwapChain->getFrameBuffer(imageIndex);

    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = lveSwapChain->getSwapChainExtent();

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {40.f / 255.f, 40.f / 255.f, 40.f / 255.f, 1.0f};
    clearValues[1].depthStencil = {1.0f, 0};
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(
        commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE
    );

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(lveSwapChain->getSwapChainExtent().width);
    viewport.height = static_cast<float>(lveSwapChain->getSwapChainExtent().height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    VkRect2D scissor{{0, 0}, lveSwapChain->getSwapChainExtent()};
    vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
    vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

    renderGameObjects(commandBuffers[imageIndex]);

    vkCmdEndRenderPass(commandBuffers[imageIndex]);
    if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS) {
      throw std::runtime_error("failed to record command buffer!");
    }
  }

  void FirstApp::renderGameObjects(VkCommandBuffer commandBuffer) {
    int i = 0;
    for (auto &obj: gameObjects) {
      obj.transform2d.rotation = glm::mod<float>(
          obj.transform2d.rotation + 0.00001f * ++i, 2.f * glm::pi<float>());
    }

    lvePipeline->bind(commandBuffer);

    for (auto &obj: gameObjects) {
      SimplePushConstantData push{};
      push.transform = obj.transform2d.mat2();
      push.offset = obj.transform2d.translation;
      push.color = obj.color;

      vkCmdPushConstants(
          commandBuffer,
          pipelineLayout,
          VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
          0,
          sizeof(SimplePushConstantData),
          &push
      );
      obj.model->bind(commandBuffer);
      obj.model->draw(commandBuffer);
    }
  }

  void FirstApp::drawFrame() {
    uint32_t imageIndex;
    auto result = lveSwapChain->acquireNextImage(&imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
      recreateSwapChain();
      return;
    }

    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
      throw std::runtime_error("failed to acquire swap chain image");
    }

    recordCommandBuffer(imageIndex);
    result = lveSwapChain->submitCommandBuffers(
        &commandBuffers[imageIndex], &imageIndex
    );
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || lveWindow.wasWindowResized()) {
      lveWindow.resetWindowResizedFlag();
      recreateSwapChain();
      return;
    }
    if (result != VK_SUCCESS) {
      throw std::runtime_error("failed to present swap chain image");
    }
  }

  void FirstApp::siserpinski(
      std::vector<LveModel::Vertex> &vertices, int depth, glm::vec2 left, glm::vec2 right, glm::vec2 top
  ) {
    if (depth <= 0) {
      vertices.push_back({top, {1.0f, 0.0f, 0.0f}});
      vertices.push_back({right, {0.0f, 1.0f, 0.0f}});
      vertices.push_back({left, {0.0f, 0.0f, 1.0f}});
    } else {
      auto leftTop = 0.5f * (left + top);
      auto rightTop = 0.5f * (right + top);
      auto leftRight = 0.5f * (left + right);
      siserpinski(vertices, depth - 1, left, leftRight, leftTop);
      siserpinski(vertices, depth - 1, leftRight, right, rightTop);
      siserpinski(vertices, depth - 1, leftTop, rightTop, top);
    }
  }

}// namespace lve
