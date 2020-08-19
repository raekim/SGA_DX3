#include "framework.h"
#include "DrawLandscape.h"
#include "../Fbx/Exporter.h"
#include "../LandScape/Terrain.h"
#include "../Objects/MeshSphere.h"
#include "../Objects/MeshCylinder.h"
#include "../Objects/MeshCapsule.h"
#include "../Objects/GameTank.h"

DrawLandscape::DrawLandscape(ExecuteValues* values) : Execute(values), selectNum(0)
{
	// Create Terrain
	{
		wstring heightMap = Contents + L"HeightMaps/HeightMap256.png";	// heightMap의 경로
		Material* material = new Material(Shaders + L"017_Terrain.hlsl");
		material->SetDiffuseMap(Textures + L"Dirt2.png");
		
		terrain = new Terrain(material, heightMap);
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

	// Create Tank
	{
		gameModels.push_back(new GameTank(Models + L"Tank/", L"Tank.material"));
		gameModels.push_back(new MeshCapsule());
		gameModels.push_back(new MeshCylinder());

		D3DXMATRIX S;
		D3DXMatrixScaling(&S, 3, 3, 3);

		for (auto& model : gameModels)
		{
			model->RootAxis(S);
			model->Position(D3DXVECTOR3(80, 10, 30));
		}
	}
}

DrawLandscape::~DrawLandscape()
{
	SAFE_DELETE(terrain);
}

void DrawLandscape::Update()
{
	D3DXVECTOR3 position = gameModels[0]->Position();

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

	position.y = terrain->Y(position);

	//D3DXVECTOR3 newPosition;
	//if (terrain->Y(&newPosition, position))
	//{
	//	position.y = newPosition.y;
	//}

	gameModels[selectNum]->Position(position);

	terrain->Update();

	for (auto& model : gameModels)
	{
		model->Update();
	}
}

void DrawLandscape::PreRender()
{
}

void DrawLandscape::Render()
{
	terrain->Render();

	// 선택 한 모델만 렌더
	gameModels[selectNum]->Render();
}

void DrawLandscape::PostRender()
{
	ImGui::SliderInt("Model", &selectNum, 0, 2);
}

void DrawLandscape::ResizeScreen()
{
}
