float4x4 g_mWorld : WORLD;
float4x4 g_mViewProj : VIEWPROJECTION;

//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4  Pos     : POSITION;
    float4  Color	: COLOR0;
};

VS_OUTPUT VertScene( float4 Pos : POSITION, float4 Color : COLOR0)
{
    VS_OUTPUT o;

    o.Pos = mul( Pos, g_mWorld );
    o.Pos = mul( o.Pos, g_mViewProj );
    o.Color = Color;

    return o;
}

float4 PixScene( float4 Color : COLOR0 ) : COLOR0
{
    return Color;
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