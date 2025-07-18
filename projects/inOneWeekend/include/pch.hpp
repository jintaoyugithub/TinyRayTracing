#pragma once

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <memory>
#include <vector>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

using vec2 = glm::vec2;
using vec3 = glm::vec3;

constexpr int imgWidth = 256;
constexpr float imgRatio = 16.0 / 9.0;
constexpr float viewportWidth = 2.0f; // in meter
