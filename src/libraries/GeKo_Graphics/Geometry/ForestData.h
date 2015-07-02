#pragma once

#include <vector>
#include <glm/ext.hpp>

///This class just provides data for an example Forest
namespace TreeData{

std::vector<glm::vec3> forest1{ glm::vec3(16.0, 0.0, 76.0), glm::vec3(23.0, 0.0, 74.0),
		glm::vec3(21.5, 0.0, 78.5), glm::vec3(15.0, 0.0, 81.5), glm::vec3(27.5, 0.0, 88.5),
		glm::vec3(28.0, 0.0, 67.0), glm::vec3(24.0, 0.0, 71.0), glm::vec3(29.0, 0.0, 74.0), glm::vec3(15.0, 0.0, 75.0),
		glm::vec3(8.0, 0.0, 76.0), glm::vec3(3.0, 0.0, 77.0), glm::vec3(21.0, 0.0, 77.5), glm::vec3(25.0, 0.0, 80.0),
		glm::vec3(21.0, 0.0, 82.5), glm::vec3(16.0, 0.0, 82.5), glm::vec3(9.0, 0.0, 82.5), glm::vec3(3.0, 0.0, 84.0),
		glm::vec3(25.0, 0.0, 85.5), glm::vec3(15.0, 0.0, 86.0), glm::vec3(21.0, 0.0, 88.0), glm::vec3(8.0, 0.0, 87.5)
	};

std::vector<glm::vec3> forestShort{ glm::vec3(16.0, 0.0, 76.0),
glm::vec3(21.5, 0.0, 78.5), glm::vec3(15.0, 0.0, 81.5),
glm::vec3(28.0, 0.0, 67.0), glm::vec3(24.0, 0.0, 71.0), glm::vec3(15.0, 0.0, 75.0),
 glm::vec3(3.0, 0.0, 77.0), glm::vec3(21.0, 0.0, 77.5), glm::vec3(25.0, 0.0, 80.0),
glm::vec3(21.0, 0.0, 82.5), glm::vec3(3.0, 0.0, 84.0),
glm::vec3(25.0, 0.0, 85.5), glm::vec3(15.0, 0.0, 86.0), glm::vec3(21.0, 0.0, 88.0)

};

std::vector<glm::vec3> forest2{
	glm::vec3(52.3, 0.0, 58.8), glm::vec3(76.1, 0.0, 57.3), glm::vec3(65.0, 0.0, 83.5),
	glm::vec3(88.2, 0.0, 73.2), glm::vec3(82.8, 0.0, 92.0)
};

std::vector<glm::vec3>Tree{
	glm::vec3(125.0, 0.0, 100.0), glm::vec3(76.1, 0.0, 57.3), glm::vec3(75.0, 0.0, 100.0)
};

std::vector<glm::vec3> foodTrees{
    glm::vec3(76.1, 0.0, 57.3), glm::vec3(65.0, 0.0, 83.5),
	glm::vec3(88.2, 0.0, 73.2), glm::vec3(82.8, 0.0, 92.0)
};
}