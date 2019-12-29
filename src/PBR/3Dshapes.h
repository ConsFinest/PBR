#ifndef _3D_SHAPES_H_
#define _3D_SHAPES_H_

#include "resources.h"
#include <vector>
#include "texture.h"

class Shape
{
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
	unsigned int indexCount;
	bool sphere;

	std::vector<Texture> textures;
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> uv;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;
	std::vector<float> data;

public:
	Shape(bool _sphere);
	~Shape();
	void render();
	void addTexture(Texture _texture);
	void bindTexture();
	void unbindTexture();


};




#endif // !_3D_SHAPES_H_
