#include "000_Header2.hlsl"

cbuffer VS_Buffer : register(b10)
{
    float3 Velocity;
    float DrawDistance;

    float4 Color;
    float3 Origin;
    float Time;

    float3 Size;
}

struct VertexInput
{
    float4 Position : POSITION0;
    float2 Uv : UV0;
    float2 Scale : SCALE0;
};

struct VertexOutput
{
    float4 Position : SV_POSITION;
    float2 Uv : UV0;
    float Alpha : ALPHA0;
    float4 Color : COLOR0;
};

VertexOutput VS(VertexInput input)
{
    VertexOutput output;

    float3 velocity = Velocity;
    velocity.xz /= input.Scale.y * 0.1f;

    float3 displace = Time * velocity;
    input.Position.xyz = Origin.xyz + (Size.xyz + (input.Position.xyz + displace.xyz) % Size.xyz) % Size.xyz - (Size.xyz * 0.5f);


    float4 position = mul(input.Position, World);
    float3 axis = normalize(-velocity);
    float3 view = position.xyz - CameraPosition();
    float3 side = normalize(cross(axis, view));

    position.xyz += (input.Uv.x - 0.5f) * side * input.Scale.x;
    position.xyz += (1.5f - input.Uv.y * 1.5f) * axis * input.Scale.y;

    position.w = 1.0f;
    output.Position = mul(position, View);
    output.Position = mul(output.Position, Projection);

    output.Uv = input.Uv;
    output.Color = Color;

    float alpha = cos(Time + (input.Position.x + input.Position.z));
    alpha = saturate(1.5f + alpha / DrawDistance * 2);

    output.Alpha = 0.2f * saturate(1 - output.Position.z / DrawDistance) * alpha;

    return output;
}

Texture2D Map : register(t10);
SamplerState Sampler : register(s10);

float4 PS(VertexOutput input) : SV_TARGET
{  
    float4 color = Map.Sample(Sampler, input.Uv);
    color.rgb += input.Color * (1 + input.Alpha) * 2.0f;
    color.a = input.Alpha;
    
    return color;
}