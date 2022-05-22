//////////////////////////////////////////////////////////////////////////////////////////
//																						//
// File: "CAABB.h"																		//
//																						//
// Author: Kahran Ghosalkar																//
//																						//
// Purpose:	Handles AABB creation and collision with other primitive types				//
//																						//
//////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "CBaseBV.h"

class CAABB : public CBaseBV
{
private:
	TAABB m_AABB;
	TAABB m_OriginalAABB;

public:
	CAABB() { m_nType = BV_AABB; }
	~CAABB() {}

	//////////////////////////////////////////////////////////////////////////////////////
	//	Function:	"CreateFromVerts"													//
	//																					//
	//	Input:		pVerts		|	The array of vertices to create from				//
	//				dwNumVerts	|	The number of vertices in the array					//
	//				dwSize		|	The stride offset of each vertex					//
	//																					//
	//	Return:		void		|	Nothing												//
	//																					//
	//	Purpose:	Creates an AABB from the vertices passed in							//
	//////////////////////////////////////////////////////////////////////////////////////
	void CreateFromVerts(D3DXVECTOR3 *pVerts, DWORD dwNumVerts, DWORD dwSize);

	//////////////////////////////////////////////////////////////////////////////////////
	//	Function:	"Clone"																//
	//																					//
	//	Input:		pBV		|	The bounding volume to duplicate 						//
	//																					//
	//	Return:		void	|	Nothing													//
	//																					//
	//	Purpose:	Duplicates an AABB from the one passed in							//
	//////////////////////////////////////////////////////////////////////////////////////
	void Clone( CBaseBV *pBV );

	//////////////////////////////////////////////////////////////////////////////////////
	//	Function:	"Update"															//
	//																					//
	//	Input:		mat		|	The transformed matrix 									//
	//																					//
	//	Return:		void	|	Nothing													//
	//																					//
	//	Purpose:	Transforms the AABB by the matrix passed in							//
	//////////////////////////////////////////////////////////////////////////////////////
	void Update(D3DXMATRIX &mat);

	//////////////////////////////////////////////////////////////////////////////////////
	//	Function:	"CreateMesh"														//
	//																					//
	//	Input:		pBV		|	The AABB to create from									//
	//																					//
	//	Return:		void	|	Nothing													//
	//																					//
	//	Purpose:	Creates and stores a bounding mesh from the passed in AABB			//
	//////////////////////////////////////////////////////////////////////////////////////
	void CreateMesh(CBaseBV *pBV);

	//////////////////////////////////////////////////////////////////////////////////////
	//	Function:	"CreateMesh															//
	//																					//
	//	Input:		pBV		|	The collision primitive to test against					//
	//																					//
	//	Return:		bool	|	The result of the collision								//
	//																					//
	//	Purpose:	Performs collision against other types of primitives				//
	//////////////////////////////////////////////////////////////////////////////////////
	bool Test(CBaseBV *pBV);

	TAABB &GetAABB() { return m_AABB; }
	TAABB &GetOriginalAABB() { return m_OriginalAABB; }
};