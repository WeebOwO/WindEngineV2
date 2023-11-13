#include "Engine.h"

#include <tracy/Tracy.hpp>

#include "Core/Log.h"
#include "ECS/Component.h"
#include "Ecs/Entity.h"
#include "Engine/RuntimeContext.h"
// renderer part
#include "Renderer/Material.h"
#include "Renderer/SceneRenderer.h"
#include "Renderer/View.h"
#include "Resource/Mesh.h"
#include "Scene/Scene.h"
#include "Window.h"
// imgui part
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

namespace wind {
Engine::Engine(Scope<Window> window) : m_window(std::move(window)) {
    Init();
    PostInit();
}

Engine::~Engine() { Quit(); }

void Engine::LoadScene() {
    m_activeSceneIndex = 0;
    m_scenes.push_back(scope::Create<Scene>());

    auto& scene = m_scenes[m_activeSceneIndex];

    scene->Init();
    auto gameobject = scene->CreateGameObject("Test");
    auto tag        = gameobject.GetComponent<TagComponent>();

    Ref<StaticMesh> mesh = ref::Create<StaticMesh>();

    // init the triangle
    StaticMeshVertexFactory::Vertex v1, v2, v3;
    v1.position = {0.0f, -0.5f, 0.0f};
    v2.position = {0.5f, 0.5f, 0.0f};
    v3.position = {-0.5f, 0.5f, 0.0f};

    mesh->meshSource.vertices = {v1, v2, v3};
    mesh->meshSource.indices  = {{0, 1, 2}};

    auto materialManager = g_runtimeContext.materialManager.get();
    mesh->material       = materialManager->GetMaterial("default_lit").get();
    mesh->InitRHI();

    gameobject.AddComponent<MeshComponent>(mesh);
}

void Engine::Run() {
#ifdef TRACY_ENABLE
    WIND_CORE_INFO("Start Tracy");
#endif
    LoadScene();
    while (!glfwWindowShouldClose(m_window->GetWindow())) {
        ZoneScoped;
        float fs = CalcDeltaTime();
        LogicTick(fs);
        RenderTick(fs);
        FrameMark;
    }
}

void Engine::Init() {
    g_runtimeContext.Init();
    m_renderThread.Init();
    WIND_CORE_INFO("Init the engine core!");
}

float Engine::CalcDeltaTime() {
    float dalta;
    {
        using namespace std::chrono;
        steady_clock::time_point tickTimePoint = steady_clock::now();
        auto timeSpan = duration_cast<duration<float>>(tickTimePoint - m_lastTickTimePoint);
        dalta         = timeSpan.count();

        m_lastTickTimePoint = tickTimePoint;
    }
    return dalta;
}

void Engine::PostInit() {
    m_window->Init();
    g_runtimeContext.PostInit(*m_window);
}

void Engine::Quit() {
    m_renderThread.Quit();
    g_runtimeContext.Quit();
    WIND_CORE_INFO("Shutdown engine");
}

void Engine::RenderTick(float delta) {
    ZoneScopedN("RenderTick");
    // imgui start part
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    for (const auto& layer : m_layerStack) {
        layer->OnImGuiRender();
    }
    // execute main render job
    m_renderThread.RenderJob(*m_window->GetSwapChain());

    // imgui end part
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
    m_renderThread.NextFrame();
}

void Engine::LogicTick(float delta) {
    ZoneScopedN("LogicTick");
    m_window->OnUpdate(delta);

    // update app logic
    for (const auto& layer : m_layerStack) {
        layer->OnUpdate(delta);
    }

    auto& activeScene = m_scenes[m_activeSceneIndex];
    activeScene->Update();

    g_runtimeContext.activeScene = activeScene.get();
}

void Engine::PushLayer(Scope<Layer> layer) {
    layer->OnAttach();
    m_layerStack.PushLayer(std::move(layer));
}

void Engine::PushOverlay(Scope<Layer> layer) {
    layer->OnAttach();
    m_layerStack.PushOverlay(std::move(layer));
}

void Engine::PopLayer(Scope<Layer> layer) {
    layer->OnDetach();
    m_layerStack.PopLayer(std::move(layer));
}

void Engine::PopOverlay(Scope<Layer> layer) {
    layer->OnDetach();
    m_layerStack.PopOverlay(std::move(layer));
}

} // namespace wind