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

cbuffer ConstBuf : register(b0)
{
    matrix transformations;
    matrix view;
    matrix projection;
    float4 color;
};

cbuffer LightBuf : register(b1)
{
    float3 ambientColor;
    float ambientStrength;
};

Texture2D diffuseMap : register(t0);
SamplerState samp : register(s0);

PS_IN VSMain(VS_IN input)
{
    PS_IN output = (PS_IN) 0;
	
    float4 pos = float4(input.pos.xyzw);
    
    pos = mul(pos, transformations);
    pos = mul(pos, view);
    pos = mul(pos, projection);
    
    output.pos = pos;
    //output.tex = input.tex.xy;
    output.tex = (0.0f, 0.0f);
	
    return output;
};

float4 PSMain(PS_IN input) : SV_Target
{
    //float3 texColor = diffuseMap.Sample(samp, input.tex);
    //float3 ambientLight = ambientColor * ambientStrength * 0.0f;
    //float3 finalColor = float3(0.0f, 0.0f, 0.0f);
    
    return float4(0.0f, 0.0f, 0.0f, 1.0f);
};