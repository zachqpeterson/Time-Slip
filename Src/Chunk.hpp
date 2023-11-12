#pragma once

#include "Defines.hpp"

struct Tile;
struct TileInstance;
struct Vector2Int;

struct Chunk
{
	void Load(Tile* tiles, const Vector2Int& position, TileInstance* instances, U32 index);
	U32 Unload();

private:
	Tile* tiles;

	U32 index{ U32_MAX };
};