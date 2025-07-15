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
