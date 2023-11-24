cbuffer cbChangedPerFrame : register(b0)
{
    matrix mvp;
};

Texture2D colorMap : register(t0);
SamplerState colorSample : register(s0);

struct VS_Input
{
    float4 pos  : POSITION;
    float4 color : COLOR;
    float3 normal : NORMAL;
    float2 tex0 : TEXCOORD0;
};

struct PS_Input
{
    float4 pos  : SV_POSITION;
    float4 color : COLOR; // Output color
    float2 tex0 : TEXCOORD0;
};

PS_Input VS_Main(VS_Input vertex)
{
    PS_Input vsOut;
    vsOut.pos = mul(vertex.pos, mvp);
    vsOut.color = vertex.color;
    vsOut.tex0 = vertex.tex0;
    return vsOut;
}

float4 PS_Main(PS_Input frag) : SV_TARGET
{
    float4 resultColor = colorMap.Sample(colorSample, frag.tex0);
    return resultColor * frag.color;
}
