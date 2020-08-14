#include "framework.h"
#include "ExeExportModel.h"
#include "../Fbx/Exporter.h"
#include "./Objects/GameTank.h"

ExeExportModel::ExeExportModel(ExecuteValues* values) : Execute(values)
{
	Fbx::Exporter* exporter = NULL;

	// Cube
	//exporter = new Fbx::Exporter(Assets + L"Meshes/Cube.fbx");
	//exporter->ExportMaterial(Models + L"Meshes/Cube/", L"Cube.material");
	//SAFE_DELETE(exporter);

	// Tank material �����
	//exporter = new Fbx::Exporter(Assets + L"Tank/Tank.fbx");
	//exporter->ExportMaterial(Models + L"/Tank/", L"Tank.material");
	//exporter->ExportMesh(Models + L"/Tank/", L"Tank.mesh");
	//SAFE_DELETE(exporter);

	tank = new GameTank(Models + L"/Tank/", L"Tank.material");

	D3DXMATRIX R;
	D3DXMatrixRotationY(&R, Math::ToRadian(90));

	tank->RootAxis(R);
}

ExeExportModel::~ExeExportModel()
{
	SAFE_DELETE(tank);
}

void ExeExportModel::Update()
{
	tank->Update();
}

void ExeExportModel::PreRender()
{
}

void ExeExportModel::Render()
{
	tank->Render();
}

void ExeExportModel::PostRender()
{
}

void ExeExportModel::ResizeScreen()
{
}