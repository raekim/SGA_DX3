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

	//// �� ��Ʈ������ �޾ƿ´�
	//D3DXMATRIX matView;
	//D3DXMatrixIdentity(&matView);
	//values->MainCamera->Matrix(&matView);
	//
	//// �� ��Ʈ������ �̵����� ����
	//memset(&matView._41, 0, sizeof(D3DXVECTOR3));
	//
	//// ������� ����
	//D3DXMatrixInverse(&matView, NULL, &matView);
	//
	//// ��ġ���� �޾ƿͼ� ��Ʈ���� ����
	//D3DXVECTOR3 BillPos = Position();
	//memcpy(&matView._41, &BillPos, sizeof(D3DXVECTOR3));
	//
	//// ũ�⸦ 10��
	//D3DXVECTOR3 scale = Scale();
	//fScale[0] = scale.x;
	//fScale[1] = scale.y;
	//fScale[2] = scale.z; //0.0f;	// ť�긦 �ǳ�ó�� ���̱� ���ؼ� 0���� ��
	//
	//// ũ�� ����
	//for (int i = 0; i < 3; ++i)
	//{
	//	for (int j = 0; j < 4; ++j)
	//		matView(i, j) *= fScale[i];
	//}
	//
	//// ����� ��Ʈ������ ����
	//World(matView);

	__super::Update();
}

void Billboard::Render()
{
	__super::Render();
}
