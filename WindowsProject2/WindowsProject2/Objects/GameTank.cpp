#include "framework.h"
#include "GameTank.h"

GameTank::GameTank(wstring matFolder, wstring matFile) : GameModel(matFolder, matFile, Models + L"Tank/", L"Tank.mesh"),
steerDegree(0), steerSpeed(45)
{
	for (ModelBone* bone : model->Bones())
	{
		bones[bone->Name()] = { bone, bone->Local() };
	}
}

GameTank::~GameTank()
{

}

void GameTank::SetBoneMatrix(wstring boneName, D3DXMATRIX matrix)
{
	bones[boneName].first->Local(matrix * bones[boneName].second);
}

void GameTank::Update()
{
	__super::Update();

	D3DXMATRIX R, R2, R3;

	// ¹ÙÄû°¡ È¸Àü
	D3DXMatrixRotationX(&R, Math::ToRadian(Time::Get()->Running() * -100));

	// sine wave
	D3DXMatrixRotationX(&R3, sinf(Time::Get()->Running() * 2.0f) * 0.25f);

	// steer
	if (Keyboard::Get()->Press(VK_LEFT))
	{
		steerDegree += steerSpeed * Time::Get()->Delta();
	}
	if (Keyboard::Get()->Press(VK_RIGHT))
	{
		steerDegree -= steerSpeed * Time::Get()->Delta();
	}
	steerDegree = Math::Clamp(steerDegree, -45, 45);
	float angle = D3DXToRadian(steerDegree);
	D3DXMatrixRotationY(&R2, sinf(angle));

	SetBoneMatrix(L"hatch_geo", R3);
	SetBoneMatrix(L"canon_geo", R3);

	SetBoneMatrix(L"l_back_wheel_geo", R);
	SetBoneMatrix(L"r_back_wheel_geo", R);
	SetBoneMatrix(L"l_front_wheel_geo", R);
	SetBoneMatrix(L"r_front_wheel_geo", R);

	SetBoneMatrix(L"l_steer_geo", R2);
	SetBoneMatrix(L"r_steer_geo", R2);
}

void GameTank::Render()
{
	__super::Render();
}
