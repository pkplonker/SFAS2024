#define MAX_POINT_LIGHTS 10
#define MAX_SPOTLIGHTS 10

struct BaseLight
{
    float4 color;
    float intensity;
};

struct PointLight
{
    BaseLight base;
    float3 position;
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
	matrix viewMatrix;
    matrix projectionMatrix;
	float3 ambientLightColor;
	float ambientLightIntensity;
	float3 cameraPosition;
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
	int pointLightCount;
    int spotlightCount;
    PointLight pointLights[MAX_POINT_LIGHTS];
    Spotlight spotLights[MAX_SPOTLIGHTS];
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
    float3 viewDir = normalize(cameraPosition - frag.position.xyz);
    float3 normalizedLightDirection = normalize(lightDirection.xyz);
    float3 normalizedNormal = normalize(frag.worldNormal);

    float diff = max(dot(normalizedNormal, normalizedLightDirection), 0.0);
    float3 diffuseColor = diff * lightColor.xyz * lightIntensity;

    float3 reflectDir = reflect(-normalizedLightDirection, normalizedNormal);
    float specStrength = pow(max(dot(viewDir, reflectDir), 0.0), 16);
    float3 specularColor = specStrength * float3(1.0, 1.0, 1.0) * lightIntensity;

    float3 ambient = ambientLightColor * ambientLightIntensity;

    float4 baseColor = useTex ? colorMap.Sample(colorSample, frag.uv) : frag.color;
    baseColor *= materialColor;

    float3 finalColor = baseColor.xyz * (ambient + diffuseColor + specularColor);

    return float4(finalColor, 1);
}


