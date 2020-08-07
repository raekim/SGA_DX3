#include "framework.h"
#include "Program.h"
#include "Executes/ExeGrid.h"

Program::Program()
{
	States::Create();

	D3DDesc desc;
	D3D::GetDesc(&desc);

	values = new ExecuteValues();
	values->ViewProjection = new ViewProjectionBuffer();
	values->Perspective = new Perspective(desc.Width, desc.Height);
	values->Viewport = new Viewport(desc.Width, desc.Height);

	executes.push_back(new ExeGrid(values));

	position = D3DXVECTOR3(2.64f, 1.35f, -3.59f);
}

Program::~Program()
{
	for (Execute* exe : executes)
		SAFE_DELETE(exe);

	SAFE_DELETE(values->ViewProjection);
	SAFE_DELETE(values->Perspective);
	SAFE_DELETE(values->Viewport);
	SAFE_DELETE(values);

	States::Delete();
}

void Program::Update()
{
	// 회전
	if (Keyboard::Get()->Press(VK_SHIFT))
	{
		if (Keyboard::Get()->Press('S'))
		{
			rotation.x -= 10.0f * Time::Delta();
		}
		else if (Keyboard::Get()->Press('W'))
		{
			rotation.x += 10.0f * Time::Delta();
		}

		if (Keyboard::Get()->Press('Q'))
		{
			rotation.z += 10.0f * Time::Delta();
		}
		else if (Keyboard::Get()->Press('E'))
		{
			rotation.z -= 10.0f * Time::Delta();
		}

		if (Keyboard::Get()->Press('A'))
		{
			rotation.y += 10.0f * Time::Delta();
		}
		else if (Keyboard::Get()->Press('D'))
		{
			rotation.y -= 10.0f * Time::Delta();
		}
	}
	else
	{
		// 이동
		if (Keyboard::Get()->Press('A'))
		{
			position.x -= 10.0f * Time::Delta();
		}
		else if (Keyboard::Get()->Press('D'))
		{
			position.x += 10.0f * Time::Delta();
		}

		if (Keyboard::Get()->Press('W'))
		{
			position.z += 10.0f * Time::Delta();
		}
		else if (Keyboard::Get()->Press('S'))
		{
			position.z -= 10.0f * Time::Delta();
		}

		if (Keyboard::Get()->Press('Q'))
		{
			position.y -= 10.0f * Time::Delta();
		}
		else if (Keyboard::Get()->Press('E'))
		{
			position.y += 10.0f * Time::Delta();
		}
	}

	D3DXVECTOR3 forward(0, 0, 1);
	D3DXVECTOR3 right(1, 0, 0);
	D3DXVECTOR3 up(0, 1, 0);

	D3DXMatrixLookAtLH(&view, &position, &(position + forward), &up);

	//D3DXMATRIX X, Y, Z;
	//D3DXMatrixIdentity(&X);
	//D3DXMatrixIdentity(&Y);
	//D3DXMatrixIdentity(&Z);
	//
	//D3DXMatrixRotationX(&X, Math::ToRadian(rotation.x));
	//D3DXMatrixRotationY(&Y, Math::ToRadian(rotation.y));
	//D3DXMatrixRotationZ(&Z, Math::ToRadian(rotation.z));
	//
	//view *= X * Y * Z;
	

	for (Execute* exe : executes)
		exe->Update();
}

void Program::PreRender()
{

}

void Program::Render()
{
	D3DXMATRIX projection;
	values->Perspective->GetMatrix(&projection);

	values->ViewProjection->SetView(view);
	values->ViewProjection->SetProjection(projection);
	values->ViewProjection->SetVSBuffer(0);

	for (Execute* exe : executes)
		exe->Render();
}

void Program::PostRender()
{
	for (Execute* exe : executes)
		exe->PostRender();

	ImGui::Text("FPS : %f", Time::Get()->FPS());

	ImGui::Text("Camera : (%.2f, %.2f, %.2f)", position.x, position.y, position.z);
}

void Program::ResizeScreen()
{
	D3DDesc desc;
	D3D::GetDesc(&desc);

	values->Perspective->Set(desc.Width, desc.Height);
	values->Viewport->Set(desc.Width, desc.Height);

	for (Execute* exe : executes)
		exe->ResizeScreen();
}