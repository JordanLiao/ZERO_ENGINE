#ifndef _SHADOWTESTER_H_
#define _SHADOWTESTER_H_

#include <vector>

#include "../Core.h"
#include "../Graphics/Instance.h"
#include "LightSource.h"
#include "../Renderer/Renderer.h"

#endif

class ShadowTester {
public:
	std::vector<Instance *> insts;
	LightSource * light;

	ShadowTester();
	void render();
};