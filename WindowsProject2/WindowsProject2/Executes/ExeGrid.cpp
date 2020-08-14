#include "framework.h"
#include "ExeGrid.h"

ExeGrid::ExeGrid(ExecuteValues * values) 
	: Execute(values), materialSelectNum(0)
{
	Texture* heightMap = new Texture(Contents + L"HeightMaps/HeightMap_Edit.png");

	vector<D3DXCOLOR> heights;
	heightMap->ReadPixels(DXGI_FORMAT_R8G8B8A8_UNORM, &heights);
	
	int mul = 20;

	width = heightMap->GetWidth() * mul - 1;
	height = heightMap->GetHeight() * mul - 1;

	material = new Material(Shaders + L"Floor.hlsl");
	material->SetDiffuseMap(Textures + L"Floor.png");

	worldBuffer = new WorldBuffer();

	texture = new Texture(Textures + L"Numbers.png");

	// Create VertexData
	{
		vertexCount = (width + 1) * (height + 1);
		vertices = new VertexType[vertexCount];

		for (UINT z = 0; z <= height; ++z)
		{
			for (UINT x = 0; x <= width; ++x)
			{
				UINT index = (width + 1) * z + x; // ���ؽ��� �ε���
				UINT rIndex = 16 * (z / mul) + (x / mul);	// �÷� r�� �ε���

				vertices[index].Position.x = (float)x;
				// (255/7) ���� ���̸� 256 �ܰ�� ������ ���̷� �Է�
				vertices[index].Position.y = (float)(heights[rIndex].r * 255) / 25;
				vertices[index].Position.z = (float)z;

				vertices[index].Uv.x = (float)x / width;
				vertices[index].Uv.y = height - (float)z / height;
			}
		}
	}

	// Create IndexData
	{
		indexCount = width * height * 6;	// grid ĭ �� * �ϳ��� ĭ�� �׸��� �� �ﰢ�� 2��, �� 6���� �ε��� �ʿ�
		indices = new UINT[indexCount];

		UINT index = 0;
		for (UINT z = 0; z < height; ++z)
		{
			for (UINT x = 0; x < width; ++x)
			{
				indices[index + 0] = (width + 1) * z + x;	// ���� �� (�ش� ĭ�� �� �׸��� ��������)
				indices[index + 1] = (width + 1) * (z + 1) + x;
				indices[index + 2] = (width + 1) * z + x + 1;
				indices[index + 3] = (width + 1) * z + x + 1;
				indices[index + 4] = (width + 1) * (z + 1) + x;
				indices[index + 5] = (width + 1) * (z + 1) + x + 1;

				index += 6;
			}
		}
	}

	CreateNormal();

	// Sampler State ���� (floor �ؽ��� �ݺ��� ���� ��)
	{
		D3D11_SAMPLER_DESC desc;
		States::GetSamplerDesc(&desc);
		desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		States::CreateSampler(&desc, &state);
	}

	// CreateVertexBuffer;
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(VertexType) * vertexCount;
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
}

ExeGrid::~ExeGrid()
{
	SAFE_RELEASE(indexBuffer);
	SAFE_RELEASE(vertexBuffer);

	SAFE_DELETE_ARRAY(indices);
	SAFE_DELETE_ARRAY(vertices);

	SAFE_DELETE(worldBuffer);
}

void ExeGrid::Update()
{

}

void ExeGrid::PreRender()
{
}

void ExeGrid::Render()
{
	UINT stride = sizeof(VertexType);
	UINT offset = 0;

	// IA : Input-Assembler Stage
	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	texture->SetShaderResource(1);

	material->PSSetBuffer();
	worldBuffer->SetVSBuffer(1);

	D3D::GetDC()->PSSetSamplers(0, 1, &state);
	D3D::GetDC()->DrawIndexed(indexCount, 0, 0);
}	

void ExeGrid::PostRender()
{
	ImGui::SliderFloat3("Diffuse", (float*)material->GetDiffuse(), 0, 1);
}

void ExeGrid::ResizeScreen()
{
}

void ExeGrid::CreateNormal()
{
	// ������ �븻 ���� ���ϱ�
	// �� ���� ���� ����� ���� ��� �븻���͸� ���� ��
	for (UINT i = 0; i < (indexCount / 3); ++i)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		VertexTextureNormal v0 = vertices[index0];
		VertexTextureNormal v1 = vertices[index1];
		VertexTextureNormal v2 = vertices[index2];

		D3DXVECTOR3 d1 = v1.Position - v0.Position;
		D3DXVECTOR3 d2 = v2.Position - v0.Position;

		D3DXVECTOR3 normal;
		D3DXVec3Cross(&normal, &d1, &d2);

		// ���� ��
		vertices[index0].Normal += normal;
		vertices[index1].Normal += normal;
		vertices[index2].Normal += normal;
	}

	// ���� 1�� ���ͷ� ����ȭ
	for (UINT i = 0; i < vertexCount; ++i)
	{
		D3DXVec3Normalize(&vertices[i].Normal, &vertices[i].Normal);
	}
}