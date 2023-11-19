cbuffer cbChangedPerFrame : register(b0)
{
    matrix mvp;
};

Texture2D colorMap : register(t0);
SamplerState colorSample : register(s0);

struct VS_Input
{
    float4 pos  : POSITION;
    float2 tex0 : TEXCOORD0;
};

struct PS_Input
{
    float4 pos  : SV_POSITION;
    float2 tex0 : TEXCOORD0;
};

PS_Input VS_Main(VS_Input vertex)
{
    PS_Input vsOut = (PS_Input) 0;
    vsOut.pos = mul(vertex.pos, mvp);
    vsOut.tex0 = vertex.tex0;
    return vsOut;
}

float4 PS_Main(PS_Input frag) : SV_TARGET
{
    return float4(1.0f, 1.0f, 0.0f, 1.0f); // Red color
}