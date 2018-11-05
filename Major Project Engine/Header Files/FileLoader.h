#pragma once
#include "RenderExternals.h"

#include <unordered_map>
#include <map>
#include <string>
#include <Windows.h>
#include <gl\GL.h>
#include <glm.hpp>
#include <fstream>
#include <sstream>
#include <glew.h>

typedef std::unordered_map<std::string, std::shared_ptr<Model>> ModelsStorage;

class FileLoader
{
public:
	~FileLoader() {};

	static FileLoader& instance()
	{
		static FileLoader inst;
		return inst;
	}

	void Init();

	void Close();

	void addModel(const std::string & path, const std::shared_ptr<Model> & pair);

	void ObjImporter(const std::string & path, std::shared_ptr<Model> & model_loc);

private:
	FileLoader();

	template<typename Out>
	Out * mallocSpace(const std::vector<Out> & tooManyVecs)
	{
		Out* arr = (Out*)malloc(tooManyVecs.size() * sizeof(Out));
		int j = 0;
		for (std::vector<Out>::const_iterator it = tooManyVecs.begin(); it != tooManyVecs.end(); ++it, j++)
			arr[j] = *it;
		return arr;
	}

	std::unique_ptr<ModelsStorage> _models;

	unsigned int model_count = 0;
	unsigned int texture_count = 0;
};

inline void FileLoader::Init()
{
	_models = std::make_unique<ModelsStorage>();
}

inline void FileLoader::Close()
{
	_models.reset();
	_models = nullptr;
}

inline void FileLoader::addModel(const std::string & path, const std::shared_ptr<Model> & model)
{
	_models->emplace(path, model);
}

inline void FileLoader::ObjImporter(
	const std::string & path, 
	std::shared_ptr<Model> & model_loc)
{
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> textures;
	std::vector<glm::u16vec3> faces;

	FILE * file_stream;
	if (fopen_s(&file_stream, path.c_str(), "r") != 0)
	{
		printf("No data found within file %s", path);
	}
	else
	{
		while (1)
		{
			char lineHeader[128];

			int res = fscanf_s(file_stream, "%s", &lineHeader, (unsigned int)_countof(lineHeader));
			if (res == EOF)
				break;

			if (strcmp(lineHeader, "v") == 0)
			{
				glm::vec3 vertex;
				fscanf_s(file_stream, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				vertices.push_back(vertex);
			}
			else if (strcmp(lineHeader, "vt") == 0)
			{
				glm::vec2 uv;
				fscanf_s(file_stream, "%f %f\n", &uv.x, &uv.y);
				textures.push_back(uv);
			}
			else if (strcmp(lineHeader, "vn") == 0)
			{
				glm::vec3 normal;
				fscanf_s(file_stream, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
				normals.push_back(normal);
			}
			else if (strcmp(lineHeader, "f") == 0)
			{
				glm::uvec3 face[3];
				int matches = fscanf_s(file_stream, "%u/%u/%u %u/%u/%u %u/%u/%u\n",
					&face[0].x, &face[0].y, &face[0].z,
					&face[1].x, &face[1].y, &face[1].z,
					&face[2].x, &face[2].y, &face[2].z);

				if (matches != 9) {
					printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				}
				faces.push_back(face[0]);
				faces.push_back(face[1]);
				faces.push_back(face[2]);
			}
			else
			{
				fgets(lineHeader, 128, file_stream);
			}
		}
		fclose(file_stream);

		printf("Model Loaded: %s\n", path);
		model_count++;
	}
}