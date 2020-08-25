#include "framework.h"
#include "Billboard.h"

Billboard::Billboard(ExecuteValues* values, wstring shaderFile, wstring diffuseFile)
	:GameModel(Materials + L"Meshes/", L"Cube.material", Models + L"Meshes/" , L"Cube.mesh")
{
	for (Material* material : model->Materials())
	{
		material->SetShader(shaderFile);
		material->SetDiffuseMap(diffuseFile);

		material->SetDiffuse(D3DXCOLOR(1, 0, 0, 1));
		material->SetShader(shader);
	}
}

Billboard::~Billboard()
{
}

void Billboard::Update()
{
	RotateDegree(D3DXVECTOR2(45, 45));
	__super::Update();
}

void Billboard::Render()
{
	__super::Render();
}
