//modified from https://pastebin.com/ynhtfCey

cbuffer cbChangedPerFrame : register(b0)
{
    matrix mvp;
    matrix worldMatrix;
    float3 camForward;
    float2 screenSize;
};

cbuffer MaterialBuffer : register(b1)
{
    float4 materialColor;
	bool useTex;
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
	float3 viewDir  : TEXCOORD1;

};

PS_Input VS_Main(VS_Input vertex)
{
    PS_Input vsOut;
    vsOut.position = vertex.position;
    vsOut.color = vertex.color;
    vsOut.normal = vertex.normal;
    vsOut.uv = vertex.uv;

    float4 worldPosition = mul(vertex.position, worldMatrix);
    vsOut.viewDir = camForward;

    return vsOut;
}

float3 CalculateSkyColor(float3 viewDir)
{
    viewDir = normalize(viewDir);

    float3 skyColorAboveHorizon = float3(0.3, 0.55, 0.8) * 1.2;
    float3 horizonColor = float3(0.42, 0.39, 0.36);
    float3 skyColorBelowHorizon = float3(0.4, 0.4, 0.4); 
	
    float blendRange = 0.2;
    float horizonStart = -blendRange;
    float horizonEnd = blendRange;

    float blendFactor = smoothstep(horizonStart, horizonEnd, viewDir.y);

    float3 col = lerp(skyColorBelowHorizon, horizonColor, blendFactor);
    col = lerp(col, skyColorAboveHorizon, blendFactor);

    return col;
}



float4 PS_Main(PS_Input frag) : SV_TARGET
{
    float3 viewDir = frag.viewDir;

    float3 col = CalculateSkyColor(viewDir);

    return float4(col, 1.0);
}

