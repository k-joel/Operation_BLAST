//	For macros
#include "../Core/Util.h" // for SAFE_RELEASE and DXERROR

#include "CModelManager.h"
#include "modelImporter.h"
#include <assert.h>		// code in assertions gets compiled out in Release mode

#pragma warning (disable : 4996)

CModelManager CModelManager::m_Instance;

///////////////////////////////////////////////////////////////////
//	Function:	"CModelManager(Constructor)"
///////////////////////////////////////////////////////////////////
CModelManager::CModelManager(void)
{
}

///////////////////////////////////////////////////////////////////
//	Function:	"CModelManager(Destructor)"
///////////////////////////////////////////////////////////////////
CModelManager::~CModelManager(void)
{
}

///////////////////////////////////////////////////////////////////
//	Function:	"GetInstance"
//
//	Last Modified:		6/26/2006
//
//	Input:		void
//
//	Return:		An instance to this class.
//
//	Purpose:	Gets an instance to this class.
///////////////////////////////////////////////////////////////////
CModelManager *CModelManager::GetInstance(void)
{
	return &m_Instance;
}

void CModelManager::ShutdownModelManager(void)
{
	for (unsigned int i = 0; i < m_Models.size(); i++)
	{
		// Remove ref.
		m_Models[i].ref = 0;

		// Release the Model if it's not being used.
		SAFE_DELETE(m_Models[i].model);
		m_Models[i].filename[0] = '\0';
	}

	// Clear the list of all loaded Models.
	m_Models.clear();
}

int CModelManager::LoadModel(const char* szFilename)
{
	//	Make sure the filename is valid.
	if (!szFilename)	return -1;

	// Make sure the Model isn't already loaded.
	for (unsigned int i = 0; i < m_Models.size(); i++)
	{
		// compare strings without caring about upper or lowercase.
		if (stricmp(szFilename, m_Models[i].filename) == 0)	// 0 means they are equal.
		{
			m_Models[i].ref++; // add a reference to this Model.
			return i;	// return the index.
		}
	}

	// Look for an open spot.
	int nID = -1;
	for (unsigned int i = 0; i < m_Models.size(); i++)
	{
		if (m_Models[i].ref == 0)
		{
			nID = i;
			break;
		}
	}

	// if we didn't find an open spot, load it in a new one
	if (nID == -1)
	{
		// A temp Model object.
		MODEL loaded;

		// Copy the filename of the loaded Model.
		strcpy(loaded.filename, szFilename);

		// Load the Model from the given file.
		loaded.model = new CModel;
		CModelImporter::GetInstance()->LoadModel((string((const char*)szFilename)+".NNmesh").c_str(), *loaded.model);
		CModelImporter::GetInstance()->LoadBindPose((string(szFilename) + ".NNskin").c_str(), *loaded.model);
		CModelImporter::GetInstance()->LoadAnimation((string(szFilename) +".NNanim").c_str(), *loaded.model);

		// AddRef.
		loaded.ref = 1;

		// Put the Model into the list.
		m_Models.push_back(loaded);

		// Return the nID of the Model.
		return (int)m_Models.size() - 1;
	}
	// we found an open spot
	else
	{
		// Make sure the Model has been released.
		SAFE_DELETE(m_Models[nID].model);

		// Copy the filename of the loaded Model.
		strcpy(m_Models[nID].filename, szFilename);

		// Load the Model from the given file.
		m_Models[nID].model = new CModel;

		CModelImporter::GetInstance()->LoadModel((string(szFilename)+".NNmesh").c_str(), *m_Models[nID].model);
		CModelImporter::GetInstance()->LoadBindPose((string(szFilename)+".NNskin").c_str(), *m_Models[nID].model);
		CModelImporter::GetInstance()->LoadAnimation((string(szFilename)+".NNanim").c_str(), *m_Models[nID].model);

		// AddRef
		m_Models[nID].ref = 1;

		// Return the nID of the Model.
		return nID;
	}
}

void CModelManager::UnloadModel(int nID)
{
	// Make sure the nID is in range.
	assert(nID > -1 && nID < (int)m_Models.size() && "nID is out of range");

	// Remove ref.
	m_Models[nID].ref--;

	// Release the Model if it's not being used.
	if (m_Models[nID].ref <= 0)
	{
		// Do a lazy delete and leave this spot empty
		SAFE_DELETE(m_Models[nID].model);
		m_Models[nID].filename[0] = '\0';
		m_Models[nID].ref = 0;
	}
}

CModel* CModelManager::GetModel(int nID)
{
	assert(nID > -1 && nID < (int)m_Models.size() && "nID is out of range");

	return m_Models[nID].model;
}

#pragma warning (default : 4996)