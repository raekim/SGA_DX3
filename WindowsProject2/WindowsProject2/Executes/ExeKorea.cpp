#include "framework.h"
#include "ExeKorea.h"

ExeKorea::ExeKorea(ExecuteValues * values) : Execute(values), vertexCount(7), indexCount(3*6), number2(0)
{
	shader = new Shader(Shaders + L"004_Texture.hlsl");

	worldBuffer = new WorldBuffer();

	colorBuffer = new ColorBuffer();

	vertices = new VertexTexture[vertexCount];

	{
		float side = 1;
		float height = (sqrt(3) / 2)*side;

		vertices[0].Position = D3DXVECTOR3(0, 0, 0);
		vertices[1].Position = D3DXVECTOR3(-side, 0, 0);
		vertices[2].Position = D3DXVECTOR3(-side / 2, height, 0);
		vertices[3].Position = D3DXVECTOR3(side / 2, height, 0);
		vertices[4].Position = D3DXVECTOR3(side, 0, 0);
		vertices[5].Position = D3DXVECTOR3(side / 2, -height, 0);
		vertices[6].Position = D3DXVECTOR3(-side / 2, -height, 0);

		vertices[0].Uv = D3DXVECTOR2(0.5, 0.5);
		vertices[1].Uv = D3DXVECTOR2(0, 0.5);
		vertices[2].Uv = D3DXVECTOR2(0.5 - 0.25, 0);
		vertices[3].Uv = D3DXVECTOR2(0.5 + 0.25, 0);
		vertices[4].Uv = D3DXVECTOR2(1, 0.5);
		vertices[5].Uv = D3DXVECTOR2(0.5 + 0.25, 1);
		vertices[6].Uv = D3DXVECTOR2(0.5 - 0.25, 1);
	}

	indices = new UINT[indexCount]{ 2,0,1, 2,3,0, 0,3,4, 5,0,4, 0,5,6, 1,0,6 };

	// CreateVertexBuffer;
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(VertexTexture) * vertexCount;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = vertices;

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
		assert(SUCCEEDED(hr));
	}

	// CreateIndexBuffer;
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(UINT) * indexCount;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = indices;

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &indexBuffer);
		assert(SUCCEEDED(hr));
	}

	// Load SRV
	{
		HRESULT hr = D3DX11CreateShaderResourceViewFromFile(D3D::GetDevice(), (Textures + L"korea.png").c_str(), NULL, NULL, &SRV, NULL);
		assert(SUCCEEDED(hr));
	}

	// 래스터라이저 스테이트
	{
		// 기본
		D3D11_RASTERIZER_DESC desc;
		States::GetRasterizerDesc(&desc);
		States::CreateRasterizer(&desc, &rasterizer[0]);

		// CULLMODE NONE (앞,뒷면 보이기)
		desc.CullMode = D3D11_CULL_NONE;
		States::CreateRasterizer(&desc, &rasterizer[1]);

		// 와이어프레임
		desc.FillMode = D3D11_FILL_WIREFRAME;
		States::CreateRasterizer(&desc, &rasterizer[2]);
	}
}

ExeKorea::~ExeKorea()
{
	SAFE_RELEASE(indexBuffer);
	SAFE_RELEASE(vertexBuffer);

	SAFE_DELETE_ARRAY(indices);
	SAFE_DELETE_ARRAY(vertices);

	SAFE_DELETE(colorBuffer);
	SAFE_DELETE(worldBuffer);
	SAFE_DELETE(shader);
}

void ExeKorea::Update()
{
	// 회전
	D3DXMATRIX Y, Z;
	D3DXMatrixIdentity(&Y);
	D3DXMatrixIdentity(&Z);

	if (Keyboard::Get()->Press('A'))
	{
		rotation.y -= 30.0f * Time::Delta();
	}
	else if (Keyboard::Get()->Press('D'))
	{
		rotation.y += 30.0f * Time::Delta();
	}

	if (Keyboard::Get()->Press('W'))
	{
		rotation.z -= 30.0f * Time::Delta();
	}
	else if (Keyboard::Get()->Press('S'))
	{
		rotation.z += 30.0f * Time::Delta();
	}

	D3DXMatrixRotationY(&Y, Math::ToRadian(rotation.y));
	D3DXMatrixRotationZ(&Z, Math::ToRadian(rotation.z));

	world = Y * Z;
}

void ExeKorea::PreRender()
{
}

void ExeKorea::Render()
{
	UINT stride = sizeof(VertexTexture);
	UINT offset = 0;

	// IA : Input-Assembler Stage, 렌더링 파이프라인의 가장 첫 번째 스테이지
	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	colorBuffer->SetPSBuffer(0);	// 셰이더의 컬러 constant 버퍼로 간다
	worldBuffer->SetVSBuffer(1);
	shader->Render();

	D3D::GetDC()->PSSetShaderResources(0, 1, &SRV);	// 셰이더에 Texture2D Map 전달

	// 래스터라이저 스테이트 설정
	D3D::GetDC()->RSSetState(rasterizer[number2]);
	worldBuffer->SetMatrix(world);
	worldBuffer->SetVSBuffer(1);

	D3D::GetDC()->DrawIndexed(indexCount, 0, 0);
}

void ExeKorea::PostRender()
{
	ImGui::Begin("Color");
	{
		ImGui::SliderInt("Number2", &number2, 0, 2);
	}
	ImGui::End();
}

void ExeKorea::ResizeScreen()
{
}
