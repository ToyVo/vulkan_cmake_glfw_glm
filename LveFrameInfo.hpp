#ifndef LVE__LVEFRAMEINFO_HPP_
#define LVE__LVEFRAMEINFO_HPP_

#include "LveCamera.hpp"

#include <vulkan/vulkan.h>
#include "LveGameObject.hpp"

namespace lve {
  struct FrameInfo {
    int frameIndex;
    float frameTime;
    VkCommandBuffer commandBuffer;
    LveCamera &camera;
    VkDescriptorSet globalDescriptorSet;
    LveGameObject::Map &gameObjects;
  };
}// namespace lve

#endif//LVE__LVEFRAMEINFO_HPP_
