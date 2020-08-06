#pragma once
#include "Execute.h"

class ExeKorea : public Execute
{
private:
	class ColorBuffer;
public:
	ExeKorea(ExecuteValues* values);
	~ExeKorea();

	void Update();

	void PreRender();
	void Render();
	void PostRender();

	void ResizeScreen();

private:
	Shader * shader;
	WorldBuffer *worldBuffer;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	UINT vertexCount, indexCount;

	VertexTexture* vertices;
	UINT* indices;

	ColorBuffer * colorBuffer;

	ID3D11ShaderResourceView * SRV;

	// 회전, 래스터라이저 스테이트
	D3DXVECTOR3 rotation;
	D3DXVECTOR2 scale;

	D3DXMATRIX world;
	int number2;

	ID3D11RasterizerState* rasterizer[3];
private:
	class ColorBuffer : public ShaderBuffer 
	{
	public:
		ColorBuffer() : ShaderBuffer(&Data, sizeof(Data))
		{
			Data.Color = D3DXCOLOR(1, 1, 1, 1);
		}

		~ColorBuffer()
		{

		}

		// struct과 정의됨과 동시에 Data라는 이름의 멤버변수를 정의
		struct Struct 
		{
			D3DXCOLOR Color;
			
		}Data;
	};
};