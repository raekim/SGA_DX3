#include "framework.h"
#include "DrawLandscape.h"
#include "../LandScape/Sky.h"
#include "../LandScape/Terrain.h"
#include "../Objects/MeshSphere.h"

DrawLandscape::DrawLandscape(ExecuteValues* values) : Execute(values)
{
	sky = new Sky(values);

	// Create Terrain
	{
		Material* material = new Material(Shaders + L"022_TerrainBrushLine.hlsl");
		
		terrain = new Terrain(values, material);
	}

	// Create Sphere
	//{
	//	sphere = new MeshSphere();
	//
	//	D3DXMATRIX S;
	//	D3DXMatrixScaling(&S, 3, 3, 3);
	//	sphere->RootAxis(S);
	//
	//	sphere->Position(D3DXVECTOR3(80, 10, 30));
	//}
}

DrawLandscape::~DrawLandscape()
{
	SAFE_DELETE(sky);
	SAFE_DELETE(terrain);
}

void DrawLandscape::Update()
{
	terrain->Update();
	sky->Update();
}

void DrawLandscape::PreRender()
{
}

void DrawLandscape::Render()
{
	sky->Render();
	terrain->Render();
	//sphere->Render();
}

void DrawLandscape::PostRender()
{
	D3DXVECTOR3 picked;

	// 현재 마우스가 가리키는 지점에 대한 terrain 상의 충돌 point가 나옴
	terrain->Y(&picked);

	ImGui::LabelText("Picked", "%.2f, %.2f, %.2f", picked.x, picked.y, picked.z);
}

void DrawLandscape::ResizeScreen()
{
}
