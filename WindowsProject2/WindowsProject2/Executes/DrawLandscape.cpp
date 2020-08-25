#include "framework.h"
#include "DrawLandscape.h"
#include "../LandScape/Sky.h"
#include "../LandScape/Terrain.h"
#include "../Objects/MeshSphere.h"
#include "../Objects/Billboard.h"

DrawLandscape::DrawLandscape(ExecuteValues* values) : Execute(values), snapped(true), canMove(true), spherePos({0,0,0}),
snapRange(1.5f)
{
	// Create Terrain
	{
		Material* material = new Material(Shaders + L"022_TerrainBrushLine.hlsl");
		
		terrain = new Terrain(values, material);
	}

	 //Create Sphere
	{
		sphere = new MeshSphere();
	
		D3DXMATRIX S;
		D3DXMatrixScaling(&S, 3, 3, 3);
		sphere->RootAxis(S);
	
		sphere->Position(D3DXVECTOR3(0, 0, 0));
	}

	{
		sky = new Sky(values);
	}

	{
		for (int i = 0; i < 20; ++i)
		{
			Billboard* tree = new  Billboard(values, Shaders + L"024_Billboard.hlsl",
				Textures + L"Tree.png");

		}
	}
}

DrawLandscape::~DrawLandscape()
{
	SAFE_DELETE(sky);
	SAFE_DELETE(terrain);
}

void DrawLandscape::Update()
{
	//D3DXVECTOR3 position = sphere->Position();

	if (Keyboard::Get()->Press('I'))
		spherePos.z += sphereSpeed * Time::Delta();
	if (Keyboard::Get()->Press('K'))
		spherePos.z -= sphereSpeed * Time::Delta();

	if (Keyboard::Get()->Press('J'))
		spherePos.x -= sphereSpeed * Time::Delta();
	if (Keyboard::Get()->Press('L'))
		spherePos.x += sphereSpeed * Time::Delta();

	D3DXVECTOR3 newPosition = spherePos;

	// Snap to grid
	{
		int spacing = terrain->GetGridLineSpacing();

		// grid 간격에 따라 이동속도 조절
		sphereSpeed = spacing * 1.3f;

		int snapX = floor(spherePos.x / spacing + 0.5f) * spacing;	// 가까운 세로 선
		int snapZ = floor(spherePos.z / spacing + 0.5f) * spacing;	// 가까운 가로 선

		if(abs(newPosition.x - snapX) < snapRange)
			newPosition.x = snapX;

		if (abs(newPosition.z - snapZ) < snapRange)
			newPosition.z = snapZ;
	}

	//position.y = terrain->Y(position);

	if (terrain->Y(&newPosition, newPosition) == true)
		spherePos.y = newPosition.y;

	sky->Update();
	sphere->Position(newPosition);
	terrain->Update();
	sphere->Update();
}

void DrawLandscape::PreRender()
{
}

void DrawLandscape::Render()
{
	sky->Render();
	terrain->Render();
	sphere->Render();
}

void DrawLandscape::PostRender()
{
	D3DXVECTOR3 picked;

	// 현재 마우스가 가리키는 지점에 대한 terrain 상의 충돌 point가 나옴
	terrain->Y(&picked);

	//snapRange
	ImGui::SliderFloat("Snap Range", &snapRange, 0.5f, 5.0f);
	ImGui::LabelText("Picked", "%.2f, %.2f, %.2f", picked.x, picked.y, picked.z);
}

void DrawLandscape::ResizeScreen()
{
}
