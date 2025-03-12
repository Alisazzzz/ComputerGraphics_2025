struct VS_IN
{
    float4 pos : POSITION0;
    float2 tex : TEXCOORD0;
};

struct PS_IN
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD0;
};

struct ConstantData
{
    float4 offset;
    float4 color;
};

cbuffer ConstBuf : register(b0)
{
    matrix transformations;
    matrix view;
    matrix projection;
    float4 color;
};

Texture2D diffuseMap : register(t0);
SamplerState samp : register(s0);

PS_IN VSMain(VS_IN input)
{
    PS_IN output = (PS_IN) 0;
	
    float4 pos = float4(input.pos.xyz, 1.0f);
    
    pos = mul(pos, transformations);
    pos = mul(pos, view);
    pos = mul(pos, projection);
    
    output.pos = pos;
    output.tex = input.tex.xy;
	
    return output;
};

float4 PSMain(PS_IN input) : SV_Target
{
    float4 texColor = diffuseMap.Sample(samp, input.tex);
    return texColor;
};