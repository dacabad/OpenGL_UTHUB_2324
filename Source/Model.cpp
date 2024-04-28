#include "Model.h"

#include <fstream>
#include <sstream>

void Model::loadModel(const std::string& path)
{
	std::ifstream is(path);
	if(!is.is_open()) return;

	std::string line;
	while(std::getline(is, line))
	{
		std::stringstream ss(line);
		std::string prefix;

		ss >> prefix;
		if(prefix == "v")
		{
			float x, y, z;
			ss >> x >> y >> z;
			
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);
		}
		else if(prefix == "f")
		{
			uint32_t a, b, c;
			ss >> a >> b >> c;

			indices.push_back(a - 1);
			indices.push_back(b - 1);
			indices.push_back(c - 1);
		}
	}
}
