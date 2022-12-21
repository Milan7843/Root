#pragma once

#include <Root/Logger.h>

#include <box2d/b2_body.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

#define ColliderPointer std::shared_ptr<Collider>

/**
 * How to use layers:
 *
 * A layermask consists of 0-16 layers.
 * You can add two or more layers together in a layermask by using |
 * E.g. LAYER_0 | LAYER_1
 *
 * You can remove a layer from a layermask using -
 * E.g. LAYER_ALL - LAYER_0
 *
 * All colliders have layermasks self: LAYER_0 and interaction: LAYER_ALL by default.
 *
 * For a collision to occur, both collider must have
 * each other's self layer mask in their interaction layer mask.
 */

#define LayerMask uint16

#define LAYER_0 1
#define LAYER_1 2
#define LAYER_2 4
#define LAYER_3 8
#define LAYER_4 16
#define LAYER_5 32
#define LAYER_6 64
#define LAYER_7 128
#define LAYER_8 256
#define LAYER_9 512
#define LAYER_10 1024
#define LAYER_11 2048
#define LAYER_12 4096
#define LAYER_13 8192
#define LAYER_14 16384
#define LAYER_15 32768
#define LAYER_ALL 65535

class Collider
{
public:

	~Collider()
	{
		glDeleteVertexArrays(1, &debugVAO);
	}

	const virtual std::vector<b2Shape*> getShapes() { return std::vector<b2Shape*>(); }

	virtual void renderDebugView() { }

	LayerMask getSelfLayerMask()
	{
		return selfLayerMask;
	}

	LayerMask getInteractionLayerMask()
	{
		return interactionLayerMask;
	}

protected:

	Collider(LayerMask selfLayerMask, LayerMask interactionLayerMask)
		: selfLayerMask(selfLayerMask)
		, interactionLayerMask(interactionLayerMask) { }

	LayerMask selfLayerMask;
	LayerMask interactionLayerMask;

	unsigned int debugVAO{ 0 };
};

