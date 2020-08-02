#include "framework.h"
#include "ExeUVAnimation.h"

ExeUVAnimation::ExeUVAnimation(ExecuteValues* values) : Execute(values), vertexCount(4), indexCount(6)
{
	shader = new Shader(Shaders + L"004_Texture.hlsl");

	worldBuffer = new WorldBuffer();

	vertices = new VertexTexture[vertexCount];

	indices = new UINT[indexCount]{ 0,1,2, 2,1,3 };

	// Set Blend State
	{
		ID3D11BlendState* blendState;
		D3D11_BLEND_DESC blendStateDesc;

		ZeroMemory(&blendStateDesc, sizeof(D3D11_BLEND_DESC));
		blendStateDesc.RenderTarget[0].BlendEnable = TRUE;
		blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;	// 픽셀 투명도 사용
		blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		HRESULT hr = D3D::GetDevice()->CreateBlendState(&blendStateDesc, &blendState);
		assert(SUCCEEDED(hr));
		D3D::GetDC()->OMSetBlendState(blendState, NULL, 0xffffffff);
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
		HRESULT hr = D3DX11CreateShaderResourceViewFromFile(D3D::GetDevice(), (Textures + L"walk_sheet.png").c_str(), NULL, NULL, &srv, NULL);
		assert(SUCCEEDED(hr));
	}

	// 텍스쳐 전체 사이즈 감안하여 적절한 vertex 좌표 설정
	{
		ID3D11Texture2D *pTextureInterface = 0;
		ID3D11Resource *res;
		srv->GetResource(&res);
		res->QueryInterface<ID3D11Texture2D>(&pTextureInterface);
		D3D11_TEXTURE2D_DESC desc;
		pTextureInterface->GetDesc(&desc);

		float textureWidth = (desc.Width) / 250.0f;
		float textureHeight = (desc.Height) / 250.0f;

		frameWidth = textureWidth / width;
		frameHeight = textureHeight / height;
	}
}

ExeUVAnimation::~ExeUVAnimation()
{
	SAFE_RELEASE(indexBuffer);
	SAFE_RELEASE(vertexBuffer);

	SAFE_DELETE_ARRAY(indices);
	SAFE_DELETE_ARRAY(vertices);

	SAFE_DELETE(worldBuffer);
	SAFE_DELETE(shader);
}

void ExeUVAnimation::NextFrame(int frameIndex)
{
	if (frameIndex >= totalFrameNumber) return;

	float w = 1.0f / width;
	float h = 1.0f / height;

	// frameIndex에 맞는 UV인덱스 설정
	vertices[0].Uv = { (frameIndex % width)*w, (frameIndex / width + 1)*h };
	vertices[1].Uv = { (frameIndex % width)*w, (frameIndex / width)*h };
	vertices[2].Uv = { (frameIndex % width + 1)*w, (frameIndex / width + 1)*h };
	vertices[3].Uv = { (frameIndex % width + 1)*w, (frameIndex / width)*h };

	if (!facingRight)
	{
		swap(vertices[0].Uv, vertices[2].Uv);
		swap(vertices[1].Uv, vertices[3].Uv);
	}
}

bool ExeUVAnimation::Move()
{
	velocity = { 0,0,0 };

	if (Keyboard::Get()->Press('A'))
	{
		velocity.x--;
	}
	if (Keyboard::Get()->Press('D'))
	{
		velocity.x++;
	}
	if (Keyboard::Get()->Press('W'))
	{
		velocity.y++;
	}
	if (Keyboard::Get()->Press('S'))
	{
		velocity.y--;
	}

	D3DXVec3Normalize(&velocity, &velocity);
	velocity *= moveSpeed;

	position += velocity * Time::Delta();

	// position 에 맞게 vertex 좌표 변경
	vertices[0].Position = D3DXVECTOR3(-frameWidth / 2, -frameHeight / 2, 0) + position;
	vertices[1].Position = D3DXVECTOR3(-frameWidth / 2, frameHeight / 2, 0) + position;
	vertices[2].Position = D3DXVECTOR3(frameWidth / 2, -frameHeight / 2, 0) + position;
	vertices[3].Position = D3DXVECTOR3(frameWidth / 2, frameHeight / 2, 0) + position;

	if (velocity.x > 0.0f)
	{
		facingRight = true;
	}
	if (velocity.x < 0.0f)
	{
		facingRight = false;
	}

	return D3DXVec3Length(&velocity) > 0.0f;
}

void ExeUVAnimation::Update() 
{
	static float count = 1.0f;

	// 이동
	bool moved = Move();
	bool animationNextFrame = count >= 1.0f / animationFPS;

	// 애니메이션 프레임 갱신
	if (animationNextFrame)
	{
		currentFrameIndex = (currentFrameIndex + 1) % (totalFrameNumber);
		
		NextFrame(currentFrameIndex);

		count = 0.0f;
	}

	if (moved || animationNextFrame)
	{
		D3D::GetDC()->UpdateSubresource(vertexBuffer, 0, NULL, vertices, sizeof(VertexColor)*vertexCount, 0);
	}

	count += Time::Delta();
}

void ExeUVAnimation::PreRender()
{

}

void ExeUVAnimation::Render()
{
	UINT stride = sizeof(VertexTexture);
	UINT offset = 0;

	// IA : Input-Assembler Stage
	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	worldBuffer->SetVSBuffer(1);
	shader->Render();

	D3D::GetDC()->PSSetShaderResources(0, 1, &srv);	// 셰이더에 Texture2D Map 전달

	D3D::GetDC()->DrawIndexed(indexCount, 0, 0);
}

void ExeUVAnimation::PostRender()
{
	ImGui::SliderInt("Animation FPS", &animationFPS, 1, 24);
}

void ExeUVAnimation::ResizeScreen()
{

}
