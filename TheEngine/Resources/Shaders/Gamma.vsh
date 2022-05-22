#ifndef GAMMA_VSH
#define GAMMA_VSH

float4 GammaOutput(float gamma, float4 color)
{
	return pow(color, 1.0 / gamma);
}

#endif