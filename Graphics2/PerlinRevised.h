/*
	Author: Luke Rapkin
*/
#pragma once
#include <math.h>

class PerlinRevised
{
public:
	struct Vector2 {
		float x, y;
	};

	static float interpolate(float a0, float a1, float w);
	
	static Vector2 randomGradient(int ix, int iy);

	static float dotGridGradient(int ix, int iy, float x, float y);

	static float perlin(float x, float y);
};

