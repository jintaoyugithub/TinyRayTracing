#pragma once

#include <random>

inline double randDoule(double min, double max) {
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
