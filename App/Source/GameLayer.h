#pragma once

#include "Layer.h"

#include <GLFW/glfw3.h>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <glm.hpp>
#include <vector>
#include <iostream>
#include <stdint.h>

struct Entity
{
	float x = 0.0f;
	float y = 0.0f;
	float scale = 0.5;

	float r = 1.0f;
	float g = 0.0f;
	float b = 0.0f;

	float speed = 0.01f;
};

class Controller
{
public:
	bool SwapRequested = false;
	bool SwapedLastFrame = false;

	void SetEntity(Entity* e) { Target = e; };
	Entity* GetEntity() { return Target; };

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		Controller* controller = static_cast<Controller*>(glfwGetWindowUserPointer(window));
		if (key == GLFW_KEY_V && action == GLFW_PRESS)
		{
			controller->SwapRequested = true;
			std::cout << "Swap pressed" << std::endl;
		}
	}
private:
	Entity* Target;
};

class GameLayer : public Core::Layer
{
public:
	GameLayer();
	virtual ~GameLayer();

	float worldHeight = 10.0f;
	float worldWidth = 10.0f;
	glm::mat4 projection = glm::ortho(
		-worldWidth,  // left
		worldWidth,  // right
		-worldHeight, // bottom
		worldHeight  // top
	);;

	Entity Player;
	std::vector<Entity> Enemies;
	Controller Controller;
	int NextHost = 0;

	void ProcessMovementInput(GLFWwindow* window, float& x, float& y, float speed);
	void ProcessSpawn(GLFWwindow* window, std::vector<Entity>& entities, float pos_x, float pos_y, float scale);

	virtual void OnUpdate(float ts) override;
	virtual void OnRender() override;
private:
	/*bool OnWindowClosed(Core::WindowClosedEvent& event);*/
	void UpdateProjection(int width, int height);
private:
	uint32_t m_Shader = 0;
	uint32_t m_VertexArray = 0;
	uint32_t m_VertexBuffer = 0;

	int m_offsetLoc;
	int m_scaleLoc;
	int m_colorLoc;
	int m_projLoc;

	float m_Time = 0.0f;
};
