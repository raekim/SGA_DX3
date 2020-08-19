#include "framework.h"
#include "MeshCapsule.h"

MeshCapsule::MeshCapsule()
	: GameModel(Materials + L"Meshes/", L"Capsule.material", Models + L"Meshes/", L"Capsule.mesh")
{
	shader = new Shader(Shaders + L"017_Sphere.hlsl");

	for (Material* material : model->Materials())
	{
		material->SetDiffuse(D3DXCOLOR(1, 0, 0, 1));
		material->SetShader(shader);
	}
}

MeshCapsule::~MeshCapsule()
{
	SAFE_DELETE(shader);
}

void MeshCapsule::Update()
{
	__super::Update();
}

void MeshCapsule::Render()
{
	__super::Render();
}
