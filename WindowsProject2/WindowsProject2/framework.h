// header.h : include file for standard system include files,
// or project specific include files
//

#pragma once

#include "targetver.h"
// Windows Header Files
#include <windows.h>
#include <assert.h>


// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// STL
#include <string>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <functional>
using namespace std;

// Direct3D
#include <dxgi1_2.h>
#include <d3dcommon.h>
#include <d3dcompiler.h>
#include <d3d11shader.h>
#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>

// 정적 라이브러리 링크를 위한 것
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

// ImGui
#include <imgui.h>
#include <imguiDx11.h>
#pragma comment(lib, "imgui.lib")

// Json Cpp
#include <json/json.h>
#pragma comment(lib, "jsoncpp.lib")
#pragma warning(disable : 4996)	// 경고를 끄는 것


// DirectxTex
#include <DirectXTex.h>
#pragma comment(lib, "directxtex.lib")

// Fbx SDK
#define FBXSDK_SHARED
#include <fbxsdk.h>
#pragma comment(lib, "libfbxsdk.lib")
using namespace fbxsdk;

const wstring Shaders = L"../../_Shaders/";
const wstring Textures = L"../../_Textures/";
const wstring Contents = L"../../_Contents/";
const wstring Assets = L"../../_Assets/";
const wstring Models = L"../../_Models/";
const wstring Materials = L"../../_Materials/";

#define SAFE_RELEASE(p){if(p) {(p)->Release(); (p) = NULL;}}
#define SAFE_DELETE(p){if(p) {delete (p); (p) = NULL;}}
#define SAFE_DELETE_ARRAY(p){if(p) {delete[] (p); (p) = NULL;}}

#include "./Systems/D3D.h"
#include "./Systems/Keyboard.h"
#include "./Systems/Mouse.h"
#include "./Systems/Time.h"

#include "./Renders/VertexLayouts.h"
#include "./Renders/States.h"
#include "./Renders/Shader.h"
#include "./Renders/ShaderBuffer.h"
#include "./Renders/GlobalBuffers.h"
#include "./Renders/Texture.h"
#include "./Renders/Material.h"

#include "./Executes/Execute.h"

#include "./Utilities/Math.h"
#include "./Utilities/Json.h"
#include "./Utilities/String.h"
#include "./Utilities/Path.h"

#include "./Model/Model.h"
#include "./Model/ModelBone.h"
#include "./Model/ModelMesh.h"

typedef VertexTextureNormal ModelVertexType;

