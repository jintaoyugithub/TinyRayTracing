#pragma once

#include <pch.hpp>

class Viewport {
public:
  Viewport() = default;
  ~Viewport() = default;

  Viewport(float width, float ratio, vec2 imgRes)
      : m_width(width), m_ratio(ratio) {
    m_height = m_width / m_ratio;
    // makesure the height of viewport is greater than 1.0
    m_height = m_height < 1.0 ? 1.0 : m_height;
    m_deltaU = m_width / imgRes.x;
    m_deltaV = m_height / imgRes.y;
  }

  inline const vec2 pixelSize() const { return vec2(m_deltaU, m_deltaV); }
  inline const vec2 size() const { return vec2(m_width, m_height); }

private:
  float m_width;
  float m_height;
  float m_ratio;
  float m_deltaU;
  float m_deltaV;
};

class Camera {
public:
  Camera() = default;
  ~Camera() = default;

  Camera(const vec3 &eye, float focalLength, const Viewport &viewport)
      : m_eye(eye), m_viewport(viewport), m_focalLength(focalLength) {}

  const Viewport viewport() const { return m_viewport; }
  const vec3 eye() const { return m_eye; }
  const float focalLength() const { return m_focalLength; }

private:
  vec3 m_eye;
  float m_focalLength;
  Viewport m_viewport;
};
