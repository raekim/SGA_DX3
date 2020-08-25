#pragma once

#include "Execute.h"

class DrawLandscape : public Execute
{
public:
	DrawLandscape(ExecuteValues* values);
	~DrawLandscape();

	void Update();

	void PreRender();
	void Render();
	void PostRender();

	void ResizeScreen();

private:
	class Terrain* terrain;
	class MeshSphere* sphere;
	class Sky* sky;

	vector<class Billboard*> trees;

	// grid snap ����
	bool snapped;
	bool canMove;
	float sphereSpeed;
	D3DXVECTOR3 spherePos;
	float snapRange;
};

