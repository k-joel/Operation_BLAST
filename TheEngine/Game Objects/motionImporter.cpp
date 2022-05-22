#include "motionImporter.h"

#include <fstream>
using namespace std;

#include <d3dx9math.h>

CMotionImporter::CMotionImporter()
{}

CMotionImporter::~CMotionImporter()
{}

void CMotionImporter::LoadMotion(char* filename, CMotion& toLoadInto)
{
	unsigned int number;
	D3DXMATRIX tempWorld;

	char *nameBuffer = 0;
	D3DXMATRIX tempOreintation;

	//clean out toLoadInto
	toLoadInto.m_Frames.clear();
	toLoadInto.m_baseControler.motionRef = &toLoadInto;
	D3DXMatrixIdentity(&toLoadInto.m_baseControler.m_world);

	fstream fin;
	fin.open(filename, ios_base::in | ios_base::binary);

	if(fin.is_open())
	{
		//version
		fin.read((char*)&(number), sizeof(unsigned int));

		//number of frames
		fin.read((char*)&(number), sizeof(unsigned int));

		//endtime
		fin.read((char*)&(toLoadInto.m_maxTime), sizeof(unsigned int));

		//for each frame
		for(unsigned int frameIndex = 0; frameIndex < number; frameIndex += 1)
		{
			tMotionFrame tempFrame;
			//time
			fin.read((char*)&(tempFrame.m_time), sizeof(float));
			//world
			fin.read((char*)&(tempFrame.m_world), sizeof(D3DXMATRIX));

			toLoadInto.m_Frames.push_back(tempFrame);
		}
	}

	fin.clear();
	fin.close();
}