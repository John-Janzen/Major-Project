#include "FileLoader.h"

std::mutex devIL_lock;

bool LoadShaderFile(const std::string vert_path, const std::string frag_path, Shader * & shader)
{
	GLuint vertexID = 0, fragID = 0;
	std::string vertString = OpenFileRead(vert_path);
	std::string fragString = OpenFileRead(frag_path);

	if (vertString.empty())
	{
		printf("Error opening file %s\n", vert_path.c_str());
		return false;
	}
	if (fragString.empty())
	{
		printf("Error opening file %s\n", frag_path.c_str());
		return false;
	}

	vertexID = CompileShader(vertString, GL_VERTEX_SHADER);
	fragID = CompileShader(fragString, GL_FRAGMENT_SHADER);

	if (vertexID == 0 || fragID == 0)
		return false;
	
	printf("Shader Loaded: %s & %s\n", vert_path.c_str(), frag_path.c_str());
	if (shader != nullptr)
	{
		shader->_shaderID_Vert = vertexID;
		shader->_shaderID_Frag = fragID;
	}
	else
	{
		shader = new Shader(std::string(vert_path + frag_path), vertexID, fragID);
	}
	return true;
}

std::string OpenFileRead(const std::string & path)
{
	std::string data;
	//std::lock_guard<std::mutex> u_lock(io_lock);
	std::ifstream File(path.c_str());
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

const GLuint CompileShader(const std::string shader, const GLenum type)
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

bool LoadOBJModelFile(const std::string path, Model * & model)
{
	std::vector<GLuint> indices;
	std::vector<GLfloat> finalData;
	
	std::map<std::string, GLuint>::iterator loc;
	std::map<std::string, GLuint> library;

	GLuint location = 0;
	std::vector<glm::vec3> vertex, normal;
	std::vector<glm::vec2> uv;
	glm::uvec3 face[3];
	char lineHeader[8];
	std::string to;

	std::string opened_file = OpenFileRead(path);
	std::stringstream ss(opened_file);
	if (!opened_file.empty())
	{
		while (std::getline(ss, to, '\n'))
		{
			sscanf_s(to.c_str(), "%s", lineHeader, (unsigned)_countof(lineHeader));
			
			if (strcmp(lineHeader, "v") == 0)
			{
				glm::vec3 v_temp;
				sscanf_s(to.c_str(), "%s %f %f %f", lineHeader, (unsigned)_countof(lineHeader), &v_temp.x, &v_temp.y, &v_temp.z);
				vertex.emplace_back(v_temp);
			}
			else if (strcmp(lineHeader, "vt") == 0)
			{
				glm::vec2 t_temp;
				sscanf_s(to.c_str(), "%s %f %f", lineHeader, (unsigned)_countof(lineHeader), &t_temp.x, &t_temp.y);
				uv.emplace_back(t_temp);
			}
			else if (strcmp(lineHeader, "vn") == 0)
			{
				glm::vec3 n_temp;
				sscanf_s(to.c_str(), "%s %f %f %f", lineHeader, (unsigned)_countof(lineHeader), &n_temp.x, &n_temp.y, &n_temp.z);
				normal.emplace_back(n_temp);
			}
			else if (strcmp(lineHeader, "f") == 0)
			{
				int matches = sscanf_s(to.c_str(), "%s %d/%d/%d %d/%d/%d %d/%d/%d", lineHeader, (unsigned)_countof(lineHeader),
					&face[0].x, &face[0].y, &face[0].z,		// Vertex // Texture // Normal
					&face[1].x, &face[1].y, &face[1].z,		// Vertex // Texture // Normal
					&face[2].x, &face[2].y, &face[2].z);	// Vertex // Texture // Normal

				if (matches != 10) {
					printf("File can't be read by our simple parser : ( Try exporting with other options )\n");
					assert(matches != 10);
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
		if (model == nullptr)
			model = new Model(path.c_str(), MallocSpace(finalData), MallocSpace(indices), (GLsizei)indices.size(), (GLsizei)finalData.size());
		else
		{
			model->_vertices = MallocSpace(finalData);
			model->_indices = MallocSpace(indices);
			model->ISize = (GLsizei)indices.size();
			model->VSize = (GLsizei)finalData.size();
		}
		return true;
	} 
	else
	{
		printf("Error opening file: %s\n", path.c_str());
		return false;
	}
	return false;
}

bool LoadTextureFile(const std::string path, Texture * & texture)
{
	std::lock_guard<std::mutex> lk(devIL_lock);
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

			GLuint texWidth = PowerOfTwo(imgWidth);
			GLuint texHeight = PowerOfTwo(imgHeight);

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
			if (texture != nullptr)
			{
				texture->imgHeight = imgHeight;
				texture->imgWidth = imgWidth;
				texture->texWidth = texWidth;
				texture->texHeight = texHeight;
				texture->_texture = (GLuint*)_data;
			}
			else
			{
				texture = new Texture(path, (GLuint*)_data, imgWidth, imgHeight, texWidth, texHeight);
			}
		}
		ilBindImage(0);
		ilDeleteImages(1, &imgID);
		return true;
	}
	else
	{
		ILenum ilError = ilGetError();
		printf("Error occured: %s\n", iluErrorString(ilError));
		return false;
	}
	return false;
}

GLuint PowerOfTwo(GLuint num)
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
