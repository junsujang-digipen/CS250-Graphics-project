/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/
#include "Mesh.h"

void MeshHandle::BindVertex()
{
	glBindVertexArray(vaoid);
}

void MeshHandle::UnbindVertex()
{
	glBindVertexArray(0);
}
