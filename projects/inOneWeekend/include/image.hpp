#pragma once

#include <camera.hpp>
#include <pch.hpp>

class Image {
public:
  Image() = default;
  ~Image() = default;
  Image(int width, float ratio) : m_width(width), m_ratio(ratio) {
    // m_height = static_cast<int>((double)m_width / m_ratio);
    m_height = static_cast<int>(m_width / m_ratio);

    data.reserve(m_width * m_height);
  }

  inline const int width() const { return m_width; }
  inline const int height() const { return m_height; }
  inline const float ratio() const {
    if (!m_height || !m_width) {
      std::cerr << "Invalid image size" << std::endl;
      return -1;
    }

    return static_cast<double>(m_width) / m_height;
  }

  void writeColor(std::ostream &out, vec3 color) {
    int r = int(255.999 * color.r);
    int g = int(255.999 * color.g);
    int b = int(255.999 * color.b);

    out << r << " " << g << " " << b << "\n";
  }

private:
  int m_width;
  int m_height;
  float m_ratio;
  std::vector<vec3> data;
};
