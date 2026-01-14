#include "glad/glad.h"
#include "Application.hpp"

#include <GLFW/glfw3.h>

#include <assert.h>
#include <iostream>
#include <ranges>

namespace Core {

    static Application* s_Application = nullptr;

    static void GLFWErrorCallback(int error, const char* description)
    {
        std::cerr << "[GLFW Error]: " << description << std::endl;
    }

    Application::Application(const ApplicationSpecification& specification)
        : m_Specification(specification)
    {
        s_Application = this;

        glfwSetErrorCallback(GLFWErrorCallback);
        glfwInit();

        // Set window title to app name if empty
        if (m_Specification.WindowSpec.Title.empty())
            m_Specification.WindowSpec.Title = m_Specification.Name;

        m_Specification.WindowSpec.EventCallback = [this](Event& event) { RaiseEvent(event); };

        m_Window = std::make_shared<Window>(m_Specification.WindowSpec);
        m_Window->Create();

    }

    Application::~Application()
    {
        m_Window->Destroy();

        glfwTerminate();

        s_Application = nullptr;
    }

    void Application::Run()
    {
        m_Running = true;

        float lastTime = GetTime();

        // --------------------
        // Loop principal
        // --------------------

        int Width = (int)m_Specification.WindowSpec.Width;
        int Height = (int)m_Specification.WindowSpec.Height;

        // Main Application loop
        while (m_Running)
        {
            glfwPollEvents();

            if (m_Window->ShouldClose())
            {
                Stop();
                break;
            }

            float currentTime = GetTime();
            float timestep = glm::clamp(currentTime - lastTime, 0.001f, 0.1f);
            lastTime = currentTime;

            glfwGetFramebufferSize(m_Window->GetHandle(), &Width, &Height);
            glViewport(0, 0, Width, Height);
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // Main layer update here
            for (const std::unique_ptr<Layer>& layer : m_LayerStack)
                layer->OnUpdate(timestep);

            // NOTE: rendering can be done elsewhere (eg. render thread)
            for (const std::unique_ptr<Layer>& layer : m_LayerStack)
                layer->OnRender();


            m_Window->Update();
        }
    }

    glm::vec2 Application::GetFramebufferSize() const
    {
        return m_Window->GetFramebufferSize();
    }

    void Application::Stop()
    {
        m_Running = false;
    }

    void Application::RaiseEvent(Event& event)
    {
        for (auto& layer : std::views::reverse(m_LayerStack))
        {
            layer->OnEvent(event);
            if (event.Handled)
                break;
        }
    }

    Application& Application::Get()
    {
        assert(s_Application);
        return *s_Application;
    }

    float Application::GetTime()
    {
        return (float)glfwGetTime();
    }

}
