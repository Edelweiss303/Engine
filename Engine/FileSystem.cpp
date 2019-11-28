#include "Core.h"
#include "FileSystem.h"
#include "GameObjectManager.h"
#include "AssetManager.h"
#include "RenderSystem.h"

void FileSystem::initialize()
{
}

void FileSystem::update(float deltaTime)
{
	if (removeFiles.size() > 0)																// if there is any file to unload
	{
		for (auto iter : removeFiles)
		{
			//GameObjectManager::instance().unloadFile(fileData.find(iter)->second);
			//AssetManager::instance().unloadFile(fileData.find(iter)->second);
			fileData.erase(fileData.find(iter));											// find the file in the cached data and removed it
		}
		removeFiles.clear();																// clear the list used to store remove fileIds
	}
}

void FileSystem::loadRenderSystem(std::string& fileName)									//Method to load a RenderSystem file
{
	json::JSON fileJSON = parseJSON(fileName);

	if (isEmptyJSON != true)
	{
		fileId = getHashCode(fileName.c_str());
		fileData.emplace(fileId, fileJSON);
		//RenderSystem::instance().loadFile(fileJSON , fileId);
	}
	else
	{
		std::cout << " JSON is empty , not in .json format or File doesn't exist" << std::endl;
	}
}

void FileSystem::load(std::string& fileName, bool isLevelFile)								//Method to load a file
{
	json::JSON fileJSON = parseJSON(fileName);

	if (isEmptyJSON != true)
	{
		fileId = getHashCode(fileName.c_str());
		fileData.emplace(fileId, fileJSON);

		if (isLevelFile == true)			// check if the file is a level file and not protected. Then set it as current level
		{
			
			levels.push_back(currentLevel);
			currentLevel = fileId;
		}

		//GameObjectManager::instance().loadFile(fileJSON , fileId);
		//AssetManager::instance().loadFile(fileJSON , fileId);
	}
	else
	{
		std::cout << " JSON is empty , not in .json format or File doesn't exist" << std::endl;
	}
}

void FileSystem::unload(std::string& fileName)												//Method to unload a file
{
	STRCODE fileID = getHashCode(fileName.c_str());											// Convert filepath to STRCODE
	bool isSafe = true;																		// A check to see if file is safe to unload

	if (fileID == currentLevel)
	{
		if (levels.empty() != true)
		{
			levels.pop_back();
			currentLevel = levels.back();
		}
		else
		{
			currentLevel = 0;
		}
	}
	//------------------------------------




	//Remove the Protected Logic-------------
	for (auto iter : protectedFiles)														// Iterate through 'protectedFiles' list to check if the file is protected
	{
		if (iter == fileId)																	// If file is present in the list i.e protected file then dont unload
		{
			isSafe = false;
			std::cout << "store the files to be removed in the removeFiles List and remove it in the next update" << std::endl;
			break;
		}
	}

	if (isSafe)						// if file is safe to unload then add to the 'removeFile' list which will be deleted on update
	{
		removeFiles.push_back(fileID);
	}
}

json::JSON FileSystem::loadAsset(std::string& fileName)										//pass the Json file to the Asset Manager
{
	return parseJSON(fileName);
}

json::JSON FileSystem::parseJSON(std::string& fileName)										//Method to parse the Json FIle 
{
	std::ifstream inputStream(fileName);
	std::string JSONstr((std::istreambuf_iterator<char>(inputStream)), std::istreambuf_iterator<char>());

	if (JSONstr.empty() == true || JSONstr == "" || !inputStream)							// check if JSON is empty , not in .json format or File doesn't exist
	{
		isEmptyJSON = true;
		return NULL;
	}
	else
	{
		return (json::JSON::Load(JSONstr));
	}
}

STRCODE FileSystem::getCurrentLevel()														//returns current loaded level
{
	if (currentLevel != NULL)
	{
		return currentLevel;
	}
	else
	{
		std::cout << "No level has been loaded" << std::endl;
	}
}