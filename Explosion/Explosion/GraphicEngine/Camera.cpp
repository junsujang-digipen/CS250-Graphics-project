/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/

#include "Camera.h"
#include "GraphicEngine.h"
#include <Imgui/imgui.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <GLFW/glfw3.h>
void Camera::init()
{
	int w{};
	int h{};
	glfwGetWindowSize(GraphicEngine::GetWindowPtr(), &w, &h);
	ar = static_cast<float>(w) / h;
	width = tan(fov / 2) * 2 * distance;
	height = width / ar;
}

void Camera::Update(double dt)	{
	arUpdate();

	cameraMoveFunc(dt);
	cameraImGuiUpdate(dt);

	matrixUpdateFunc();
}

void Camera::resetAngle()
{
	upDirection={ 0,1,0 };
	rightDirection={ 1,0,0 };
	backDirection={ 0,0,1 };
}

void Camera::setRotate(glm::vec3 angle)
{
	glm::mat3 XRotMat{ rotationMat(glm::vec3{1,0,0},angle.x) };
	glm::mat3 YRotMat{ rotationMat(glm::vec3{0,1,0},angle.y) };
	glm::mat3 ZRotMat{ rotationMat(glm::vec3{0,0,1},angle.z) };
	backDirection = glm::normalize((XRotMat*YRotMat* ZRotMat)  * glm::vec3{ 0,0,1 });
	rightDirection = glm::normalize(glm::cross(glm::vec3{ 0,1,0 }, backDirection));
	upDirection = glm::normalize(glm::cross(backDirection, rightDirection));
	matrixUpdate = true;
}

void Camera::arUpdate() {
	int w{};
	int h{};
	glfwGetWindowSize(GraphicEngine::GetWindowPtr(),&w,&h);
	glm::vec2 winSize = { w,h };
	ar = width / height;
	if (ar != winSize.x / winSize.y ) {
		ar = winSize.x / winSize.y;
		//width = tan(fov / 2) * 2 * distance;
		//height = width / ar;

		const float prevHeight = width / ar;
		width = ar * height;
		matrixUpdate = true;
		distance = (near + far) / 2.f;
	}
}

void Camera::matrixUpdateFunc()
{
	if (matrixUpdate == true) {
		const float nearMinusFar = near - far;
		cameraToWorld = glm::mat4{ glm::vec4{rightDirection,0.f},glm::vec4{upDirection,0.f},glm::vec4{backDirection,0.f},glm::vec4{position,1.f} };
		worldToCamera = glm::inverse(cameraToWorld);
		cameraToNDC = glm::mat4{ 
			glm::vec4{2.f * distance / width,0.f,0.f,0.f},
			glm::vec4{0.f,2.f * distance / height,0.f,0.f},
			glm::vec4{0.f,0.f,(near + far) / (nearMinusFar),-1.f},
			glm::vec4{0.f,0.f,2.f * near * far / (nearMinusFar),0.f} };
		//glm::frustum(leftPlane, rightPlane, bottomPlane, topPlane, nearPlane, farPlane);
		worldToCameraToNDC = cameraToNDC*worldToCamera;

		matrixUpdate = false;
	}
}
void Camera::cameraMoveFunc(double /*dt*/)
{
	
}

glm::mat3 rotationMat(glm::vec3 axis,float angle) {
	const float cosA{ cos(angle) };
	const float sinA{ sin(angle)};
	const float rCosA{1-cosA};
	const float x{axis.x};
	const float y{ axis.y};
	const float z{ axis.z};
	return glm::mat3{
		glm::vec3{cosA + x * x * rCosA, y * x * rCosA + z * sinA,z * x * rCosA - y * sinA},
		glm::vec3{x * y * rCosA - z * sinA,cosA + y * y * rCosA,z * y * rCosA + x * sinA},
		glm::vec3{x * z * rCosA + y * sinA,y * z * rCosA - x * sinA,cosA + z * z * rCosA}
	};
}

void Camera::cameraImGuiUpdate(double dt)
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Camera"))
		{
			if (isMove == true) {
				ImGui::Text("Move: W, S, A, D");
				ImGui::Text("Angle: Drag left mouse");

				ImGui::InputFloat("Fov", &fov);
				ImGui::InputFloat("Near", &near);
				ImGui::InputFloat("Far", &far);
				//ImGui::InputFloat("Distance", &distance);
				ImGui::SliderFloat("PositionX", &position.x, -1000.f, 1000.f);
				ImGui::SliderFloat("PositionY", &position.y, -1000.f, 1000.f);
				ImGui::SliderFloat("PositionZ", &position.z, -1000.f, 1000.f);
				matrixUpdate = true;
			}
			ImGui::EndMenu();
			
		}
		ImGui::EndMainMenuBar();
	}
	if (isMove == true) {


		if (ImGui::IsAnyItemFocused() == false && ImGui::IsAnyItemHovered() == false) {

			if (ImGui::IsMouseDragging(ImGuiMouseButton_::ImGuiMouseButton_Left) == true)
			{
				glm::vec2 dragDel{
					ImGui::GetMouseDragDelta(ImGuiMouseButton_::ImGuiMouseButton_Left).x * -0.005f,
					ImGui::GetMouseDragDelta(ImGuiMouseButton_::ImGuiMouseButton_Left).y * -0.005f };
				glm::mat3 upRotMat{ rotationMat(rightDirection,dragDel.y) };
				glm::mat3 rightRotMat{ rotationMat(glm::vec3{0,1,0},dragDel.x) };
				backDirection = glm::normalize(rightRotMat * backDirection);
				backDirection = glm::normalize(upRotMat * backDirection);
				rightDirection = glm::normalize(glm::cross(glm::vec3{ 0,1,0 }, backDirection));
				upDirection = glm::normalize(glm::cross(backDirection, rightDirection));

				matrixUpdate = true;
				ImGui::ResetMouseDragDelta(ImGuiMouseButton_::ImGuiMouseButton_Left);
			}
			if (ImGui::IsMouseDragging(ImGuiMouseButton_::ImGuiMouseButton_Middle) == true)
			{
				glm::vec2 dragDel{
					ImGui::GetMouseDragDelta(ImGuiMouseButton_::ImGuiMouseButton_Middle).x ,
					ImGui::GetMouseDragDelta(ImGuiMouseButton_::ImGuiMouseButton_Middle).y * 0.01f };

				if (distance >= near && distance <= far) {
					distance -= dragDel.y;
					matrixUpdate = true;
				}
				else if (distance < near) {
					distance = near;
				}
				else if (distance > far) {
					distance = far;
				}
				//ImGui::ResetMouseDragDelta(ImGuiMouseButton_::ImGuiMouseButton_Middle);
			}
		}
		float moveSpeed{ speed * static_cast<float>(dt) };
		if (ImGui::IsKeyDown(ImGuiKey_::ImGuiKey_LeftShift) == true) {
			setPosition(position - glm::vec3{ 0,1,0 } *moveSpeed);
			matrixUpdate = true;
		}
		if (ImGui::IsKeyDown(ImGuiKey_::ImGuiKey_Space) == true) {
			setPosition(position + glm::vec3{ 0,1,0 } *moveSpeed);
			matrixUpdate = true;
		}
		if (ImGui::IsKeyDown(ImGuiKey_::ImGuiKey_W) == true) {
			setPosition(position - backDirection * moveSpeed);
			matrixUpdate = true;
		}
		if (ImGui::IsKeyDown(ImGuiKey_::ImGuiKey_S) == true) {
			setPosition(position + backDirection * moveSpeed);
			matrixUpdate = true;
		}
		if (ImGui::IsKeyDown(ImGuiKey_::ImGuiKey_D) == true) {
			setPosition(position + rightDirection * moveSpeed);
			matrixUpdate = true;
		}if (ImGui::IsKeyDown(ImGuiKey_::ImGuiKey_A) == true) {
			setPosition(position - rightDirection * moveSpeed);
			matrixUpdate = true;
		}
	}

}

const glm::vec3 Camera::getCameraPos() const
{
	return position;
}