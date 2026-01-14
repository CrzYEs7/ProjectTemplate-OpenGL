#include <glad/glad.h>
#include "GameLayer.h"
#include "Core/Application.hpp"
#include <iostream>

#include <iostream>

GameLayer::GameLayer()
{
    Enemies.reserve(50);
    Controller.SetEntity(&Player);

    GLFWwindow* window_handle = Core::Application::Get().GetWindow()->GetHandle();

    glfwSetWindowUserPointer(window_handle, &Controller);
    glfwSetKeyCallback(window_handle, Controller.key_callback);

    const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;

        uniform vec2 offset;     
        uniform float scale;     
        uniform mat4 projection;

        void main()
        {
            vec2 worldPos = aPos.xy * scale + offset;
            gl_Position = projection * vec4(worldPos, 0.0, 1.0);
        }
    )";

    const char* fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;

        uniform vec3 color;

        void main()
        {
            FragColor = vec4(color, 1.0);
        }
    )";

    // 4. Compilar vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    // 5. Compilar fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    // 6. Criar programa de shader
    m_Shader = glCreateProgram();
    glAttachShader(m_Shader, vertexShader);
    glAttachShader(m_Shader, fragmentShader);
    glLinkProgram(m_Shader);

    m_offsetLoc = glGetUniformLocation(m_Shader, "offset");
    m_scaleLoc = glGetUniformLocation(m_Shader, "scale");
    m_colorLoc = glGetUniformLocation(m_Shader, "color");
    m_projLoc = glGetUniformLocation(m_Shader, "projection");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // 7. Dados do triângulo

    float vertices[] = {
        // posições
        -0.5f, -0.5f, 0.0f, // 0: canto inferior esquerdo
        0.5f, -0.5f, 0.0f, // 1: canto inferior direito
        0.5f,  0.5f, 0.0f, // 2: canto superior direito
        -0.5f,  0.5f, 0.0f  // 3: canto superior esquerdo
    };


    unsigned int indices[] = {
        0, 1, 2, // primeiro triângulo
        2, 3, 0  // segundo triângulo
    };


    unsigned int EBO;
    glGenVertexArrays(1, &m_VertexArray);
    glGenBuffers(1, &m_VertexBuffer);
    glGenBuffers(1, &EBO);

    glBindVertexArray(m_VertexArray);

    // VBO
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // layout
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

GameLayer::~GameLayer()
{
    glDeleteVertexArrays(1, &m_VertexArray);
    glDeleteBuffers(1, &m_VertexBuffer);

    glDeleteProgram(m_Shader);
}

 void GameLayer::OnEvent(Core::Event& event)
{
	std::cout << event.ToString() << std::endl;

	Core::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Core::WindowClosedEvent>([this](Core::WindowClosedEvent& e) { return OnWindowClosed(e); });
}

void GameLayer::ProcessMovementInput(GLFWwindow* window, float& x, float& y, float speed)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        y += speed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        y -= speed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        x -= speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        x += speed;
}

void GameLayer::UpdateProjection(int width, int height)
{
    float aspect = (float)width / height;

    worldWidth = worldHeight * aspect;

    projection = glm::ortho(
        -worldWidth,  // left
        worldWidth,  // right
        -worldHeight, // bottom
        worldHeight  // top
    );
}

void GameLayer::ProcessSpawn(GLFWwindow* window, std::vector<Entity>& entities, float pos_x, float pos_y, float scale)
{
    static bool spacePressedLastFrame = false;

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        if (!spacePressedLastFrame)
        {
            float r = rand() / (float)RAND_MAX;
            float g = rand() / (float)RAND_MAX;
            float b = rand() / (float)RAND_MAX;
            entities.push_back({ pos_x, pos_y, scale, r, g, b }); // novo quadrado no centro
        }
        spacePressedLastFrame = true;
    }
    else
    {
        spacePressedLastFrame = false;
    }
}

void GameLayer::OnRender()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(m_Shader);
    glBindVertexArray(m_VertexArray);

    glUniformMatrix4fv(m_projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    for (const auto& sq : Enemies)
    {
        glUniform2f(m_offsetLoc, sq.x, sq.y);
        glUniform1f(m_scaleLoc, sq.scale);
        glUniform3f(m_colorLoc, sq.r, sq.g, sq.b);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    glUniform2f(m_offsetLoc, Player.x, Player.y);
    glUniform1f(m_scaleLoc, Player.scale);
    glUniform3f(m_colorLoc, Player.r, Player.g, Player.b);

    glm::vec2 framebufferSize = Core::Application::Get().GetFramebufferSize();
    glViewport(0, 0, static_cast<GLsizei>(framebufferSize.x), static_cast<GLsizei>(framebufferSize.y));

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void GameLayer::OnUpdate(float ts)
{
    GLFWwindow* window_handle = Core::Application::Get().GetWindow()->GetHandle();

    auto size = Core::Application::Get().GetFramebufferSize();
    UpdateProjection((int)size.x, (int)size.y);

    Entity* current_entity = Controller.GetEntity();

    float velocity = current_entity->speed * ts * 60.0f;

    GameLayer::ProcessMovementInput(window_handle, current_entity->x, current_entity->y, velocity);
    current_entity->x = glm::clamp(current_entity->x, -worldWidth + current_entity->scale, worldWidth - current_entity->scale);
    current_entity->y = glm::clamp(current_entity->y, -worldHeight + current_entity->scale, worldHeight - current_entity->scale);

    GameLayer::ProcessSpawn(window_handle, Enemies, current_entity->x, current_entity->y, 0.2f);

    if (Enemies.size() > 0 && Controller.SwapRequested)
    {
        if (NextHost < Enemies.size())
        {
            Controller.SetEntity(&Enemies[NextHost]);
            std::cout << "Host index: " << NextHost << std::endl;
            NextHost = (NextHost + 1);
            Controller.SwapedLastFrame = true;
        }
        else
        {
            Controller.SetEntity(&Player);
            NextHost = 0;
            Controller.SwapedLastFrame = true;
        }
    }
    Controller.SwapRequested = false;
}

bool GameLayer::OnWindowClosed(Core::WindowClosedEvent& event)
{
	std::cout << "Window Closed!\n";

	return false;
}
