#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace DoNotOpenGL {

inline constexpr unsigned int screenWidth = 1600;
inline constexpr unsigned int screenHeight = 1200;

inline constexpr unsigned int videoWidth = 3.0f / 5.0f * (float)screenWidth;
inline constexpr unsigned int videoHeight = 3.0f / 5.0f * (float)screenHeight;

} // namespace DoNotOpenGL