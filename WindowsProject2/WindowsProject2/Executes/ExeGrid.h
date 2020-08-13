#pragma once
#include "Execute.h"

class ExeGrid : public Execute
{
private:
	class ColorBuffer;
public:
	ExeGrid(ExecuteValues* values);
	~ExeGrid();

	void Update();

	void PreRender();
	void Render();
	void PostRender();

	void ResizeScreen();

private:
	Shader * shader;
	WorldBuffer *worldBuffer;

	UINT width, height;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	UINT vertexCount, indexCount;

	Vertex* vertices;
	UINT* indices;

	ColorBuffer * colorBuffer;

	int fillModeNumber;

	ID3D11RasterizerState* fillMode[2];
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