#include "framework.h"
#include "ExeExportModel.h"
#include "../Fbx/Exporter.h"
#include "./Objects/GameModel.h"

ExeExportModel::ExeExportModel(ExecuteValues* values) : Execute(values)
{
	Fbx::Exporter* exporter = NULL;

	// Cube
	//exporter = new Fbx::Exporter(Assets + L"Meshes/Cube.fbx");
	//exporter->ExportMaterial(Models + L"Meshes/Cube/", L"Cube.material");
	//SAFE_DELETE(exporter);

	// Tank material ¸¸µé±â
	//exporter = new Fbx::Exporter(Assets + L"Tank/Tank.fbx");
	//exporter->ExportMaterial(Models + L"/Tank/", L"Tank.material");
	//exporter->ExportMesh(Models + L"/Tank/", L"Tank.mesh");
	//SAFE_DELETE(exporter);

	tank = new GameModel(Models + L"/Tank/", L"Tank.material", Models + L"/Tank/", L"Tank.mesh");
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
