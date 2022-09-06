/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/
#pragma once

#include <glm/glm.hpp>

class Camera {
	glm::vec3 position{};
	glm::vec3 upDirection{ 0,1,0 };
	glm::vec3 rightDirection{ 1,0,0 };
	glm::vec3 backDirection{0,0,1};

	//glm::mat2x2 extend{};
	glm::mat4 worldToCamera{};
	glm::mat4 cameraToWorld{};
	glm::mat4 cameraToNDC{};
	glm::mat4 worldToCameraToNDC{};
	//extend variable
	//bool WidthUpdate{false};
	bool matrixUpdate{ false };
	float fov{ 0.2f * 3.14f };
	float height{};
	float width{};
	float ar{ 1.5f };
	float near{1.f};
	float far{10000.f};
	float distance{ (near+ far)*0.5f};
	
	bool isMove{true};
	glm::dvec3 prevMousePos{};
	glm::dvec3 curMousePos{};

	void arUpdate();
	void matrixUpdateFunc();
	void cameraMoveFunc(double dt);
	void cameraImGuiUpdate(double);
	
	float speed{5000.f};
public:
	Camera() = default;
	void init();
	const glm::vec3 getCameraPos()const;
	void Update(double dt);
	//rotate function?
	void setPosition(glm::vec3 pos) { position = pos; matrixUpdate = true; }
	const glm::mat4 getWorldToCameraToNDC()const { return worldToCameraToNDC; }
	const glm::mat4 getWorldToCamera()const { return worldToCamera; }
	const glm::mat4 getCameraToNDC()const { return cameraToNDC; }
	const glm::mat4 getCameraToWorld()const { return cameraToWorld; }
	void setCameraHeight(float hei) { height = hei; }
	void updateCameraHeight(float AH) { height += AH; }
	void resetAngle();
	void setisMove(bool TF) { isMove = TF; }
	const float getCameraWidth()const { return width; }
	const float getCameraHeight()const { return height; }
	float &getNear() { return near; }
	float &getFar() { return far; }
	float &getFov() { return fov; }
	float &getAr() { return ar; }
	float getheight() { return height; }
	float getFarWidth() { return width/distance*far; }
	glm::vec3 getViewDirec() { return backDirection* -1.f ; }

	void setDistance(float dist) { distance = dist; matrixUpdate = true;}
	void setNear(float Near) { near = Near; matrixUpdate = true; }
	void setFar(float Far) {  far = Far;matrixUpdate = true;}
	void setFov(float Fov) {  fov = Fov;matrixUpdate = true;}
	void setAr(float Ar) {  ar = Ar;matrixUpdate = true;}
	void setIsUpdate(bool TF) { matrixUpdate = TF; };
	void setRotate(glm::vec3 angle);
	void setSpeed(float sp) { speed = sp; }
};
glm::mat3 rotationMat(glm::vec3 axis, float angle);