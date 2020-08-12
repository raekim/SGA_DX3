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

	struct FbxVertex 
	{
		int ControlPoint;
		string MaterialName;

		ModelVertexType Vertex;
	};

	struct FbxMeshPartData 
	{
		string MaterialName;

		vector<ModelVertexType> Vertices;
		vector<UINT> Indices;
	};

	struct FbxMeshData
	{
		string Name;
		int ParentBone;

		FbxMesh* Mesh;

		vector<FbxVertex*> Vertices;
		vector<FbxMeshPartData*> MeshParts;
	};
}
