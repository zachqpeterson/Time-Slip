#pragma once

#include "Timeslip.hpp"

struct Tile;
struct TileInstance;
struct Vector2Int;

struct Chunk
{
	void Create(const Vector2Int& position, TileInstance* instances, U32 offset);
	void Load(U8 direction);

private:
	TileInstance* instances;
	Vector2Int position;
	U32 offset;

	friend class World;
};