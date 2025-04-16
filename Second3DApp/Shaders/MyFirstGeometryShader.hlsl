struct GS_IN
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 worldPosition : TEXCOORD2;
};

struct GS_OUT
{
    float4 pos : SV_POSITION;
};

cbuffer ShadowConstants : register(b0)
{
    float4 lightPos;
    matrix worldViewProj; // Добавляем матрицу преобразования
};

bool IsSilhouetteEdge(float3 normal1, float3 normal2, float3 lightDir)
{
    float dot1 = dot(normal1, lightDir);
    float dot2 = dot(normal2, lightDir);
    return (dot1 * dot2) < 0.0;
}

[maxvertexcount(18)]
void main(triangleadj GS_IN input[6], inout TriangleStream<GS_OUT> triStream)
{
    float3 lightDir;
    if (lightPos.w == 0.0)
    {
        lightDir = normalize(lightPos.xyz); // Directional light
    }
    else
    {
        lightDir = normalize(lightPos.xyz - input[0].worldPosition); // Исправлено: используем worldPosition
    }

    GS_IN v0 = input[0];
    GS_IN v2 = input[2];
    GS_IN v4 = input[4];

    bool isSilhouette = IsSilhouetteEdge(v0.normal, v2.normal, lightDir) ||
                       IsSilhouetteEdge(v2.normal, v4.normal, lightDir) ||
                       IsSilhouetteEdge(v4.normal, v0.normal, lightDir);

    if (isSilhouette)
    {
        GS_OUT output;
        
        // Экструзия в мировых координатах
        float3 extrudeWorld0 = v0.worldPosition + lightDir * 1000.0;
        float3 extrudeWorld2 = v2.worldPosition + lightDir * 1000.0;
        float3 extrudeWorld4 = v4.worldPosition + lightDir * 1000.0;
        
        // Преобразование в пространство проекции
        float4 extrudePos0 = mul(float4(extrudeWorld0, 1.0), worldViewProj);
        float4 extrudePos2 = mul(float4(extrudeWorld2, 1.0), worldViewProj);
        float4 extrudePos4 = mul(float4(extrudeWorld4, 1.0), worldViewProj);
        
        // Основные позиции (уже в пространстве проекции)
        float4 basePos0 = v0.pos;
        float4 basePos2 = v2.pos;
        float4 basePos4 = v4.pos;

        output.pos = v0.pos;
        triStream.Append(output);
        output.pos = v2.pos;
        triStream.Append(output);
        output.pos = extrudePos2;
        triStream.Append(output);
        triStream.RestartStrip();

        output.pos = v2.pos;
        triStream.Append(output);
        output.pos = extrudePos2;
        triStream.Append(output);
        output.pos = extrudePos0;
        triStream.Append(output);
        triStream.RestartStrip();

        output.pos = v2.pos;
        triStream.Append(output);
        output.pos = v4.pos;
        triStream.Append(output);
        output.pos = extrudePos4;
        triStream.Append(output);
        triStream.RestartStrip();

        output.pos = v4.pos;
        triStream.Append(output);
        output.pos = extrudePos4;
        triStream.Append(output);
        output.pos = extrudePos2;
        triStream.Append(output);
        triStream.RestartStrip();

        output.pos = v4.pos;
        triStream.Append(output);
        output.pos = v0.pos;
        triStream.Append(output);
        output.pos = extrudePos0;
        triStream.Append(output);
        triStream.RestartStrip();

        output.pos = v0.pos;
        triStream.Append(output);
        output.pos = extrudePos0;
        triStream.Append(output);
        output.pos = extrudePos4;
        triStream.Append(output);
        triStream.RestartStrip();
    };
};
