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

	// Create Sky
	{
		sky = new Sky(values);
	}

	// Create Treees
	{
		for (int i = 0; i < 20; ++i)
		{
			Billboard* tree = new  Billboard(values, Shaders + L"024_Billboard.hlsl",
				Textures + L"Tree.png");

			D3DXVECTOR3 P;
			P.x = Math::Random(0.0f, 255.0f);
			P.z = Math::Random(0.0f, 255.0f);
			P.y = terrain->Y(P);

			D3DXVECTOR3 S;
			//S.x = Math::Random(10.0f, 30.0f);
			//S.y = Math::Random(10.0f, 30.0f);
			S.x = Math::Random(0.1f, 0.3f);
			S.y = Math::Random(0.1f, 0.3f);
			S.z = 0.1f;

			//P.y += S.y *0.5f;
			P.y += S.y * 80.0f;

			tree->Position(P);
			tree->Scale(S);

			trees.push_back(tree);
		}
	}
}

DrawLandscape::~DrawLandscape()
{
	for (Billboard* tree : trees)
		SAFE_DELETE(tree);

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

	for (Billboard* tree : trees)
		tree->Update();
}

void DrawLandscape::PreRender()
{
}

void DrawLandscape::Render()
{
	sky->Render();
	terrain->Render();
	sphere->Render();

	for (Billboard* tree : trees)
		tree->Render();
}

void DrawLandscape::PostRender()
{
	if (terrain->GetBrushBuffer()->Data.Type > 0)
	{
		D3DXVECTOR3 picked;

		// 현재 마우스가 가리키는 지점에 대한 terrain 상의 충돌 point가 나옴
		terrain->Y(&picked);
		ImGui::LabelText("Picked", "%.2f, %.2f, %.2f", picked.x, picked.y, picked.z);
	}

	//snapRange
	ImGui::SliderFloat("Snap Range", &snapRange, 0.5f, 5.0f);
}

void DrawLandscape::ResizeScreen()
{
}
