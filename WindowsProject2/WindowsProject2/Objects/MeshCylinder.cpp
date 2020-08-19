#include "framework.h"
#include "MeshCylinder.h"

MeshCylinder::MeshCylinder()
	: GameModel(Materials + L"Meshes/", L"Cylinder.material", Models + L"Meshes/", L"Cylinder.mesh")
{
	shader = new Shader(Shaders + L"017_Sphere.hlsl");

	for (Material* material : model->Materials())
	{
		material->SetDiffuse(D3DXCOLOR(1, 0, 0, 1));
		material->SetShader(shader);
	}
}

MeshCylinder::~MeshCylinder()
{
	SAFE_DELETE(shader);
}

void MeshCylinder::Update()
{
	__super::Update();
}

void MeshCylinder::Render()
{
	__super::Render();
}
