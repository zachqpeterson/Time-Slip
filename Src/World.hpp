#pragma once

#include "Defines.hpp"
#include "Math\Math.hpp"
#include "Containers\Freelist.hpp"

enum WorldSize
{
	WORLD_SIZE_TEST = 280,
	WORLD_SIZE_SMALL = 2800,
	WORLD_SIZE_MEDIUM = 4200,
	WORLD_SIZE_LARGE = 5600
};

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
	static bool Initialize(WorldSize size);
	static void Shutdown();

	static void Update(Camera& camera);

private:
	static void GenerateWorld();
	static Tile* GetTile(I16 x, I16 y);
	static Chunk* GetChunk(I16 x, I16 y);
	static I64 GenerateSeed();

	static I64 SEED;
	static I16 TILE_COUNT_X;
	static I16 TILE_COUNT_Y;
	static I16 TILE_OFFSET_X;
	static I16 TILE_OFFSET_Y;
	static I16 CHUNK_COUNT_X;
	static I16 CHUNK_COUNT_Y;
	static I16 CHUNK_OFFSET_X;
	static I16 CHUNK_OFFSET_Y;

	static Vector2Int chunkPos;
	static Vector2Int prevChunkPos;

	static Freelist freeChunks;

	static Tile* tiles;
	static Chunk* chunks;

	STATIC_CLASS(World);
	friend struct Chunk;
};