#pragma once

#include "Window.hpp"

#include <string>
#include <memory>
#include <vector>
#include <set>
#include <functional>

namespace Core {

struct ApplicationSpecification
{
    std::string Name = "Application";
    WindowSpecification WindowSpec;
};

class Application
{
public:
    Application(const ApplicationSpecification& specification = ApplicationSpecification());
    ~Application();

    void Run();
    void Stop();
    
    std::shared_ptr<Window> GetWindow() const { return m_Window; }

    static Application& Get();
    static float GetTime();

private:
    ApplicationSpecification m_Specification;
    std::shared_ptr<Window> m_Window;
    bool m_Running = false;


};

}
