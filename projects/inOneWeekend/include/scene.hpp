#pragma once

#include <image.hpp>
#include <pch.hpp>
#include <primitive.hpp>

class Scene {
public:
  Scene() = default;
  ~Scene() = default;

  inline std::vector<std::shared_ptr<HittableObject>> primitives() const {
    return m_objs;
  }

public:
  void add(std::shared_ptr<HittableObject> obj) { m_objs.push_back(obj); }

private:
  std::vector<std::shared_ptr<HittableObject>> m_objs;
};
