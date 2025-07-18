#pragma once

#include <pch.hpp>
#include <random>

inline double randDouble(double min, double max) {
  static std::uniform_real_distribution<double> dist(0.0, 1.0);
  static std::mt19937 generator;
  auto rand = dist(generator);
  return min + (max - min) * rand;
}

inline double clamp(double x, double min, double max) {
  if (x < min)
    return min;
  if (x > max)
    return max;
  return x;
}

inline double gammaCorrection(double linearValue, float gamma = 0.5f) {
  if (linearValue > 0.0) {
    // hardcode gamma = 0.5
    return std::sqrt(linearValue);
  }
  return 0.0;
}

inline vec3 randVec3(double min, double max) {
  return glm::vec3(randDouble(min, max), randDouble(min, max),
                   randDouble(min, max));
}

inline vec3 randUnitVec3() {
  vec3 randVec{0.0f};
  while (true) {
    randVec = randVec3(-1.0, 1.0);
    auto vecLen = glm::length(randVec);
    if (1e-160 < vecLen && vecLen <= 1) {
      randVec = randVec / vecLen;
      break;
    }
  }
  return randVec;
}
