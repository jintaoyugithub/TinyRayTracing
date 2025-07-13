#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <iostream>

using vec2 = glm::vec2;
using vec3 = glm::vec3;

constexpr int imgWidth = 256;
constexpr float imgRatio = 16.0 / 9.0;
constexpr float viewportWidth = 2.0f; // in meter

class Ray {
public:
  Ray() = default;
  ~Ray() = default;
  Ray(const vec3 &orig, const vec3 &dir) : m_orig(orig), m_dir(dir) {}

  inline const vec3 origin() const { return m_orig; };
  inline const vec3 direction() const { return m_dir; };

  vec3 at(float t) { return m_orig + m_dir * t; }
  // TODO
  vec3 hit(vec3 hitPoint) {}

private:
  vec3 m_orig;
  vec3 m_dir;
};

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

  inline const vec2 getPixelSize() const { return vec2(m_deltaU, m_deltaV); }

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

  Camera(const Viewport &viewport, float focalLength)
      : m_viewport(viewport), m_focalLength(focalLength) {}

  const Viewport getViewport() const { return m_viewport; }

private:
  Viewport m_viewport;
  float m_focalLength;
};

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

  void render(Camera cam, bool save = true) {
    Viewport vp = cam.getViewport();
    const vec2 pixelSize = vp.getPixelSize();
    std::clog << "pixel size: " << std::endl;
    vec2 startPixelPos = vec2(0.0f, 0.0f) + pixelSize / 2.0f;
    // vec3 eye = vec3(m_width / 2.0f, m_height / 2.0f, 0.0f);
    vec3 eye = vec3(0.0f);

    std::cout << "P3\n" << m_width << " " << m_height << "\n255\n";
    for (int col = 0; col < m_height; col++) {
      std::clog << "\rReamining lines: " << m_height - col - 1 << std::flush;
      for (int row = 0; row < m_width; row++) {
        // we assume the viewport is 1 meter away from the camera
        vec3 pixelPos =
            vec3(startPixelPos + vec2(row, col) * vp.getPixelSize(), -1.0f);

        // std::clog << "pixel pos: " << pixelPos.x << " " << pixelPos.y << " "
        //           << pixelPos.z << std::endl;

        Ray ray(eye, std::move(pixelPos - eye));

        // background color
        auto uintDir = glm::normalize(ray.direction());
        auto a = 0.5f * uintDir.y + 0.5f;
        vec3 color = glm::mix(vec3(0.0f), vec3(0.5, 0.0, 0.0), a);
        // writeColor(std::cout, vec3(uintDir.y));
        writeColor(std::cout, vec3(color));
      }
    }

    std::clog << "\n\rDone.\n";
  }

private:
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

int main() {
  Image img(imgWidth, imgRatio);
  Viewport vp(viewportWidth, img.ratio(), vec2(img.width(), img.height()));
  Camera cam(vp, 1);
  img.render(cam);
}
