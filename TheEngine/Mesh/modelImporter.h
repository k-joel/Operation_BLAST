#ifndef _MODELIMPORTER_
#define _MODELIMPORTER_

class CModel;

class CModelImporter
{
private:
	CModelImporter();
	~CModelImporter();
public:
	static CModelImporter* GetInstance()
	{
		static CModelImporter toReturn;
		return &toReturn;
	}

	
void LoadAnimation(const char* filename, CModel& toFillOut);
void LoadBindPose(const char* filename, CModel& toFillOut);
void LoadModel(const char* filename, CModel& toFillOut);
};

#endif //_MODELIMPORTER_