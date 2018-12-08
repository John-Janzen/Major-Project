#include "FileLoader.h"

Shader * load_shader(const std::string & vert_path, const std::string & frag_path)
{
	GLuint shaderID = 0;
	std::string vertString = openFileRead(vert_path);
	std::string fragString = openFileRead(frag_path);

	if (!vertString.empty() && !fragString.empty())
	{
		shaderID = glCreateShader(type);

		const GLchar* shaderSource = vertString.c_str();
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
		printf("Issue loading Shaders\n");
		return nullptr;
	}
	printf("Shader Loaded: %s & %s\n", vert_path.c_str(), frag_path.c_str());
	return new Shader(,);
}

std::string openFileRead(const std::string & path)
{
	std::ifstream File(path.c_str());
	std::string data;
	if (File.is_open())
	{
		data.assign(std::istreambuf_iterator<char>(File), std::istreambuf_iterator<char>());
		File.close();
	}
	else
	{
		printf("Unable to open file %s\n", path.c_str());
		return;
	}
	return std::string();
}

Model * load_obj_file(
	const std::string & path)
{

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

		printf("Model Loaded: %s\n", path.c_str());
		return new Model(mallocSpace(finalData), mallocSpace(indices), (GLsizei)indices.size(), (GLsizei)finalData.size());
	}
}

Texture * load_texture(const std::string & path)
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
			
			printf("Texure Loaded: %s\n", path.c_str());

			return new Texture((GLuint*)_data, imgWidth, imgHeight, texWidth, texHeight);
		}
		ilBindImage(0);
		ilDeleteImages(1, &imgID);
		return nullptr;
	}
	else
	{
		ILenum ilError = ilGetError();
		printf("Error occured: %s\n", iluErrorString(ilError));
		return nullptr;
	}
}


