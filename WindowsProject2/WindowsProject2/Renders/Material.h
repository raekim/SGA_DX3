#pragma once

class Material
{
private:
	class Buffer;

public:
	Material();
	Material(wstring shaderFile);
	~Material();

	Shader* GetShader() { return shader; }
	void SetShader(string file);
	void SetShader(wstring file);
	void SetShader(Shader* shader);

	D3DXCOLOR* GetDiffuse() { return &buffer->Data.Diffuse; }
	void SetDiffuse(D3DXCOLOR& color) { buffer->Data.Diffuse = color; }
	void SetDiffuse(float r, float g, float b, float a = 1.0f)
	{
		buffer->Data.Diffuse = D3DXCOLOR(r, g, b, a);
	}

	Texture* GetDiffuseMap() { return diffuseMap; }
	void SetDiffuseMap(string file, D3DX11_IMAGE_LOAD_INFO* info = NULL);
	void SetDiffuseMap(wstring file, D3DX11_IMAGE_LOAD_INFO* info = NULL);

	void PSSetBuffer();

private:
	Buffer* buffer;

	bool bShaderDelete;
	Shader* shader;

	Texture* diffuseMap;

private:
	class Buffer : public ShaderBuffer
	{
	public:
		Buffer() : ShaderBuffer(&Data, sizeof(Struct))
		{
			Data.Ambient = D3DXCOLOR(0, 0, 0, 1);
			Data.Diffuse = D3DXCOLOR(1, 1, 1, 1);
			Data.Specular = D3DXCOLOR(1, 1, 1, 1);

			Data.Shininess = 1;
		}

		struct Struct
		{
			D3DXCOLOR Ambient;
			D3DXCOLOR Diffuse;
			D3DXCOLOR Specular;

			float Shininess;

			float Padding[3];
		} Data;
	};
};