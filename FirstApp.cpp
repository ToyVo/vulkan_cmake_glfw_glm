#include "FirstApp.hpp"
#include "KeyboardMovementController.hpp"
#include "LveBuffer.hpp"
#include "LveCamera.hpp"
#include "SimpleRenderSystem.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <array>
#include <chrono>

namespace lve {

  struct GlobalUbo {
    glm::mat4 projectionView{1.f};
    glm::vec3 lightDirection = glm::normalize(glm::vec3(1.f, -3.f, -1.f));
  };

  FirstApp::FirstApp() {
    loadGameObjects();
  }

  FirstApp::~FirstApp() {
  }

  void FirstApp::run() {

    std::vector<std::unique_ptr<LveBuffer>> uboBuffers(LveSwapChain::MAX_FRAMES_IN_FLIGHT);
    for (int i = 0; i < uboBuffers.size(); i++) {
      uboBuffers[i] = std::make_unique<LveBuffer>(
          lveDevice,
          sizeof(GlobalUbo),
          1,
          VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
          VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
      );
      uboBuffers[i]->map();
    }

    SimpleRenderSystem simpleRenderSystem{
        lveDevice, lveRenderer.getSwapChainRenderPass()
    };
    LveCamera camera{};
    camera.setViewTarget(glm::vec3{-1.f, -2.f, -2.f}, glm::vec3{0.f, 0.f, 2.5f});

    auto viewerObject = LveGameObject::createGameObject();
    KeyboardMovementController cameraController{};

    auto currentTime = std::chrono::high_resolution_clock::now();

        while (!lveWindow.shouldClose()) {
            glfwPollEvents();

            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime =
                    std::chrono::duration<float, std::chrono::seconds::period>(newTime -
                                                                               currentTime)
                            .count();
            currentTime = newTime;

            cameraController.moveInPlaneXZ(lveWindow.getGLFWwindow(), frameTime,
                                           viewerObject);
            camera.setViewYXZ(viewerObject.transform.translation,
                              viewerObject.transform.rotation);

            float aspect = lveRenderer.getAspectRatio();
            camera.setPerspectiveProjection(glm::radians(50.f), aspect, .1f, 10.f);

            if (auto commandBuffer = lveRenderer.beginFrame()) {
              int frameIndex = lveRenderer.getFrameIndex();
              FrameInfo frameInfo{frameIndex, frameTime, commandBuffer, camera};

              // update
              GlobalUbo ubo{};
              ubo.projectionView = camera.getProjection() * camera.getView();
              uboBuffers[frameIndex]->writeToBuffer(&ubo);

              // render
              lveRenderer.beginSwapChainRenderPass(commandBuffer);
              simpleRenderSystem.renderGameObjects(frameInfo, gameObjects);
              lveRenderer.endSwapChainRenderPass(commandBuffer);
              lveRenderer.endFrame();
            }
        }

        vkDeviceWaitIdle(lveDevice.device());
    }

    void FirstApp::loadGameObjects() {
      std::shared_ptr<LveModel> lveModel = LveModel::createModelFromFile(lveDevice, "models/flat_vase.obj");
      auto flatVase = LveGameObject::createGameObject();
      flatVase.model = lveModel;
      flatVase.transform.translation = {-.5f, .5f, 2.5f};
      flatVase.transform.scale = {3.f, 1.5f, 3.f};
      gameObjects.push_back(std::move(flatVase));

      lveModel = LveModel::createModelFromFile(lveDevice, "models/smooth_vase.obj");
      auto smoothVase = LveGameObject::createGameObject();
      smoothVase.model = lveModel;
      smoothVase.transform.translation = {.5f, .5f, 2.5f};
      smoothVase.transform.scale = {3.f, 1.5f, 3.f};
      gameObjects.push_back(std::move(smoothVase));
    }

}// namespace lve
