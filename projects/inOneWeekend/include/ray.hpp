#pragma once

#include <pch.hpp>

class Ray {
public:
  Ray() = default;
  ~Ray() = default;
  Ray(const vec3 &orig, const vec3 &dir) : m_orig(orig), m_dir(dir) {}

  inline const vec3 origin() const { return m_orig; };
  inline const vec3 direction() const { return m_dir; };

  vec3 at(float t) const { return m_orig + m_dir * t; }

private:
  vec3 m_orig;
  vec3 m_dir;
};
