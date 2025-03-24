struct VS_IN
{
    float4 pos : POSITION0;
    float2 tex : TEXCOORD0;
    float3 norm : NORMAL;
};

struct PS_IN
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 norm : TEXCOORD1;
    float3 worldPosition : TEXCOORD2;
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
    
    float3 diffuseLightColor;
    float diffuseLightStrength;
    float4 diffuseLightPosition;
    float4 attenuation;
};

Texture2D diffuseMap : register(t0);
SamplerState samp : register(s0);

PS_IN VSMain(VS_IN input)
{
    PS_IN output = (PS_IN) 0;
	
    float4 pos = float4(input.pos.xyzw);
    
    pos = mul(pos, transformations);
    
    output.worldPosition = pos;
    
    pos = mul(pos, view);
    pos = mul(pos, projection);
    
    output.pos = pos;
    
    output.tex = input.tex.xy;
    output.norm = normalize(mul(float4(input.norm, 0.0f), transformations));
	
    return output;
};

float4 PSMain(PS_IN input) : SV_Target
{
    float3 texColor = diffuseMap.Sample(samp, input.tex);
    float3 appliedLight = float3(0.0f, 0.0f, 0.0f);
    
    //ambient light
    float3 ambientLight = ambientColor * ambientStrength;
    appliedLight += ambientLight;
    
    //diffuse light
    float3 vectorToLight = normalize(diffuseLightPosition.xyz - input.worldPosition);
    
    float dist = distance(diffuseLightPosition.xyz, input.worldPosition);
    float attenuationFactor = 1 / (attenuation.x + attenuation.y * dist + attenuation.z * pow(dist, 2));
    
    float3 diffuseLightIntencity = max(dot(vectorToLight, input.norm), 0);
    diffuseLightIntencity *= attenuationFactor;
    
    float3 diffuseLight = diffuseLightIntencity * diffuseLightColor * diffuseLightStrength;
    
    appliedLight += diffuseLight;
    
    float3 finalColor = texColor * appliedLight;
    
    return float4(finalColor, 1.0f);
};