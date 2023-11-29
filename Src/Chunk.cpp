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
			//TODO: guard against out of bounds
			Tile* leftTile = World::GetTile(position.x + x - 1, position.y + y);
			Tile* rightTile = World::GetTile(position.x + x + 1, position.y + y);
			Tile* topTile = World::GetTile(position.x + x, position.y + y + 1);
			Tile* bottomTile = World::GetTile(position.x + x, position.y + y - 1);

			F32 variation = (((position.x + x) ^ 2 * (position.y + y) + Math::Abs(World::SEED)) % 3) * TILE_TEX_WIDTH;

			decorationInstance->position = pos;
			decorationInstance->texcoord = { variation, 0.0f };
			decorationInstance->maskTexcoord = Vector2Zero;
			decorationInstance->color = Vector3One;
			decorationInstance->texIndex = Timeslip::GetTextureIndex(2, tile->decoration);
			decorationInstance->maskIndex = U16_MAX;

			Vector2 mask = {
				((leftTile->block != U8_MAX) + (rightTile->block != U8_MAX) * 2.0f) * MASK_TEX_WIDTH,
				((topTile->block != U8_MAX) + (bottomTile->block != U8_MAX) * 2.0f) * MASK_TEX_HEIGHT
			};

			blockInstance->position = pos;
			blockInstance->texcoord = { variation, 0.0f };
			blockInstance->maskTexcoord = mask;
			blockInstance->color = Vector3One;
			blockInstance->texIndex = Timeslip::GetTextureIndex(1, tile->block);
			blockInstance->maskIndex = Timeslip::GetMaskIndex(0);

			mask = {
				((leftTile->wall != U8_MAX) + (rightTile->wall != U8_MAX) * 2.0f) * MASK_TEX_WIDTH,
				((topTile->wall != U8_MAX) + (bottomTile->wall != U8_MAX) * 2.0f) * MASK_TEX_HEIGHT
			};

			wallInstance->position = pos;
			wallInstance->texcoord = { variation, 0.0f };
			wallInstance->maskTexcoord = mask;
			wallInstance->color = Vector3One;
			wallInstance->texIndex = Timeslip::GetTextureIndex(0, tile->wall);
			wallInstance->maskIndex = Timeslip::GetMaskIndex(0);

			pos.x += TILE_WIDTH;
			++tile;
			++decorationInstance;
			++blockInstance;
			++wallInstance;
		}

		pos.x = position.x * TILE_WIDTH;
		pos.y += TILE_HEIGHT;
		tile += World::TILE_COUNT_X - 8;
	}
}