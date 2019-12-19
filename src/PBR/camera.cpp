#include "camera.h"
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 760

void Camera::updateCameraVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);
	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));
}

Camera::Camera()
{
	Rotation = { 0,0,0 };
	Position = { 0,0,0 };
	WorldUp = { 0,1,0 };
	Yaw = -90.0f;
	Pitch = 0.0f;
	updateCameraVectors();
	MovementSpeed = 0.10f;
}

Camera::~Camera()
{

}

glm::mat4 Camera::GetViewMat()
{
	glm::mat4 Model(glm::mat4(1.0f));
	Model = glm::translate(Model, Position);
	Model = glm::rotate(Model, glm::radians(Rotation.x), glm::vec3(1, 0, 0));
	Model = glm::rotate(Model, glm::radians(Rotation.y), glm::vec3(0, 1, 0));
	Model = glm::rotate(Model, glm::radians(Rotation.z), glm::vec3(0, 0, 1));
	Model = glm::scale(Model, { 1,1,1 });
	return Model;
}

glm::mat4 Camera::GetProjMat()
{
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(60.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
	return projection;
}

void Camera::movement(float _deltaTime)
{
	float velocity = MovementSpeed * _deltaTime;
	SDL_Event event = { 0 };

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_a:
				std::cout << "Going Left" << std::endl;
				Position -= Right * velocity;
				break;
			case SDLK_w:
				std::cout << "Going Up" << std::endl;
				Position += Front * velocity;
				break;
			case SDLK_s:
				std::cout << "Going Down" << std::endl;
				Position -= Front * velocity;
				break;
			case SDLK_d:
				std::cout << "Going Right" << std::endl;
				Position += Right * velocity;
				break;
			}
		}
		if (event.type == SDL_MOUSEMOTION)
		{
			int mouseX = event.motion.x;
			int mouseY = event.motion.y;
		}
	}

}

glm::vec3 Camera::GetPos()
{
	return Position;
}


