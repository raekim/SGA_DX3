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

		// struct과 정의됨과 동시에 Data라는 이름의 멤버변수를 정의
		struct Struct 
		{
			D3DXCOLOR Color;
			
		}Data;
	};
};