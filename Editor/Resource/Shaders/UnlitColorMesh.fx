cbuffer PerObject : register(b0)
{
    matrix WorldMatrix;
    matrix InverseTransposeWorldMatrix;
    matrix WorldViewProjectionMatrix;
}

struct AppData
{
    float4 Position : POSITION;
	float4 Color    : COLOR;
    float3 Normal   : NORMAL;
    float2 TexCoord : TEXCOORD;
};

struct VertexShaderOutput
{
    float4 PositionWS   : TEXCOORD1;
    float3 NormalWS     : TEXCOORD2;
    float2 TexCoord     : TEXCOORD0;
    float4 Position     : SV_Position;
	float4 Color : COLOR;
};

VertexShaderOutput VS_Main(AppData IN)
{
    VertexShaderOutput OUT;

    OUT.Position = mul(WorldViewProjectionMatrix, IN.Position);
    OUT.PositionWS = mul(WorldMatrix, IN.Position);
    OUT.NormalWS = mul((float3x3)InverseTransposeWorldMatrix, IN.Normal);
    OUT.TexCoord = IN.TexCoord;
	OUT.Color = IN.Color;
    return OUT;
}

Texture2D Texture : register(t0);
sampler Sampler : register(s0);

struct Material
{
    float4 Emissive;    // Emissive color
    float4 Ambient;     // Ambient color
    float4 Diffuse;     // Diffuse color
    float4 Specular;    // Specular color
    float SpecularPower;
    bool UseTexture;    // Whether to use a texture
    float2 Padding;
	float4 Color;       // Material color
};

cbuffer MaterialProperties : register(b0)
{
    Material material;
};

float4 PS_Main(VertexShaderOutput IN) : SV_TARGET
{
    float4 texColor = IN.Color * material.Color;
    
    if (material.UseTexture)
    {
        texColor *= Texture.Sample(Sampler, IN.TexCoord);
    }

    float4 finalColor = material.Emissive + material.Ambient + texColor;
    return finalColor;
}
