#pragma once

#include <camera.hpp>
#include <image.hpp>
#include <pch.hpp>
#include <primitive.hpp>

class Scene {
public:
  Scene() = default;
  ~Scene() = default;

  void add(std::shared_ptr<HittableObject> obj) { m_objs.push_back(obj); }
  void render(const Camera &cam, Image &img) {
    Viewport vp = cam.viewport();
    vec2 pixelSize = vp.pixelSize();
    vec2 viewportTopleft = cam.eye() - vec3(0.0, 0.0, -cam.focalLength()) -
                           vec3(vp.size() / 2.0f, 0.0);
    HitRecord cloestHitRec;

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
                 -cam.focalLength());
        Ray ray(cam.eye(), std::move(pixelPos - cam.eye()));

        bool hitAnything = false;
        HitRecord tempRec;
        auto curCloestHit = std::numeric_limits<float>::max();
        /// for each pixel, get the cloest hit
        for (const auto &obj : m_objs) {
          if (obj->isHitBy(ray, tempRec)) {
            if (tempRec.t < curCloestHit) {
              curCloestHit = tempRec.t;
              cloestHitRec = tempRec;
            }
            hitAnything = true;
          }
        }

        if (hitAnything) {
          img.writeColor(std::cout, 0.5f * (cloestHitRec.hitNormal + 1.0f));
          continue;
        }

        // background color
        auto uintDir = glm::normalize(ray.direction());
        auto a = 0.5f * uintDir.y + 0.5f;
        vec3 color = glm::mix(vec3(1.0f), vec3(0.5f, 0.8, 0.9), a);
        // writeColor(std::cout, vec3(uintDir.y));
        img.writeColor(std::cout, vec3(color));
      }
    }

    std::clog << "\n\rDone.\n";
  }

private:
  std::vector<std::shared_ptr<HittableObject>> m_objs;
};
