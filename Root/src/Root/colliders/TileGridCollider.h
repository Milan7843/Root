#pragma once

#include "Collider.h"
#include "LoopCollider.h"

#include <Root/components/TileGrid.h>

#include <glm/glm.hpp>

#include <box2d/b2_chain_shape.h>

#include <vector>

enum class Direction
{
	UP,
	RIGHT,
	DOWN,
	LEFT
};

/**
 * The collision type.
 * When the collision type is set to OUTSIDE,
 * collision will only happen with bodies that are outside of the tiles.
 * This means that colliding bodies cannot get onto/into the tiles.
 *
 * When the collision type is set to INSIDE,
 * collision will only happen with bodies that are inside of the tiles.
 * This means that colliding bodies cannot get out of the tiles.
 */
enum class CollisionType {
	OUTSIDE,
	INSIDE
};

class TileGridCollider : public Collider
{
public:

	/**
	 * Create a new tile grid collider.
	 *
	 * \param tileGrid:				the tile grid to create the collider for.
	 * \param selfLayerMask:		a layer mask which defines which layers this rigidbody is on. Default = LAYER_0.
	 * \param interactionLayerMask:	a layer mask of all layers which this rigidbody can interact with. Default = LAYER_ALL.
	 * \param collisionType:		the type of the collision (INSIDE or OUTSIDE). Default = OUTSIDE.
	 * \param layerIndex:			the index of the layer to create the collider for. Default = 0.
	 * \param sensor:				whether the collider is a sensor. A sensor does not collide
	 *								with other objects, but it reports overlaps anyways.
	 * \param density:				the density of the collider. Default = 1.0.
	 * \param friction:				the friction of the collider. Default = 0.3.
	 */
	static std::shared_ptr<Collider> create(
		TileGrid* tileGrid,
		LayerMask selfLayerMask = LAYER_0,
		LayerMask interactionLayerMask = LAYER_ALL,
		CollisionType collisionType = CollisionType::OUTSIDE,
		int layerIndex = 0,
		bool sensor = false,
		float density = 1.0f,
		float friction = 0.3f);

	~TileGridCollider();

	const virtual std::vector<b2Shape*> getShapes() override;

	void renderDebugView() override;

	/**
	 * Update this collider's data with a tile grid and layer index.
	 *
	 * \param tileGrid: the tile grid to update the collider for.
	 * \param collisionType: the type of the collision (INSIDE or OUTSIDE). Default = OUTSIDE.
	 * \param layerIndex: the index of the layer to update the collider for. Default = 0.
	 */
	void updateWithTileGrid(
		TileGrid* tileGrid,
		CollisionType collisionType = CollisionType::OUTSIDE,
		unsigned int layerIndex = 0);

private:

	/**
	 * Create a new tile grid collider.
	 *
	 * \param tileGrid:				the tile grid to create the collider for.
	 * \param selfLayerMask:		a layer mask which defines which layers this rigidbody is on. Default = LAYER_0.
	 * \param interactionLayerMask:	a layer mask of all layers which this rigidbody can interact with. Default = LAYER_ALL.
	 * \param collisionType:		the type of the collision (INSIDE or OUTSIDE). Default = OUTSIDE.
	 * \param layerIndex:			the index of the layer to create the collider for. Default = 0.
	 * \param sensor:				whether the collider is a sensor. A sensor does not collide
	 *								with other objects, but it reports overlaps anyways.
	 * \param density:				the density of the collider. Default = 1.0.
	 * \param friction:				the friction of the collider. Default = 0.3.
	 */
	TileGridCollider(
		TileGrid* tileGrid,
		LayerMask selfLayerMask,
		LayerMask interactionLayerMask,
		CollisionType collisionType,
		int layerIndex,
		bool sensor,
		float density,
		float friction);

	void generateDebugVAO();

	void updateShapes();

	/**
	 * Get a point from a square and the direction we are moving.
	 */
	glm::vec2 getPointFromSquare(TileGrid* tileGrid, glm::ivec2 square, Direction directionMoving);

	bool isEmpty(TileGrid* tileGrid,
		unsigned int layerIndex,
		glm::ivec2 sample);

	// The loop colliders that make up this tile grid collider
	std::vector<std::shared_ptr<LoopCollider>> loopColliders;

	// The shapes that make up this tile grid collider
	std::vector<b2Shape*> shapes;
};

