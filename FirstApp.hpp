#ifndef LVE_FIRSTAPP_HPP
#define LVE_FIRSTAPP_HPP

#include "LveDevice.hpp"
#include "LveGameObject.hpp"
#include "LveRenderer.hpp"
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
    void loadGameObjects();

    void siserpinski(
        std::vector<LveModel::Vertex> &vertices, int depth, glm::vec2 left, glm::vec2 right, glm::vec2 top
    );

    LveWindow lveWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
    LveDevice lveDevice{lveWindow};
    LveRenderer lveRenderer{lveWindow, lveDevice};
    std::vector<LveGameObject> gameObjects;
  };

}// namespace lve

#endif// LVE_FIRSTAPP_HPP
