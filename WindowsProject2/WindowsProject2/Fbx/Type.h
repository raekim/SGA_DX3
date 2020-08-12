#pragma once
#include "framework.h"

namespace Fbx
{
	struct FbxMaterial
	{
		string Name;

		D3DXCOLOR Diffuse;

		string DiffuseFile;
	};

	struct FbxBoneData
	{
		int Index;
		string Name;

		int Parent;

		D3DXMATRIX LocalTransform;
		D3DXMATRIX GlobalTransform;
	};
}
