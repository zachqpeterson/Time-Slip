#include "World.hpp"

#include "Core\Logger.hpp"
#include "Memory\Memory.hpp"
#include "Math\Random.hpp"
#include "Math\Math.hpp"
#include "Resources\ResourceDefines.hpp"
#include "Resources\Settings.hpp"
#include "Platform\Input.hpp"

#include "Tile.hpp"
#include "Chunk.hpp"
#include "Time-Slip.hpp"

constexpr I64 TOTAL_TILE_COUNT = WORLD_SIZE_LARGE * (U64)((F32)WORLD_SIZE_LARGE / 3.5f);
constexpr I64 TOTAL_CHUNK_COUNT = TOTAL_TILE_COUNT / 64;
constexpr I64 VIEW_CHUNKS_X = 12;
constexpr I64 VIEW_CHUNKS_Y = 8;
constexpr I64 CHUNK_SIZE = 8;

constexpr I64 MAX_SEED = 90000000000; //9000000000000000

I64 World::SEED;
I16 World::TILE_COUNT_X;
I16 World::TILE_COUNT_Y;
I16 World::TILE_OFFSET_X;
I16 World::TILE_OFFSET_Y;
I16 World::CHUNK_COUNT_X;
I16 World::CHUNK_COUNT_Y;
I16 World::CHUNK_OFFSET_X;
I16 World::CHUNK_OFFSET_Y;

Vector2Int World::chunkPos = Vector2IntZero;
Vector2Int World::prevChunkPos = Vector2IntZero;

Freelist World::freeChunks{ VIEW_CHUNKS_X * VIEW_CHUNKS_Y };

Tile* World::tiles{ nullptr };
Chunk* World::chunks{ nullptr };

bool World::Initialize(WorldSize size)
{
	SEED = GenerateSeed();

	TILE_COUNT_X = (U16)size;
	TILE_COUNT_Y = (U16)(TILE_COUNT_X / 3.5f);
	TILE_OFFSET_X = TILE_COUNT_X / 2;
	TILE_OFFSET_Y = TILE_COUNT_Y / 2;
	CHUNK_COUNT_X = TILE_COUNT_X / CHUNK_SIZE;
	CHUNK_COUNT_Y = TILE_COUNT_Y / CHUNK_SIZE;
	CHUNK_OFFSET_X = CHUNK_COUNT_X / 2;
	CHUNK_OFFSET_Y = CHUNK_COUNT_Y / 2;

	if (!tiles) { Memory::AllocateStaticArray(&tiles, TOTAL_TILE_COUNT); }
	if (!chunks) { Memory::AllocateStaticArray(&chunks, TOTAL_CHUNK_COUNT); } //TODO: There is no point to having this many chunks, only need VIEW_CHUNKS_X * VIEW_CHUNKS_Y

	GenerateWorld();

	//assume spawn is at 0,0 for now

	Tile* startTiles = GetTile(-VIEW_CHUNKS_X / 2 * CHUNK_SIZE, -VIEW_CHUNKS_Y / 2 * CHUNK_SIZE);
	Chunk* startChunks = GetChunk(-VIEW_CHUNKS_X / 2, -VIEW_CHUNKS_Y / 2);
	U64 tileOffset = 0;

	I32 x = -(I32)(VIEW_CHUNKS_X / 2 * CHUNK_SIZE);
	Vector2Int position = { x, -(I32)(VIEW_CHUNKS_Y / 2 * CHUNK_SIZE) };

	TileInstance instances[192];

	U32 i = 0;
	for (U32 y = 0; y < VIEW_CHUNKS_Y; ++y)
	{
		for (U32 x = 0; x < VIEW_CHUNKS_X; ++x, ++i)
		{
			startChunks[x].Load(startTiles + tileOffset, position, instances, freeChunks.GetFree());

			TimeSlip::UploadTiles(sizeof(TileInstance) * CountOf32(instances), instances);

			position.x += CHUNK_SIZE;
			tileOffset += CHUNK_SIZE;
		}

		tileOffset = 0;
		position.x = x;
		position.y += CHUNK_SIZE;
		startTiles += TILE_COUNT_X * CHUNK_SIZE;
		startChunks += CHUNK_COUNT_X;
	}

	return true;
}

void World::Shutdown()
{

}

void World::Update(Camera& camera)
{
	if (!Settings::InEditor())
	{
		F32 speed = 10.0f;
		if (Input::ButtonDown(BUTTON_CODE_ALT)) { speed *= 100; }
		else if (Input::ButtonDown(BUTTON_CODE_SHIFT)) { speed *= 10; }

		if (Input::ButtonDown(BUTTON_CODE_A))
		{
			camera.SetPosition(camera.Position() + Vector3Right * (F32)Time::DeltaTime() * speed);
		}

		if (Input::ButtonDown(BUTTON_CODE_D))
		{
			camera.SetPosition(camera.Position() + Vector3Left * (F32)Time::DeltaTime() * speed);
		}

		if (Input::ButtonDown(BUTTON_CODE_W))
		{
			camera.SetPosition(camera.Position() + Vector3Down * (F32)Time::DeltaTime() * speed);
		}

		if (Input::ButtonDown(BUTTON_CODE_S))
		{
			camera.SetPosition(camera.Position() + Vector3Up * (F32)Time::DeltaTime() * speed);
		}

		camera.SetPosition(camera.Position().Clamped({ -TILE_COUNT_X / 2 * 3 + 120.0f, -TILE_COUNT_Y / 2 * 3 + 67.5f, 0.0f }, { TILE_COUNT_X / 2 * 3 - 120.0f, TILE_COUNT_Y / 2 * 3 - 67.5f, 0.0f }));
	}

	//TODO: Still breaks at the corners of the world
	Vector3 pos = -camera.Position() / 24;
	if (pos.x < 0.0f) { pos.x -= 1.0f; }
	if (pos.y < 0.0f) { pos.y -= 1.0f; }
	chunkPos = Vector2Int{ (I32)pos.x, (I32)pos.y }.Clamped({ -CHUNK_COUNT_X / 2 + 5, -CHUNK_COUNT_Y / 2 + 3 }, { CHUNK_COUNT_X / 2 - 6, CHUNK_COUNT_Y / 2 - 4 });
	
	if (chunkPos != prevChunkPos)
	{
		bool loadHorizontal = false;
		TileInstance instances[192];

		if (chunkPos.x > prevChunkPos.x) //Unload left, load right
		{
			loadHorizontal = true;
			Tile* startTiles = GetTile((I16)((prevChunkPos.x - VIEW_CHUNKS_X / 2) * CHUNK_SIZE), (I16)((prevChunkPos.y + VIEW_CHUNKS_Y / 2) * CHUNK_SIZE));
			Chunk* oldChunk = GetChunk((I16)(prevChunkPos.x - VIEW_CHUNKS_X / 2), (I16)(prevChunkPos.y - VIEW_CHUNKS_Y / 2));
			Chunk* newChunk = GetChunk((I16)(prevChunkPos.x + VIEW_CHUNKS_X / 2), (I16)(prevChunkPos.y - VIEW_CHUNKS_Y / 2));
			Vector2Int position = (prevChunkPos + Vector2Int{ VIEW_CHUNKS_X / 2, -VIEW_CHUNKS_Y / 2 }) * CHUNK_SIZE;
			
			for (U32 y = 0; y < VIEW_CHUNKS_Y; ++y)
			{
				U32 index = oldChunk->Unload();

				newChunk->Load(startTiles, position, instances, index);

				TimeSlip::UpdateTiles(sizeof(TileInstance) * CountOf32(instances), sizeof(TileInstance) * CountOf32(instances) * index, instances); //TODO: batch these

				oldChunk += CHUNK_COUNT_X;
				newChunk += CHUNK_COUNT_X;
				startTiles += TILE_COUNT_X * CHUNK_SIZE;
				position.y += CHUNK_SIZE;
			}
		}
		else if (chunkPos.x < prevChunkPos.x) //Unload right, load left
		{
			loadHorizontal = true;
			Tile* startTiles = GetTile((I16)((prevChunkPos.x + VIEW_CHUNKS_X / 2) * CHUNK_SIZE), (I16)((prevChunkPos.y + VIEW_CHUNKS_Y / 2) * CHUNK_SIZE));
			Chunk* oldChunk = GetChunk((I16)(prevChunkPos.x + VIEW_CHUNKS_X / 2 - 1), (I16)(prevChunkPos.y - VIEW_CHUNKS_Y / 2));
			Chunk* newChunk = GetChunk((I16)(prevChunkPos.x - VIEW_CHUNKS_X / 2 - 1), (I16)(prevChunkPos.y - VIEW_CHUNKS_Y / 2));
			Vector2Int position = (prevChunkPos - Vector2Int{ VIEW_CHUNKS_X / 2, VIEW_CHUNKS_Y / 2 }) * CHUNK_SIZE;

			for (U32 y = 0; y < VIEW_CHUNKS_Y; ++y)
			{
				U32 index = oldChunk->Unload();

				newChunk->Load(startTiles, position, instances, index);

				TimeSlip::UpdateTiles(sizeof(TileInstance) * CountOf32(instances), sizeof(TileInstance) * CountOf32(instances) * index, instances); //TODO: batch these

				oldChunk += CHUNK_COUNT_X;
				newChunk += CHUNK_COUNT_X;
				startTiles += TILE_COUNT_X * CHUNK_SIZE;
				position.y += CHUNK_SIZE;
			}
		}

		if (chunkPos.y > prevChunkPos.y) //unload bottom, load top
		{
			if (loadHorizontal) { prevChunkPos.x = chunkPos.x; }

			Tile* startTiles = GetTile((I16)((prevChunkPos.x - VIEW_CHUNKS_X / 2) * CHUNK_SIZE), (I16)((prevChunkPos.y + VIEW_CHUNKS_Y / 2) * CHUNK_SIZE));
			Chunk* oldChunk = GetChunk((I16)(prevChunkPos.x - VIEW_CHUNKS_X / 2), (I16)(prevChunkPos.y - VIEW_CHUNKS_Y / 2));
			Chunk* newChunk = GetChunk((I16)(prevChunkPos.x - VIEW_CHUNKS_X / 2), (I16)(prevChunkPos.y + VIEW_CHUNKS_Y / 2));
			Vector2Int position = (prevChunkPos + Vector2Int{ -VIEW_CHUNKS_X / 2, VIEW_CHUNKS_Y / 2 }) * CHUNK_SIZE;

			for (U32 x = 0; x < VIEW_CHUNKS_X; ++x)
			{
				U32 index = oldChunk->Unload();

				newChunk->Load(startTiles, position, instances, index);

				TimeSlip::UpdateTiles(sizeof(TileInstance) * CountOf32(instances), sizeof(TileInstance) * CountOf32(instances) * index, instances); //TODO: batch these

				oldChunk += 1;
				newChunk += 1;
				startTiles += CHUNK_SIZE;
				position.x += CHUNK_SIZE;
			}
		}
		else if (chunkPos.y < prevChunkPos.y) //unload top, load bottom
		{
			if (loadHorizontal) { prevChunkPos.x = chunkPos.x; }

			Tile* startTiles = GetTile((I16)((prevChunkPos.x - VIEW_CHUNKS_X / 2) * CHUNK_SIZE), (I16)((prevChunkPos.y - VIEW_CHUNKS_Y / 2) * CHUNK_SIZE));
			Chunk* oldChunk = GetChunk((I16)(prevChunkPos.x - VIEW_CHUNKS_X / 2), (I16)(prevChunkPos.y + VIEW_CHUNKS_Y / 2 - 1));
			Chunk* newChunk = GetChunk((I16)(prevChunkPos.x - VIEW_CHUNKS_X / 2), (I16)(prevChunkPos.y - VIEW_CHUNKS_Y / 2 - 1));
			Vector2Int position = (prevChunkPos + Vector2Int{ -VIEW_CHUNKS_X / 2, -VIEW_CHUNKS_Y / 2 }) * CHUNK_SIZE;

			for (U32 x = 0; x < VIEW_CHUNKS_X; ++x)
			{
				U32 index = oldChunk->Unload();

				newChunk->Load(startTiles, position, instances, index);

				TimeSlip::UpdateTiles(sizeof(TileInstance) * CountOf32(instances), sizeof(TileInstance) * CountOf32(instances) * index, instances); //TODO: batch these

				oldChunk += 1;
				newChunk += 1;
				startTiles += CHUNK_SIZE;
				position.x += CHUNK_SIZE;
			}
		}
	}

	prevChunkPos = chunkPos;
}

Tile* World::GetTile(I16 x, I16 y)
{
	return tiles + TILE_OFFSET_X + x + TILE_COUNT_X * (TILE_OFFSET_Y + y);
}

Chunk* World::GetChunk(I16 x, I16 y)
{
	return chunks + CHUNK_OFFSET_X + x + CHUNK_COUNT_X * (CHUNK_OFFSET_Y + y);
}

void World::GenerateWorld()
{
	static constexpr F64 frequency = 0.1f;

	for (I16 x = 0; x < TILE_COUNT_X; ++x)
	{
		for (I16 y = 0; y < TILE_COUNT_Y; ++y)
		{
			F64 noise = Math::Simplex2((SEED + x) * frequency, (SEED + y) * frequency);

			//TODO: Bias
			if (noise < 0) { tiles[x + y * TILE_COUNT_X] = {}; }
			else { tiles[x + y * TILE_COUNT_X] = { 1, 0, U8_MAX }; }
		}
	}
}

I64 World::GenerateSeed()
{
	return (I64)(MAX_SEED * ((I64)Random::TrueRandomInt() / (F64)I64_MAX));
}