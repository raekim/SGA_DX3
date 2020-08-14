#pragma once
#include "Execute.h"

class ExeGrid : public Execute
{
private:
	typedef VertexTextureNormal VertexType;

	int materialSelectNum;
public:
	ExeGrid(ExecuteValues* values);
	~ExeGrid();

	void Update();

	void PreRender();
	void Render();
	void PostRender();

	void ResizeScreen();

private:
	Material* material;
	WorldBuffer *worldBuffer;

	UINT width, height;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	UINT vertexCount, indexCount;

	VertexType* vertices;
	UINT* indices;

	Texture* texture;

	ID3D11SamplerState* state;
private:
	void CreateNormal();
};