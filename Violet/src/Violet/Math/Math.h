#pragma once
#include <glm/glm.hpp>

namespace Violet::Math {

	VIOLET_API bool decomposeTransformationMatrix(const glm::mat4& transform, glm::vec3& outTranslation, glm::vec3& outRotation, glm::vec3& outScale);

}