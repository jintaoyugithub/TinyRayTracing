#pragma once

#include <pch.hpp>
#include <primitive.hpp>
#include <ray.hpp>

class Material {
public:
  virtual ~Material() = default;

  virtual void scatter(const Ray &rayIn, HitRecord &rec, Ray &scatteredRay,
                       vec3 &attenuation) = 0;
};

class DiffuseMat : public Material {
public:
  DiffuseMat() = default;
  ~DiffuseMat() = default;

  DiffuseMat(vec3 albedo) : m_albedo(albedo) {}

  void scatter(const Ray &rayIn, HitRecord &hitRec, Ray &scatteredRay,
               vec3 &attenuation) override {

    vec3 randVec;
    while (true) {
      randVec = vec3(randDoule(-1.0, 1.0), randDoule(-1.0, 1.0),
                     randDoule(-1.0, 1.0));
      auto vecLen = glm::length(randVec);
      if (1e-160 < vecLen && vecLen <= 1) {
        randVec = randVec / vecLen;
        break;
      }
    }

    vec3 scatterDir = hitRec.hitNormal + randVec;
    scatteredRay = Ray(hitRec.hitPos, scatterDir);
    attenuation = m_albedo;
  }

private:
  vec3 m_albedo{0.0f};
};

class MetalMat : public Material {
public:
  void scatter(const Ray &rayIn, HitRecord &hitRec, Ray &scatteredRay,
               vec3 &attenuation) override {}
};
