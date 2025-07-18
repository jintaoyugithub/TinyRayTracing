#pragma once

#include <image.hpp>
#include <material.hpp>
#include <scene.hpp>

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
  double m_deltaU;
  double m_deltaV;
};

struct CameraConfig {
  vec3 eye{vec3(0.0f)};
  float flocalLength{1.0f};
  uint16_t sampLevel{1u};
  uint16_t maxRayBounce{10u};
  SamplingMode mode{SamplingMode::Random};
  Viewport viewport;
};

class Camera {
public:
  Camera() = default;
  ~Camera() = default;
  Camera(const Camera &cam) = delete;
  Camera operator=(const Camera &cam) = delete;

  Camera(const CameraConfig &config) : m_camConfig(config) {}

  inline const Viewport viewport() const { return m_camConfig.viewport; }
  inline const vec3 eye() const { return m_camConfig.eye; }
  inline const float focalLength() const { return m_camConfig.flocalLength; }
  inline const uint16_t sampLevel() const { return m_camConfig.sampLevel; }
  inline const SamplingMode sampMode() const { return m_camConfig.mode; }

public:
  // necessary?
  Ray genRay(vec3 pixelPos, vec2 offset) {
    vec3 realPos = pixelPos + vec3(offset, 0.0f);
    return Ray(m_camConfig.eye, std::move(realPos - m_camConfig.eye));
  }

  // recursively invoked
  vec3 pixelColor(const Ray &ray, const Scene &scene,
                  uint16_t bounceCount) const {
    // achieved the max bounce count
    // if (bounceCount < 0), ERROR: segmentation fault
    if (bounceCount <= 0)
      return vec3(0.0f);

    HitRecord closetRec;
    HitRecord tempRec;
    bool hitAnything = false;
    auto curClosetHit = std::numeric_limits<float>::max();

    for (const auto &obj : scene.primitives()) {
      if (obj->isHitBy(ray, tempRec)) {
        if (tempRec.t < curClosetHit && tempRec.t > 0.0f) {
          curClosetHit = tempRec.t;
          closetRec = tempRec;
        }
        hitAnything = true;
      }
    }

    // todo: wrong
    if (hitAnything) {
      Ray scattered;
      vec3 attenuation;
      closetRec.mat->scatter(ray, closetRec, scattered, attenuation);
      // 0.5 is attenuation
      return attenuation *
             pixelColor(std::move(scattered), scene, bounceCount - 1);
    }

    // it not hit anything, return the background color
    return glm::mix(vec3(1.0f), vec3(0.5f, 0.7f, 1.0f),
                    0.5f * glm::normalize(ray.direction()).y + 0.5f);
  }

  void render(const Scene &scene, Image &img) {
    vec2 pixelSize = m_camConfig.viewport.pixelSize();
    vec2 viewportTopleft = m_camConfig.eye -
                           vec3(0.0, 0.0, -m_camConfig.flocalLength) -
                           vec3(m_camConfig.viewport.size() / 2.0f, 0.0f);
    uint16_t sampLevel = m_camConfig.sampLevel;

    std::cout << "P3\n" << img.width() << " " << img.height() << "\n255\n";
    for (int col = 0; col < img.height(); col++) {
      std::clog << "\rReamining lines: " << img.height() - col - 1
                << std::flush;
      for (int row = 0; row < img.width(); row++) {
        // out draw order is from top to bottom
        //  -------->
        //  |
        //  |
        //  v
        // but the generated ray is from bottom to top
        //  ^
        //  |
        //  |
        //  -------->
        vec3 pixelPos =
            vec3(viewportTopleft + vec2(row, img.height() - col) * pixelSize,
                 -m_camConfig.flocalLength);
        vec3 finalColor = vec3(0.0f);

        // Multisampling
        for (int idx = 0; idx < sampLevel; idx++) {
          // randomly generate some offset
          double xOffset = randDoule(0.0, pixelSize.x / sampLevel) * idx;
          double yOffset = randDoule(0.0, pixelSize.y / sampLevel) * idx;
          vec2 pixelOffset = vec2(xOffset, yOffset);
          Ray ray = genRay(pixelPos, pixelOffset);

          // accumulate the color no matter if hit something or not
          finalColor += pixelColor(ray, scene, m_camConfig.maxRayBounce);
        }
        img.writeColor(std::cout, finalColor / vec3(sampLevel));
      }
    }
    std::clog << "\n\rDone.\n";
  }

private:
  CameraConfig m_camConfig;
};
