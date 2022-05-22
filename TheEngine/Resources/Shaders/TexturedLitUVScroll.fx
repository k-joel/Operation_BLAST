#include "Gamma.vsh"

struct TDirLight
{
	float4 m_Ambient;
	float4 m_Diffuse;
	float3 m_vDirection;
};

float4x4 g_mWorld : WORLD;
float4x4 g_mViewProj : VIEWPROJECTION;
float g_fGamma;
texture g_txScene;
TDirLight	g_tLight;

float g_fTime;
float2 g_vDirection;
float g_fRate;

//--------------------------------------------------------------------------------------
// Texture samplers
//--------------------------------------------------------------------------------------
sampler g_samScene =
sampler_state
{
    Texture = <g_txScene>;
    MinFilter = None;
    MagFilter = None;
    MipFilter = None;
    AddressU = Wrap;
    AddressV = Wrap;
};

struct VS_INPUT
{
    float3  Pos             : POSITION;
    float3  Normal          : NORMAL;
    float2  Tex0            : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4  Pos     : POSITION;
    float2  Tex0    : TEXCOORD0;
};

VS_OUTPUT VertScene( in VS_INPUT i )
{
    VS_OUTPUT o;

    o.Pos = mul( float4(i.Pos, 1.0), g_mWorld );
    o.Pos = mul( o.Pos, g_mViewProj );

    // normalize normals
    //o.Normal = normalize( mul( float4(i.Normal, 1.0), g_mWorld ) );
    
    // copy the input texture coordinate through
    float2 scrollAmt = mul(g_vDirection, (g_fTime * g_fRate));
    o.Tex0 = i.Tex0 + scrollAmt;

    return o;
}

float4 PixScene( float4  Normal	: NORMAL,
				 float2  Tex0 : TEXCOORD0 ) : COLOR0
{
	float4 texCol = tex2D( g_samScene, Tex0 );
	return GammaOutput(g_fGamma, texCol);
}

//--------------------------------------------------------------------------------------
// Techniques
//--------------------------------------------------------------------------------------

technique RenderScene
{
    pass p0
    {
        VertexShader = compile vs_2_0 VertScene();
        PixelShader = compile ps_2_0 PixScene();
    }
}