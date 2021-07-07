/*
	Author: Luke Rapkin
*/
#pragma once

#include "SceneNodeTile.h"
#include "Graphics2.h"
#include <math.h>

class SceneNodeWater : public SceneNodeTile
{
	//Gerstner wave
public:
	SceneNodeWater(wstring name) : SceneNodeTile(name) {};

	bool Initialise() override;
	virtual void Shutdown() override;
	virtual void Tick(XMMATRIX& completeTransform) override;
	virtual void SetupMesh() override;

private:
	float waveY = 0;
	float waveAmplitude = 2.0f;
	float waveSpeed = 0.001f;
};