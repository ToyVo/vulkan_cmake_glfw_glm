#ifndef LVE_LVEFRAMEINFO_HPP
#define LVE_LVEFRAMEINFO_HPP

#include "LveCamera.hpp"

#include "LveGameObject.hpp"
#include <vulkan/vulkan.h>

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

#endif//LVE_LVEFRAMEINFO_HPP
