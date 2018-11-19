#pragma once

#ifndef _FILELOADER_H
#define _FILELOADER_H

#include "RenderExternals.h"

#include <unordered_map>
#include <map>
#include <string>
#include <Windows.h>
#include <gl\GL.h>
#include <glm.hpp>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <chrono>
#include <iostream>

typedef std::unordered_map<std::string, std::shared_ptr<Model>> ModelsStorage;
typedef std::unordered_map<std::string, std::shared_ptr<Shader>> ShaderStorage;

class FileLoader
{
public:
	~FileLoader()
	{

	};

	static FileLoader& Instance()
	{
		static FileLoader inst;
		return inst;
	}

	void Init();

	void Close();

	void add_model(const std::string & path, const std::shared_ptr<Model> & model) { _models.emplace(path, model); }

	void add_shader(const std::string & path, const std::shared_ptr<Shader> & shader) { _shaders.emplace(path, shader); }

	void loadShaderFromFile(const std::string & path, const GLenum & type, std::shared_ptr<Shader> & shader);

	void obj_file_importer(const std::string & path, std::shared_ptr<Model> & model_loc);

private:
	FileLoader() {}

	template<typename Out>
	Out * mallocSpace(const std::vector<Out> & tooManyVecs)
	{
		Out* arr = (Out*)malloc(tooManyVecs.size() * sizeof(Out));
		int j = 0;
		for (auto it = tooManyVecs.begin(); it != tooManyVecs.end(); ++it, j++)
			arr[j] = *it;
		return arr;
	};

	ModelsStorage _models;
	ShaderStorage _shaders;

	unsigned int model_count = 0;
	unsigned int texture_count = 0;
};

inline void FileLoader::Init()
{
	_models = ModelsStorage();
	_shaders = ShaderStorage();
}

inline void FileLoader::Close()
{
	_models.clear();
	_shaders.clear();
}

inline void FileLoader::loadShaderFromFile(const std::string & path, const GLenum & type, std::shared_ptr<Shader> & shader_loc)
{
	std::unordered_map<std::string, std::shared_ptr<Shader>>::iterator it;
	if ((it = _shaders.find(path)) != _shaders.end())
	{
		shader_loc = (*it).second;
		return;
	}

	GLuint shaderID = 0;
	std::string shaderString;
	std::ifstream sourceFile(path.c_str());
	if (sourceFile.is_open())
	{
		shaderString.assign((std::istreambuf_iterator<char>(sourceFile)), std::istreambuf_iterator<char>());
		shaderID = glCreateShader(type);

		const GLchar* shaderSource = shaderString.c_str();
		glShaderSource(shaderID, 1, (const GLchar**)&shaderSource, NULL);

		GLint shaderCompiled = GL_FALSE;
		glCompileShader(shaderID);

		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &shaderCompiled);
		if (shaderCompiled != GL_TRUE)
		{
			printf("Unable to compile shader %d!\n\nSource:\n%s\n", shaderID, shaderSource);
			glDeleteShader(shaderID);
			shaderID = 0;
			return;
		}
		sourceFile.close();
	}
	else
	{
		printf("Unable to open file %s\n", path.c_str());
		return;
	}
	add_shader(path, shader_loc = std::make_shared<Shader>(shaderID, type));
	printf("Shader Loaded: %s\n", path.c_str());
}

inline void FileLoader::obj_file_importer(
	const std::string & path,
	std::shared_ptr<Model> & model_loc)
{
	std::unordered_map<std::string, std::shared_ptr<Model>>::iterator it;
	if ((it = _models.find(path)) != _models.end())
	{
		model_loc = (*it).second;
		return;
	}

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> textures;
	std::vector<glm::vec3> normals;
	std::vector<GLuint> indices;
	std::vector<GLfloat> finalData;

	std::map<std::string, GLuint>::iterator loc;
	std::map<std::string, GLuint> library;

	FILE * file_stream;

	glm::vec3 vertex, normal;
	glm::vec2 uv;
	glm::uvec3 face[3];
	std::stringstream ss;
	GLuint location = 0;
	char lineHeader[16];


	if (fopen_s(&file_stream, path.c_str(), "r") != 0)
	{
		printf("No data found within file %s\n", path.c_str());
	}
	else
	{
		while (1)
		{
			int res = fscanf_s(file_stream, "%s", &lineHeader, (unsigned int)_countof(lineHeader));
			if (res == EOF)
				break;

			if (strcmp(lineHeader, "v") == 0)
			{
				fscanf_s(file_stream, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				vertices.push_back(vertex);
			}
			else if (strcmp(lineHeader, "vt") == 0)
			{
				fscanf_s(file_stream, "%f %f\n", &uv.x, &uv.y);
				textures.push_back(uv);
			}
			else if (strcmp(lineHeader, "vn") == 0)
			{
				fscanf_s(file_stream, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
				normals.push_back(normal);
			}
			else if (strcmp(lineHeader, "f") == 0)
			{
				int matches = fscanf_s(file_stream, "%u/%u/%u %u/%u/%u %u/%u/%u\n",
					&face[0].x, &face[0].y, &face[0].z,		// Vertex // Texture // Normal
					&face[1].x, &face[1].y, &face[1].z,		// Vertex // Texture // Normal
					&face[2].x, &face[2].y, &face[2].z);	// Vertex // Texture // Normal

				if (matches != 9) {
					printf("File can't be read by our simple parser : ( Try exporting with other options\n");
					break;
				}
				else
				{
					for (int i = 0; i < 3; i++)
					{
						ss << face[i].x << '/' << face[i].y << '/' << face[i].z;
						if ((loc = library.find(ss.str())) == library.end())
						{
							indices.emplace_back(location);
							library.emplace(ss.str(), location++);
							finalData.emplace_back(vertices.at(face[i].x - 1).x);
							finalData.emplace_back(vertices.at(face[i].x - 1).y);
							finalData.emplace_back(vertices.at(face[i].x - 1).z);

							finalData.emplace_back(textures.at(face[i].y - 1).x);
							finalData.emplace_back(textures.at(face[i].y - 1).y);

							finalData.emplace_back(normals.at(face[i].z - 1).x);
							finalData.emplace_back(normals.at(face[i].z - 1).y);
							finalData.emplace_back(normals.at(face[i].z - 1).z);
						}
						else
						{
							indices.emplace_back(loc->second);
						}
						ss.str(std::string());
					}
				}
			}
			else
			{
				fgets(lineHeader, 16, file_stream);
			}
		}
		fclose(file_stream);
		std::shared_ptr<Model> new_model = std::make_shared<Model>();	
		new_model->setVertices(mallocSpace(finalData));
		new_model->setIndices(mallocSpace(indices));
		new_model->ISize = (GLsizei)indices.size();
		new_model->VSize = (GLsizei)finalData.size();
		model_loc = new_model;

		add_model(path, new_model);

		printf("Model Loaded: %s\n", path.c_str());
		model_count++;
	}

}

#endif // !_FILELOADER_H
