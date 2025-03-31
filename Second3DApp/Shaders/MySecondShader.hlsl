struct VS_IN
{
    float4 pos : POSITION0;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct PS_IN
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : TEXCOORD1;
    float3 worldPosition : TEXCOORD2;
};

cbuffer ConstBuf : register(b0)
{
    matrix transformations;
    matrix view;
    matrix projection;
    float4 color;
};

//Lighting
struct Material
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
};

struct DirectionalLight
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float4 direction;
};

struct PointLight
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float3 position;
    float range;
    float4 attentuation;
};

cbuffer LightBuf : register(b1)
{
    Material material;
    DirectionalLight dirLight;
    PointLight pntLights[8];
    float4 spectatorLocation;
};

//Lighting functions
void DirectionalLightComputing(Material material, DirectionalLight light, float3 normal, float3 vector2spectator,
    out float4 ambient, out float4 diffuse, out float4 specular)
{
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    //ambient
    ambient = material.ambient * light.ambient;
    
    //diffusion
    float3 vector2light = -light.direction.xyz;
    float diffuseFactor = max(dot(vector2light, normal), 0.0f);
    diffuse = diffuseFactor * material.diffuse * light.diffuse;
    
    //specular
    float3 reflectedLight = reflect(-vector2light, normal);
    float angle = min(max(dot(normalize(reflectedLight), normalize(vector2light)) + 1.0f, 0.0f), 1.0f);   
    float specularFactor = pow(max(dot(reflectedLight, vector2spectator), 0.0f), material.specular.w) * angle;
    specular = specularFactor * material.specular * light.specular;   
};

void PointLightComputing(Material material, PointLight light, float3 position, float3 normal, float3 vector2spectator,
    out float4 ambient, out float4 diffuse, out float4 specular)
{
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    float3 vector2light = light.position - position;
    float distance2light = length(vector2light);
    if (distance2light > light.range)
        return;
    
    vector2light /= distance2light;
    
    //ambient
    ambient = material.ambient * light.ambient;
    
    //diffusion
    float diffuseFactor = max(dot(vector2light, normal), 0.0f);
    diffuse = diffuseFactor * material.diffuse * light.diffuse;
    
    //specular
    float3 reflectedLight = reflect(-vector2light, normal);
    float angle = min(max(dot(normalize(reflectedLight), normalize(vector2light)) + 1.0f, 0.0f), 1.0f);
    float specularFactor = pow(max(dot(reflectedLight, vector2spectator), 0.0f), material.specular.w) * angle;
    specular = specularFactor * material.specular * light.specular;   
    
    //attentuation
    float att = 1 / dot(light.attentuation.xyz, float3(1.0f, distance2light, pow(distance2light, 2)));
    diffuse *= att;
    specular *= att;
};

Texture2D diffuseMap : register(t0);
SamplerState samp : register(s0);


//Shaders themselves
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
    output.normal = normalize(mul(float4(input.normal, 0.0f), transformations));
	
    return output;
};

float4 PSMain(PS_IN input) : SV_Target
{
    float3 texColor = diffuseMap.Sample(samp, input.tex);
    float3 appliedLight = float3(0.0f, 0.0f, 0.0f);
    
    float3 vector2spectator = normalize(spectatorLocation.xyz - input.worldPosition);
    float4 ambient, diffuse, specular;
    
    DirectionalLightComputing(material, dirLight, input.normal, vector2spectator, ambient, diffuse, specular);
    appliedLight = appliedLight + ambient.xyz + diffuse.xyz + specular.xyz;
    
    for (int i = 0; i < 8; i++)
    {
        PointLightComputing(material, pntLights[i], input.worldPosition, input.normal, vector2spectator, ambient, diffuse, specular);
        appliedLight = appliedLight + ambient.xyz + diffuse.xyz + specular.xyz;
    }
    
    float3 finalColor = texColor * appliedLight;
    
    return float4(finalColor, 1.0f);
};