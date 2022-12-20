#include "TileGridCollider.h"

std::shared_ptr<Collider> TileGridCollider::create(
	TileGrid* tileGrid,
	CollisionType collisionType,
	int layerIndex)
{
	TileGridCollider* collider = new TileGridCollider(tileGrid, collisionType, layerIndex);
	std::shared_ptr<TileGridCollider> pointer{ collider };
	return pointer;
}

TileGridCollider::TileGridCollider(
	TileGrid* tileGrid,
	CollisionType collisionType,
	int layerIndex)
{
	updateWithTileGrid(tileGrid, collisionType, layerIndex);
}

bool TileGridCollider::isEmpty(TileGrid* tileGrid,
	unsigned int layerIndex,
	glm::ivec2 sample)
{
	if (sample.x >= tileGrid->tileGridSize.x || sample.x < 0 || sample.y >= tileGrid->tileGridSize.y || sample.y < 0)
	{
		return true;
	}

	// The 1D index of the square to check
	unsigned int sampleIndex
		= sample.x
		+ sample.y * tileGrid->tileGridSize.x
		+ layerIndex * tileGrid->tileGridSize.x * tileGrid->tileGridSize.y;

	return tileGrid->tileIndices[sampleIndex] == -1;
}

glm::vec2 TileGridCollider::getPointFromSquare(TileGrid* tileGrid, glm::ivec2 square, Direction directionMoving)
{
	// Top left
	glm::vec2 basePosition = -(glm::vec2)tileGrid->getGridSize() / 2.0f + glm::vec2(0.5f, 0.5f);

	glm::vec2 squarePosition = basePosition + (glm::vec2)square;

	switch (directionMoving)
	{
		// UP so we take the top left
		case Direction::UP:
			return (squarePosition + glm::vec2(-0.5f, 0.5f)) * tileGrid->tileSize * glm::vec2(1, -1);

		// RIGHT so we take the top right
		case Direction::RIGHT:
			return (squarePosition + glm::vec2(0.5f, 0.5f)) * tileGrid->tileSize * glm::vec2(1, -1);

		// DOWN so we take the bottom right
		case Direction::DOWN:
			return (squarePosition + glm::vec2(0.5f, -0.5f)) * tileGrid->tileSize * glm::vec2(1, -1);

		// LEFT so we take the bottom left
		case Direction::LEFT:
			return (squarePosition + glm::vec2(-0.5f, -0.5f)) * tileGrid->tileSize * glm::vec2(1, -1);
	}

	return glm::vec2(0.0f);
}

void removeValue(std::vector<glm::ivec2>& vec, glm::ivec2 v)
{
	for (unsigned int i{ 0 }; i < vec.size(); i++)
	{
		if (vec[i] == v)
		{
			vec.erase(vec.begin() + i);
		}
	}
	return;
}

void TileGridCollider::updateWithTileGrid(
	TileGrid* tileGrid,
	CollisionType collisionType,
	unsigned int layerIndex)
{
	// The layer index must be a valid value
	if (layerIndex >= tileGrid->layerCount)
	{
		Logger::logError("The layer index " + std::to_string(layerIndex)
			+ "is an invalid value for the tile grid with "
			+ std::to_string(tileGrid->layerCount) + " layers.");
	}

	// Clearing any previous data
	loopColliders.clear();

	// Retrieving grid data
	int* gridData = tileGrid->getData();
	glm::ivec2 gridSize = tileGrid->getGridSize();


	// First, we find all squares on the grid for which
	// the square above is not a grid tile (empty or out of bounds)
	std::vector<glm::ivec2> squaresWithEmptyAbove;

	for (unsigned int y{ 0 }; y < gridSize.y; y++)
	{
		for (unsigned int x{ 0 }; x < gridSize.x; x++)
		{
			// This space must not be empty and the space above it must be
			if (!isEmpty(tileGrid, layerIndex, glm::ivec2(x, y)) &&
				isEmpty(tileGrid, layerIndex, glm::ivec2(x, y+1)))
			{
				squaresWithEmptyAbove.push_back(glm::ivec2(x, y));
			}
		}
	}

	// While there are squares left with empty above them,
	// loop around them and remove the current and any we come across.
	while (squaresWithEmptyAbove.size() != 0)
	{
		glm::ivec2 currentSquare = squaresWithEmptyAbove[0];

		glm::vec2 startPoint = getPointFromSquare(tileGrid, currentSquare, Direction::UP);

		std::vector<glm::vec2> points;

		// Add the top left point
		points.push_back(startPoint);

		Direction directionMoving = Direction::RIGHT;

		while (true)
		{
			// First we add the point
			glm::vec2 point = getPointFromSquare(tileGrid, currentSquare, directionMoving);

			// Check if we have arrived back at the beginning
			if (point == startPoint) break;

			points.push_back(point);

			// This space must not be empty and the space above it must be to remove it from the list
			if (!isEmpty(tileGrid, layerIndex, currentSquare) &&
				isEmpty(tileGrid, layerIndex, currentSquare + glm::ivec2(0, 1)))
			{
				removeValue(squaresWithEmptyAbove, currentSquare);
			}

			// Then we check if we can move
			switch (directionMoving)
			{
				case Direction::UP:
				{
					bool upEmpty = isEmpty(tileGrid, layerIndex, currentSquare + glm::ivec2(0, 1));
					bool upLeftEmpty = isEmpty(tileGrid, layerIndex, currentSquare + glm::ivec2(-1, 1));

					// Check for movable:
					if (upEmpty)
					{
						// Found an upper left corner, rotate right
						directionMoving = Direction::RIGHT;
						break;
					}
					if (upLeftEmpty)
					{
						// Found a straight edge, just keep going
						currentSquare += glm::ivec2(0, 1);
						break;
					}
					// Found a corner, rotate left
					directionMoving = Direction::LEFT;
					// Moving to top left
					currentSquare += glm::ivec2(-1, 1);
					break;
				}


				case Direction::RIGHT:
				{
					bool rightEmpty = isEmpty(tileGrid, layerIndex, currentSquare + glm::ivec2(1, 0));
					bool upRightEmpty = isEmpty(tileGrid, layerIndex, currentSquare + glm::ivec2(1, 1));

					// Check for movable:
					if (rightEmpty)
					{
						// Found an upper right corner, rotate right
						directionMoving = Direction::DOWN;
						break;
					}
					if (upRightEmpty)
					{
						// Found a straight edge, just keep going
						currentSquare += glm::ivec2(1, 0);
						break;
					}
					// Found a corner, rotate left
					directionMoving = Direction::UP;
					// Moving to top right
					currentSquare += glm::ivec2(1, 1);
					break;
				}


				case Direction::DOWN:
				{
					bool downEmpty = isEmpty(tileGrid, layerIndex, currentSquare + glm::ivec2(0, -1));
					bool downRightEmpty = isEmpty(tileGrid, layerIndex, currentSquare + glm::ivec2(1, -1));

					// Check for movable:
					if (downEmpty)
					{
						// Found an bottom right corner, rotate right
						directionMoving = Direction::LEFT;
						break;
					}
					if (downRightEmpty)
					{
						// Found a straight edge, just keep going
						currentSquare += glm::ivec2(0, -1);
						break;
					}
					// Found a corner, rotate left
					directionMoving = Direction::RIGHT;
					// Moving to bottom right
					currentSquare += glm::ivec2(1, -1);
					break;
				}


				case Direction::LEFT:
				{
					bool leftEmpty = isEmpty(tileGrid, layerIndex, currentSquare + glm::ivec2(-1, 0));
					bool downLeftEmpty = isEmpty(tileGrid, layerIndex, currentSquare + glm::ivec2(-1, -1));

					// Check for movable:
					if (leftEmpty)
					{
						// Found an bottom right corner, rotate right
						directionMoving = Direction::UP;
						break;
					}
					if (downLeftEmpty)
					{
						// Found a straight edge, just keep going
						currentSquare += glm::ivec2(-1, 0);
						break;
					}
					// Found a corner, rotate left
					directionMoving = Direction::DOWN;
					// Moving to bottom left
					currentSquare += glm::ivec2(-1, -1);
					break;
				}
			}
		}

		// The collision should be inverted when using INSIDE collision mode
		std::shared_ptr<LoopCollider> loopCollider
			= std::static_pointer_cast<LoopCollider>(
				LoopCollider::create(points, collisionType == CollisionType::INSIDE));

		loopColliders.push_back(loopCollider);
	}
	updateShapes();
}

TileGridCollider::~TileGridCollider()
{
	Logger::destructorMessage("Tile grid collider");
}

const std::vector<b2Shape*> TileGridCollider::getShapes()
{
	return shapes;
}

void TileGridCollider::renderDebugView()
{
	for (std::shared_ptr<LoopCollider>& loopCollider : loopColliders)
	{
		loopCollider->renderDebugView();
	}
}

void TileGridCollider::generateDebugVAO()
{
	for (std::shared_ptr<LoopCollider>& loopCollider : loopColliders)
	{
		loopCollider->generateDebugVAO();
	}
}

void TileGridCollider::updateShapes()
{
	shapes.clear();

	for (std::shared_ptr<LoopCollider>& loopCollider : loopColliders)
	{
		for (b2Shape* shape : loopCollider->getShapes())
		{
			shapes.push_back(shape);
		}
	}
}
