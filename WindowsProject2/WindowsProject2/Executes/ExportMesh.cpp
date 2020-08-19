#include "framework.h"
#include "ExportMesh.h"
#include "../Fbx/Exporter.h"
#include "../Objects/GameModel.h"

ExportMesh::ExportMesh(ExecuteValues* values) : Execute(values)
{
	Fbx::Exporter* exporter = NULL;
	
	vector<wstring> str = { L"Capsule", L"Cube", L"Cylinder", L"Plane", L"Quad", L"Sphere", L"Teapot"};
	
	for (auto& modelName : str)
	{
		exporter = new Fbx::Exporter(Assets + L"Meshes/" + modelName + L".fbx");
	
		exporter->ExportMaterial(Materials + L"Meshes/", modelName + L".material");
		exporter->ExportMesh(Models + L"Meshes/", modelName + L".mesh");
	
		SAFE_DELETE(exporter);
	}

	//for (int i = 0; i < 20; ++i)
	//{
	//	GameModel* model = new GameModel(Materials + L"Meshes/", L"Teapot.material", Models + L"Meshes/", L"Teapot.mesh" );
	//
	//	// ¸ðµ¨ À§Ä¡ ·£´ý ¼³Á¤
	//	D3DXVECTOR3 temp;
	//	temp.x = Math::Random(-20, 20);
	//	temp.y = 0;
	//	temp.z = Math::Random(-20, 20);
	//	model->Position(temp);
	//
	//	// ¸ðµ¨ È¸Àü ·£´ý ¼³Á¤
	//	D3DXMATRIX R;
	//	D3DXMatrixRotationY(&R, Math::ToRadian(Math::Random(-180, 180)));
	//	model->RootAxis(R);
	//
	//	models.push_back(model);
	//}
	
	Models::Delete();
}

ExportMesh::~ExportMesh()
{
}

void ExportMesh::Update() 
{
}

void ExportMesh::PreRender()
{

}

void ExportMesh::Render()
{
}

void ExportMesh::PostRender()
{

}

void ExportMesh::ResizeScreen()
{

}
