#include "framework.h"
#include "ExeTexture.h"

ExeTexture::ExeTexture(ExecuteValues * values) : Execute(values), vertexCount(4), indexCount(6), number(0),
number2(0), location(0,0), rotation(0,0,0), scale(1,1)
{
	shader = new Shader(Shaders + L"006_TextureUv.hlsl");

	worldBuffer = new WorldBuffer();

	colorBuffer = new ColorBuffer();

	vertices = new VertexTexture[vertexCount];

	{
		float side = 1;
		float height = (sqrt(3) / 2)*side;

		vertices[0].Position = D3DXVECTOR3(-0.5f, -0.5f, 0);
		vertices[1].Position = D3DXVECTOR3(-0.5f, 0.5f, 0);
		vertices[2].Position = D3DXVECTOR3(0.5f, -0.5f, 0);
		vertices[3].Position = D3DXVECTOR3(0.5f, 0.5f, 0);

		vertices[0].Uv = D3DXVECTOR2(0, 1);
		vertices[1].Uv = D3DXVECTOR2(0, 0);
		vertices[2].Uv = D3DXVECTOR2(1, 1);
		vertices[3].Uv = D3DXVECTOR2(1, 0);
	}

	indices = new UINT[indexCount]{ 0,1,2, 2,1,3 };

	// Sampler State ����
	{
		D3D11_SAMPLER_DESC desc;
		States::GetSamplerDesc(&desc);
		States::CreateSampler(&desc, &state[0]);	// �⺻ ���ʹ� D3D11_FILTER_MIN_MAG_MIP_LINEAR (�ε巴�� �����ϴ� ��)
		
		desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		States::CreateSampler(&desc, &state[1]);
		//
		//desc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
		//desc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
		//States::CreateSampler(&desc, &state[2]);
		//
		//
		//D3DXCOLOR color = D3DXCOLOR(1, 0, 0, 1);	// border ����.. ���İ��� �ȸԴµ�
		//memcpy(desc.BorderColor, &color, sizeof(D3DXCOLOR));
		//desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		//desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		//States::CreateSampler(&desc, &state[3]);

		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;	// ����Ʈ�� �ִ��� �ȼ� �״�� ��ĥ��
		States::CreateSampler(&desc, &state[1]);
	}

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
		HRESULT hr = D3DX11CreateShaderResourceViewFromFile(D3D::GetDevice(), (Textures + L"Box.png").c_str(), NULL, NULL, &srv, NULL);
		assert(SUCCEEDED(hr));
	}

	// Load SRV2
	{
		HRESULT hr = D3DX11CreateShaderResourceViewFromFile(D3D::GetDevice(), (Textures + L"Wall.png").c_str(), NULL, NULL, &srv2, NULL);
		assert(SUCCEEDED(hr));
	}

	{
		D3D11_RASTERIZER_DESC desc;
		States::GetRasterizerDesc(&desc);
		States::CreateRasterizer(&desc, &rasterizer[0]);

		desc.FillMode = D3D11_FILL_WIREFRAME;
		States::CreateRasterizer(&desc, &rasterizer[1]);
	}
}

ExeTexture::~ExeTexture()
{
	SAFE_RELEASE(indexBuffer);
	SAFE_RELEASE(vertexBuffer);

	SAFE_DELETE_ARRAY(indices);
	SAFE_DELETE_ARRAY(vertices);

	SAFE_DELETE(colorBuffer);
	SAFE_DELETE(worldBuffer);
	SAFE_DELETE(shader);
}

void ExeTexture::Update()
{
	//D3D::GetDC()->UpdateSubresource(vertexBuffer, 0, NULL, vertices, sizeof(VertexColor)*vertexCount, 0);

	D3DXMATRIX Y, Z;
	D3DXMatrixIdentity(&Y);
	D3DXMatrixIdentity(&Z);

	D3DXMATRIX T;
	D3DXMatrixIdentity(&T);

	if (Keyboard::Get()->Press(VK_SHIFT))
	{
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
	}
	else
	{
		if (Keyboard::Get()->Press('A'))
		{
			location.x -= 3.0f * Time::Delta();
		}
		else if (Keyboard::Get()->Press('D'))
		{
			location.x += 3.0f * Time::Delta();
		}

		if (Keyboard::Get()->Press('W'))
		{
			location.y += 3.0f * Time::Delta();
		}
		else if (Keyboard::Get()->Press('S'))
		{
			location.y -= 3.0f * Time::Delta();
		}
	}

	D3DXMatrixTranslation(&T, location.x, location.y, 0);

	D3DXMatrixRotationY(&Y, Math::ToRadian(rotation.y));
	D3DXMatrixRotationZ(&Z, Math::ToRadian(rotation.z));

	world = Y * Z * T;
}

void ExeTexture::PreRender()
{
}

void ExeTexture::Render()
{
	UINT stride = sizeof(VertexTexture);
	UINT offset = 0;

	// IA : Input-Assembler Stage
	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	colorBuffer->SetPSBuffer(0);	// ���̴��� �÷� constant ���۷� ����
	worldBuffer->SetVSBuffer(1);
	shader->Render();

	D3D::GetDC()->PSSetSamplers(0, 1, &state[number]);
	D3D::GetDC()->PSSetShaderResources(0, 1, &srv);	// ���̴��� Texture2D Map ����
	D3D::GetDC()->PSSetShaderResources(1, 1, &srv2);	// ���̴��� Texture2D Map2 ����

	// �����Ͷ����� ������Ʈ ����
	D3D::GetDC()->RSSetState(rasterizer[number2]);
	worldBuffer->SetMatrix(world);
	worldBuffer->SetVSBuffer(1);

	D3D::GetDC()->DrawIndexed(indexCount, 0, 0);
}

void ExeTexture::PostRender()
{
	ImGui::Begin("Color");
	{
		ImGui::SliderInt("Number", &number, 0, 1);
		ImGui::SliderInt("Number2", &number2, 0, 1);
	}
	ImGui::End();
}

void ExeTexture::ResizeScreen()
{
}
