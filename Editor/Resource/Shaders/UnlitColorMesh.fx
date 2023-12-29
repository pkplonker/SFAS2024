cbuffer cbChangedPerFrame : register(b0)
{
    matrix mvp;
	matrix worldMatrix;
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
    
    // Basic diffuse lighting
    float diff = max(dot(normalizedNormal, normalizedLightDirection), 0.0);
    float4 diffuseColor = diff * float4(1,1,1,1);

    // Combine with the existing color logic
    float4 finalColor = frag.color;  
    if(useTex)
    {
        finalColor = colorMap.Sample(colorSample, frag.uv);
    }

    if(materialColor.x != 1.0f || materialColor.y != 1.0f || materialColor.z != 1.0f || materialColor.w != 1.0f )
    {
        finalColor = materialColor;
    }

    // Apply lighting
    finalColor *= diffuseColor;

    return float4(lightColor.xyz,1);
}
