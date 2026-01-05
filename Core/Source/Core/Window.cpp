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
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
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
        glfwSwapInterval(m_Specification.VSync ? 1 : 0);
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

    bool Window::ShouldClose() const
    {
        return glfwWindowShouldClose(m_Handle) != 0;
    }

}
