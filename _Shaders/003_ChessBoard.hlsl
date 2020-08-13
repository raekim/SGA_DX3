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
};

struct PixelInput
{
	float4 worldPosition : POSITION0;
    float4 position : SV_POSITION;
};

PixelInput VS(VertexInput input)
{
	PixelInput output;

	output.worldPosition = mul(input.position, _world);	// 픽셀 월드 포지션 저장

	output.position = mul(input.position, _world);
	output.position = mul(output.position, _view);
	output.position = mul(output.position, _projection);

	return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
	float4 color = float4(1, 1, 1, 1);

	// 픽셀 월드 포지션에 따라 색상 결정
	float4 black = float4(0, 0, 0, 1);
	float4 white = float4(1, 1, 1, 1);

	int val = (int)floor(input.worldPosition.x) + (int)floor(input.worldPosition.z);
	color = ((val % 2) ? white : black);

	return color;
}