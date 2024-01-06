#define MAX_POINT_LIGHTS 10
#define MAX_SPOTLIGHTS 10

struct BaseLight
{
    float4 color;
    float intensity;
    int type; // 0 for Point Light, 1 for Spotlight
};

struct PointLight
{
    BaseLight base;
    float3 position;
    float padding;
};

struct Spotlight
{
    BaseLight base;
    float3 position;
    float3 direction;
    float innerCone;
    float outerCone;
};

cbuffer cbChangedPerFrame : register(b0)
{
    matrix mvp;
    matrix worldMatrix;
	float3 ambientLightColor;
	float ambientLightIntensity;
};

cbuffer MaterialBuffer : register(b1)
{
    float4 materialColor;
	bool useTex;
};

cbuffer DirectionalLightBuffer : register(b2)
{
    float4 lightDirection;
    float4 lightColor;
	float lightIntensity;
};


cbuffer LightBuffer : register(b3)
{ 
    PointLight pointLights[MAX_POINT_LIGHTS];
    Spotlight spotlights[MAX_SPOTLIGHTS];
    int pointLightCount;
    int spotlightCount;
};


Texture2D colorMap : register(t0);
SamplerState colorSample : register(s0);

struct VS_Input
{
    float4 position : POSITION;
    float4 color    : COLOR;
    float3 normal   : NORMAL;
    float2 uv       : TEXCOORD;
};

struct PS_Input
{
    float4 position : SV_POSITION;
    float4 color    : COLOR;
    float3 normal   : NORMAL;
    float2 uv       : TEXCOORD;
	float3 worldNormal : NORMAL1;
};

PS_Input VS_Main(VS_Input vertex)
{
    PS_Input vsOut;
    vsOut.position = mul(vertex.position, mvp);
    vsOut.color = vertex.color;
    vsOut.normal = vertex.normal;
    vsOut.uv = vertex.uv;
	vsOut.worldNormal = mul(vertex.normal, (float3x3)worldMatrix);
    return vsOut;
}

float4 PS_Main(PS_Input frag) : SV_TARGET
{
    float3 normalizedLightDirection = normalize(lightDirection.xyz);
    float3 normalizedNormal = normalize(frag.worldNormal);

    float diff = max(dot(normalizedNormal, normalizedLightDirection), 0.0);
    float4 diffuseColor = diff * lightColor * lightIntensity;

    float3 ambient = ambientLightColor * ambientLightIntensity;

    float4 baseColor = useTex ? colorMap.Sample(colorSample, frag.uv) : frag.color;

    baseColor *= materialColor;

    float4 finalColor = baseColor * (diffuseColor + float4(ambient, 0));

    return float4(finalColor.xyz, 1);
}


