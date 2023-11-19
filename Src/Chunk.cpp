#include "Chunk.hpp"

#include "Rendering\Renderer.hpp"

#include "Timeslip.hpp"
#include "World.hpp"
#include "Tile.hpp"

void Chunk::Create(const Vector2Int& position_, TileInstance* instances, U32 offset)
{
	this->instances = instances;
	this->offset = offset;
	position = position_ * CHUNK_SIZE;

	Vector2 pos = position * Vector2{ TILE_WIDTH, TILE_HEIGHT };

	Tile* tile = World::GetTile(position.x, position.y);
	TileInstance* instance = instances;

	for (U32 y = 0; y < 8; ++y)
	{
		for (U32 x = 0; x < 8; ++x)
		{
			instance->color = Vector3One;
			instance->position = Vector3{ pos, 3.0f };
			instance->texcoord = Vector2Zero;
			instance->texIndex = Timeslip::GetTextureIndex(tile->decoration);
			++instance;

			instance->color = Vector3One;
			instance->position = Vector3{ pos, 4.0f };
			instance->texcoord = Vector2Zero;
			instance->texIndex = Timeslip::GetTextureIndex(tile->block);
			++instance;

			instance->color = Vector3One;
			instance->position = Vector3{ pos, 5.0f };
			instance->texcoord = Vector2Zero;
			instance->texIndex = Timeslip::GetTextureIndex(tile->wall);
			++instance;

			pos.x += TILE_WIDTH;
			++tile;
		}

		pos.x = position.x * TILE_WIDTH;
		pos.y += TILE_HEIGHT;
		tile += World::TILE_COUNT_X - 8;
	}
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

	Vector2 pos = position * Vector2{ TILE_WIDTH, TILE_HEIGHT };

	Tile* tile = World::GetTile(position.x, position.y);
	TileInstance* instance = instances;

	for (U32 y = 0; y < 8; ++y)
	{
		for (U32 x = 0; x < 8; ++x)
		{
			instance->color = Vector3One;
			instance->position = Vector3{ pos, 3.0f };
			instance->texcoord = Vector2Zero;
			instance->texIndex = Timeslip::GetTextureIndex(tile->decoration);
			++instance;

			instance->color = Vector3One;
			instance->position = Vector3{ pos, 4.0f };
			instance->texcoord = Vector2Zero;
			instance->texIndex = Timeslip::GetTextureIndex(tile->block);
			++instance;

			instance->color = Vector3One;
			instance->position = Vector3{ pos, 5.0f };
			instance->texcoord = Vector2Zero;
			instance->texIndex = Timeslip::GetTextureIndex(tile->wall);
			++instance;

			pos.x += TILE_WIDTH;
			++tile;
		}

		pos.x = position.x * TILE_WIDTH;
		pos.y += TILE_HEIGHT;
		tile += World::TILE_COUNT_X - 8;
	}
}