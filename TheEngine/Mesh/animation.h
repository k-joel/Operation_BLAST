#ifndef _ANIMATIONCONTROLER_
#define _ANIMATIONCONTROLER_

#include <D3DX9Math.h>
#include <vector>
#include "..\Collision\CollisionObject.h"

class CModelImporter;

using namespace std;

enum boneID{
	BONEID_DEFAULT = 0,
	BONEID_GUNHAND = 1, BONEID_HEADHAT = 2,
	BONEID_BOSS_RLEG_END = 3, BONEID_BOSS_RLEG_ANKLE  = 4, BONEID_BOSS_LLEG_END = 5, BONEID_BOSS_LLEG_ANKLE = 6,
	BONEID_BOSS_RARM_ROOT = 7, BONEID_BOSS_RARM_ONE = 8, BONEID_BOSS_RARM_TWO = 9, BONEID_BOSS_RARM_THREE = 10, BONEID_BOSS_RARM_END = 11,
	BONEID_BOSS_LARM_ROOT = 12, BONEID_BOSS_LARM_ONE = 13, BONEID_BOSS_LARM_TWO = 14, BONEID_BOSS_LARM_THREE = 15, BONEID_BOSS_LARM_END = 16,
	BONEID_BODY = 17,
	BONEID_MAX
};

struct tBindBone
{
	int m_parentID;
	unsigned int m_selfID;
	unsigned int m_specialID;
	D3DXMATRIX m_inv_world_matrix;
};

struct tBindPose
{
	vector<tBindBone> m_pose;
};

struct tBone
{
	D3DXMATRIX m_world_matrix;
};

struct tKeyFrame
{
	std::vector<tBone> m_bones;

	float m_time;
};

struct tAnimation
{
	vector<tKeyFrame> m_keyframes;
};

class CAnimationControler;

class CAnimation
{
	friend class CModelImporter;
	friend class CAnimationControler;
	tBindPose m_pose;
	tAnimation m_animation;
	float startTime, endTime;
	//lua file connection
};

class CMesh;
class CModel;
class CAnimationControler
{
	friend class CModel;
	friend class CModelImporter;
private:
	CModel* modelRef;
	CAnimation* animRef;

	vector<D3DXMATRIX> m_palet;
	vector<D3DXMATRIX> m_worlds;

	bool m_resetLoops;
	int m_loopCount[4];
	float m_blendAnimStartTime[4];
	float m_blendAnimEndTime[4];
	float m_fElapsedTimes[4];
	float m_fRates[4];

	float m_blendAnimationWeight[4];
	float m_blendAnimationChange[4];
public:
	void reset();
	void SetAnimation(char* animName);
	void BlendAnimation(char* animName, float timeToBlend);
	void update(float dt);
	D3DXMATRIX* getMatrixByID(unsigned int specialID);
	D3DXMATRIX* getMatrixByIndex(unsigned int boneIndex);
	tBindBone* getBoneByID(unsigned int specialID);
	tBindBone* getBoneByIndex(unsigned int boneIndex);
	D3DXMATRIX* getMatrixPalet();
	unsigned int getNumberOfMatrixsInPalet();
	unsigned int getLoopCount();

	CModel* GetModel();
	CMesh* GetMesh();
};

#endif //_ANIMATIONCONTROLER_