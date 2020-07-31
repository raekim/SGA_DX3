#include "000_Header.hlsl"

cbuffer VS_Ocean : register(b10)
{
    float WaveFrequency;
    float WaveAmplitude;

    float2 TextureScale;

    float2 BumpSpeed;
    float BumpHeight;
    float RunningTime;
}

cbuffer PS_Ocean : register(b10)
{
    float4 ShallowColor; //������
    float4 DeepColor; //������

    float FresnelBias; //����
    float FresnelPower; //����
    float FresnelAmount; //������
    float ShoreBlend; //���İ� ��ȭ����

    float2 OceanSize; //���ؽ��� ��üũ��
    float HeightRatio; //���� ������
}

struct PixelInput
{
    float4 Position : SV_POSITION;
    float4 oPosition : POSITION0;
    float4 wPosition : POSITION1;
    float2 TexScale : TEXSCALE0;
    float3 Normal : NORMAL0;
    float3 View : VIEW0;
    
    float2 Bump[3] : BUMP0;
    float3 Tangent[3] : TANGENT0;
};

struct Wave
{
    float Frequency; //�ĵ�
    float Amplitude; //����
    float Phase; //����
    float2 Direction; //����
};

float EvaluateWave(Wave wave, float2 position, float time)
{
    float s = sin(dot(wave.Direction, position) * wave.Frequency + time) + wave.Phase;

    return wave.Amplitude * s;
}

float EvaluateDifferent(Wave wave, float2 position, float time)
{
    float s = cos(dot(wave.Direction, position) * wave.Frequency + time) + wave.Phase;

    return wave.Amplitude * wave.Frequency * s;
}

PixelInput VS(VertexTexture input)
{
    PixelInput output;

    Wave wave[3] =
    {
        0.0f, 0.0f, 0.50f, float2(-1.0f,  0.0f),
        0.0f, 0.0f, 1.30f, float2(-0.7f, -0.7f),
        0.0f, 0.0f, 0.25f, float2( 0.2f,  0.1f),
    };

    wave[0].Frequency = WaveFrequency;
    wave[0].Amplitude = WaveAmplitude;
    wave[1].Frequency = WaveFrequency * 2.0f;
    wave[1].Amplitude = WaveAmplitude * 0.5f;
    wave[2].Frequency = WaveFrequency * 3.0f;
    wave[2].Amplitude = WaveAmplitude * 1.0f;

    float ddx = 0, ddy = 0;
    for (int i = 0; i < 3; i++)
    {
        input.Position.y += EvaluateWave(wave[i], input.Position.xz, RunningTime);

        float diff = EvaluateDifferent(wave[i], input.Position.xz, RunningTime);
        ddx += diff * wave[i].Direction.x;
        ddy += diff * wave[i].Direction.y;
    }


    float3 T = float3(1, ddx, 0);
    float3 B = float3(-ddx, 1, -ddy);
    float3 N = float3(0, ddy, 1);

    float3x3 matTangent = float3x3
    (
        normalize(T) * BumpHeight,
        normalize(B) * BumpHeight,
        normalize(N)
    );

    output.Tangent[0] = mul(World[0].xyz, matTangent);
    output.Tangent[1] = mul(World[1].xyz, matTangent);
    output.Tangent[2] = mul(World[2].xyz, matTangent);

    
    output.TexScale = input.Uv * TextureScale;

    float tempTime = fmod(RunningTime, 100);
    output.Bump[0] = output.TexScale + tempTime * BumpSpeed;
    output.Bump[1] = output.TexScale * 2.0f + tempTime * BumpSpeed * 4.0f;
    output.Bump[2] = output.TexScale * 4.0f + tempTime * BumpSpeed * 8.0f;


    output.oPosition = input.Position;

    output.Position = mul(input.Position, World);
    output.wPosition = output.Position;
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);

    output.View = ViewInverse._41_42_43 - output.wPosition.xyz;

    return output;
}

Texture2D HeightMap : register(t5);
SamplerState HeightSampler : register(s5);

bool WithInBound(float3 position)
{
    return (position.x > 0.0f && position.z > 0.0f && position.x < OceanSize.x && position.z < OceanSize.y);
}

float EvaluteShoreBlend(float3 position)
{
    float2 temp = float2(position.x / OceanSize.x, position.z / OceanSize.y);
    float color = HeightMap.Sample(HeightSampler, temp).r / HeightRatio;

    return 1.0f - color * ShoreBlend;
}

float4 PS(PixelInput input) : SV_TARGET
{   
    float4 t0 = NormalMap.Sample(NormalSampler, input.Bump[0]) * 2.0f - 1.0f;
    float4 t1 = NormalMap.Sample(NormalSampler, input.Bump[1]) * 2.0f - 1.0f;
    float4 t2 = NormalMap.Sample(NormalSampler, input.Bump[2]) * 2.0f - 1.0f;

    //float3 normal = t0.xyz;
    float3 normal = t0.xyz + t1.xyz + t2.xyz;


    float3x3 matTangent;
    matTangent[0] = input.Tangent[0];
    matTangent[1] = input.Tangent[1];
    matTangent[2] = input.Tangent[2];

    normal = mul(normal, matTangent);
    normal = normalize(normal);

    
    float4 color = 0;
    
    float facing = 1.0f - saturate(dot(input.View, normal));
    float fresnel = FresnelBias + (1.0f - FresnelBias) * pow(facing, FresnelPower);

    float alpha = 0;
    color = lerp(DeepColor, ShallowColor, facing);
    if(ShoreBlend > 0 && WithInBound(input.oPosition.xyz))
    {
        alpha = EvaluteShoreBlend(input.oPosition.xyz);
        color.rgb = lerp(2, color.rgb, color.a);
    }
    color.rgb = color.rgb * Ambient.rgb * fresnel;


    DiffuseLighting(color, Diffuse, normal);
    SpecularLighting(color, normal, input.View);

   
    return float4(color.rgb, alpha);
}