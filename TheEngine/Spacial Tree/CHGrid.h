//////////////////////////////////////////////////////////////////////////////////////////
//																						//
// File: "CHGrid.h"																		//
//																						//
// Author: Kahran Ghosalkar																//
//																						//
// Purpose: A hierarchical grid used to reduce the collisions tests per object			//
//			to a cell.																	//
//																						//
//////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <list>
using std::list;

#include "../Game Objects/CBase.h"

#define HGRID_MAX_LEVELS 8
#define NUM_BUCKETS 1024
#define MIN_CELL_SIZE 128.0f

const float SPHERE_TO_CELL_RATIO = 1.0f/4.0f;
const float CELL_TO_CELL_RATIO = 2.0f;

// Cell position
struct Cell {
	Cell(int px, int py, int pz) { x = px; y = py; z = pz; }
	int x, y, z;
};

class CHGrid
{
	unsigned int m_nOccupiedLevelsMask;
	int m_nObjectsAtLevel[HGRID_MAX_LEVELS];
	CBase* m_ObjectBucket[NUM_BUCKETS];
	int m_nTimeStamp[NUM_BUCKETS];
	int m_nTick;

	CHGrid();

	~CHGrid() {}
	CHGrid(const CHGrid&);
	CHGrid& operator=(const CHGrid&);

	static CHGrid		m_Instance;			// An instance to this class.

	// Computes hash bucket index in range [0, NUM_BUCKETS-1]
	int ComputeHashBucketIndex(Cell cellPos);

public:

	static CHGrid *GetInstance(void);

	void AddObjectToHGrid(CBase *pObj);

	void RemoveObjectFromHGrid(CBase *pObj);

	// Test collisions between object and all objects in hgrid
	void CheckObjAgainstGrid(CBase *pObj);
};