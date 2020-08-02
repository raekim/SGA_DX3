#pragma once

#include "Execute.h"

class ExeUVAnimation : public Execute
{
private:
	class ColorBuffer;
	const int width = 6;
	const int height = 4;
	const int totalFrameNumber = width * height;
	float frameWidth, frameHeight;

	int currentFrameIndex = 0;
	int animationFPS = 24;
	
	bool facingRight = true;
	float moveSpeed = 1.2f;
	D3DXVECTOR3 velocity;
	D3DXVECTOR3 position = { 0,0,0 };
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

	void NextFrame(int frameIndex);	// frameNumber에 해당하는 uv좌표를 UvIndexArray에 저장한다
	bool Move();
};

