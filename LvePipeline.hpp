#ifndef LVE_LVEPIPELINE_HPP
#define LVE_LVEPIPELINE_HPP

#include <string>
#include <vector>

namespace lve {
  class LvePipeline {
  public:
    LvePipeline(const std::string &vertFilePath, const std::string &fragFilePath);

  private:
    static std::vector<char> readFile(const std::string &filePath);

    void createGraphicsPipeline(const std::string &vertFilePath, const std::string &fragFilePath);
  };
}// namespace lve

#endif//LVE_LVEPIPELINE_HPP
