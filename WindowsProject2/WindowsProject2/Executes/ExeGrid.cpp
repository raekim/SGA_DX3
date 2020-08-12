#include "framework.h"
#include "ExeGrid.h"

ExeGrid::ExeGrid(ExecuteValues * values) 
	: Execute(values), shaderSelectNum(0)
{
	Texture* heightMap = new Texture(Contents + L"HeightMaps/HeightMap.png");

	vector<D3DXCOLOR> heights;
	heightMap->ReadPixels(DXGI_FORMAT_R8G8B8A8_UNORM, &heights);

	// �ؽ��� �̹����� �ȼ� �� ��ŭ ���ؽ��� ����� ����
	width = heightMap->GetWidth() - 1;
	height = heightMap->GetHeight() - 1;

	material = new Material(Shaders + L"012_MyDiffuse_Height.hlsl");
	material->SetDiffuseMap(Textures + L"Rock.png");

	//shader[0] = new Shader(Shaders + L"010_MyHeightMap.hlsl");	// ���̿� ���� �ؽ��� ����
	//shader[1] = new Shader(Shaders + L"010_MyHeightMap2.hlsl");	// ������ ���ĸʿ� ���� �ؽ��� ����

	worldBuffer = new WorldBuffer();

	texture[0] = new Texture(Textures + L"Dirt.png");
	texture[1] = new Texture(Textures + L"Grass.png");
	//texture[2] = new Texture(Contents + L"HeightMaps/AlphaMap.png");

	// Create VertexData
	{
		vertexCount = (width + 1) * (height + 1);
		vertices = new VertexType[vertexCount];

		for (UINT z = 0; z <= height; ++z)
		{
			for (UINT x = 0; x <= width; ++x)
			{
				UINT index = (width + 1) * z + x;

				vertices[index].Position.x = (float)x;
				// (255/7) ���� ���̸� 256 �ܰ�� ������ ���̷� �Է�
				vertices[index].Position.y = (float)(heights[index].r * 255.0f) / 7;
				vertices[index].Position.z = (float)z;

				vertices[index].Uv.x = (float)x / width; // (float)(width - x) / width;
				vertices[index].Uv.y = (float)z / height;
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

	// Ǯ, ��, ����, ���ĸ� �ؽ���
	//for (int i = 0; i < 3 + shaderSelectNum; ++i) texture[i]->SetShaderResource(i);

	texture[0]->SetShaderResource(1);
	texture[1]->SetShaderResource(2);

	material->PSSetBuffer();
	worldBuffer->SetVSBuffer(1);

	D3D::GetDC()->DrawIndexed(indexCount, 0, 0);
}	

void ExeGrid::PostRender()
{
	ImGui::SliderInt("Shader Number", &shaderSelectNum, 0, 1);
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
