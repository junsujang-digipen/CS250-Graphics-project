/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/
#include <glm/ext/matrix_transform.hpp>
#include "Entity.h"

#include "../GraphicEngine/Uniform.h"


Entity::Entity(std::string name, glm::vec3 pos, glm::vec3 scale, glm::vec3 rotate)
	: name(name), pos(pos), scale(scale), rotate(rotate), model(name), updateMatrix(true)
{
	AddUniform(new u_modelToNDC(objectMatrix));
	AddUniform(new u_color(color));
}

Entity::~Entity()
{}

void Entity::Update(double dt)
{
	if (vel.x != 0 || vel.y != 0) {
		UpdatePosition(vel * static_cast<float>(dt));
	}
	if (updateMatrix == true)
	{
		glm::mat4 Translate_mat = glm::translate(glm::mat4(1.0f), pos);
		glm::mat4 scale_mat = glm::scale(glm::mat4(1.0f), scale);
		glm::mat4 rotate_x = glm::rotate(glm::mat4(1.0f), rotate.x, glm::vec3{ 1.f,0.f,0.f });
		glm::mat4 rotate_y = glm::rotate(glm::mat4(1.0f), rotate.y, glm::vec3{ 0.f,1.f,0.f });
		glm::mat4 rotate_z = glm::rotate(glm::mat4(1.0f), rotate.z, glm::vec3{ 0.f,0.f,1.f });
		glm::mat4 rotate_mat = rotate_x * rotate_y * rotate_z;
		objectMatrix = Translate_mat * rotate_mat * scale_mat;
		updateMatrix = false;
	}
}

void Entity::Draw()
{
	DrawBinding();
	glPolygonMode(faceMode, drawMode);
	DrawModel();
	DrawUnBinding();
}


void Entity::DrawBinding()
{
	glPolygonMode(faceMode, drawMode);
	model.DrawBinding();
}

void Entity::DrawModel()
{
	model.DrawModel();
}

void Entity::DrawUnBinding()
{
	model.DrawUnBinding();
}


void Entity::SetPos(glm::vec3 newPos)
{
	pos = newPos;
	updateMatrix = true;
}

void Entity::SetVel(glm::vec3 newVel)
{
	vel = newVel;
	updateMatrix = true;
}

void Entity::SetScale(glm::vec3 newScale)
{
	scale = newScale;
	updateMatrix = true;
}

void Entity::SetRotate(glm::vec3 newRotate)
{
	rotate = newRotate;
	updateMatrix = true;
}

void Entity::SetColor(glm::vec4 newColor)
{
	color = newColor;
}


void Entity::UpdatePosition(glm::vec3 adjustPosition)
{
	pos += adjustPosition;
	updateMatrix = true;
}

void Entity::UpdateVelocity(glm::vec3 adjustVelocity)
{
	vel += adjustVelocity;
}
void Entity::UpdateRotation(glm::vec3 newRotationAmount)
{
	rotate += newRotationAmount;
	updateMatrix = true;
}

void Entity::UpdateColor(glm::vec4 adjustColor)
{
	color += adjustColor;
}



glm::mat4& Entity::GetMatrix()
{
	if(updateMatrix == true)
	{
		glm::mat4 Translate_mat = glm::translate(glm::mat4(1.0f),pos);
		glm::mat4 scale_mat = glm::scale(glm::mat4(1.0f), scale);
		glm::mat4 rotate_x = glm::rotate(glm::mat4(1.0f) ,rotate.x, glm::vec3{ 1.f,0.f,0.f });
		glm::mat4 rotate_y = glm::rotate(glm::mat4(1.0f), rotate.y, glm::vec3{ 0.f,1.f,0.f });
		glm::mat4 rotate_z = glm::rotate(glm::mat4(1.0f), rotate.z, glm::vec3{ 0.f,0.f,1.f });
		glm::mat4 rotate_mat = rotate_x * rotate_y * rotate_z;
		objectMatrix = Translate_mat * rotate_mat * scale_mat;
		updateMatrix = false;
	}
	
	return objectMatrix;
}
