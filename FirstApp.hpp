#ifndef LVE_FIRSTAPP_HPP
#define LVE_FIRSTAPP_HPP

#include "LvePipeline.hpp"
#include "LveWindow.hpp"

namespace lve {

  class FirstApp {
  public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    void run();

  private:
    LveWindow lveWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
    LvePipeline lvePipeline{"shaders/simple_shader.frag.spv", "shaders/simple_shader.vert.spv"};
  };

}// namespace lve

#endif//LVE_FIRSTAPP_HPP
