#pragma once

#include "Execute.h"

class ExeUVAnimation : public Execute
{
private:
	class ColorBuffer;
	int currentFrameIndex = 0;
	const int width = 6;
	const int height = 4;
	int animationFPS = 24;
	const int totalFrameNumber = width * height;

public:
	ExeUVAnimation(ExecuteValues* values);
	~ExeUVAnimation();

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

	ID3D11ShaderResourceView * srv;

	void NextFrame(int frameNumber, VertexTexture* UvIndexArray);	// frameNumber에 해당하는 uv좌표를 UvIndexArray에 저장한다
};

