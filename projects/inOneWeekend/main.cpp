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
  camConfig.sampLevel = 100u;
  camConfig.flocalLength = 0.5f;
  Camera cam(camConfig);

  auto matCenter = std::make_shared<DiffuseMat>(vec3(0.1, 0.2, 0.5));
  auto matGround = std::make_shared<DiffuseMat>(vec3(0.8, 0.8, 0.0));
  auto matLeft = std::make_shared<DiffuseMat>(vec3(0.8, 0.8, 0.8));
  auto matRight = std::make_shared<DiffuseMat>(vec3(0.8, 0.6, 0.2));

  Scene scene;
  scene.add(std::make_shared<Sphere>(vec3(0, -100.5, -1), 100, matGround));
  scene.add(std::make_shared<Sphere>(vec3(0, 0, -1.2), 0.5f, matCenter));
  scene.add(std::make_shared<Sphere>(vec3(-1.0, 0, -1), 0.5f, matLeft));
  scene.add(std::make_shared<Sphere>(vec3(1.0, 0, -1), 0.5f, matRight));

  cam.render(scene, img);
}
