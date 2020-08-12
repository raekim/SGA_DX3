#include "framework.h"
#include "Material.h"

Material::Material()
	: shader(NULL)
	, diffuseMap(NULL)
	, bShaderDelete(false)
{
	buffer = new Buffer();
}

Material::Material(wstring shaderFile)
	: diffuseMap(NULL)
{
	assert(shaderFile.length() > 0);

	buffer = new Buffer();

	bShaderDelete = true;
	shader = new Shader(shaderFile);
}

Material::~Material()
{
	if (bShaderDelete == true)
		SAFE_DELETE(shader);

	SAFE_DELETE(diffuseMap);
}

void Material::SetShader(string file)
{
	SetShader(String::ToWString(file));
}

void Material::SetShader(wstring file)
{
	if (bShaderDelete == true)
		SAFE_DELETE(shader);

	bShaderDelete = false;
	if (file.length() > 0)
	{
		shader = new Shader(file);
		bShaderDelete = true;
	}
}

void Material::SetShader(Shader * shader)
{
	if (bShaderDelete == true)
		SAFE_DELETE(shader);

	this->shader = shader;
	bShaderDelete = false;
}

void Material::SetDiffuseMap(string file, D3DX11_IMAGE_LOAD_INFO * info)
{
	SetDiffuseMap(String::ToWString(file), info);
}

void Material::SetDiffuseMap(wstring file, D3DX11_IMAGE_LOAD_INFO * info)
{
	SAFE_DELETE(diffuseMap);

	diffuseMap = new Texture(file, info);
}

void Material::PSSetBuffer()
{
	if (shader != NULL)
		shader->Render();

	UINT slot = 0;
	if (diffuseMap != NULL)
	{
		diffuseMap->SetShaderResource(slot);
		diffuseMap->SetShaderSampler(slot);
	}
	else
	{
		Texture::SetBlankShaderResource(slot);
		Texture::SetBlankSamplerState(slot);
	}

	buffer->SetPSBuffer(1);
}
