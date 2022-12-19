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

class TileGridCollider : public Collider
{
public:

	/**
	 * Create a new tile grid collider.
	 *
	 * \param tileGrid: the tile grid to create the collider for.
	 * \param layerIndex: the index of the layer to create the collider for. Default = 0.
	 */
	static std::shared_ptr<Collider> create(
		TileGrid* tileGrid,
		int layerIndex = 0);

	~TileGridCollider();

	const virtual std::vector<b2Shape*> getShapes() override;

	void renderDebugView() override;

	/**
	 * Update this collider's data with a tile grid and layer index.
	 *
	 * \param tileGrid: the tile grid to update the collider for.
	 * \param layerIndex: the index of the layer to update the collider for. Default = 0.
	 */
	void updateWithTileGrid(
		TileGrid* tileGrid,
		int layerIndex = 0);

private:

	TileGridCollider(
		TileGrid* tileGrid,
		int layerIndex);

	void generateDebugVAO();

	void updateShapes();

	/**
	 * Get a point from a square and the direction we are moving.
	 */
	glm::vec2 getPointFromSquare(TileGrid* tileGrid, glm::ivec2 square, Direction directionMoving);

	bool isEmpty(TileGrid* tileGrid,
		int layerIndex,
		glm::ivec2 sample);

	// The loop colliders that make up this tile grid collider
	std::vector<std::shared_ptr<LoopCollider>> loopColliders;

	// The shapes that make up this tile grid collider
	std::vector<b2Shape*> shapes;
};

