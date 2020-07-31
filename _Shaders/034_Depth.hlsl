#include "000_Header.hlsl"

cbuffer PS_Normal : register(b10)
{
    int VisibleNormal;
}

struct PixelInput
{
    float4 Position : SV_POSITION;
    float4 pPosition : POSITION0;
};

PixelInput VS(VertexTextureNormalTangent input)
{
    PixelInput output;

    matrix world = Bones[BoneIndex];
    output.Position = mul(input.Position, world);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);

    output.pPosition = output.Position;

    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    float depth = input.pPosition.z / input.pPosition.w;

    float4 color = 0;

    [branch]
    if(depth < 0.9f)
    {
        color = float4(1, 0, 0, 1);
    }
    else if(depth > 0.925f)
    {
        color = float4(0, 0, 1, 1);
    }
    else if(depth > 0.9f)
    {
        color = float4(0, 1, 0, 1);
    }
    
    return color;
}