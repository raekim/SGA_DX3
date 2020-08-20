#include "framework.h"
#include "DrawLandscape.h"
#include "../Fbx/Exporter.h"
#include "../LandScape/Terrain.h"
#include "../Objects/MeshSphere.h"

DrawLandscape::DrawLandscape(ExecuteValues* values) : Execute(values)
{
	// Create Terrain
	{
		Material* material = new Material(Shaders + L"020_TerrainSplatting.hlsl");	// 019_TerrainBrush.hlsl 적용하면 브러쉬 컬러 적용 가능
		
		terrain = new Terrain(values, material);
	}

	// Create Sphere
	{
		sphere = new MeshSphere();

		D3DXMATRIX S;
		D3DXMatrixScaling(&S, 3, 3, 3);
		sphere->RootAxis(S);

		sphere->Position(D3DXVECTOR3(80, 10, 30));
	}
}

DrawLandscape::~DrawLandscape()
{
	SAFE_DELETE(sphere);
	SAFE_DELETE(terrain);
}

void DrawLandscape::Update()
{
	D3DXVECTOR3 position = sphere->Position();

	if (Keyboard::Get()->Press('I'))
	{
		position.z += 10.0f * Time::Delta();
	}
	else if (Keyboard::Get()->Press('K'))
	{
		position.z -= 10.0f * Time::Delta();
	}

	if (Keyboard::Get()->Press('J'))
	{
		position.x -= 10.0f * Time::Delta();
	}
	else if (Keyboard::Get()->Press('L'))
	{
		position.x += 10.0f * Time::Delta();
	}

	//position.y = terrain->Y(position);

	D3DXVECTOR3 newPosition;
	if (terrain->Y(&newPosition, position))
	{
		position.y = newPosition.y;
	}

	sphere->Position(position);

	terrain->Update();
	sphere->Update();
}

void DrawLandscape::PreRender()
{
}

void DrawLandscape::Render()
{
	terrain->Render();
	sphere->Render();
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
