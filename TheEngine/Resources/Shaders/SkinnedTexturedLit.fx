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

#ifndef MATRIX_PALETTE_SIZE_DEFAULT
#define MATRIX_PALETTE_SIZE_DEFAULT 26
#endif

float4x4 amPalette[ MATRIX_PALETTE_SIZE_DEFAULT ];

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

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float3  Pos             : POSITION;
    float3  Normal          : NORMAL;
    float2  Tex0            : TEXCOORD0;
    float4  BlendWeights    : BLENDWEIGHT;
    uint4	BlendIndices    : BLENDINDICES;
};

struct VS_OUTPUT
{
    float4  Pos     : POSITION;
    float2  Tex0    : TEXCOORD0;
};

VS_OUTPUT VertSkinning(in VS_INPUT i )
{
    VS_OUTPUT	o;
    float3      Pos = 0.0f;
    float3      Normal = 0.0f;
    float		fWeight = 0.0f;

    //blending math
    for(uint blendIndex = 0; blendIndex < 4; blendIndex++)
    {
		fWeight = i.BlendWeights[blendIndex];
        Pos += mul( float4(i.Pos, 1.0), amPalette[ i.BlendIndices[blendIndex] ] ).xyz * fWeight;
        Normal += mul( i.Normal, (float3x3)amPalette[ i.BlendIndices[blendIndex] ] ) * fWeight;
    }

    // transform position from world space into world, then view, and then projection space
    o.Pos = mul( float4(Pos, 1.0), g_mWorld );
    o.Pos = mul( o.Pos, g_mViewProj );

    // normalize normals
    //o.Normal = normalize( mul( float4(Normal, 1.0), g_mWorld ) );

    // copy the input texture coordinate through
    o.Tex0  = i.Tex0;

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
        VertexShader = compile vs_2_0 VertSkinning();
        PixelShader = compile ps_2_0 PixScene();
    }
}