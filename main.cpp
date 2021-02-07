#include "pr.hpp"
#include <iostream>
#include <memory>

int main() {
    using namespace pr;

    Config config;
    config.ScreenWidth = 1920;
    config.ScreenHeight = 1080;
    config.SwapInterval = 1;
    Initialize(config);

    Camera3D camera;
    camera.origin = { 4, 4, 4 };
    camera.lookat = { 0, 0, 0 };
    camera.zUp = true;

    double e = GetElapsedTime();

    while (pr::NextFrame() == false) {
        if (IsImGuiUsingMouse() == false) {
            UpdateCameraBlenderLike(&camera);
        }

        ClearBackground(0.1f, 0.1f, 0.1f, 1);

        BeginCamera(camera);

        PushGraphicState();

        DrawGrid(GridAxis::XY, 1.0f, 10, { 128, 128, 128 });
        DrawXYZAxis(1.0f);

        static glm::vec3 v0 = { 1, 0, 0 };
        static glm::vec3 v1 = { 0, 1, 0 };
        static glm::vec3 v2 = { 0, 0, 1 };
        ManipulatePosition(camera, &v0, 0.3f);
        ManipulatePosition(camera, &v1, 0.3f);
        ManipulatePosition(camera, &v2, 0.3f);

        DrawArrow(v0, v1, 0.01f, { 255,0,255 });
        DrawArrow(v1, v2, 0.01f, { 255,0,255 });
        DrawArrow(v2, v0, 0.01f, { 255,0,255 });

        glm::vec3 e0 = v0 - v2;
        glm::vec3 e1 = v1 - v0;
        glm::vec3 e2 = v2 - v1;

        // left hand
        // float nx = v1.y * e0.z + v2.y * e1.z + v0.y * e2.z; // xy
        // float ny = v1.z * e0.x + v2.z * e1.x + v0.z * e2.x; // zx
        // float nz = v1.x * e0.y + v2.x * e1.y + v0.x * e2.y; // xy

        // right hand
        float nx = v1.z * e0.y + v2.z * e1.y + v0.z * e2.y; // xy
        float ny = v1.x * e0.z + v2.x * e1.z + v0.x * e2.z; // zx
        float nz = v1.y * e0.x + v2.y * e1.x + v0.y * e2.x; // xy

        glm::vec3 n = glm::normalize( glm::vec3( nx, ny, nz ) );
        glm::vec3 nc = glm::normalize( glm::cross( e2, e0 ) );

        glm::vec3 c = (v0 + v1 + v2) / 3.0f;
        DrawArrow(c, c + n * 0.5f, 0.01f, { 255, 0, 0 });
        

        PopGraphicState();
        EndCamera();

        BeginImGui();

        ImGui::SetNextWindowSize({ 500, 800 }, ImGuiCond_Once);
        ImGui::Begin("Panel");
        ImGui::Text("fps = %f", GetFrameRate());
        ImGui::Text("dot(e0, n) = %.20f", glm::dot(e0, n));
        ImGui::Text("dot(e1, n) = %.20f", glm::dot(e1, n));
        ImGui::Text("dot(e2, n) = %.20f", glm::dot(e2, n));

        glm::dvec3 nt = glm::normalize(glm::cross(glm::dvec3(e2), glm::dvec3(e0)));

        double d0 = glm::length(glm::dvec3(n) - nt);
        double d1 = glm::length(glm::dvec3(nc) - nt);
        ImGui::Text("d0 = %.20f", d0);
        ImGui::Text("d1 = %.20f", d1);

        ImGui::End();

        EndImGui();
    }

    pr::CleanUp();
}
