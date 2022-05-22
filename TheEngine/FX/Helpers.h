#pragma once

namespace Helpers
{
	static float GetRandomFloat(float a, float b)
	{
		if(a == b) return a;
		return ((b-a)*((float)rand()/RAND_MAX))+a;
	}

	static D3DXVECTOR3 GetRandomVec3(D3DXVECTOR3 vMin, D3DXVECTOR3 vMax)
	{
		D3DXVECTOR3 newVec;
		newVec.x = GetRandomFloat(vMin.x, vMax.x);
		newVec.y = GetRandomFloat(vMin.y, vMax.y);
		newVec.z = GetRandomFloat(vMin.z, vMax.z);
		return newVec;
	}

	static bool ConvertStringToBool(const char* s)
	{
		return s && _stricmp(s,"true")==0;
	}

	static float ConvertStringToFloat(const char* s)
	{
		return (float)strtod(s,NULL);
	}

	static int ConvertStringToInt(const char* s)
	{
		return atoi(s);
	}

	static unsigned int ConvertStringToUInt(const char* s)
	{
		return (unsigned int)strtoul(s,NULL,10);
	}

	static D3DXVECTOR3 MultiplyMatrixVector(D3DXVECTOR3 &v, D3DXMATRIX &m)
	{
		D3DXVECTOR3 r;
		//r.x = m._11 * v.x + m._12 * v.y + m._13 * v.z;
		//r.y = m._21 * v.x + m._22 * v.y + m._23 * v.z;
		//r.z = m._31 * v.x + m._32 * v.y + m._33 * v.z;
		r.x =  m._11 * v.x + m._21 * v.y + m._31 * v.z;
		r.y =  m._12 * v.x + m._22 * v.y + m._32 * v.z;
		r.z =  m._13 * v.x + m._23 * v.y + m._33 * v.z;
		return r;
	}
}