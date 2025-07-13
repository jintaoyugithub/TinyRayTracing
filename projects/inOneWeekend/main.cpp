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

class Ray {
public:
  Ray() = default;
  ~Ray() = default;
  Ray(const vec3 &orig, const vec3 &dir) : m_orig(orig), m_dir(dir) {}

  inline const vec3 origin() const { return m_orig; };
  inline const vec3 direction() const { return m_dir; };

  vec3 at(float t) { return m_orig + m_dir * t; }

private:
  vec3 m_orig;
  vec3 m_dir;
};

struct HitRecord {
  vec3 hitPos;
  vec3 hitNormal;
  float t;
  bool isInside;
  // TODO: maybe material here

  // adjust the inside and outside normal
};

class HittableObject {
public:
  virtual ~HittableObject() = default;
  virtual bool isHitBy(const Ray &ray, HitRecord &rec) = 0;
};

class Sphere : HittableObject {
public:
  Sphere() = default;
  ~Sphere() = default;
  Sphere(const vec3 &center, double radius)
      : m_center(center), m_radius(radius) {}

  bool isHitBy(const Ray &ray, HitRecord &rec) override {
    auto a = glm::length2(ray.direction());
    // h = -2b, optimization
    auto h = glm::dot(ray.direction(), m_center - ray.origin());
    auto c = glm::length2(m_center - ray.origin()) - m_radius * m_radius;

    float discriminant = h * h - a * c;
    // TODO
    if (discriminant < 0)
      return false;

    // find the cloest hit point, i.e. smallest t
    rec.t = (h - std::sqrt(discriminant)) / a;
    rec.hitPos = ray.origin() + ray.direction() * rec.t;
    rec.hitNormal = glm::normalize(rec.hitPos - m_center);

    return true;
  }

private:
  vec3 m_center;
  double m_radius;
};

class Scene {
public:
  Scene() = default;
  ~Scene() = default;

  void add(std::shared_ptr<HittableObject> obj) { m_objs.push_back(obj); }

private:
  std::vector<std::shared_ptr<HittableObject>> m_objs;
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
    Viewport vp = cam.viewport();
    const vec2 pixelSize = vp.pixelSize();
    std::clog << "pixel size: " << std::endl;
    vec2 viewportTopleft = cam.eye() - vec3(0.0, 0.0, -cam.focalLength()) -
                           vec3(vp.size() / 2.0f, 0.0);
    Sphere sphere(vec3(0, 0, 1), 0.3f);
    HitRecord hitRec;

    std::cout << "P3\n" << m_width << " " << m_height << "\n255\n";
    for (int col = 0; col < m_height; col++) {
      std::clog << "\rReamining lines: " << m_height - col - 1 << std::flush;
      for (int row = 0; row < m_width; row++) {
        vec3 pixelPos =
            vec3(viewportTopleft + vec2(row, col) * pixelSize, -1.0f);
        Ray ray(cam.eye(), std::move(pixelPos - cam.eye()));
        if (sphere.isHitBy(ray, hitRec)) {
          vec3 color = 0.5f * (hitRec.hitNormal + 1.0f);
          writeColor(std::cout, color);
          continue;
        }

        // background color
        auto uintDir = glm::normalize(ray.direction());
        auto a = 0.5f * uintDir.y + 0.5f;
        vec3 color = glm::mix(vec3(0.5f, 0.8, 0.9), vec3(1.0f), a);
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
  Camera cam(vec3(0.0f), 1.0f, vp);
  img.render(cam);
}
