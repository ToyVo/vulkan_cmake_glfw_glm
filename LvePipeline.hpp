#ifndef LVE_LVEPIPELINE_HPP
#define LVE_LVEPIPELINE_HPP

#include "LveDevice.hpp"
#include <string>
#include <vector>
namespace lve {
  struct PiplineConfigInfo {
    VkViewport viewport;
    VkRect2D scissor;
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
    VkPipelineRasterizationStateCreateInfo rasterizationInfo;
    VkPipelineMultisampleStateCreateInfo multisampleInfo;
    VkPipelineColorBlendAttachmentState colorBlendAttachment;
    VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
    VkPipelineLayout pipelineLayout = nullptr;
    VkRenderPass renderPass = nullptr;
    uint32_t subpass = 0;
  };

  class LvePipeline {
  public:
    LvePipeline(
        LveDevice &device,
        const std::string &vertFilePath,
        const std::string &fragFilePath,
        const PiplineConfigInfo &configInfo
    );

    ~LvePipeline();

    LvePipeline(const LvePipeline &) = delete;

    void operator=(const LvePipeline &) = delete;

    void bind(VkCommandBuffer commandBuffer);

    static PiplineConfigInfo defaultPipelineConfigInfo(
        uint32_t width, uint32_t height
    );

  private:
    static std::vector<char> readFile(const std::string &filePath);

    void createGraphicsPipeline(
        const std::string &vertFilePath, const std::string &fragFilePath, const PiplineConfigInfo &configInfo
    );

    void createShaderModule(
        const std::vector<char> &code, VkShaderModule *shaderModule
    );

    LveDevice &lveDevice;
    VkPipeline graphicsPipeline;
    VkShaderModule vertShaderModule;
    VkShaderModule fragShaderModule;
  };
}// namespace lve

#endif// LVE_LVEPIPELINE_HPP
