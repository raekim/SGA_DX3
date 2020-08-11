cbuffer VS_ViewProjection : register(b0)
{
	matrix _view;
	matrix _projection;
}

cbuffer VS_World : register(b1)
{
	matrix _world;
}

cbuffer PS_Color : register(b0)
{
	float4 Color;
}

struct VertexInput
{
	float4 position : POSITION0;
	float2 uv : UV0;
};

struct PixelInput
{
	float4 worldPosition : POSITION0;
	float4 position : SV_POSITION;
	float2 uv : UV0;
};

SamplerState Sampler : register(s0);
Texture2D Map : register(t0);
Texture2D Map2 : register(t1);
Texture2D Map3 : register(t2);

PixelInput VS(VertexInput input)
{
	PixelInput output;

	output.worldPosition = input.position;
	output.position = mul(input.position, _world);
	output.position = mul(output.position, _view);
	output.position = mul(output.position, _projection);

	output.uv = input.uv;

	return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
	float4 output;

	float4 t = Map.Sample(Sampler, input.uv);
	float4 t2 = Map2.Sample(Sampler, input.uv);
	float4 t3 = Map3.Sample(Sampler, input.uv);

	float val = input.worldPosition.y / 36;

	if (input.worldPosition.y > 30)
	{
		output = t3;
	}
	else if (input.worldPosition.y > 20)
	{
		output = t2;
	}
	else
	{
		output = t;
	}

	return output;

	//return float4(1, 1, 1, 1);
}