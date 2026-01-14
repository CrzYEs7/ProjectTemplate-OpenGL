#include "WindowEvents.h"

#include "glad/glad.h"

#include "Window.hpp"

#include <iostream>
#include <assert.h>

namespace Core {

    Window::Window(const Core::WindowSpecification& specification)
    : m_Specification(specification)
    {
    }

    Window::~Window()
    {
        Destroy();
    }

    void Window::Create()
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_Handle = glfwCreateWindow(m_Specification.Width, 
                                    m_Specification.Height, 
                                    m_Specification.Title.c_str(), 
                                    nullptr, nullptr);

        if(!m_Handle)
        {
            std::cerr << "Failed to initialize GLFW window\n";
            assert(false);
        }

        glfwMakeContextCurrent(m_Handle);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cerr << "Failed to initialize GLAD\n";
            assert(false);
        }

        glfwSwapInterval(m_Specification.VSync ? 1 : 0);

        glfwSetWindowUserPointer(m_Handle, this);
        
        glfwSetWindowCloseCallback(m_Handle, [](GLFWwindow* handle)
                {
                Window& window = *((Window*)glfwGetWindowUserPointer(handle));

                WindowClosedEvent event;
                window.RaiseEvent(event);
                });

        glfwSetWindowSizeCallback(m_Handle, [](GLFWwindow* handle, int width, int height)
                {
                Window& window = *((Window*)glfwGetWindowUserPointer(handle));

                WindowResizeEvent event((uint32_t)width, (uint32_t)height);
                window.RaiseEvent(event);
                });

    }

    void Window::Destroy()
    {
        if(m_Handle)
            glfwDestroyWindow(m_Handle);

        m_Handle = nullptr;
    }

    void Window::Update()
    {
        glfwSwapBuffers(m_Handle);
    }

    void Window::RaiseEvent(Event& event)
    {
        if (m_Specification.EventCallback)
            m_Specification.EventCallback(event);
    }

    glm::vec2 Window::GetFramebufferSize() const
    {
        int width, height;
        glfwGetFramebufferSize(m_Handle, &width, &height);
        return { width, height };
    }

    bool Window::ShouldClose() const
    {
        return glfwWindowShouldClose(m_Handle) != 0;
    }

}
