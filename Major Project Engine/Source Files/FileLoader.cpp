#include "FileLoader.h"

std::mutex io_lock, devIL_lock;

Shader * load_shader(const std::string & vert_path, const std::string & frag_path)
{
	GLuint vertexID = 0, fragID = 0;
	std::string vertString = openFileRead(vert_path);
	std::string fragString = openFileRead(frag_path);

	if (vertString.empty())
	{
		printf("Error opening file %s\n", vert_path.c_str());
		return nullptr;
	}
	if (fragString.empty())
	{
		printf("Error opening file %s\n", frag_path.c_str());
		return nullptr;
	}

	vertexID = compileShader(vertString, GL_VERTEX_SHADER);
	fragID = compileShader(fragString, GL_FRAGMENT_SHADER);

	if (vertexID == 0 || fragID == 0)
		return nullptr;
	
	printf("Shader Loaded: %s & %s\n", vert_path.c_str(), frag_path.c_str());
	return new Shader(vertexID, fragID);
}

std::string openFileRead(const std::string & path)
{
	std::unique_lock<std::mutex> u_lock(io_lock);
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
		return NULL;
	}
	return data;
}

const GLuint compileShader(const std::string shader, const GLenum type)
{
	GLuint compile = glCreateShader(type);

	const GLchar* source = shader.c_str();
	glShaderSource(compile, 1, (const GLchar**)&source, NULL);

	GLint shaderCompiled = GL_FALSE;
	glCompileShader(compile);

	glGetShaderiv(compile, GL_COMPILE_STATUS, &shaderCompiled);
	if (shaderCompiled != GL_TRUE)
	{
		printf("Unable to compile shader %d!\n\nSource:\n%s\n", compile, source);
		glDeleteShader(compile);
		compile = 0;
	}
	return compile;
}

Model * load_obj_file(const std::string & path)
{
	std::vector<GLuint> indices;
	std::vector<GLfloat> finalData;
	
	std::map<std::string, GLuint>::iterator loc;
	std::map<std::string, GLuint> library;

	GLuint location = 0;
	glm::vec3 vertex[50] = {}, normal[50] = {};
	glm::vec2 uv[50] = {};
	glm::uvec3 face[3];
	char lineHeader[8];
	std::string to;

	std::size_t v_count = 0, n_count = 0, t_count = 0;

	std::string opened_file = openFileRead(path);
	std::stringstream ss(opened_file);
	if (!opened_file.empty())
	{
		while (std::getline(ss, to, '\n'))
		{
			sscanf_s(to.c_str(), "%s", lineHeader, (unsigned)_countof(lineHeader));
			
			if (strcmp(lineHeader, "v") == 0)
			{
				sscanf_s(to.c_str(), "%s %f %f %f", lineHeader, (unsigned)_countof(lineHeader), &vertex[v_count].x, &vertex[v_count].y, &vertex[v_count].z);
				v_count++;
			}
			else if (strcmp(lineHeader, "vt") == 0)
			{
				sscanf_s(to.c_str(), "%s %f %f", lineHeader, (unsigned)_countof(lineHeader), &uv[t_count].x, &uv[t_count].y);
				t_count++;
			}
			else if (strcmp(lineHeader, "vn") == 0)
			{
				sscanf_s(to.c_str(), "%s %f %f %f", lineHeader, (unsigned)_countof(lineHeader), &normal[n_count].x, &normal[n_count].y, &normal[n_count].z);
				n_count++;
			}
			else if (strcmp(lineHeader, "f") == 0)
			{
				int matches = sscanf_s(to.c_str(), "%s %d/%d/%d %d/%d/%d %d/%d/%d", lineHeader, (unsigned)_countof(lineHeader),
					&face[0].x, &face[0].y, &face[0].z,		// Vertex // Texture // Normal
					&face[1].x, &face[1].y, &face[1].z,		// Vertex // Texture // Normal
					&face[2].x, &face[2].y, &face[2].z);	// Vertex // Texture // Normal

				if (matches != 10) {
					printf("File can't be read by our simple parser : ( Try exporting with other options )\n");
					break;
				}
				else
				{
					std::stringstream ss_name;
					for (int i = 0; i < 3; i++)
					{
						ss_name << face[i].x << '/' << face[i].y << '/' << face[i].z;
						if ((loc = library.find(ss_name.str())) == library.end())
						{
							indices.emplace_back(location);
							library.emplace(ss_name.str(), location++);
							finalData.emplace_back(vertex[face[i].x - 1].x);
							finalData.emplace_back(vertex[face[i].x - 1].y);
							finalData.emplace_back(vertex[face[i].x - 1].z);

							finalData.emplace_back(uv[face[i].y - 1].x);
							finalData.emplace_back(uv[face[i].y - 1].y);

							finalData.emplace_back(normal[face[i].z - 1].x);
							finalData.emplace_back(normal[face[i].z - 1].y);
							finalData.emplace_back(normal[face[i].z - 1].z);
						}
						else
						{
							indices.emplace_back(loc->second);
						}
						ss_name.str(std::string());
					}
				}
			}
		}
	} 
	else
	{
		printf("Error opening file: %s\n", path.c_str());
		return nullptr;
	}
	return new Model(mallocSpace(finalData), mallocSpace(indices), (GLsizei)indices.size(), (GLsizei)finalData.size());
	//return nullptr;
}

Texture * load_texture(const std::string & path)
{
	std::unique_lock<std::mutex> lk(devIL_lock);
	ilInit();
	iluInit();

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

GLuint powerOfTwo(GLuint num)
{
	if (num != 0)
	{
		num--;
		num |= (num >> 1); //Or first 2 bits
		num |= (num >> 2); //Or next 2 bits
		num |= (num >> 4); //Or next 4 bits
		num |= (num >> 8); //Or next 8 bits
		num |= (num >> 16); //Or next 16 bits
		num++;
	}
	return num;
}
