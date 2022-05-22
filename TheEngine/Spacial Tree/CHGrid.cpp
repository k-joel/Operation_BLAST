//////////////////////////////////////////////////////////////////////////////////////////
//																						//
// File: "CHGrid.cpp"																		//
//																						//
// Author: Kahran Ghosalkar																//
//																						//
// Purpose: A hierarchical grid used to reduce the collisions tests per object			//
//			to a cell.																	//
//																						//
//////////////////////////////////////////////////////////////////////////////////////////

#include "CHGrid.h"
#include <assert.h>

CHGrid CHGrid::m_Instance;

CHGrid::CHGrid()
{
	m_nOccupiedLevelsMask = 0;
	memset(m_nObjectsAtLevel, 0, HGRID_MAX_LEVELS*4);
	memset(m_ObjectBucket, NULL, NUM_BUCKETS*4);
	memset(m_nTimeStamp, 0, NUM_BUCKETS*4);
	m_nTick = 0;
}

CHGrid *CHGrid::GetInstance(void)
{
	return &m_Instance;
}

int CHGrid::ComputeHashBucketIndex( Cell cellPos )
{
	const int h1 = 0x8da6b343; // Large multiplicative constants;
	const int h2 = 0xd8163841; // here arbitrarily chosen primes
	const int h3 = 0xcb1ab31f;
	int n = h1 * cellPos.x + h2 * cellPos.y + h3 * cellPos.z;
	n = n % NUM_BUCKETS;
	if (n < 0) n += NUM_BUCKETS;
	return n;
}

void CHGrid::CheckObjAgainstGrid( CBase *pObj )
{
	float size = MIN_CELL_SIZE;
	int startLevel = 0;
	unsigned int occupiedLevelsMask = m_nOccupiedLevelsMask;
	//Point pos = obj->pos;
	D3DXVECTOR3 pos = pObj->GetBVSphere().GetSphere().c;

	 //If all objects are tested at the same time, the appropriate starting
	 //grid level can be computed as:
	/*float diameter = 2.0f * pObj->GetBV().GetSphere().r;
	for ( ; size * SPHERE_TO_CELL_RATIO < diameter; startLevel++) {
		size *= CELL_TO_CELL_RATIO;
		occupiedLevelsMask >>= 1;
	 }*/

	// For each new query, increase time stamp counter
	m_nTick++;

	for (int level = startLevel; level < HGRID_MAX_LEVELS;
		size *= CELL_TO_CELL_RATIO, occupiedLevelsMask >>= 1, level++) {
			// If no objects in rest of grid, stop now
			if (occupiedLevelsMask == 0) break;
			// If no objects at this level, go on to the next level
			if ((occupiedLevelsMask & 1) == 0) continue;

			// Compute ranges [x1..x2, y1..y2] of cells overlapped on this level. To
			// make sure objects in neighboring cells are tested, by increasing range by
			// the maximum object overlap: size * SPHERE_TO_CELL_RATIO
			float delta = pObj->GetBVSphere().GetSphere().r + size * SPHERE_TO_CELL_RATIO + EPSILON;
			float ooSize = 1.0f / size;
			int x1 = (int)floorf((pos.x - delta) * ooSize);
			int y1 = (int)floorf((pos.y - delta) * ooSize);
			int x2 = (int) ceilf((pos.x + delta) * ooSize);
			int y2 = (int) ceilf((pos.y + delta) * ooSize);

			// Check all the grid cells overlapped on current level
			for (int x = x1; x <= x2; x++) {
				for (int y = y1; y <= y2; y++) {
					// Treat level as a third dimension coordinate
					Cell cellPos(x, y, level);

					int bucket = ComputeHashBucketIndex(cellPos);

					// Has this hash bucket already been checked for this object?
					if (m_nTimeStamp[bucket] == m_nTick) continue;
					m_nTimeStamp[bucket] = m_nTick;

					// Loop through all objects in the bucket to find nearby objects
					CBase *p = m_ObjectBucket[bucket];
					while (p) {
						if (p != pObj)
						{
							if(pObj->GetType() != p->GetType())
							{
								if(pObj->CheckCollision(p))
								{
									pObj->CollisionReact(p);
									//p->CollisionReact(pObj);
								}
							}
							//float dist2 = Sqr(pos.x - p->pos.x) + Sqr(pos.y - p->pos.y);
							//if (dist2 <= Sqr(obj->radius + p->radius + EPSILON))
							//	pCallbackFunc(obj, p); // Close, call callback function
						}
						p = p->GetNextObject();
					}
				}
			}
	} // end for level
}

void CHGrid::RemoveObjectFromHGrid( CBase *pObj )
{
	// One less object on this grid level. Mark level as unused if no objects left
	if (--m_nObjectsAtLevel[pObj->GetLevel()] == 0)
		m_nOccupiedLevelsMask &= ~(1 << pObj->GetLevel());

	// Now scan through list and unlink object 'obj'
	int bucket= pObj->GetBucket();
	CBase *p = m_ObjectBucket[bucket];
	// Special-case updating list header when object is first in list
	if (p == pObj) {
		m_ObjectBucket[bucket] = pObj->GetNextObject();
		return;
	}
	// Traverse rest of list, unlinking 'obj' when found
	while (p) {
		// Keep q as trailing pointer to previous element
		CBase *q = p;
		p = p->GetNextObject();
		if (p == pObj) {
			q->SetNextObject(p->GetNextObject()); // unlink by bypassing
			return;
		}
	}
	assert(0); // No such object in hgrid
}

void CHGrid::AddObjectToHGrid( CBase *pObj )
{
	// Find lowest level where object fully fits inside cell, taking RATIO into account
	int level;
	float size = MIN_CELL_SIZE, diameter = 2.0f * pObj->GetBVSphere().GetSphere().r;
	for (level = 0; size * SPHERE_TO_CELL_RATIO < diameter; level++)
		size *= CELL_TO_CELL_RATIO;

	// Assert if object is larger than largest grid cell
	assert(level < HGRID_MAX_LEVELS);

	// Add object to grid square, and remember cell and level numbers,
	// treating level as a third dimension coordinate
	Cell cellPos((int)(pObj->GetBVSphere().GetSphere().c.x / size), (int)(pObj->GetBVSphere().GetSphere().c.y / size), level);
	int bucket = ComputeHashBucketIndex(cellPos);
	pObj->SetBucket(bucket);
	pObj->SetLevel(level);
	pObj->SetNextObject(m_ObjectBucket[bucket]);
	m_ObjectBucket[bucket] = pObj;

	// Mark this level as having one more object. Also indicate level is in use
	m_nObjectsAtLevel[level]++;
	m_nOccupiedLevelsMask |= (1 << level);
}