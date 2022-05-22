#ifndef _CMOTION_
#define _CMOTION_

#include <vector>
using namespace std;

#include <d3dx9math.h>

class CMotion;
class CMotionImporter;
class CMotionControler;

struct tMotionFrame
{
	float m_time;
	D3DXMATRIX m_world;
};

class CMotionControler
{
	friend class CMotion;
	friend class CMotionImporter;
private:
	CMotion* motionRef;
	float m_currentTime;
	D3DXMATRIX m_world, m_goal;
	tMotionFrame *m_currentFrame, *m_nextFrame;
public:
	CMotionControler();
	~CMotionControler();
	D3DXMATRIX GetWorldMatrix();
	D3DXMATRIX GetWorldMatrix(unsigned int currentFrame);
	D3DXMATRIX GetWorldMatrix(float currentTime, float dt, float rate);
};

class CMotion
{
	friend class CMotionImporter;
	friend class CMotionControler;
private:


	float m_maxTime;
	vector<tMotionFrame> m_Frames;
	CMotionControler m_baseControler;
public:
	void duplicateControler(CMotionControler& toDuplicateInto);
};



#endif //_CMOTION_