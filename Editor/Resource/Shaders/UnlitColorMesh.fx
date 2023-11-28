cbuffer cbChangedPerFrame : register(b0)
{
    matrix mvp;
};

cbuffer MaterialBuffer : register(b1)
{
    float4 materialColor;
};

struct VS_Input
{
    float4 position : POSITION;
    float4 color    : COLOR;
    float3 normal   : NORMAL;
    float2 uv       : TEXCOORD0;
};

struct PS_Input
{
    float4 position : SV_POSITION;
    float4 color    : COLOR;
    float3 normal   : NORMAL;
    float2 uv       : TEXCOORD0;
};

PS_Input VS_Main(VS_Input vertex)
{
    PS_Input vsOut;
    vsOut.position = mul(vertex.position, mvp);
    vsOut.color = vertex.color;
    vsOut.normal = vertex.normal;
    vsOut.uv = vertex.uv;
    return vsOut;
}

float4 PS_Main(PS_Input frag) : SV_TARGET
{
	float4 finalColor = frag.color;  // Corrected this line
    if(materialColor.x != 1.0f || materialColor.y != 1.0f || materialColor.z != 1.0f || materialColor.w != 1.0f )
    {
        finalColor = materialColor;
    }
    return finalColor;
}
