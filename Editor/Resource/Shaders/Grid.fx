//modified from https://bgolus.medium.com/the-best-darn-grid-shader-yet-727f9278b9d8
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
	float3 worldPos : TEXCOORD1;
};

static const float GridScale = 1.0;
static const float LineWidthX = 0.01;
static const float LineWidthY = 0.01;
static const float4 BaseColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
static const float4 LineColor =  materialColor; 

PS_Input VS_Main(VS_Input vertex)
{
    PS_Input vsOut;
    vsOut.position = mul(vertex.position, mvp); 
    vsOut.worldPos = mul(vertex.position, worldMatrix).xyz; 
    // Pass through the other vertex attributes
    vsOut.color = vertex.color;
    vsOut.normal = vertex.normal;
    vsOut.uv = vertex.uv;
    return vsOut;
}

float PristineGrid(float2 gridPosition, float2 lineWidth)
{
    lineWidth = saturate(lineWidth);
    float2 gridUV = abs(frac(gridPosition) - 0.5);
    float2 grid = saturate((0.5 - gridUV - lineWidth) / fwidth(gridPosition));
    return min(grid.x, grid.y);
}

float4 PS_Main(PS_Input frag) : SV_TARGET
{
    float2 gridPositionXZ = frag.worldPos.xy * GridScale;
    float grid = PristineGrid(gridPositionXZ, float2(LineWidthX, LineWidthY));

    grid = 1.0 - grid;

    float4 color = lerp(BaseColor, LineColor, grid);

    return color;
}