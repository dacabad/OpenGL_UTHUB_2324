#pragma once

#include <string>
#include <vector>

#include "glm.hpp"

struct Transform
{
	glm::vec3 location{0.f};
	glm::vec3 rotation{0.f};
	glm::vec3 scale{1.f};
};

class Model
{
public:

	Transform transform_;
	std::vector<float>	vertices;
	std::vector<int>	indices;

public:

	Model(const std::string& path) : transform_()
	{
		loadModel(path);
	}

private:

	void loadModel(const std::string& path);
};


