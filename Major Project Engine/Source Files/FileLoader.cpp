#include "FileLoader.h"

void FileLoader::FileLoader::Init()
{
	_models = ModelsStorage();
	_shaders = ShaderStorage();
	_textures = TextureStorage();
	ilInit();
}

void FileLoader::Close()
{
	_models.clear();
	_shaders.clear();
}

void FileLoader::load_shader(const std::string & path, const GLenum & type, Shader * & shader_loc)
{
	std::unordered_map<std::string, Shader*>::iterator it;
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
		sourceFile.close();
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
	}
	else
	{
		printf("Unable to open file %s\n", path.c_str());
		return;
	}
	add_shader(path, shader_loc = new Shader(shaderID, type));
	printf("Shader Loaded: %s\n", path.c_str());
}

void FileLoader::load_obj_file(
	const std::string & path,
	Model * & model_loc)
{
	std::unordered_map<std::string, Model*>::iterator it;
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
	char lineHeader[8];


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
				fgets(lineHeader, 8, file_stream);
			}
		}
		fclose(file_stream);
		Model * new_model = new Model();
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

void FileLoader::load_texture(const std::string & path, Texture * & text_loc)
{
	ILuint imgID = 0;
	ilGenImages(1, &imgID);
	ilBindImage(imgID);
	ILboolean success = ilLoadImage(path.c_str());
	if (success == IL_TRUE)
	{
		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		if (success == IL_TRUE)
		{
			GLuint imgWidth = (GLuint)ilGetInteger(IL_IMAGE_WIDTH);
			GLuint imgHeight = (GLuint)ilGetInteger(IL_IMAGE_HEIGHT);

			GLuint texWidth = powerOfTwo(imgWidth);
			GLuint texHeight = powerOfTwo(imgHeight);

			if (imgWidth != texWidth || imgHeight != texHeight)
			{
				iluImageParameter(ILU_PLACEMENT, ILU_UPPER_LEFT);
				iluEnlargeCanvas((int)texWidth, (int)texHeight, 1);
			}
			ILint size = ilGetInteger(IL_IMAGE_SIZE_OF_DATA);
			void* _data = malloc(size);
			ILubyte* data = ilGetData();
			memcpy(_data, data, size);

			Texture * new_texture = new Texture((GLuint*)_data, imgWidth, imgHeight, texWidth, texHeight);
			text_loc = new_texture;
			_textures.emplace(path, new_texture);

			printf("Texure Loaded: %s\n", path.c_str());
		}
		ilBindImage(0);
		ilDeleteImages(1, &imgID);
	}
	else
	{
		ILenum ilError = ilGetError();
		printf("Error occured: %s\n", iluErrorString(ilError));
	}
}
