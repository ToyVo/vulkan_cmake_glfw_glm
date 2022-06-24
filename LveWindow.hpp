#ifndef LVE_LVEWINDOW_HPP
#define LVE_LVEWINDOW_HPP

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <string>

namespace lve {

  class LveWindow {
  public:
    LveWindow(int w, int h, std::string name);

    ~LveWindow();

    LveWindow(const LveWindow &) = delete;

    LveWindow &operator=(const LveWindow &) = delete;

    bool shouldClose();

  private:
    void initWindow();

    const int width;
    const int height;

    std::string windowName;
    GLFWwindow *window;
  };

}// namespace lve

#endif//LVE_LVEWINDOW_HPP
