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
     
	float4 finalColor = frag.color;  
	if(useTex)
	{
		finalColor = colorMap.Sample(colorSample, frag.uv);
	}

	
    if(materialColor.x != 1.0f || materialColor.y != 1.0f || materialColor.z != 1.0f || materialColor.w != 1.0f )
    {
        finalColor = materialColor;
    }
    return finalColor;
}
