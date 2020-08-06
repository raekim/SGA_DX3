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

	// ȸ��, �����Ͷ����� ������Ʈ
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

		// struct�� ���ǵʰ� ���ÿ� Data��� �̸��� ��������� ����
		struct Struct 
		{
			D3DXCOLOR Color;
			
		}Data;
	};
};