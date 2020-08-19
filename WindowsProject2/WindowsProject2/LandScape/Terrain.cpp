#include "framework.h"
#include "Terrain.h"

Terrain::Terrain(ExecuteValues* values, Material* material, wstring heightMap)
	: values(values), material(material)
{
	heightTexture = new Texture(heightMap);
	worldBuffer = new WorldBuffer();

	CreateData();
	CreateBuffer();

	// Create Rasterizer
	{
		D3D11_RASTERIZER_DESC desc;
		States::GetRasterizerDesc(&desc);
		States::CreateRasterizer(&desc, &rasterizer[0]);

		desc.FillMode = D3D11_FILL_WIREFRAME;
		States::CreateRasterizer(&desc, &rasterizer[1]);
	}
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
	UINT stride = sizeof(VertexTextureNormal);
	UINT offset = 0;
	
	// IA : Input-Assembler Stage
	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	worldBuffer->SetVSBuffer(1);
	material->PSSetBuffer();
	
	D3D::GetDC()->RSSetState(rasterizer[1]);
	D3D::GetDC()->DrawIndexed(indices.size(), 0, 0);
	D3D::GetDC()->RSSetState(rasterizer[0]);
}

// 해당 position의 높이값을 얻어 온다
float Terrain::Y(D3DXVECTOR3 & position)
{
	UINT x = (UINT)position.x;
	UINT z = (UINT)position.z;

	if (x >= width || z >= height)	// if(x < 0 || x >= width) but x cannot be less than 0 since it's type UINT
	{
		return 0.0f;
	}

	UINT index[4];
	index[0] = (width + 1)*z + x;
	index[1] = (width + 1)*(z + 1) + x;
	index[2] = (width + 1)*z + (x + 1);
	index[3] = (width + 1)*(z + 1) + (x + 1);

	D3DXVECTOR3 v[4];
	for (int i = 0; i < 4; ++i)
	{
		v[i] = vertices[index[i]].Position;
	}

	float ddx = (position.x - v[0].x) / 1.0f;
	float ddz = (position.z - v[0].z) / 1.0f;

	D3DXVECTOR3 temp;
	// 오차범위
	if (ddx + ddz <= 1)
	{
		temp = v[0] + (v[2] - v[0])*ddx + (v[1] - v[0]) * ddz;
	}
	else
	{
		ddx = 1 - ddx;
		ddz = 1 - ddz;

		temp = v[3] + (v[1] - v[3])*ddx + (v[2] - v[3]) * ddz;
	}

	return temp.y;
}

bool Terrain::Y(OUT D3DXVECTOR3 * out, D3DXVECTOR3 & position)
{
	D3DXVECTOR3 start = D3DXVECTOR3(position.x, 1000, position.z);
	D3DXVECTOR3 direction = D3DXVECTOR3(0, -1, 0);

	UINT x = (UINT)position.x;
	UINT z = (UINT)position.z;

	if (x >= width || z >= height)	// if(x < 0 || x >= width) but x cannot be less than 0 since it's type UINT
	{
		return false;
	}

	UINT index[4];
	index[0] = (width + 1)*z + x;
	index[1] = (width + 1)*(z + 1) + x;
	index[2] = (width + 1)*z + (x + 1);
	index[3] = (width + 1)*(z + 1) + (x + 1);

	D3DXVECTOR3 p[4];
	for (int i = 0; i < 4; ++i)
	{
		p[i] = vertices[index[i]].Position;
	}

	float u, v, distance;

	// 첫 번째 면에 Ray 쏘기
	if (D3DXIntersectTri(&p[0], &p[1], &p[2], &start, &direction, &u, &v, &distance))
	{
		*out = p[0] + (p[1] - p[0]) * u + (p[2] - p[0]) * v;
		return true;
	}
	// 두 번째 면에 Ray 쏘기
	if (D3DXIntersectTri(&p[3], &p[1], &p[2], &start, &direction, &u, &v, &distance))
	{
		*out = p[3] + (p[1] - p[3]) * u + (p[2] - p[3]) * v;
		return true;
	}

	return false;
}

bool Terrain::Y(OUT D3DXVECTOR3 * out)
{
	D3DXVECTOR3 start;
	values->MainCamera->Position(&start);

	D3DXVECTOR3 direction = values->MainCamera->Direction(values->Viewport, values->Perspective);

	for (UINT z = 0; z < height; ++z)
	{
		for (UINT x = 0; x < width; ++x)
		{
			UINT index[4];
			index[0] = (width + 1)*z + x;
			index[1] = (width + 1)*(z + 1) + x;
			index[2] = (width + 1)*z + (x + 1);
			index[3] = (width + 1)*(z + 1) + (x + 1);

			D3DXVECTOR3 p[4];
			for (int i = 0; i < 4; ++i)
			{
				p[i] = vertices[index[i]].Position;
			}

			float u, v, distance;

			// 첫 번째 면에 Ray 쏘기
			if (D3DXIntersectTri(&p[0], &p[1], &p[2], &start, &direction, &u, &v, &distance))
			{
				*out = p[0] + (p[1] - p[0]) * u + (p[2] - p[0]) * v;
				return true;
			}
			// 두 번째 면에 Ray 쏘기
			if (D3DXIntersectTri(&p[3], &p[1], &p[2], &start, &direction, &u, &v, &distance))
			{
				*out = p[3] + (p[1] - p[3]) * u + (p[2] - p[3]) * v;
				return true;
			}
		}
	}

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
				//vertices[index].Position.y = 0.0f;
				vertices[index].Position.z = (float)z;

				vertices[index].Uv.x = (float)x / width;
				vertices[index].Uv.y = (float)z / height; // (height - (float)z) / height; 
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
				indices[index + 2] = (width + 1) * z + (x + 1);

				indices[index + 3] = (width + 1) * z + (x + 1);
				indices[index + 4] = (width + 1) * (z + 1) + x;
				indices[index + 5] = (width + 1) * (z + 1) + (x + 1);
	
				index += 6;
			}
		}
	}

	// CreateNormal
	{
		// 정점의 노말 벡터 구하기
	// 그 점이 속한 면들이 가진 모든 노말벡터를 합한 값
		for (UINT i = 0; i < (indices.size() / 3); ++i)
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

			// 더해 줌
			vertices[index0].Normal += normal;
			vertices[index1].Normal += normal;
			vertices[index2].Normal += normal;
		}

		for (UINT i = 0; i < vertices.size(); ++i)
		{
			D3DXVec3Normalize(&vertices[i].Normal, &vertices[i].Normal);
		}
	}
}

void Terrain::CreateBuffer()
{
	// CreateVertexBuffer;
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(VertexTextureNormal) * vertices.size();//vertexCount;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	
		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = &vertices[0];
	
		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
		assert(SUCCEEDED(hr));
	}
	
	// CreateIndexBuffer;
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(UINT) * indices.size();
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	
		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = &indices[0];
	
		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &indexBuffer);
		assert(SUCCEEDED(hr));
	}
}
