struct VS_IN
{
    float4 pos : POSITION0;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct GS_IN
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : TEXCOORD1;
    float3 worldPosition : TEXCOORD2;
};

struct GS_OUT
{
    float4 position : SV_Position;
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
    Matrix lightSpace;
};

Texture2D diffuseMap : register(t0);
SamplerState samp : register(s0);

Texture2D shadowMap : register(t1);
SamplerComparisonState shadowSamp : register(s1);

//Shaders themselves
GS_IN VSMain(VS_IN input)
{
    GS_IN output = (GS_IN) 0;
	
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

bool IsSilhouetteEdge(float3 normal1, float3 normal2, float3 lightDir)
{
    float dot1 = dot(normal1, lightDir);
    float dot2 = dot(normal2, lightDir);
    return (dot1 * dot2) < 0.0;
}

[maxvertexcount(18)]
void GSMain(triangleadj GS_IN input[6], inout TriangleStream<GS_OUT> triStream)
{
    float3 lightDir = normalize(dirLight.direction);
    GS_IN v0 = input[0];
    GS_IN v2 = input[2];
    GS_IN v4 = input[4];

    bool isSilhouette = IsSilhouetteEdge(v0.normal, v2.normal, lightDir) 
            || IsSilhouetteEdge(v2.normal, v4.normal, lightDir)
            || IsSilhouetteEdge(v4.normal, v0.normal, lightDir);
    
    if (isSilhouette)
    {
        GS_OUT output;
        float3 extrudeWorld0 = v0.worldPosition + lightDir * 10000.0;
        float3 extrudeWorld2 = v2.worldPosition + lightDir * 10000.0;
        float3 extrudeWorld4 = v4.worldPosition + lightDir * 10000.0;
        
        float4 extrudePos0 = mul(float4(extrudeWorld0, 1.0), mul(view, projection));
        float4 extrudePos2 = mul(float4(extrudeWorld2, 1.0), mul(view, projection));
        float4 extrudePos4 = mul(float4(extrudeWorld4, 1.0), mul(view, projection));
        
        float4 basePos0 = v0.pos;
        float4 basePos2 = v2.pos;
        float4 basePos4 = v4.pos;

        output.position = v0.pos;
        triStream.Append(output);
        output.position = v2.pos;
        triStream.Append(output);
        output.position = extrudePos2;
        triStream.Append(output);
        triStream.RestartStrip();
        
        output.position = v2.pos;
        triStream.Append(output);
        output.position = extrudePos2;
        triStream.Append(output);
        output.position = extrudePos0;
        triStream.Append(output);
        triStream.RestartStrip();
        
        output.position = v2.pos;
        triStream.Append(output);
        output.position = v4.pos;
        triStream.Append(output);
        output.position = extrudePos4;
        triStream.Append(output);
        triStream.RestartStrip();
        
        output.position = v4.pos;
        triStream.Append(output);
        output.position = extrudePos4;
        triStream.Append(output);
        output.position = extrudePos2;
        triStream.Append(output);
        triStream.RestartStrip();
        
        output.position = v4.pos;
        triStream.Append(output);
        output.position = v0.pos;
        triStream.Append(output);
        output.position = extrudePos0;
        triStream.Append(output);
        triStream.RestartStrip();
        
        output.position = v0.pos;
        triStream.Append(output);
        output.position = extrudePos0;
        triStream.Append(output);
        output.position = extrudePos4;
        triStream.Append(output);
        triStream.RestartStrip();
    };    
};