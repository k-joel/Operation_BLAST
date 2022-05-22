#ifndef _LAYOUT_LOADER_
#define _LAYOUT_LOADER_

#include <vector>
using namespace std;

#include "../Mesh/CModel.h"

class CLayoutImporter
{
	enum ImportType{
		IMPORT_DEFAULT = 0,
		IMPORT_GROUND = 1, IMPORT_PLATFORM = 2,
		IMPORT_SPLIT = 3, IMPORT_MERGE = 4,
		IMPORT_TRANSPARENT = 5,
		IMPORT_JUMPPAD = 6,
		IMPORT_TRIGGER = 7,
		IMPORT_SPAWN = 8,
		IMPORT_GOAL = 9,
		IMPORT_WALL = 10,
		IMPORT_ALPHATEST = 11,
		IMPORT_UVSCROLL = 12,
		IMPORT_UVATLAS = 13,
		IMPORT_SHADOWRECEIVE = 14,
		IMPORT_PARTICLETRIGGER = 15,
		OBJ_MAX };

	private:
	CLayoutImporter();
	~CLayoutImporter();
public:
	static CLayoutImporter* GetInstance()
	{
		static CLayoutImporter toReturn;
		return &toReturn;
	}

	void LoadLayout(char* filename);
};

#endif //_LAYOUT_LOADER_