#pragma once

#include <pch.hpp>
#include <ray.hpp>

class Material;

struct HitRecord {
  vec3 hitPos;
  vec3 hitNormal;
  float t;
  bool isInside;
  // TODO: maybe material here
  std::shared_ptr<Material> mat;

  // adjust the inside and outside normal
};

class HittableObject {
public:
  virtual ~HittableObject() = default;
  virtual bool isHitBy(const Ray &ray, HitRecord &rec) = 0;
};

class Sphere : public HittableObject {
public:
  Sphere() = default;
  ~Sphere() = default;
  Sphere(const vec3 &center, double radius, std::shared_ptr<Material> mat)
      : m_center(center), m_radius(radius), m_mat(mat) {}

  bool isHitBy(const Ray &ray, HitRecord &rec) override {
    auto a = glm::length2(ray.direction());
    // h = -2b, optimization
    auto h = glm::dot(ray.direction(), m_center - ray.origin());
    auto c = glm::length2(m_center - ray.origin()) - m_radius * m_radius;

    float discriminant = h * h - a * c;
    if (discriminant < 0)
      return false;

    // find the cloest hit point, i.e. smallest t
    auto root = (h - std::sqrt(discriminant)) / a; // TODO: could be a problem
    if (root < 0.001) {
      root = (h + std::sqrt(discriminant)) / a;
      if (root < 0.001)
        return false;
    }
    rec.t = root;
    rec.hitPos = ray.at(rec.t);
    rec.hitNormal = glm::normalize(rec.hitPos - m_center);
    // if the ray direction align with the hit normal
    // the ray is inside the obj
    rec.isInside = glm::dot(ray.direction(), rec.hitNormal) < 0 ? false : true;
    rec.mat = m_mat;

    return true;
  }

private:
  vec3 m_center;
  double m_radius;
  std::shared_ptr<Material> m_mat;
};
