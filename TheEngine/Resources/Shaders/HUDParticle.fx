
float4x4 g_mWorld : WORLD;
texture g_txScene;

float g_fGamma;

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
    float3	StartVel	:	TEXCOORD0;
    float3	EndVel		:	TEXCOORD1;
    float3	StartAcc	:	TEXCOORD2;
    float3	EndAcc		:	TEXCOORD3;
    float3	OrbitLoc	:	TEXCOORD4;
    float3  OrbitRot	:	TEXCOORD5;
    float3	OrbitRotRate :	TEXCOORD6; 
	float4	Life		:	TEXCOORD7;
    float4	Rot			:	TEXCOORD8;
    float4	Size		:	TEXCOORD9;
    //float4	SubUV		:	TEXCOORD10;
    float4	StartColor	:	COLOR0;
    float4	EndColor	:	COLOR1;
};

struct VS_OUTPUT
{
    float4  Pos     : POSITION;
    float2  Tex0    : TEXCOORD0;
    float	Size	: PSIZE;
    float4	Color	: COLOR0;
    float4	RotMat	: COLOR1;
};

VS_OUTPUT VertScene(in VS_INPUT i)
{
    // Zero out our output.
	VS_OUTPUT o = (VS_OUTPUT)0;

	float curLife = i.Life[0];
	float totalLife = i.Life[1];
	float startRot = i.Rot[0];
	float endRot = i.Rot[1];
	float rotRate = i.Rot[2];
	float startSize = i.Size[0];
	float endSize = i.Size[1];
	
	// time scale
	float s = curLife/totalLife;
	
	float3 vel = lerp(i.StartVel, i.EndVel, s);
	float3 acc = lerp(i.StartAcc, i.EndAcc, s);
	
	// Constant acceleration.
	o.Pos.xyz = i.Pos + vel*curLife + 0.5f * acc * curLife * curLife;
	
	// Orbit Rotation
	float3 orbitRot = float3(i.OrbitRot.x + i.OrbitRotRate.x*curLife,
								i.OrbitRot.y + i.OrbitRotRate.y*curLife,
								i.OrbitRot.z + i.OrbitRotRate.z*curLife);
								
	float4x4 matRotX = float4x4( 1.0f,			0.0f, 0.0f,				0.0f,
								0.0f, cos(orbitRot.x), -sin(orbitRot.x),0.0f,
								0.0f, sin(orbitRot.x), cos(orbitRot.x), 0.0f,
								0.0f,			0.0f,			0.0f,	1.0f );
								
	float4x4 matRotY = float4x4( cos(orbitRot.y), 0.0f, sin(orbitRot.y), 0.0f,
											0.0f, 1.0f, 0.0f,			 0.0f,
								-sin(orbitRot.y), 0.0f, cos(orbitRot.y), 0.0f,
								0.0f,			  0.0f,			   0.0f, 1.0f ); 
								
	float4x4 matRotZ = float4x4( cos(orbitRot.z), -sin(orbitRot.z), 0.0f, 0.0f,
								sin(orbitRot.z), cos(orbitRot.z), 0.0f,	  0.0f,	
								0.0f,		    0.0f,			  1.0f,	  0.0f,
								0.0f,			0.0f,			  0.0f,	  1.0f);
	
	//float4x4 matTrans = float4x4( 1.0f, 0.0f, 0.0f, i.OrbitLoc.x,
								//0.0f, 1.0f, 0.0f, i.OrbitLoc.y,
								//0.0f, 0.0f, 1.0f, i.OrbitLoc.z,
								//0.0f, 0.0f, 0.0f, 1.0f);
	
	o.Pos.xyz += i.OrbitLoc;
	
	float4x4 matFinal = mul(matRotZ, mul(matRotX, matRotY)); //matRotZ * matRotY * matRotX;
	o.Pos = mul( float4(o.Pos.xyz, 1.0), matFinal );
							 
	// Transform to homogeneous clip space.
	o.Pos = mul( float4(o.Pos.xyz, 1.0), g_mWorld );
	
	
	// Rotate
	float rot = lerp(startRot, endRot, s);
	float theta = rot + rotRate*curLife;
	
	float sine = sin(theta);
	float cosine = cos(theta);
	
	float4 rotationMatrix = float4(cosine, -sine, sine, cosine);        
	rotationMatrix *= 0.5;    
	rotationMatrix += 0.5;        
	o.RotMat = rotationMatrix;

	// Color
	o.Color = lerp(i.StartColor, i.EndColor, s);
	
	// Size
	o.Size = lerp(startSize, endSize, s);
	
	// UV Atlas properties
	//float2 scale = 1 / float2( i.SubUV.x, i.SubUV.y );	// How much to scale the UVs to fit a tile
	//float index = floor(curLife * i.SubUV.z);			// Which Item in the Atlas to pick (INT)
	////
	//o.SubUV.xy = float2( index , floor(index / i.SubUV.y));
	//o.SubUV.zw = scale;
	
	// Done--return the output.
    return o;
}

float4 PixScene( float4  Pos     : POSITION,
   				 float2  Tex0    : TEXCOORD0,
   				 float	 Size	 : PSIZE,
   				 float4	 Color	 : COLOR0,
   				 float4	 RotMat  : COLOR1//,
   				 /*float4  SubUV	 : COLOR2*/ ) : COLOR0
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

	//Tex0 = Tex0 + float2( SubUV.x*10.0f, SubUV.y*10.0f );
	//Tex0 *= SubUV.zw;
	
	float4 texCol = tex2D( g_samScene, Tex0 );
	return texCol * Color;
}

//--------------------------------------------------------------------------------------
// Techniques
//--------------------------------------------------------------------------------------

technique NoBlend
{
    pass p0
    {
        VertexShader = compile vs_2_0 VertScene();
        PixelShader = compile ps_2_0 PixScene();

        AlphaBlendEnable = True;
		SrcBlend     = One;
	    DestBlend    = One;
		BlendOp = Add;
		
		PointSpriteEnable = True;
		PointScaleEnable = True;
		
	    ZWriteEnable = False;
    }
}

technique AdditiveBlend
{
    pass p0
    {
        VertexShader = compile vs_2_0 VertScene();
        PixelShader = compile ps_2_0 PixScene();

        AlphaBlendEnable = True;
	    SrcBlend     = SrcAlpha;
	    DestBlend    = One;
		BlendOp = Add;

		PointSpriteEnable = True;
		PointScaleEnable = True;
		//PointScale_A = 1.0f;
		//PointScale_B = 1.0f;
		//PointScale_C = 0.5f;
		
	    ZWriteEnable = False;
    }
}

technique TransparencyBlend
{
    pass p0
    {
        VertexShader = compile vs_2_0 VertScene();
        PixelShader = compile ps_2_0 PixScene();

        AlphaBlendEnable = True;
	    SrcBlend     = SrcAlpha;
	    DestBlend    = InvSrcAlpha;
		BlendOp = Add;
		
		AlphaTestEnable = True;
		
		PointSpriteEnable = True;
		PointScaleEnable = True;
      
	    ZWriteEnable = False;
    }
}