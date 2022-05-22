#include "modelImporter.h"
#include "CModel.h"

#include <fstream>
#include <string>

#include "../Rendering/CDirect3D.h"
#include "../Rendering/CTextureManager.h"

CModelImporter::CModelImporter()
{}

CModelImporter::~CModelImporter()
{}

void CModelImporter::LoadModel(const char* filename, CModel& toFillOut)
{
	unsigned int number, size;
	char* textureName = 0;
	char* vertBuffer = 0, *triBuffer = 0;

	//set model name
	std::string fixedName;
	fixedName = filename;
	fixedName = fixedName.substr(fixedName.find_last_of("/")+1, fixedName.find_last_of(".") - fixedName.find_last_of("/")-1);

	memcpy(toFillOut.m_modelName, fixedName.c_str(), min(64, fixedName.size() + 1));

	std::fstream fin;
	fin.open(filename, std::ios_base::in | std::ios_base::binary);

	if(fin.is_open())
	{
		//version
		fin.read((char*)&number, sizeof(unsigned int));

		//number of textures
		fin.read((char*)&number, sizeof(unsigned int));
		unsigned int numberOfTextures = number;

		if(numberOfTextures > 1)
		{
			int badShit = 0;
		}

		//for each texture
		for(unsigned int textureIndex = 0; textureIndex < numberOfTextures; textureIndex += 1)
		{
			//uv layer
			fin.read((char*)&number, sizeof(unsigned int));

			//size of name
			fin.read((char*)&size, sizeof(unsigned int));

			delete[] textureName;
			textureName = new char[size];
			//char array of name
			fin.read(textureName, size);

			//there should be only one texture
			break;
		}

		//HACK: FILEPATH WILL BE FIXED
		std::string fullPath;
		fullPath += "Resources/Textures/";
		fullPath += textureName;

		//TODONE: load texture
		toFillOut.m_Mesh.SetTextureID(CTextureManager::GetInstance()->LoadTexture(fullPath.c_str()));

		delete[] textureName;

		//number of verts
		fin.read((char*)&number, sizeof(unsigned int));
		unsigned int numberOfVerts = number;

		//number of uv layers
		fin.read((char*)&number, sizeof(unsigned int));
		unsigned int numberOfUVs = number;

		unsigned int bufferSize = (6 + numberOfUVs*2)*numberOfVerts*sizeof(float);
		vertBuffer = new char[bufferSize];

		//vert buffer
		fin.read(vertBuffer, bufferSize);

		//number of triangle indexes
		fin.read((char*)&number, sizeof(unsigned int));
		unsigned int numberOfTris = number;

		bufferSize = sizeof(unsigned int) * 3 * numberOfTris;
		triBuffer = new char[bufferSize];

		//index buffer
		fin.read(triBuffer, bufferSize);

		unsigned int numIndices = 3*numberOfTris;

		toFillOut.m_Mesh.BuildMesh(D3DPT_TRIANGLELIST, numberOfTris, (void*)vertBuffer, numberOfVerts, sizeof(VertexPNT),
			(void*)triBuffer, numIndices, sizeof(int));

		delete[] vertBuffer;
		delete[] triBuffer;

		toFillOut.m_animControler.modelRef = &toFillOut;
		toFillOut.m_animControler.animRef = &toFillOut.m_animationData;
	}
	else
	{
			char szBuffer[256] = {0};
			sprintf(szBuffer, "Failed to Create Model - %s", filename); 
			MessageBox(0, szBuffer, "Model Importer Error", MB_OK);
	}

	fin.clear();
	fin.close();
}

void CModelImporter::LoadBindPose(const char* filename, CModel& toFillOut)
{
	unsigned int number, size;
	char* weightBuffer;

	std::fstream fin;
	fin.open(filename, std::ios_base::in | std::ios_base::binary);

	if(fin.is_open())
	{
		//version
		fin.read((char*)&number, sizeof(unsigned int));

		//number of verts
		fin.read((char*)&number, sizeof(unsigned int));
		unsigned int numberOfVerts = number;

		if(numberOfVerts != toFillOut.m_Mesh.GetVertexCount())
		{
			int badShit = 0;
		}

		size = sizeof(float)*8*numberOfVerts;
		weightBuffer = new char[size];

		//weight buffer
		fin.read(weightBuffer, size);

		toFillOut.m_Mesh.FillWeightBuffer((void*)weightBuffer, numberOfVerts, 32);

		//Free memory
		delete[] weightBuffer;

		//number of joints
		fin.read((char*)&number, sizeof(unsigned int));
		unsigned int numberOfJoints = number;

		toFillOut.m_animationData.m_pose.m_pose.clear();
		//for each joint
		for(unsigned int jointIndex = 0; jointIndex < numberOfJoints; jointIndex += 1)
		{
			tBindBone toAdd;
			//parent id
			fin.read((char*)&number, sizeof(unsigned int));
			toAdd.m_parentID = number;

			//self id
			fin.read((char*)&number, sizeof(unsigned int));
			toAdd.m_selfID = number;

			//special id
			fin.read((char*)&number, sizeof(unsigned int));
			toAdd.m_specialID = number;

			//inverse matrix
			fin.read((char*)&toAdd.m_inv_world_matrix, sizeof(D3DXMATRIX));

			toFillOut.m_animationData.m_pose.m_pose.push_back(toAdd);
		}

		toFillOut.m_animControler.m_palet.resize(toFillOut.m_animationData.m_pose.m_pose.size());
		toFillOut.m_animControler.m_worlds.resize(toFillOut.m_animationData.m_pose.m_pose.size());
	}

	fin.clear();
	fin.close();
}

void CModelImporter::LoadAnimation(const char* filename, CModel& toFillOut)
{
	unsigned int number;
	float fNumber;

	std::fstream fin;
	fin.open(filename, std::ios_base::in | std::ios_base::binary);

	if(fin.is_open())
	{
		//version
		fin.read((char*)&number, sizeof(unsigned int));

		//anim start time
		fin.read((char*)&fNumber, sizeof(float));
		toFillOut.m_animationData.startTime = fNumber;
		//anim end time
		fin.read((char*)&fNumber, sizeof(float));
		toFillOut.m_animationData.endTime = fNumber;

		//number bones (for reference)
		fin.read((char*)&number, sizeof(unsigned int));
		unsigned int numberOfBones = number;

		if(numberOfBones != toFillOut.m_animationData.m_pose.m_pose.size())
		{
			int badShitHere = 0;
		}

		//number of frames
		fin.read((char*)&number, sizeof(unsigned int));
		unsigned int numberOfFrames = number;

		toFillOut.m_animationData.m_animation.m_keyframes.clear();
		//for each frame
		for(unsigned int frameIndex = 0; frameIndex < numberOfFrames; frameIndex += 1)
		{
			tKeyFrame toAdd;
			//frame start time
			fin.read((char*)&toAdd.m_time, sizeof(float));

			//for each bone
			for(unsigned int jointIndex = 0; jointIndex < numberOfBones; jointIndex +=1)
			{
				tBone alsoToAdd;
				//matrix
				fin.read((char*)&alsoToAdd.m_world_matrix, sizeof(D3DXMATRIX));
				toAdd.m_bones.push_back(alsoToAdd);
			}

			toFillOut.m_animationData.m_animation.m_keyframes.push_back(toAdd);
		}

		toFillOut.m_animControler.m_fElapsedTimes[0] = 0.0f;
		toFillOut.m_animControler.m_fElapsedTimes[1] = 0.0f;
		toFillOut.m_animControler.m_fElapsedTimes[2] = 0.0f;
		toFillOut.m_animControler.m_fElapsedTimes[3] = 0.0f;
		toFillOut.m_animControler.SetAnimation("default");
		toFillOut.m_animControler.update(0.0f);
	}

	fin.clear();
	fin.close();
}