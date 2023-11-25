#pragma once

#include "Defines.hpp"
#include "Math\Math.hpp"

enum WorldSize
{
	WORLD_SIZE_TEST = 280,
	WORLD_SIZE_SMALL = 2800,
	WORLD_SIZE_MEDIUM = 4200,
	WORLD_SIZE_LARGE = 5600
};

constexpr F32 TILE_WIDTH = 3.0f;
constexpr F32 TILE_HEIGHT = 3.0f;
constexpr F32 TILE_TEX_WIDTH = 1.0f / 3.0f;
constexpr F32 TILE_TEX_HEIGHT = 1.0f;
constexpr F32 MASK_TEX_WIDTH = 1.0f / 8.0f;
constexpr F32 MASK_TEX_HEIGHT = 1.0f / 8.0f;
constexpr I64 TOTAL_TILE_COUNT = WORLD_SIZE_LARGE * (U64)((F32)WORLD_SIZE_LARGE / 3.5f);
constexpr I64 TOTAL_CHUNK_COUNT = TOTAL_TILE_COUNT / 64;
constexpr I64 VIEW_CHUNKS_X = 12;
constexpr I64 VIEW_CHUNKS_Y = 8;
constexpr I64 VIEW_OFFSET_X = VIEW_CHUNKS_X / 2;
constexpr I64 VIEW_OFFSET_Y = VIEW_CHUNKS_Y / 2;
constexpr I64 CHUNK_SIZE = 8;
constexpr I64 CHUNK_TILE_COUNT = CHUNK_SIZE * CHUNK_SIZE;
constexpr I64 CHUNK_INSTANCE_COUNT = CHUNK_TILE_COUNT * 3;

constexpr I64 MAX_SEED = 90000000000; //9000000000000000

struct TileVertex
{
	Vector3 position;
	Vector2 texcoord;
	Vector2 maskTexcoord;
};

struct TileInstance
{
	Vector2 position;
	Vector2 texcoord;
	Vector2 maskTexcoord;
	Vector3 color;
	U32 texIndex;
	U32 maskIndex;
};

struct TilePushConstant
{
	Vector4 globalColor;
};
