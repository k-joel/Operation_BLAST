#include "Gamma.vsh"

float4x4 g_mWorld : WORLD;
float g_fGamma;
texture g_txScene;

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

    // copy the input texture coordinate through
    o.Tex0 = i.Tex0;

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