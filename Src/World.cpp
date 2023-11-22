#include "World.hpp"

#include "Core\Logger.hpp"
#include "Memory\Memory.hpp"
#include "Math\Random.hpp"
#include "Math\Math.hpp"
#include "Resources\ResourceDefines.hpp"
#include "Resources\Settings.hpp"
#include "Rendering\Pipeline.hpp"
#include "Platform\Input.hpp"

#include "Tile.hpp"
#include "Chunk.hpp"
#include "Timeslip.hpp"

I64 World::SEED;
I16 World::TILE_COUNT_X;
I16 World::TILE_COUNT_Y;
I16 World::TILE_OFFSET_X;
I16 World::TILE_OFFSET_Y;
I16 World::FIRST_CHUNK_X;
I16 World::LAST_CHUNK_X;
I16 World::FIRST_CHUNK_Y;
I16 World::LAST_CHUNK_Y;

Vector2Int World::chunkPos = Vector2IntZero;
Vector2Int World::prevChunkPos = Vector2IntZero;

TileInstance* World::wallInstances;
TileInstance* World::blockInstances;
TileInstance* World::decorationInstances;
Tile* World::tiles{ nullptr };
Chunk World::chunks[VIEW_CHUNKS_X * VIEW_CHUNKS_Y];
U16 World::leftIndex{ 0 };
U16 World::rightIndex{ VIEW_CHUNKS_X - 1 };
U16 World::bottomIndex{ 0 };
U16 World::topIndex{ VIEW_CHUNKS_Y - 1 };

bool World::Initialize(WorldSize size)
{
	SEED = GenerateSeed();

	TILE_COUNT_X = (U16)size;
	TILE_COUNT_Y = (U16)(TILE_COUNT_X / 3.5f);
	TILE_OFFSET_X = TILE_COUNT_X / 2;
	TILE_OFFSET_Y = TILE_COUNT_Y / 2;
	FIRST_CHUNK_X = -TILE_OFFSET_X / CHUNK_SIZE + VIEW_OFFSET_X;
	LAST_CHUNK_X = TILE_OFFSET_X / CHUNK_SIZE - VIEW_OFFSET_X;
	FIRST_CHUNK_Y = -TILE_OFFSET_Y / CHUNK_SIZE + VIEW_OFFSET_Y;
	LAST_CHUNK_Y = TILE_OFFSET_Y / CHUNK_SIZE - VIEW_OFFSET_Y;

	if (!tiles) { Memory::AllocateStaticArray(&tiles, TOTAL_TILE_COUNT); }
	if (!wallInstances) { Memory::AllocateStaticArray(&wallInstances, CHUNK_TILE_COUNT * VIEW_CHUNKS_X * VIEW_CHUNKS_Y); }
	if (!blockInstances) { Memory::AllocateStaticArray(&blockInstances, CHUNK_TILE_COUNT * VIEW_CHUNKS_X * VIEW_CHUNKS_Y); }
	if (!decorationInstances) { Memory::AllocateStaticArray(&decorationInstances, CHUNK_TILE_COUNT * VIEW_CHUNKS_X * VIEW_CHUNKS_Y); }

	GenerateWorld();

	Vector2Int position = { -VIEW_OFFSET_X, -VIEW_OFFSET_Y };

	U32 i = 0;
	for (U32 y = 0; y < VIEW_CHUNKS_Y; ++y)
	{
		for (U32 x = 0; x < VIEW_CHUNKS_X; ++x, ++i)
		{
			U32 offset = CHUNK_TILE_COUNT * i;
			chunks[i].Create(position, wallInstances + offset, blockInstances + offset, decorationInstances + offset, offset);

			++position.x;
		}

		position.x = -VIEW_OFFSET_X;
		++position.y;
	}

	Timeslip::UploadTiles(sizeof(TileInstance) * CHUNK_TILE_COUNT * VIEW_CHUNKS_X * VIEW_CHUNKS_Y, decorationInstances);
	Timeslip::UploadTiles(sizeof(TileInstance) * CHUNK_TILE_COUNT * VIEW_CHUNKS_X * VIEW_CHUNKS_Y, blockInstances);
	Timeslip::UploadTiles(sizeof(TileInstance) * CHUNK_TILE_COUNT * VIEW_CHUNKS_X * VIEW_CHUNKS_Y, wallInstances);

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

		camera.SetPosition(camera.Position().Clamped({ -TILE_OFFSET_X * 3 + 120.0f, -TILE_OFFSET_Y * 3 + 67.5f, 0.0f }, { TILE_OFFSET_X * 3 - 120.0f, TILE_OFFSET_Y * 3 - 67.5f, 0.0f }));
	}

	Vector3 pos = -camera.Position() / 24;
	if (pos.x < 0.0f) { pos.x -= 1.0f; }
	if (pos.y < 0.0f) { pos.y -= 1.0f; }
	chunkPos = Vector2Int{ (I32)pos.x, (I32)pos.y }.Clamped({ FIRST_CHUNK_X, FIRST_CHUNK_Y }, { LAST_CHUNK_X, LAST_CHUNK_Y });

	BufferCopy decorationWrites[VIEW_CHUNKS_X + VIEW_CHUNKS_Y];
	BufferCopy blockWrites[VIEW_CHUNKS_X + VIEW_CHUNKS_Y];
	BufferCopy wallWrites[VIEW_CHUNKS_X + VIEW_CHUNKS_Y];
	U32 writeCount = 0;

	//TODO: Edge case of moving multiple chunks in one frame
	//TODO: If far enough away, ignore previous chunks and just start fresh
	if (chunkPos != prevChunkPos)
	{
		bool loadHorizontal = false;

		if (chunkPos.x > prevChunkPos.x) //Unload left, load right
		{
			loadHorizontal = true;
			Chunk* chunk = chunks + leftIndex;

			for (U32 y = 0; y < VIEW_CHUNKS_Y; ++y)
			{
				chunk->Load(1);

				BufferCopy write{};
				write.srcOffset = sizeof(TileInstance) * chunk->offset;
				write.dstOffset = write.srcOffset;
				write.size = sizeof(TileInstance) * CHUNK_TILE_COUNT;

				decorationWrites[writeCount] = write;
				write.dstOffset += sizeof(TileInstance) * CHUNK_TILE_COUNT * VIEW_CHUNKS_X * VIEW_CHUNKS_Y;
				blockWrites[writeCount] = write;
				write.dstOffset += sizeof(TileInstance) * CHUNK_TILE_COUNT * VIEW_CHUNKS_X * VIEW_CHUNKS_Y;
				wallWrites[writeCount] = write;
				++writeCount;

				chunk += VIEW_CHUNKS_X;
			}

			++leftIndex %= VIEW_CHUNKS_X;
			++rightIndex %= VIEW_CHUNKS_X;
		}
		else if (chunkPos.x < prevChunkPos.x) //Unload right, load left
		{
			loadHorizontal = true;
			Chunk* chunk = chunks + rightIndex;

			for (U32 y = 0; y < VIEW_CHUNKS_Y; ++y)
			{
				chunk->Load(0);

				BufferCopy write{};
				write.srcOffset = sizeof(TileInstance) * chunk->offset;
				write.dstOffset = write.srcOffset;
				write.size = sizeof(TileInstance) * CHUNK_TILE_COUNT;

				decorationWrites[writeCount] = write;
				write.dstOffset += sizeof(TileInstance) * CHUNK_TILE_COUNT * VIEW_CHUNKS_X * VIEW_CHUNKS_Y;
				blockWrites[writeCount] = write;
				write.dstOffset += sizeof(TileInstance) * CHUNK_TILE_COUNT * VIEW_CHUNKS_X * VIEW_CHUNKS_Y;
				wallWrites[writeCount] = write;
				++writeCount;

				chunk += VIEW_CHUNKS_X;
			}

			if (--leftIndex == U16_MAX) { leftIndex = VIEW_CHUNKS_X - 1; }
			if (--rightIndex == U16_MAX) { rightIndex = VIEW_CHUNKS_X - 1; }
		}

		if (chunkPos.y > prevChunkPos.y) //unload bottom, load top
		{
			if (loadHorizontal) { prevChunkPos.x = chunkPos.x; }

			Chunk* chunk = chunks + bottomIndex * VIEW_CHUNKS_X;

			for (U32 x = 0; x < VIEW_CHUNKS_X; ++x)
			{
				chunk->Load(2);

				BufferCopy write{};
				write.srcOffset = sizeof(TileInstance) * chunk->offset;
				write.dstOffset = write.srcOffset;
				write.size = sizeof(TileInstance) * CHUNK_TILE_COUNT;

				decorationWrites[writeCount] = write;
				write.dstOffset += sizeof(TileInstance) * CHUNK_TILE_COUNT * VIEW_CHUNKS_X * VIEW_CHUNKS_Y;
				blockWrites[writeCount] = write;
				write.dstOffset += sizeof(TileInstance) * CHUNK_TILE_COUNT * VIEW_CHUNKS_X * VIEW_CHUNKS_Y;
				wallWrites[writeCount] = write;
				++writeCount;

				++chunk;
			}

			++bottomIndex %= VIEW_CHUNKS_Y;
			++topIndex %= VIEW_CHUNKS_Y;
		}
		else if (chunkPos.y < prevChunkPos.y) //unload top, load bottom
		{
			if (loadHorizontal) { prevChunkPos.x = chunkPos.x; }

			Chunk* chunk = chunks + topIndex * VIEW_CHUNKS_X;

			for (U32 x = 0; x < VIEW_CHUNKS_X; ++x)
			{
				chunk->Load(3);

				BufferCopy write{};
				write.srcOffset = sizeof(TileInstance) * chunk->offset;
				write.dstOffset = write.srcOffset;
				write.size = sizeof(TileInstance) * CHUNK_TILE_COUNT;

				decorationWrites[writeCount] = write;
				write.dstOffset += sizeof(TileInstance) * CHUNK_TILE_COUNT * VIEW_CHUNKS_X * VIEW_CHUNKS_Y;
				blockWrites[writeCount] = write;
				write.dstOffset += sizeof(TileInstance) * CHUNK_TILE_COUNT * VIEW_CHUNKS_X * VIEW_CHUNKS_Y;
				wallWrites[writeCount] = write;
				++writeCount;

				++chunk;
			}

			if (--bottomIndex == U16_MAX) { bottomIndex = VIEW_CHUNKS_Y - 1; }
			if (--topIndex == U16_MAX) { topIndex = VIEW_CHUNKS_Y - 1; }
		}

		if (writeCount)
		{
			Timeslip::UpdateTiles(writeCount, decorationWrites, sizeof(TileInstance) * CHUNK_TILE_COUNT * VIEW_CHUNKS_X * VIEW_CHUNKS_Y, decorationInstances);
			Timeslip::UpdateTiles(writeCount, blockWrites, sizeof(TileInstance) * CHUNK_TILE_COUNT * VIEW_CHUNKS_X * VIEW_CHUNKS_Y, blockInstances);
			Timeslip::UpdateTiles(writeCount, wallWrites, sizeof(TileInstance) * CHUNK_TILE_COUNT * VIEW_CHUNKS_X * VIEW_CHUNKS_Y, wallInstances);
		}
	}

	prevChunkPos = chunkPos;
}

Tile* World::GetTile(I16 x, I16 y)
{
	x += TILE_OFFSET_X;
	y += TILE_OFFSET_Y;

	if (x < 0 || x >= TILE_COUNT_X || y < 0 || y >= TILE_COUNT_Y) { BreakPoint; }

	return tiles + x + TILE_COUNT_X * y;
}

void World::GenerateWorld()
{
	static constexpr CatmullRomSpline<F64> inlandness(-50.0, -50.0, -40.0, -30.0, -10.0, -5.0, 0.0, 5.0, 10.0, 15.0, 25.0, 35.0, 40.0, 40.0);
	static constexpr F64 inlandFrequency = 0.001f;

	static constexpr F64 frequency = 0.01f;
	static constexpr F64 amplitude = 5.0f;
	static constexpr I16 height = 800;

	for (I16 x = 0; x < TILE_COUNT_X; ++x)
	{
		F64 inland = inlandness[Math::Abs(Math::Simplex1((SEED + x) * inlandFrequency)) * (F64)inlandness.Count()];

		I16 heightmap = height + (I16)inland;

		for (I16 y = 0; y < heightmap; ++y)
		{
			tiles[x + y * TILE_COUNT_X] = { 1, 0, U8_MAX };
		}

		for (I16 y = heightmap; y < TILE_COUNT_Y; ++y)
		{
			tiles[x + y * TILE_COUNT_X] = {};
		}
	}
}

I64 World::GenerateSeed()
{
	return (I64)(MAX_SEED * ((I64)Random::TrueRandomInt() / (F64)I64_MAX));
}