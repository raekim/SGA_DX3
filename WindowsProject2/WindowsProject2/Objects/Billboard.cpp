#include "framework.h"
#include "Billboard.h"

Billboard::Billboard(ExecuteValues* values, wstring shaderFile, wstring diffuseFile)
	:GameModel(Materials + L"Meshes/", L"Cube.material", Models + L"Meshes/" , L"Cube.mesh"), values(values)
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
	//RotateDegree(D3DXVECTOR2(45, 45));

	//// 뷰 매트릭스를 받아온다
	//D3DXMATRIX matView;
	//D3DXMatrixIdentity(&matView);
	//values->MainCamera->Matrix(&matView);
	//
	//// 뷰 매트릭스의 이동값을 제외
	//memset(&matView._41, 0, sizeof(D3DXVECTOR3));
	//
	//// 역행렬을 구함
	//D3DXMatrixInverse(&matView, NULL, &matView);
	//
	//// 위치값을 받아와서 매트릭스 적용
	//D3DXVECTOR3 BillPos = Position();
	//memcpy(&matView._41, &BillPos, sizeof(D3DXVECTOR3));
	//
	//// 크기를 10배
	//D3DXVECTOR3 scale = Scale();
	//fScale[0] = scale.x;
	//fScale[1] = scale.y;
	//fScale[2] = scale.z; //0.0f;	// 큐브를 판넬처럼 보이기 위해서 0으로 줌
	//
	//// 크기 적용
	//for (int i = 0; i < 3; ++i)
	//{
	//	for (int j = 0; j < 4; ++j)
	//		matView(i, j) *= fScale[i];
	//}
	//
	//// 연산된 매트릭스를 적용
	//World(matView);

	__super::Update();
}

void Billboard::Render()
{
	__super::Render();
}
