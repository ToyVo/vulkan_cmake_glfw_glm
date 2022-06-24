#ifndef LVE__LVEFRAMEINFO_HPP_
#define LVE__LVEFRAMEINFO_HPP_

#include "LveCamera.hpp"

#include <vulkan/vulkan.h>

namespace lve {
  struct FrameInfo {
    int frameIndex;
    float frameTime;
    VkCommandBuffer commandBuffer;
    LveCamera &camera;
  };
}// namespace lve

#endif//LVE__LVEFRAMEINFO_HPP_
