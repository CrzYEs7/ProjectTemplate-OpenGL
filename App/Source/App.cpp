#include "Core/Application.hpp"
#include "GameLayer.h"

int main()
{
    Core::ApplicationSpecification appSpec;
    appSpec.Name = "OpenGL Template";
    appSpec.WindowSpec.Width = 1920;
    appSpec.WindowSpec.Height = 1080;

    Core::Application application(appSpec);
    application.PushLayer<GameLayer>();
    application.Run();
}
