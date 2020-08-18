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

	// Tank 
	//exporter = new Fbx::Exporter(Assets + L"Tank/Tank.fbx");
	//exporter->ExportMaterial(Models + L"/Tank/", L"Tank.material");
	//exporter->ExportMesh(Models + L"/Tank/", L"Tank.mesh");
	//SAFE_DELETE(exporter);
}

ExeExportModel::~ExeExportModel()
{
}

void ExeExportModel::Update()
{
}

void ExeExportModel::PreRender()
{
}

void ExeExportModel::Render()
{
}

void ExeExportModel::PostRender()
{
}

void ExeExportModel::ResizeScreen()
{
}
