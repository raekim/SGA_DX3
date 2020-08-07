#include "framework.h"
#include "ExeDice.h"

ExeDice::ExeDice(ExecuteValues * values) : Execute(values), vertexCount(4 * 6), indexCount(6 * 6), number(0),
number2(0), location(0, 0), rotation(0, 0, 0), scale(1, 1), rotateSpeed(30.0f)
{
	shader = new Shader(Shaders + L"004_Texture.hlsl");

	worldBuffer = new WorldBuffer();

	colorBuffer = new ColorBuffer();

	vertices = new VertexTexture[vertexCount];
	{
		// 버텍스 위치 좌표
		{
			float s = 0.5f;

			// front
			vertices[0].Position = D3DXVECTOR3(-s, s, -s);
			vertices[1].Position = D3DXVECTOR3(s, s, -s);
			vertices[2].Position = D3DXVECTOR3(-s, -s, -s);
			vertices[3].Position = D3DXVECTOR3(s, -s, -s);

			// back
			vertices[4].Position = D3DXVECTOR3(-s, s, s);
			vertices[5].Position = D3DXVECTOR3(s, s, s);
			vertices[6].Position = D3DXVECTOR3(-s, -s, s);
			vertices[7].Position = D3DXVECTOR3(s, -s, s);

			// left
			vertices[8].Position = vertices[1].Position;
			vertices[9].Position = vertices[5].Position;
			vertices[10].Position = vertices[3].Position;
			vertices[11].Position = vertices[7].Position;

			// right
			vertices[12].Position = vertices[4].Position;
			vertices[13].Position = vertices[0].Position;
			vertices[14].Position = vertices[6].Position;
			vertices[15].Position = vertices[2].Position;

			// top
			vertices[16].Position = vertices[4].Position;
			vertices[17].Position = vertices[5].Position;
			vertices[18].Position = vertices[0].Position;
			vertices[19].Position = vertices[1].Position;

			// bottom
			vertices[20].Position = vertices[6].Position;
			vertices[21].Position = vertices[7].Position;
			vertices[22].Position = vertices[2].Position;
			vertices[23].Position = vertices[3].Position;
		}
		
		// 버텍스 UV 좌표
		{
			float w = 1.0f / 4;
			float h = 1.0f / 3;

			// front
			vertices[0].Uv = D3DXVECTOR2(2 * w, h);
			vertices[1].Uv = D3DXVECTOR2(3 * w, h);
			vertices[2].Uv = D3DXVECTOR2(2 * w, 2 * h);
			vertices[3].Uv = D3DXVECTOR2(3 * w, 2 * h);

			// back
			vertices[4].Uv = D3DXVECTOR2(0, h);
			vertices[5].Uv = D3DXVECTOR2(w, h);
			vertices[6].Uv = D3DXVECTOR2(0, 2 * h);
			vertices[7].Uv = D3DXVECTOR2(w, 2 * h);

			// left
			vertices[8].Uv = D3DXVECTOR2(3 * w, h);
			vertices[9].Uv = D3DXVECTOR2(4 * w, h);
			vertices[10].Uv = D3DXVECTOR2(3 * w, 2 * h); 
			vertices[11].Uv = D3DXVECTOR2(4 * w, 2 * h); 

			// right
			vertices[12].Uv = D3DXVECTOR2(w, h);
			vertices[13].Uv = D3DXVECTOR2(2 * w, h);
			vertices[14].Uv = D3DXVECTOR2(w, 2 * h);
			vertices[15].Uv = D3DXVECTOR2(2 * w, 2 * h);

			// top
			vertices[16].Uv = D3DXVECTOR2(2 * w, 0);
			vertices[17].Uv = D3DXVECTOR2(3 * w, 0);
			vertices[18].Uv = D3DXVECTOR2(2 * w, 1 * h);
			vertices[19].Uv = D3DXVECTOR2(3 * w, 1 * h);

			// bottom
			vertices[20].Uv = D3DXVECTOR2(2 * w, 3 * h); 
			vertices[21].Uv = D3DXVECTOR2(3 * w, 3 * h); 
			vertices[22].Uv = D3DXVECTOR2(2 * w, 2 * h);
			vertices[23].Uv = D3DXVECTOR2(3 * w, 2 * h);
		}
	}

	//0, 1, 2, 2, 1, 3,	// front
	//	4, 0, 6, 6, 0, 2,	// right
	//	7, 5, 6, 6, 5, 4,	// back
	//	3, 1, 7, 7, 1, 5,	// left
	//	4, 5, 0, 0, 5, 1,	// top
	//	3, 7, 2, 2, 7, 6,	// bottom

	indices = new UINT[indexCount]{
		0,1,2, 2,1,3,	// front
		12,13,14, 14,13,15,	// right
		7,5,6, 6,5,4,	// back
		10,8,11, 11,8,9,	// left
		16,17,18, 18,17,19,	// top
		23,21,22, 22,21,20,	// bottom
	};

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
		HRESULT hr = D3DX11CreateShaderResourceViewFromFile(D3D::GetDevice(), (Textures + L"Dice.png").c_str(), NULL, NULL, &srv, NULL);
		assert(SUCCEEDED(hr));
	}

	{
		D3D11_RASTERIZER_DESC desc;
		States::GetRasterizerDesc(&desc);
		States::CreateRasterizer(&desc, &rasterizer[0]);

		//desc.CullMode = D3D11_CULL_NONE;
		desc.FillMode = D3D11_FILL_WIREFRAME;
		States::CreateRasterizer(&desc, &rasterizer[1]);
	}
}

ExeDice::~ExeDice()
{
	SAFE_RELEASE(indexBuffer);
	SAFE_RELEASE(vertexBuffer);

	SAFE_DELETE_ARRAY(indices);
	SAFE_DELETE_ARRAY(vertices);

	SAFE_DELETE(colorBuffer);
	SAFE_DELETE(worldBuffer);
	SAFE_DELETE(shader);
}

void ExeDice::Update()
{
	D3DXMATRIX X, Y, Z;
	D3DXMatrixIdentity(&X);
	D3DXMatrixIdentity(&Y);
	D3DXMatrixIdentity(&Z);

	D3DXMATRIX T;
	D3DXMatrixIdentity(&T);

	if (Keyboard::Get()->Press(VK_SHIFT))
	{
		if (Keyboard::Get()->Press('D'))
		{
			rotation.y -= rotateSpeed * Time::Delta();
		}
		else if (Keyboard::Get()->Press('A'))
		{
			rotation.y += rotateSpeed * Time::Delta();
		}

		if (Keyboard::Get()->Press('E'))
		{
			rotation.z -= rotateSpeed * Time::Delta();
		}
		else if (Keyboard::Get()->Press('Q'))
		{
			rotation.z += rotateSpeed * Time::Delta();
		}

		if (Keyboard::Get()->Press('S'))
		{
			rotation.x -= rotateSpeed * Time::Delta();
		}
		else if (Keyboard::Get()->Press('W'))
		{
			rotation.x += rotateSpeed * Time::Delta();
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

	D3DXMatrixRotationX(&X, Math::ToRadian(rotation.x));
	D3DXMatrixRotationY(&Y, Math::ToRadian(rotation.y));
	D3DXMatrixRotationZ(&Z, Math::ToRadian(rotation.z));

	world = X * Y * Z * T;
}

void ExeDice::PreRender()
{
}

void ExeDice::Render()
{
	UINT stride = sizeof(VertexTexture);
	UINT offset = 0;

	// IA : Input-Assembler Stage
	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	colorBuffer->SetPSBuffer(0);	// 셰이더의 컬러 constant 버퍼로 간다
	worldBuffer->SetVSBuffer(1);
	shader->Render();

	D3D::GetDC()->PSSetShaderResources(0, 1, &srv);	// 셰이더에 Texture2D Map 전달

	// 래스터라이저 스테이트 설정
	D3D::GetDC()->RSSetState(rasterizer[number2]);
	worldBuffer->SetMatrix(world);
	worldBuffer->SetVSBuffer(1);
	D3D::GetDC()->DrawIndexed(indexCount, 0, 0);
}	

void ExeDice::PostRender()
{
	ImGui::Begin("Settings");
	{
		ImGui::SliderInt("Rasterizer", &number2, 0, 1);
		ImGui::DragFloat("Rotation Speed", &rotateSpeed, 1.0f, 30.f, 60.0f);
	}
	ImGui::End();
}

void ExeDice::ResizeScreen()
{
}
