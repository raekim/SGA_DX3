#include "framework.h"
#include "Camera.h"

Camera::Camera()
	: position(0, 0, 0), rotation(0, 0)
	, forward(0, 0, 1), right(1, 0, 0), up(0, 1, 0)
	, pivot(NULL), pivotDistance(5)
{
	D3DXMatrixIdentity(&matRotation);
	D3DXMatrixIdentity(&matView);

	Move();
	Rotation();
}

Camera::~Camera()
{
}

D3DXVECTOR3 Camera::Direction(Viewport* vp, Perspective* perspective)
{
	D3DXVECTOR2 screenSize;
	screenSize.x = vp->GetWidth();
	screenSize.y = vp->GetHeight();
	
	D3DXVECTOR3 mouse = Mouse::Get()->GetPosition();
	
	D3DXVECTOR2 point;
	
	//Viewport Invert
	{
		point.x = ((2.0f * mouse.x) / screenSize.x) - 1.0f;
		point.y = (((2.0f * mouse.y) / screenSize.y) - 1.0f) * -1.0f;
	}
	
	//projection Invert
	{
		D3DXMATRIX projection;
		perspective->GetMatrix(&projection);
	
		point.x = point.x / projection._11;
		point.y = point.y / projection._22;
	}
	
	D3DXVECTOR3 dir(0, 0, 0);
	
	
	{
		D3DXMATRIX invView;
		D3DXMatrixInverse(&invView, NULL, &matView);
	
		D3DXVec3TransformNormal(&dir, &D3DXVECTOR3(point.x, point.y, 1), &invView);
		D3DXVec3Normalize(&dir, &dir);
	}
	
	return dir;
	return D3DXVECTOR3();
}

void Camera::Move()
{
	View();
}

void Camera::Rotation()
{
	D3DXMATRIX x, y;
	D3DXMatrixRotationX(&x, rotation.x);
	D3DXMatrixRotationY(&y, rotation.y);

	matRotation = x * y;

	D3DXVec3TransformNormal(&forward, &D3DXVECTOR3(0, 0, 1), &matRotation);
	D3DXVec3TransformNormal(&right, &D3DXVECTOR3(1, 0, 0), &matRotation);
	D3DXVec3TransformNormal(&up, &D3DXVECTOR3(0, 1, 0), &matRotation);

	D3DXMATRIX pivotRotMatrix;
	D3DXMatrixIdentity(&pivotRotMatrix);
	D3DXMatrixRotationYawPitchRoll(&pivotRotMatrix, rotation.y, rotation.x, 0.0f);

	if (IsPivot())
		matView = pivotRotMatrix * matView;

	
	//D3DXVec3TransformNormal�� 2��° ������ ���Ϳ� 3��° ������ ��Ʈ������ ���ϰ� 1��° ���ڿ� �ִ´�.
	//D3DXVec3TransformCoord�� 2��° ������ ���Ϳ� 3��° ������ ��Ʈ������ ������ 1��° ���ڿ��ִ´�.
	//�̵� �Լ��� �������� D3DXVec3TransformCoord�� ��Ʈ������ 4��°(w)�� 1�� ����� �������� ��ǥ������ ������.
	//D3DXVec3TransformNormal�� 4��°(w)�� 0���� ����� �������� ���ⰳ���� ������.


}

void Camera::View()
{
	if(!IsPivot())
		D3DXMatrixLookAtLH(&matView, &position, &(position + forward), &up);
	else
	{
		D3DXMatrixLookAtLH(&matView, &position, pivot, &up);
	}
}
