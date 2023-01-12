#ifndef _BONE_H_
#define _BONE_H_

#include <string>
#include "../Core.h"

/*
	NOTE TO SELF: Assimp specifies that each bone knows which vertices it affects and their weights, 
	but what we really want is a structure such that each vertex knows what bones affect it and how
	much do they affect it. The latter structure is better because we would be able to do the 
	weighted mesh transformation all in the vertex shader.
*/
class Bone {
public:
	glm::mat4 offsetMat; //the binding pose matrix
	glm::mat4 model; //the final model matrix

	Bone(glm::mat4 bindingPoseOffset);
};

#endif
