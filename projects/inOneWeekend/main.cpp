#include <camera.hpp>
#include <image.hpp>
#include <pch.hpp>
#include <scene.hpp>

int main() {
  Image img(imgWidth, imgRatio);
  Viewport vp(viewportWidth, img.ratio(), vec2(img.width(), img.height()));

  CameraConfig camConfig{};
  camConfig.viewport = vp;
  camConfig.maxRayBounce = 10u;
  // camConfig.sampLevel = 100u;
  Camera cam(camConfig);

  Scene scene;
  scene.add(std::make_shared<Sphere>(vec3(0, 0, -1), 0.3f));
  scene.add(std::make_shared<Sphere>(vec3(1.0, 0.5, -1), 0.3f));
  // scene.add(std::make_shared<Sphere>(vec3(0, -3, -3), 3.0f));

  cam.render(scene, img);
}
