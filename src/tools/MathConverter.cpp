#include "MathConverter.h"

glm::mat4 MathConverter::getMat4FromAiMatrix(aiMatrix4x4& aiMat) {
	glm::mat4 mat;
	mat[0][0] = aiMat.a1; mat[0][1] = aiMat.b1; mat[0][2] = aiMat.c1; mat[0][3] = aiMat.d1;
	mat[1][0] = aiMat.a2; mat[1][1] = aiMat.b2; mat[1][2] = aiMat.c2; mat[1][3] = aiMat.d2;
	mat[2][0] = aiMat.a3; mat[2][1] = aiMat.b3; mat[2][2] = aiMat.c3; mat[2][3] = aiMat.d3;
	mat[3][0] = aiMat.a4; mat[3][1] = aiMat.b4; mat[3][2] = aiMat.c4; mat[3][3] = aiMat.d4;
	return mat;
}

void MathConverter::printMatrix(glm::mat4 mat) {
	std::cout << mat[0][0] << " " << mat[0][1] << " " << mat[0][2] << " " << mat[0][3] << std::endl;
	std::cout << mat[1][0] << " " << mat[1][1] << " " << mat[1][2] << " " << mat[1][3] << std::endl;
	std::cout << mat[2][0] << " " << mat[2][1] << " " << mat[2][2] << " " << mat[2][3] << std::endl;
	std::cout << mat[3][0] << " " << mat[3][1] << " " << mat[3][2] << " " << mat[3][3] << std::endl;
}
