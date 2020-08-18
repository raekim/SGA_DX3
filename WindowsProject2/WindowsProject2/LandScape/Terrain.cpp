#include "framework.h"
#include "Terrain.h"

Terrain::Terrain(Material* material, wstring heightMap)
	: material(material)
{
	heightTexture = new Texture(heightMap);
	worldBuffer = new WorldBuffer();

	CreateData();
	CreateBuffer();

	//
	//
	
	//
	//// CreateVertexBuffer;
	//{
	//	D3D11_BUFFER_DESC desc = { 0 };
	//	desc.Usage = D3D11_USAGE_DEFAULT;
	//	desc.ByteWidth = sizeof(VertexColor) * vertexCount;
	//	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//
	//	D3D11_SUBRESOURCE_DATA data = { 0 };
	//	data.pSysMem = vertices;
	//
	//	HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
	//	assert(SUCCEEDED(hr));
	//}
	//
	//// CreateIndexBuffer;
	//{
	//	D3D11_BUFFER_DESC desc = { 0 };
	//	desc.Usage = D3D11_USAGE_DEFAULT;
	//	desc.ByteWidth = sizeof(UINT) * indexCount;
	//	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//
	//	D3D11_SUBRESOURCE_DATA data = { 0 };
	//	data.pSysMem = indices;
	//
	//	HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &indexBuffer);
	//	assert(SUCCEEDED(hr));
	//}
	//
	//{
	//	D3D11_RASTERIZER_DESC desc;
	//	States::GetRasterizerDesc(&desc);
	//	States::CreateRasterizer(&desc, &fillMode[0]);
	//
	//	desc.FillMode = D3D11_FILL_WIREFRAME;
	//	States::CreateRasterizer(&desc, &fillMode[1]);
	//}
}

Terrain::~Terrain()
{
	SAFE_DELETE(material);
	SAFE_DELETE(heightTexture);
	SAFE_DELETE(worldBuffer);
}

void Terrain::Update()
{

}


void Terrain::Render()
{
	//UINT stride = sizeof(VertexColor);
	//UINT offset = 0;
	//
	//// IA : Input-Assembler Stage
	//D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	//D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//
	//D3D::GetDC()->RSSetState(fillMode[fillModeNumber]);
	//
	//colorBuffer->SetPSBuffer(0);	// ���̴��� �÷� constant ���۷� ����
	//worldBuffer->SetVSBuffer(1);
	//shader->Render();
	//
	//D3D::GetDC()->DrawIndexed(indexCount, 0, 0);
}
float Terrain::Y(D3DXVECTOR3 & position)
{
	return 0.0f;
}

bool Terrain::Y(OUT D3DXVECTOR3 * out, D3DXVECTOR3 & position)
{
	return false;
}

void Terrain::CreateData()
{
	vector<D3DXCOLOR> heights;
	heightTexture->ReadPixels(DXGI_FORMAT_R8G8B8A8_UNORM, &heights);
	// �ؽ��� �̹����� �ȼ� �� ��ŭ ���ؽ��� ����� ����
	width = heightTexture->GetWidth() - 1;
	height = heightTexture->GetHeight() - 1;
	
	// Create VertexData
	{
		vertices.assign((width + 1) *(height + 1), VertexTextureNormal());
	
		for (UINT z = 0; z <= height; ++z)
		{
			for (UINT x = 0; x <= width; ++x)
			{
				UINT index = (width + 1) * z + x;
	
				vertices[index].Position.x = (float)x; // (float)x * (unit meter)
				vertices[index].Position.y = (float)(heights[index].r * 255.0f) / 10;
				vertices[index].Position.z = (float)z;

				vertices[index].Uv.x = (float)x / width;
				vertices[index].Uv.y = (height - (float)z) / height; // (float)z / height
			}
		}
	}
	
	// Create IndexData
	{
		indices.assign(width * height * 6, UINT());
		//indexCount = width * height * 6;	// grid ĭ �� * �ϳ��� ĭ�� �׸��� �� �ﰢ�� 2���� �ʿ��ϹǷ� 6���� �ε��� �ʿ�
	
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

	// CreateNormal
	{
		for (UINT i = 0; i < (indices.size() / 3); ++i)
		{

		}
	}
}

void Terrain::CreateBuffer()
{
}
