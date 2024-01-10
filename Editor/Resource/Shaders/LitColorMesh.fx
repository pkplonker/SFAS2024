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

float DoAttenuation( Light light, float d )
{
    return 1.0f / ( light.ConstantAttenuation + light.LinearAttenuation * d + light.QuadraticAttenuation * d * d );
}

struct LightingResult
{
    float4 Diffuse;
    float4 Specular;
};

LightingResult DoPointLight( Light light, float3 V, float4 P, float3 N )
{
    LightingResult result;

    float3 L = ( light.Position - P ).xyz;
    float distance = length(L);
    L = L / distance;

    float attenuation = DoAttenuation( light, distance );

    result.Diffuse = DoDiffuse( light, L, N ) * attenuation;
    result.Specular = DoSpecular( light, V, L, N ) * attenuation;

    return result;
}

LightingResult DoDirectionalLight( Light light, float3 V, float4 P, float3 N )
{
    LightingResult result;

    float3 L = -light.Direction.xyz;

    result.Diffuse = DoDiffuse( light, L, N );
    result.Specular = DoSpecular( light, V, L, N );

    return result;
}

float DoSpotCone( Light light, float3 L )
{
    float minCos = cos( light.SpotAngle );
    float maxCos = ( minCos + 1.0f ) / 2.0f;
    float cosAngle = dot( light.Direction.xyz, -L );
    return smoothstep( minCos, maxCos, cosAngle ); 
}

LightingResult DoSpotLight( Light light, float3 V, float4 P, float3 N )
{
    LightingResult result;

    float3 L = ( light.Position - P ).xyz;
    float distance = length(L);
    L = L / distance;

    float attenuation = DoAttenuation( light, distance );
    float spotIntensity = DoSpotCone( light, L );

    result.Diffuse = DoDiffuse( light, L, N ) * attenuation * spotIntensity;
    result.Specular = DoSpecular( light, V, L, N ) * attenuation * spotIntensity;

    return result;
}

LightingResult ComputeLighting( float4 P, float3 N )
{
    float3 V = normalize( EyePosition - P ).xyz;

    LightingResult totalResult = { {0, 0, 0, 0}, {0, 0, 0, 0} };

    [unroll]
    for( int i = 0; i < MAX_LIGHTS; ++i )
    {
        LightingResult result = { {0, 0, 0, 0}, {0, 0, 0, 0} };

        if ( !Lights[i].Enabled ) continue;
        
        switch( Lights[i].LightType )
        {
        case DIRECTIONAL_LIGHT:
            {
                result = DoDirectionalLight( Lights[i], V, P, N );
            }
            break;
        case POINT_LIGHT: 
            {
                result = DoPointLight( Lights[i], V, P, N );
            }
            break;
        case SPOT_LIGHT:
            {
                result = DoSpotLight( Lights[i], V, P, N );
				//return DoSpotLight( Lights[i], V, P, N );
            }
            break;
        }
        totalResult.Diffuse += result.Diffuse;
        totalResult.Specular += result.Specular;
    }

    totalResult.Diffuse = saturate(totalResult.Diffuse);
    totalResult.Specular = saturate(totalResult.Specular);

    return totalResult;
}


float4 PS_Main( VertexShaderOutput IN ) : SV_TARGET
{
    LightingResult lit = ComputeLighting( IN.PositionWS, normalize(IN.NormalWS) );
    //LightingResult lit;
	//lit.Diffuse = float4(0,0,0,0);
	//lit.Specular = float4(0,0,0,0);

    float4 emissive = material.Emissive;
    float4 ambient = material.Ambient * GlobalAmbient;
    float4 diffuse = material.Diffuse * lit.Diffuse;
    float4 specular = material.Specular * lit.Specular;

    float4 texColor = IN.Color * material.Color;
    
    if ( material.UseTexture )
    {
        texColor = Texture.Sample( Sampler, IN.TexCoord )* material.Color;
    }

    float4 finalColor = ( emissive + ambient + diffuse + specular ) * texColor;

    return finalColor;
	return ambient * texColor;
	return float4(1,0,0,1);
}