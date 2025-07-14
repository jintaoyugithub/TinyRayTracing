#include <camera.hpp>
#include <image.hpp>
#include <pch.hpp>
#include <scene.hpp>

int main() {
  Image img(imgWidth, imgRatio);
  Viewport vp(viewportWidth, img.ratio(), vec2(img.width(), img.height()));

  Camera cam(vec3(0.0f), 1.0f, vp);

  Scene scene;
  scene.add(std::make_shared<Sphere>(vec3(0, 0, -1), 0.3f));
  scene.add(std::make_shared<Sphere>(vec3(1.0, 0.5, -1), 0.3f));
  scene.add(std::make_shared<Sphere>(vec3(-1.0, -0.5, -10), 1.0f));

  scene.render(cam, img);
}
