#pragma once
#include "GameModel.h"

class GameTank : public GameModel
{
public:
	GameTank(wstring matFolder, wstring matFile);
	~GameTank();

	void Update();
	void Render();

private:
	ModelBone* leftBackWheelBone;
	D3DXMATRIX leftBackWheel;

	ModelBone* rightBackWheelBone;
	D3DXMATRIX rightBackWheel;

	ModelBone* leftFrontWheelBone;
	D3DXMATRIX leftFrontWheel;

	ModelBone* rightFrontWheelBone;
	D3DXMATRIX rightFrontWheel;

	ModelBone* turretBone;
	D3DXMATRIX turret;

	ModelBone* cannonBone;
	D3DXMATRIX cannon;
};