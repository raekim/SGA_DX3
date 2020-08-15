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
	unordered_map<wstring, pair<ModelBone*, D3DXMATRIX>> bones;
	float steerDegree;
	float steerSpeed;

	void SetBoneMatrix(wstring boneName, D3DXMATRIX matrix);
};