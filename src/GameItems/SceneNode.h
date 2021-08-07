#ifndef _SCENENODE_H_
#define _SCENENODE_H_

#include "../Core.h"

class SceneNode {
public:
	virtual void render(glm::mat4 m = glm::mat4(0)) = 0;
};

#endif
