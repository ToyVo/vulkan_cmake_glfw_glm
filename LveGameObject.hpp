#ifndef LVE_LVEGAMEOBJECT_HPP
#define LVE_LVEGAMEOBJECT_HPP

#include "LveModel.hpp"
#include <memory>

namespace lve {
  struct Transform2dComponent {
    glm::vec2 translation{};
    glm::vec2 scale{1.f, 1.f};
    float rotation;

    glm::mat2 mat2() {
      const float sin = glm::sin(rotation);
      const float cos = glm::cos(rotation);
      const glm::mat2 rotationMatrix = {{cos,  sin},
                                        {-sin, cos}};
      const glm::mat2 scaleMatrix{{scale.x, .0f},
                                  {0.f,     scale.y}};
      return rotationMatrix * scaleMatrix;
    }
  };

  class LveGameObject {
  public:
    using id_t = unsigned int;

    static LveGameObject createGameObject() {
      static id_t currentId = 0;
      return {currentId++};
    }

    LveGameObject(const LveGameObject &) = delete;

    LveGameObject &operator=(const LveGameObject &) = delete;

    LveGameObject(LveGameObject &&) = default;

    LveGameObject &operator=(LveGameObject &&) = default;

    id_t getId() {
      return id;
    }

    std::shared_ptr<LveModel> model{};
    glm::vec3 color{};
    Transform2dComponent transform2d{};

  private:
    LveGameObject(id_t objId) : id(objId) {
    }

    id_t id;
  };
}// namespace lve

#endif// LVE_LVEGAMEOBJECT_HPP
