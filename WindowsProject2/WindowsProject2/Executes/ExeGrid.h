#pragma once
#include "Execute.h"

class ExeGrid : public Execute
{
private:
	typedef VertexTextureNormal VertexType;

	int shaderSelectNum;
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

	Shader* shader[2];
	Texture* texture[4];
private:
	void CreateNormal();
};