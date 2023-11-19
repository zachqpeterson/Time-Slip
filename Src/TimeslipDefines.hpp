#pragma once

#include "Defines.hpp"

enum WorldSize
{
	WORLD_SIZE_TEST = 280,
	WORLD_SIZE_SMALL = 2800,
	WORLD_SIZE_MEDIUM = 4200,
	WORLD_SIZE_LARGE = 5600
};

constexpr F32 TILE_WIDTH = 3.0f;
constexpr F32 TILE_HEIGHT = 3.0f;
constexpr F32 TILE_TEX_WIDTH = 1.0f / 6.0f;
constexpr F32 TILE_TEX_HEIGHT = 1.0f / 8.0f;
constexpr I64 TOTAL_TILE_COUNT = WORLD_SIZE_LARGE * (U64)((F32)WORLD_SIZE_LARGE / 3.5f);
constexpr I64 TOTAL_CHUNK_COUNT = TOTAL_TILE_COUNT / 64;
constexpr I64 VIEW_CHUNKS_X = 12;
constexpr I64 VIEW_CHUNKS_Y = 8;
constexpr I64 VIEW_OFFSET_X = VIEW_CHUNKS_X / 2;
constexpr I64 VIEW_OFFSET_Y = VIEW_CHUNKS_Y / 2;
constexpr I64 CHUNK_SIZE = 8;
constexpr I64 CHUNK_INSTANCE_COUNT = CHUNK_SIZE * CHUNK_SIZE * 3;

constexpr I64 MAX_SEED = 90000000000; //9000000000000000