#include "FirstApp.hpp"
#include "SimpleRenderSystem.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <array>
#include <stdexcept>

namespace lve {

  FirstApp::FirstApp() {
    loadGameObjects();
  }

  FirstApp::~FirstApp() {
  }

  void FirstApp::run() {
    SimpleRenderSystem simpleRenderSystem{
        lveDevice, lveRenderer.getSwapChainRenderPass()
    };

    while (!lveWindow.shouldClose()) {
      glfwPollEvents();

      if (auto commandBuffer = lveRenderer.beginFrame()) {
        lveRenderer.beginSwapChainRenderPass(commandBuffer);
        simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects);
        lveRenderer.endSwapChainRenderPass(commandBuffer);
        lveRenderer.endFrame();
      }
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
