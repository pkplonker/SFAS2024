// https://www.3dgep.com/texturing-lighting-directx-11/#Materials_Properties

cbuffer PerObject : register( b0 )
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

VertexShaderOutput VS_Main( AppData IN )
{
    VertexShaderOutput OUT;

    OUT.Position = mul( WorldViewProjectionMatrix, IN.Position );
    OUT.PositionWS = mul( WorldMatrix, IN.Position );
    OUT.NormalWS = mul( (float3x3)InverseTransposeWorldMatrix, IN.Normal );
    OUT.TexCoord = IN.TexCoord;
	OUT.Color = IN.Color;
    return OUT;
}

#define MAX_LIGHTS 8

// Light types.
#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2

Texture2D Texture : register(t0);
sampler Sampler : register(s0);

struct Material
{
    float4  Emissive;       
    float4  Ambient;        
    float4  Diffuse;        
    float4  Specular;       
    float   SpecularPower;  
    bool    UseTexture;     
    float2  Padding;        
	float4  Color;
};

cbuffer MaterialProperties : register(b0)
{
    Material material;
};

struct Light
{
    float4      Position;               
    float4      Direction;              
    float4      Color;                  
    float       SpotAngle;              
    float       ConstantAttenuation;    
    float       LinearAttenuation;      
    float       QuadraticAttenuation;  
    int         LightType;            
    bool        Enabled;          
    int2        Padding;    
};

cbuffer LightProperties : register(b1)
{
    float4 EyePosition;                
    float4 GlobalAmbient;              
    Light Lights[MAX_LIGHTS];          
};                       

float4 DoDiffuse( Light light, float3 L, float3 N )
{
    float NdotL = max( 0, dot( N, L ) );
    return light.Color * NdotL;
}

float4 DoSpecular( Light light, float3 V, float3 L, float3 N )
{
    // Phong lighting.
    float3 R = normalize( reflect( -L, N ) );
    float RdotV = max( 0, dot( R, V ) );

    // Blinn-Phong lighting
    float3 H = normalize( L + V );
    float NdotH = max( 0, dot( N, H ) );

    return light.Color * pow( NdotH, material.SpecularPower );
}

float4 PS_Main( VertexShaderOutput IN ) : SV_TARGET
{
  
	return float4(1,0,1,1);
}