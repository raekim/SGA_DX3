#include "000_Header.hlsl"

struct PixelInput
{
	float4 worldPosition : POSITION0;	// ���� �������� ��� ����
    float4 Position : SV_POSITION;
    float2 Uv : UV0;
    float3 Normal : NORMAL0;
};


Texture2D NumberAlpha : register(t1);	// ���ڰ� �׷��� ���ĸ�

PixelInput VS(VertexTextureNormal input)
{
    PixelInput output;

	output.worldPosition = input.Position;  // ���� �������� ��� ����

    output.Position = mul(input.Position, World);

	output.worldPosition = input.Position;  // ���� �������� ��� ����

    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);

    output.Normal = mul(input.Normal, (float3x3) World);

    output.Uv = input.Uv;

    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
	float2 floorUv = input.Uv * 4;

	float4 diffuse = DiffuseMap.Sample(DiffuseSampler, floorUv);
	float4 alpha = NumberAlpha.Sample(DiffuseSampler, input.Uv);

	// �⺻ �ؽ��� ���� ���� �������
	if (alpha.r > 0.4f)
	{
		diffuse = float4(1 - alpha.r, 1 - alpha.r, 1 - alpha.r, 1);
	}

	float4 color = 0;
	DiffuseLighting(color, diffuse, input.Normal);

	//return input.Normal * 0.5f + 0.5f;
	return color;
}