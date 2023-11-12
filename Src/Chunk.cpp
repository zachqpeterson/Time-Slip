#include "Chunk.hpp"

#include "Rendering\Renderer.hpp"

#include "World.hpp"
#include "Tile.hpp"
#include "Time-Slip.hpp"

constexpr F32 TILE_WIDTH = 3.0f;
constexpr F32 TILE_HEIGHT = 3.0f;

void Chunk::Load(Tile* tiles, const Vector2Int& position, TileInstance* instances, U32 index)
{
	this->tiles = tiles;
	this->index = index;

	TileInstance* instance = instances;

	Vector2 pos = position * Vector2{ TILE_WIDTH, TILE_HEIGHT };

	Tile* currentTile = tiles;

	for (U32 y = 0; y < 8; ++y)
	{
		for (U32 x = 0; x < 8; ++x)
		{
			instance->color = Vector3One;
			instance->position = Vector3{ pos, 3.0f };
			instance->texcoord = Vector2Zero;
			instance->texIndex = TimeSlip::GetTextureIndex(currentTile->decoration);
			++instance;

			instance->color = Vector3One;
			instance->position = Vector3{ pos, 4.0f };
			instance->texcoord = Vector2Zero;
			instance->texIndex = TimeSlip::GetTextureIndex(currentTile->block);
			++instance;

			instance->color = Vector3One;
			instance->position = Vector3{ pos, 5.0f };
			instance->texcoord = Vector2Zero;
			instance->texIndex = TimeSlip::GetTextureIndex(currentTile->wall);
			++instance;

			pos.x += TILE_WIDTH;
			++currentTile;
		}

		pos.x = position.x * TILE_WIDTH;
		pos.y += TILE_HEIGHT;
		currentTile += World::TILE_COUNT_X - 8;
	}
}

U32 Chunk::Unload()
{
	U32 result = index;
	index = U32_MAX;

	return result;
}