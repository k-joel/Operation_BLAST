#pragma once

// The include files for Direct3D9
#include "CModel.h"
#include <vector>
#include <string>
using std::vector;
using std::string;

class CModelManager
{
private:
	// All the data we need to contain a model.
	typedef struct _MODEL
	{
		char				filename[_MAX_PATH];	// The filename of this model.
		int					ref;					// The number of times this model has be loaded.
		CModel*				model;				// A pointer to the model.

		// Default constructor.
		_MODEL()
		{
			filename[0] = '\0';
			ref = 0;
			model = 0;
		}
	} MODEL, *LPMODEL;

private:
	///////////////////////////////////////////////////////////////////
	//	Function:	CModelManager(Constructors)
	///////////////////////////////////////////////////////////////////
	CModelManager(void);
	CModelManager(CModelManager &ref);
	CModelManager &operator=(CModelManager &ref);

private:
	vector<MODEL>				m_Models;			// A list of all the loaded models.
	static CModelManager		m_Instance;			// An instance to this class.

public:
	CModel* GetModel(int nID);

public:
	~CModelManager(void);

	static CModelManager *GetInstance(void);

	void ShutdownModelManager(void);

	int LoadModel(const char* szFilename);

	void UnloadModel(int nID);
};