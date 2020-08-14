#pragma once

#include "Execute.h"

class ExeExportModel : public Execute
{
public:
	ExeExportModel(ExecuteValues* values);
	~ExeExportModel();

	void Update();

	void PreRender();
	void Render();
	void PostRender();

	void ResizeScreen();

private:
	class GameTank* tank;
};

