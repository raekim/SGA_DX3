#include "framework.h"
#include "ExeGrid.h"

ExeGrid::ExeGrid(ExecuteValues * values) 
	: Execute(values)
{
	Texture* heightMap = new Texture(Contents + L"HeightMaps/HeightMap.png");

	vector<D3DXCOLOR> heights;
	heightMap->ReadPixels(DXGI_FORMAT_R8G8B8A8_UNORM, &heights);
	// �ؽ��� �̹����� �ȼ� �� ��ŭ ���ؽ��� ����� ����
	width = heightMap->GetWidth() - 1;
	height = heightMap->GetHeight() - 1;

	shader = new Shader(Shaders + L"010_HeightMap.hlsl");

	worldBuffer = new WorldBuffer();

	colorBuffer = new ColorBuffer();

	// Create VertexData
	{
		vertexCount = (width + 1) * (height + 1);
		vertices = new VertexColor[vertexCount];

		for (UINT z = 0; z <= height; ++z)
		{
			for (UINT x = 0; x <= width; ++x)
			{
				UINT index = (width + 1) * z + x;

				vertices[index].Position.x = (float)x; // (float)x * (unit meter)
				// (255/7.5 = 34) ���� ���̸� 256 �ܰ�� ������ ���̷� �Է�
				vertices[index].Position.y = (float)(heights[index].r * 255.0f) / 7.5f;
				vertices[index].Position.z = (float)z;

				vertices[index].Color = D3DXCOLOR(1, 1, 1, 1);
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

	// CreateVertexBuffer;
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(VertexColor) * vertexCount;
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

	{
		D3D11_RASTERIZER_DESC desc;
		States::GetRasterizerDesc(&desc);
		States::CreateRasterizer(&desc, &fillMode[0]);

		desc.FillMode = D3D11_FILL_WIREFRAME;
		States::CreateRasterizer(&desc, &fillMode[1]);
	}
}

ExeGrid::~ExeGrid()
{
	SAFE_RELEASE(indexBuffer);
	SAFE_RELEASE(vertexBuffer);

	SAFE_DELETE_ARRAY(indices);
	SAFE_DELETE_ARRAY(vertices);

	SAFE_DELETE(colorBuffer);
	SAFE_DELETE(worldBuffer);
	SAFE_DELETE(shader);
}

void ExeGrid::Update()
{

}

void ExeGrid::PreRender()
{
}

void ExeGrid::Render()
{
	UINT stride = sizeof(VertexColor);
	UINT offset = 0;

	// IA : Input-Assembler Stage
	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3D::GetDC()->RSSetState(fillMode[fillModeNumber]);

	colorBuffer->SetPSBuffer(0);	// ���̴��� �÷� constant ���۷� ����
	worldBuffer->SetVSBuffer(1);
	shader->Render();

	D3D::GetDC()->DrawIndexed(indexCount, 0, 0);
}	

void ExeGrid::PostRender()
{
	ImGui::SliderInt("wireframe", &fillModeNumber, 0, 1);
}

void ExeGrid::ResizeScreen()
{
}
