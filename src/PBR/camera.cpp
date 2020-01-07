#include "camera.h"
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 760

void Camera::updateCameraVectors()
{
	glm::mat4 t(1.0f);
	t = glm::translate(t, glm::vec3(0, 1, 0));
	Up = t * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	Up = glm::normalize(Up);

	t = glm::mat4(1.0f);
	t = glm::rotate(t, glm::radians(angleX), glm::vec3(0, 1, 0));
	t = glm::translate(t, glm::vec3(0, 0, -1));
	Front = t * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	Front = glm::normalize(Front);

	t = glm::mat4(1.0f);
	t = glm::rotate(t, glm::radians(angleX), glm::vec3(0, 1, 0));
	t = glm::translate(t, glm::vec3(1, 0, 0));
	Right = t * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	Right = glm::normalize(Right);

}

Camera::Camera()
{
	Rotation = { 0,0,0 };
	Position = { 0,0,0 };

	MovementSpeed = 0.10f;
	lastX = WINDOW_WIDTH / 2;
	lastY = WINDOW_HEIGHT / 2;
	angleX = 0;
	angleY = 0;
	updateCameraVectors();
}

Camera::~Camera()
{

}

glm::mat4 Camera::GetViewMat()
{
	glm::mat4 Model(glm::mat4(1.0f));
	Model = glm::translate(Model, Position);
	Model = glm::rotate(Model, glm::radians(angleX), glm::vec3(0, 1, 0));
	Model = glm::rotate(Model, glm::radians(angleY), glm::vec3(1, 0, 0));
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

void Camera::movement(float _deltaTime, SDL_Window *_window)
{

	float velocity = MovementSpeed * _deltaTime;
	
	SDL_Event event = { 0 };
	SDL_SetRelativeMouseMode(SDL_TRUE);
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_a:
				//std::cout << "Going Left" << std::endl;
				Position -= Right * velocity;
				break;
			case SDLK_w:
				//std::cout << "Going Up" << std::endl;
				Position += Front * velocity;
				break;
			case SDLK_s:
				//std::cout << "Going Down" << std::endl;
				Position -= Front * velocity;
				break;
			case SDLK_d:
				//std::cout << "Going Right" << std::endl;
				Position += Right * velocity;
				break;
			case SDLK_l:
				std::cout << "Mouse locked" << std::endl;
				SDL_ShowCursor(0);
				SDL_SetWindowGrab(_window, SDL_TRUE);
				SDL_WarpMouseInWindow(_window, (WINDOW_WIDTH / 2), (WINDOW_HEIGHT / 2));
				break;
			case SDLK_ESCAPE:
				std::cout << "Unlocking Mouse" << std::endl;
				SDL_ShowCursor(1);
				SDL_SetWindowGrab(_window, SDL_FALSE);
				break;
			}
		}
		if (event.type == SDL_MOUSEMOTION)
		{
			float rotspeed = 1.0f;
			const Uint32 mouse = SDL_GetMouseState(&mouseX, &mouseY);
			
			if (mouseX > (WINDOW_WIDTH / 2))
			{
				angleX -= rotspeed;
			}
			if (mouseX < (WINDOW_WIDTH / 2))
			{
				angleX += rotspeed;

			}
			if (mouseY > (WINDOW_HEIGHT / 2))
			{
				angleY -= rotspeed * 0.8f;

			}
			if (mouseY < (WINDOW_HEIGHT / 2))
			{
				angleY += rotspeed * 0.8f;
			}
			
			
		}
		updateCameraVectors();
		GetViewMat();
	}
	

}

glm::vec3 Camera::GetPos()
{
	return Position;
}


