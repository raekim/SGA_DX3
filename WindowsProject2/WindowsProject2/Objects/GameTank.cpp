#include "framework.h"
#include "GameTank.h"

GameTank::GameTank(wstring matFolder, wstring matFile) : GameModel(matFolder, matFile, Models + L"Tank/", L"Tank.mesh")
{
	leftBackWheelBone = model->BoneByName(L"l_back_wheel_geo");
	rightBackWheelBone = model->BoneByName(L"r_back_wheel_geo");	
	leftFrontWheelBone = model->BoneByName(L"l_front_wheel_geo");
	rightFrontWheelBone = model->BoneByName(L"r_front_wheel_geo");	

	//cannonBone = model->BoneByName(L"canon_geo");
	turretBone = model->BoneByName(L"turret_geo");

	leftBackWheel = leftBackWheelBone->Local();
	rightBackWheel = rightBackWheelBone->Local();
	leftFrontWheel = leftFrontWheelBone->Local();
	rightFrontWheel = rightFrontWheelBone->Local();
	turret = turretBone->Local();
	//cannon = cannonBone->Local();
}

GameTank::~GameTank()
{

}

void GameTank::Update()
{
	__super::Update();

	D3DXMATRIX R, R2, R3;

	// ¹ÙÄû°¡ ¿Ô´Ù¸®°¬´Ù¸®
	//D3DXMatrixRotationX(&R, sinf(Time::Get()->Running() * 2.0f) * 0.25f);

	D3DXMatrixRotationY(&R2, sinf(Time::Get()->Running() * 2.0f) * 0.5f);
	turretBone->Local(R2 * turret);
	//cannonBone->Local(R3 * cannon);

	// ¹ÙÄû°¡ È¸Àü
	D3DXMatrixRotationX(&R, Math::ToRadian(Time::Get()->Running() * -100));

	leftBackWheelBone->Local(R * leftBackWheel);
	rightBackWheelBone->Local(R * rightBackWheel);
	leftFrontWheelBone->Local(R * leftFrontWheel);
	rightFrontWheelBone->Local(R * rightFrontWheel);
}

void GameTank::Render()
{
	__super::Render();
}
