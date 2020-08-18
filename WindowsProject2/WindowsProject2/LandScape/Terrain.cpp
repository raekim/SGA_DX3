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
	//colorBuffer->SetPSBuffer(0);	// 셰이더의 컬러 constant 버퍼로 간다
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
	// 텍스쳐 이미지의 픽셀 수 만큼 버텍스를 만들기 위함
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
		//indexCount = width * height * 6;	// grid 칸 수 * 하나의 칸을 그리는 데 삼각형 2개가 필요하므로 6개의 인덱스 필요
	
		UINT index = 0;
		for (UINT z = 0; z < height; ++z)
		{
			for (UINT x = 0; x < width; ++x)
			{
				indices[index + 0] = (width + 1) * z + x;	// 왼쪽 위 (해당 칸의 점 그리기 시작지점)
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
