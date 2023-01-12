#include "SkeletalAnimation.h"

NodeChannel::NodeChannel(Node* root, aiNodeAnim* animNode) {
	name = animNode->mNodeName.C_Str();
	node = root->find(name); //associate the correspond node of this channel
	if (node == nullptr)
		std::cerr << "Channel " << name << " does not have a matching node." << std::endl;

	positionKeys.resize(animNode->mNumPositionKeys);
	scalingKeys.resize(animNode->mNumScalingKeys);
	rotationKeys.resize(animNode->mNumRotationKeys);

	aiVectorKey* aiPositionKeys = animNode->mPositionKeys, *aiScalingKeys = animNode->mScalingKeys;
	aiQuatKey*  aiRotationKeys = animNode->mRotationKeys;

	//process position keyframes
	for (unsigned int i = 0; i < animNode->mNumPositionKeys; i++) {
		glm::vec3 val = glm::vec3((float)aiPositionKeys[i].mValue.x, (float)aiPositionKeys[i].mValue.y, (float)aiPositionKeys[i].mValue.z);
		positionKeys[i] = std::pair<float, glm::vec3>{aiPositionKeys[i].mTime, val};
	}
	//process scaling keyframes
	for (unsigned int i = 0; i < animNode->mNumScalingKeys; i++) {
		glm::vec3 val = glm::vec3((float)aiScalingKeys[i].mValue.x, (float)aiScalingKeys[i].mValue.y, (float)aiScalingKeys[i].mValue.z);
		scalingKeys[i] = std::pair<float, glm::vec3>{ aiScalingKeys[i].mTime, val};
	}
	//process rotation keyframes
	for (unsigned int i = 0; i < animNode->mNumRotationKeys; i++) {
		glm::quat val = glm::quat((float)aiRotationKeys[i].mValue.w, (float)aiRotationKeys[i].mValue.x,
		                          (float)aiRotationKeys[i].mValue.y, (float)aiRotationKeys[i].mValue.z);
		rotationKeys[i] = std::pair<float, glm::quat>{aiRotationKeys[i].mTime, val};
	}

	prestate = convertAnimationBehavior(animNode->mPreState);
	poststate = convertAnimationBehavior(animNode->mPostState);

	glm::mat4 rotation = glm::toMat4(glm::quat(rotationKeys.back().second));
	glm::mat4 scale = glm::scale(scalingKeys.back().second);
	glm::mat4 translation = glm::translate(positionKeys.back().second);
	constPostStateModel = translation * rotation * scale;
	rotation = glm::toMat4(glm::quat(rotationKeys.front().second));
	scale = glm::scale(scalingKeys.front().second);
	translation = glm::translate(positionKeys.front().second);
	constPreStateModel = translation * rotation * scale;
}

AnimationBehavior NodeChannel::convertAnimationBehavior(aiAnimBehaviour& ani) {
	if (ani == aiAnimBehaviour::aiAnimBehaviour_DEFAULT)
		return AnimationBehavior::base;
	else if (ani == aiAnimBehaviour::aiAnimBehaviour_CONSTANT)
		return AnimationBehavior::constant;
	else if (ani == aiAnimBehaviour::aiAnimBehaviour_LINEAR)
		return AnimationBehavior::linear;
	else if (ani == aiAnimBehaviour::aiAnimBehaviour_REPEAT)
		return AnimationBehavior::repeat;
	else
		return AnimationBehavior::base;
}

SkeletalAnimation::SkeletalAnimation(Node* root, aiAnimation* anim, std::unordered_map<std::string, unsigned int>& boneNameToID) {
	name = anim->mName.C_Str();
	if (anim->mNumChannels == 0)
		std::cerr << "Animation " << name << " is not a skeletal animation.";

	this->root = root;
	numChannels = anim->mNumChannels;
	this->boneNameToID = boneNameToID;
	durationInTicks = (float)anim->mDuration;
	ticksPerSecond = (float)anim->mTicksPerSecond;
	started = false;
	startTime = 0.0;

	channels.resize(numChannels);
	for (size_t i = 0; i < numChannels; i++) {
		channels[i] = new NodeChannel(root, anim->mChannels[i]);
	}

	currKeyframes = std::vector<glm::ivec3>(numChannels, glm::ivec3(0));
}

SkeletalAnimation::~SkeletalAnimation() {
	size_t numChannels = channels.size();
	for(size_t i = 0; i < numChannels; i++) {
		delete channels[i];
	}
}

void SkeletalAnimation::start(float time) {
	if (started)
		return;
	started = true;
	startTime = time;
}

void SkeletalAnimation::end() {
	started = false;
	currKeyframes = std::vector<glm::ivec3>(numChannels, glm::ivec3(0));
}

void SkeletalAnimation::compute(float time) {
	size_t size = channels.size();
	float timePassed = (time - startTime) * ticksPerSecond; //time passed in ticks
	for (size_t i = 0; i < size; i++) {
		computeChannel(channels[i],currKeyframes[i], timePassed);
	}
}

void SkeletalAnimation::computeChannel(NodeChannel* channel, glm::ivec3& currKey, float timePassed) {
	if (timePassed >= durationInTicks) { //animation has been fully played
		if (channel->poststate == AnimationBehavior::constant) {
			channel->node->model = channel->constPostStateModel;
			return;
		} else {
			timePassed = std::fmod(timePassed, durationInTicks); //for linear and repeat, need to loop from the 0th index
		}
	}

	glm::vec3 scale;
	computeScaling(scale, channel, timePassed, currKey);
	glm::quat quat;
	computeRotation(quat, channel, timePassed, currKey);
	glm::vec3 trans;
	computeTranslation(trans, channel, timePassed, currKey);
	glm::mat4 scaleMtx = glm::scale(scale);
	glm::mat4 rotMtx = glm::toMat4(quat);
	glm::mat4 transMtx = glm::translate(trans);
	channel->node->model = transMtx * rotMtx * scaleMtx;
}

void SkeletalAnimation::computeScaling(glm::vec3& scale, NodeChannel* channel, float timePassed, glm::ivec3& currKey) {
	int size = (int)channel->scalingKeys.size();
	int currScalingKey = (int)currKey[SCALING_INDEX];

	if (channel->scalingKeys[currScalingKey].first > timePassed) //if timePassed has wrapped around
		currScalingKey = 0;

	int i;
	for (i = currScalingKey; i < size - 1; i++) {
		if (channel->scalingKeys[i + 1].first >= timePassed)
			break;
	}
	currKey[SCALING_INDEX] = i; //update the current key

	float delta = channel->scalingKeys[i + 1].first - channel->scalingKeys[i].first; //delta in time
	float factor = (timePassed - channel->scalingKeys[i].first) / delta;
	scale = channel->scalingKeys[i].second + (channel->scalingKeys[i + 1].second - channel->scalingKeys[i].second) * factor;
}

void SkeletalAnimation::computeRotation(glm::quat& quat, NodeChannel* channel, float timePassed, glm::ivec3& currKey) {
	int size = (int)channel->rotationKeys.size();
	int currRotationKey = (int)currKey[ROTATION_INDEX];

	if (channel->rotationKeys[currRotationKey].first > timePassed) //if timePassed has wrapped around
		currRotationKey = 0;

	int i;
	for (i = currRotationKey; i < size - 1; i++) {
		if (channel->rotationKeys[i + 1].first >= timePassed)
			break;
	}
	currKey[ROTATION_INDEX] = i; //update the current key

	float delta = channel->rotationKeys[i + 1].first - channel->rotationKeys[i].first; //delta in time
	float factor = (timePassed - channel->rotationKeys[i].first) / delta;
	quat = glm::slerp(channel->rotationKeys[i].second, channel->rotationKeys[i+1].second, factor);
}

void SkeletalAnimation::computeTranslation(glm::vec3& trans, NodeChannel* channel, float timePassed, glm::ivec3& currKey) {
	int size = (int)channel->positionKeys.size();
	int currPositionKey = (int)currKey[TRANSLATION_INDEX];

	if (channel->positionKeys[currPositionKey].first > timePassed) //if timePassed has wrapped around
		currPositionKey = 0;

	int i;
	for (i = currPositionKey; i < size - 1; i++) {
		if (channel->positionKeys[i + 1].first >= timePassed)
			break;
	}
	currKey[TRANSLATION_INDEX] = i; //update the current key

	float delta = channel->positionKeys[i + 1].first - channel->positionKeys[i].first; //delta in time
	float factor = (timePassed - channel->positionKeys[i].first) / delta;
	trans = channel->positionKeys[i].second + (channel->positionKeys[i + 1].second - channel->positionKeys[i].second) * factor;
}
