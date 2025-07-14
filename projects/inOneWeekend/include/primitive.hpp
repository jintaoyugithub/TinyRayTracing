#pragma once

#include <pch.hpp>
#include <ray.hpp>

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

class Sphere : public HittableObject {
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
    rec.t = (h - std::sqrt(discriminant)) / a; // TODO: could be a problem
    rec.hitPos = ray.origin() + ray.direction() * rec.t;
    rec.hitNormal = glm::normalize(rec.hitPos - m_center);
    // if the ray direction align with the hit normal
    // the ray is inside the obj
    rec.isInside = glm::dot(ray.direction(), rec.hitNormal) < 0 ? false : true;

    return true;
  }

private:
  vec3 m_center;
  double m_radius;
};
