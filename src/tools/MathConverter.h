#ifndef _MATHCONVERTER_H_
#define _MATHCONVERTER_H_

#include "../Core.h"
#include <assimp/scene.h>

class MathConverter {
public:
	//convert a Assimp 4*4 matrix and return an equivalent glm::mat4 matrix. 
	static glm::mat4 getMat4FromAiMatrix(aiMatrix4x4& aiMat);
	static void printMatrix(glm::mat4 mat);
};

#endif // !_MATHCONVERTER_H_
