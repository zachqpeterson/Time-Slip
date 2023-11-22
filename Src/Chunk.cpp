#include "Chunk.hpp"

#include "Rendering\Renderer.hpp"

#include "Timeslip.hpp"
#include "World.hpp"
#include "Tile.hpp"

void Chunk::Create(const Vector2Int& position_, TileInstance* wallInstances, TileInstance* blockInstances, TileInstance* decorationInstances, U32 offset)
{
	this->blockInstances = blockInstances;
	this->wallInstances = wallInstances;
	this->decorationInstances = decorationInstances;
	this->offset = offset;
	position = position_ * CHUNK_SIZE;

	LoadTiles();
}

void Chunk::Load(U8 direction)
{
	switch (direction)
	{
	case 0: { position.x -= VIEW_CHUNKS_X * CHUNK_SIZE; } break; //left
	case 1: { position.x += VIEW_CHUNKS_X * CHUNK_SIZE; } break; //right
	case 2: { position.y += VIEW_CHUNKS_Y * CHUNK_SIZE; } break; //up
	case 3: { position.y -= VIEW_CHUNKS_Y * CHUNK_SIZE; } break; //down
	}

	LoadTiles();
}

void Chunk::LoadTiles()
{
	Vector2 pos = position * Vector2{ TILE_WIDTH, TILE_HEIGHT };

	Tile* tile = World::GetTile(position.x, position.y);
	TileInstance* decorationInstance = decorationInstances;
	TileInstance* blockInstance = blockInstances;
	TileInstance* wallInstance = wallInstances;

	for (U32 y = 0; y < 8; ++y)
	{
		for (U32 x = 0; x < 8; ++x)
		{
			decorationInstance->position = pos;
			decorationInstance->texcoord = Vector2Zero;
			decorationInstance->color = Vector3One;
			decorationInstance->texIndex = Timeslip::GetTextureIndex(tile->decoration);
			++decorationInstance;

			blockInstance->position = pos;
			blockInstance->texcoord = Vector2Zero;
			blockInstance->color = Vector3One;
			blockInstance->texIndex = Timeslip::GetTextureIndex(tile->block);
			++blockInstance;

			wallInstance->position = pos;
			wallInstance->texcoord = Vector2Zero;
			wallInstance->color = Vector3One;
			wallInstance->texIndex = Timeslip::GetTextureIndex(tile->wall);
			++wallInstance;

			pos.x += TILE_WIDTH;
			++tile;
		}

		pos.x = position.x * TILE_WIDTH;
		pos.y += TILE_HEIGHT;
		tile += World::TILE_COUNT_X - 8;
	}
}