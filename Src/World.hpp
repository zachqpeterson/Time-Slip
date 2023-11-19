#pragma once

#include "TimeslipDefines.hpp"
#include "Math\Math.hpp"
#include "Containers\Freelist.hpp"

struct TileInstance
{
	Vector3 position;
	Vector2 texcoord;
	Vector3 color;
	U32 texIndex;
};

constexpr U64 size = sizeof(TileInstance);

struct Tile;
struct Chunk;
struct Camera;

class World
{
public:
	static Tile* GetTile(I16 x, I16 y);

private:
	static bool Initialize(WorldSize size);
	static void Shutdown();

	static void Update(Camera& camera);

	static void GenerateWorld();
	static I64 GenerateSeed();

	static I64 SEED;
	static I16 TILE_COUNT_X;
	static I16 TILE_COUNT_Y;
	static I16 TILE_OFFSET_X;
	static I16 TILE_OFFSET_Y;
	static I16 FIRST_CHUNK_X;
	static I16 LAST_CHUNK_X;
	static I16 FIRST_CHUNK_Y;
	static I16 LAST_CHUNK_Y;

	static Vector2Int chunkPos;
	static Vector2Int prevChunkPos;

	static TileInstance instances[];
	static Tile* tiles;
	static Chunk chunks[];
	static U8 leftIndex;
	static U8 rightIndex;
	static U8 bottomIndex;
	static U8 topIndex;

	STATIC_CLASS(World);
	friend class Timeslip;
	friend struct Chunk;
};