#pragma once

#include "Timeslip.hpp"

struct Tile;
struct TileInstance;
struct Vector2Int;

struct Chunk
{
private:
	void Create(const Vector2Int& position, TileInstance* wallInstances, TileInstance* blockInstances, TileInstance* decorationInstances, U32 offset);
	void Load(U8 direction);

	void LoadTiles();

	TileInstance* wallInstances;
	TileInstance* blockInstances;
	TileInstance* decorationInstances;
	Vector2Int position;
	U32 offset;

	friend class World;
};