#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "resources.h"
#include "core.h"
#include <vector>


class Camera
{
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	glm::vec3 Rotation;

	float Yaw;
	float Pitch;
	float Zoom;
	float MovementSpeed;

	int mouseX;
	int mouseY;
	float angleX;
	float angleY;
	float lastX;
	float lastY;
	bool firstMouse;

	void updateCameraVectors();

public:
	Camera();
	~Camera();
	glm::mat4 GetViewMat();
	glm::mat4 GetProjMat();
	void movement(float _deltaTime, SDL_Window *_window);
	glm::vec3 GetPos();

};




#endif