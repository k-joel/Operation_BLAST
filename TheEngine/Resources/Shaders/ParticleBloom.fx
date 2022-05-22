
float4x4 g_mWorld : WORLD;
float4x4 g_mViewProj : VIEWPROJECTION;
texture g_txScene;

int g_SrcBlend;
int g_DestBlend;

float g_BloomThreshold = 0.25f;

float BloomIntensity = 1.0f;
float BaseIntensity = 0.5f;
float BloomSaturation = 1.0f;
float BaseSaturation = 0.5f;

//--------------------------------------------------------------------------------------
// Texture samplers
//--------------------------------------------------------------------------------------
sampler g_samScene =
sampler_state
{
    Texture = <g_txScene>;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Point;
    AddressU = Clamp;
    AddressV = Clamp;
};

struct VS_INPUT
{
    float3  Pos			:	POSITION;
    float3	Vel			:	TEXCOORD0;
    float3	Acc			:	TEXCOORD1;
    float	Life		:	TEXCOORD2;
    float	TotalLife	:	TEXCOORD3;
    float	Rot			:   TEXCOORD4;
    float	RotRate		:	TEXCOORD5;
    float	StartSize	:	TEXCOORD6;
    float	EndSize		:	TEXCOORD7;  
    //float	Size		:	PSIZE;
    float4	StartColor	:	COLOR0;
    float4	EndColor	:	COLOR1;
};

struct VS_OUTPUT
{
    float4  Pos     : POSITION;
    float2  Tex0    : TEXCOORD0;
    float4	RotMat	: COLOR1;
    float	Size	: PSIZE;
    float4	Color	: COLOR0;
};

VS_OUTPUT VertScene(in VS_INPUT i)
{
    // Zero out our output.
	VS_OUTPUT o = (VS_OUTPUT)0;

	// time scale
	float s = i.Life/i.TotalLife;
	
	// Constant acceleration.
	o.Pos.xyz = i.Pos + i.Vel*i.Life + 0.5f * i.Acc * i.Life * i.Life;
	
	// Transform to homogeneous clip space.
	o.Pos = mul( float4(o.Pos.xyz, 1.0), g_mWorld );
    o.Pos = mul( o.Pos, g_mViewProj );
	
	// Rotate
	float theta = i.Rot + i.RotRate*i.Life;
	
	float sine = sin(theta);
	float cosine = cos(theta);
	
	float4 rotationMatrix = float4(cosine, -sine, sine, cosine);        
	rotationMatrix *= 0.5;    
	rotationMatrix += 0.5;        
	o.RotMat = rotationMatrix;

	// Color
	o.Color = lerp(i.StartColor, i.EndColor, s);
	
	// Size
	o.Size = lerp(i.StartSize, i.EndSize, s);
	
	// Done--return the output.
    return o;
}

float3 AdjustSaturation(float3 color, float saturation)
{
    float grey = dot(color, float3(0.3, 0.59, 0.11));
    return lerp(grey, color.rgb, saturation);
}

float4 PixScene( float4  Pos     : POSITION,
   				 float2  Tex0    : TEXCOORD0,
   				 float4	 RotMat  : COLOR1,
   				 float	 Size	 : PSIZE,
   				 float4	 Color	 : COLOR0  ) : COLOR0
{
	// We want to rotate around the middle of the particle, not the origin,    
	// so we offset the texture coordinate accordingly.  
	Tex0 -= 0.5;        
	// Apply the rotation matrix, after rescaling it back from the packed    
	// color interpolator format into a full -1 to 1 range.    
	float4 rot = RotMat * 2 - 1;
	Tex0 = mul(Tex0, float2x2(rot));        
	// scale the edges of the square so it doesnt get clipped
	Tex0 *= sqrt(2);
	// Undo the offset used to control the rotation origin.    
	Tex0 += 0.5; 

	// sample the tex and mult the color
	float4 texCol = tex2D( g_samScene, Tex0 ) * Color;
	
	// MORE BLOOM
    float3 base = texCol.rgb / texCol.a;
    float3 bloom = saturate((base - g_BloomThreshold) / (1 - g_BloomThreshold));
    
    // Adjust color saturation and intensity.
    bloom = AdjustSaturation(bloom, BloomSaturation) * BloomIntensity;
    base = AdjustSaturation(base, BaseSaturation) * BaseIntensity;
    
    // Darken down the base image in areas where there is a lot of bloom,
    // to prevent things looking excessively burned-out.
    base *= (1 - saturate(bloom));
    
    // Combine the two images.
    return float4((base + bloom) * texCol.a, texCol.a);
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

        AlphaBlendEnable = True;
	    //SrcBlend     = SrcAlpha;
	    //DestBlend    = One;

		PointSpriteEnable = True;
		PointScaleEnable = True;
		PointScale_A = 0.0f;
		PointScale_B = 0.0f;
		PointScale_C = 100.0f;
		//PointSize_Min = 0.0f;
		//PointSize_Max = 9999.0f;
      
	    ZWriteEnable = False;
    }
}