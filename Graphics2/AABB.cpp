#include "AABB.h"

void AABB::SetPosition(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

void AABB::SetSize(float width, float height, float length)
{
	this->width = width;
	this->height = height;
	this->length = length;
}