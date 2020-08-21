#include "framework.h"
#include "Sky.h"
#include "../Objects/GameModel.h"

Sky::Sky(ExecuteValues* values) : values(values)
{
	model = new GameModel(Materials + L"Meshes/", L"Sphere.material", Models + L"Meshes/", L"Sphere.mesh");

	for (Material* material : model->GetModel()->Materials())
	{
		material->SetShader(Shaders + L"021_Sky.hlsl");
	}

	buffer = new Buffer();

	// Create Rasterizer
	{
		D3D11_RASTERIZER_DESC desc;
		States::GetRasterizerDesc(&desc);
		States::CreateRasterizer(&desc, &cullMode[0]);

		desc.FrontCounterClockwise = true;
		States::CreateRasterizer(&desc, &cullMode[1]);
	}

	// Create DepthState
	{
		D3D11_DEPTH_STENCIL_DESC desc;
		States::GetDepthStencilDesc(&desc);
		States::CreateDepthStencil(&desc, &depthMode[0]);

		desc.DepthEnable = false;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		States::CreateDepthStencil(&desc, &depthMode[1]);
	}
}

Sky::~Sky()
{
	SAFE_DELETE(buffer);
}

void Sky::Update()
{
	D3DXVECTOR3 camPos;
	values->MainCamera->Position(&camPos);	// 메인카메라의 포지션을 받아온다

	model->Position(camPos);
	model->Update();
}

void Sky::Render()
{
	ImGui::Separator();
	ImGui::Text("Sky");
	ImGui::Separator();

	ImGui::SliderFloat3("Apex", (float*)buffer->Data.Apex, 0, 1);
	ImGui::SliderFloat3("Center", (float*)buffer->Data.Center, 0, 1);
	ImGui::SliderFloat("Height", &buffer->Data.Height, 0, 10);
	ImGui::Separator();

	buffer->SetPSBuffer(10);

	// model render 할 때만 [1] 설정 적용
	D3D::GetDC()->OMSetDepthStencilState(depthMode[1], 0xFF);	// OM : output merge state
	D3D::GetDC()->RSSetState(cullMode[1]);
	{
		model->Render();
	}

	// default 설정으로 돌아감
	D3D::GetDC()->RSSetState(cullMode[0]);
	D3D::GetDC()->OMSetDepthStencilState(depthMode[0], 0xFF);
}
