#pragma once

#include "GLFW/glfw3.h"

#include <glm.hpp>
#include <stdint.h>
#include <string>
#include <functional>

namespace Core {
    struct WindowSpecification
    {
        std::string Title;
        uint32_t Width = 800;
        uint32_t Height = 600;

        bool VSync = true;
    };

    class Window
    {
    public:
        Window(const WindowSpecification& specification = WindowSpecification());
        ~Window();

        void Create();
        void Destroy();

        void Update();

        glm::vec2 GetFramebufferSize() const;
        
    bool ShouldClose() const;

        GLFWwindow* GetHandle() const { return m_Handle; }

    private:
        WindowSpecification m_Specification;

        GLFWwindow* m_Handle = nullptr;
    };
}
