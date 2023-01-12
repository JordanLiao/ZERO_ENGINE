#ifndef _SKELETALANIMATION_H_
#define _SKELETALANIMATION_H_

#include "../Core.h"
#include "glm/gtx/quaternion.hpp"

#include "Node.h"
#include "Bone.h"

#include <vector>
#include <string>
#include <unordered_map>
#include <assimp/scene.h>

#define NUM_OF_KEYFRAME_COMPONENTS 3 //three components for each keyframe: scaling, rotation, translation.
#define SCALING_INDEX 0
#define ROTATION_INDEX 1
#define TRANSLATION_INDEX 2

enum class AnimationBehavior {
	base, //value of the default node transformation
	constant, //The nearest key value without interpolation
	linear,  //linear interpolation of the nearest TWO keys
	repeat //animation is repeated 
};

/*
	Animation channel class for skeletal/Node based animation.
	Describes animation for a node of a scene/object
*/
class NodeChannel {

public:
	std::string name;
	Node* node; //the node that this channel animates
	//the following three vectors store keyframe values and their time. Their sizes may not be the same
	std::vector<std::pair<float, glm::vec3>> positionKeys;
	std::vector<std::pair<float, glm::vec3>> scalingKeys;
	std::vector<std::pair<float, glm::quat>> rotationKeys; //rotations are stored as quarternions.

	//channel extrapolation modes
	AnimationBehavior prestate, poststate;

	//precomputed extrapolated transformation for pre and post state CONSTANT behavior 
	glm::mat4 constPostStateModel;
	glm::mat4 constPreStateModel;

	NodeChannel(Node* root, aiNodeAnim* animNode);

private:
	//convert an assimp animation extrapolation bahavior to an AnimationBehavior enum type
	AnimationBehavior convertAnimationBehavior(aiAnimBehaviour& ani);
};

/*
	Animation class for skeletal/Node based animation.
*/
class SkeletalAnimation {
public:
	std::string name;
	float durationInTicks; //length of the animation in ticks
	float ticksPerSecond;
	std::vector<NodeChannel*> channels;
	std::unordered_map<std::string, unsigned int> boneNameToID;
	float startTime; //starting time of the current animation in milliseconds
	bool started;

	SkeletalAnimation(Node* root, aiAnimation* anim, std::unordered_map<std::string, unsigned int>& boneNameToID);
	~SkeletalAnimation();

	void start(float time);
	void end();
	void compute(float time);

private:
	Node* root;
	size_t numChannels;
	//the most recent keyframe for each channels
	std::vector<glm::ivec3> currKeyframes;

	void computeChannel(NodeChannel* channel, glm::ivec3& currKey, float timePassed);
	void computeScaling(glm::vec3& scale, NodeChannel* channel, float timePassed, glm::ivec3& currKey);
	void computeRotation(glm::quat& quat, NodeChannel* channel, float timePassed, glm::ivec3& currKey);
	void computeTranslation(glm::vec3& trans, NodeChannel* channel, float timePassed, glm::ivec3& currKey);
};

#endif
