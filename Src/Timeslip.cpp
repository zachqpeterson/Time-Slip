#include "Timeslip.hpp"

#include "Resources\ResourceDefines.hpp"
#include "Resources\Resources.hpp"
#include "Resources\Settings.hpp"
#include "Resources\Scene.hpp"
#include "Rendering\UI.hpp"
#include "Rendering\Renderer.hpp"
#include "Rendering\Shader.hpp"
#include "Rendering\Pipeline.hpp"
#include "Platform\Input.hpp"

#include "World.hpp"

struct TileVertex
{
	Vector3 position;
	Vector2 texcoord;
};

Shader* Timeslip::tileShader;
Pipeline* Timeslip::tilePipeline;
PipelineGraph Timeslip::tilePipelineGraph;
TilePushConstant Timeslip::tilePushConstant;

Scene* Timeslip::gameScene;

String textureNames[]{
	"textures/GrasslandDirt.nhtex",
	"textures/GrasslandDirtWall.nhtex",
	"textures/GrasslandStone.nhtex",
	"textures/GrasslandDirtWall.nhtex",
};

U32 textureIndices[CountOf(textureNames)];

bool Timeslip::Initialize()
{
	//SamplerInfo sampler{};
	//sampler.minFilter = FILTER_TYPE_NEAREST;
	//sampler.magFilter = FILTER_TYPE_NEAREST;
	//sampler.mipFilter = SAMPLER_MIPMAP_MODE_NEAREST;
	//
	//Resources::UploadTexture("CoalOre.bmp", sampler);
	//Resources::UploadTexture("CopperOre.bmp", sampler);
	//Resources::UploadTexture("DesertDirt.bmp", sampler);
	//Resources::UploadTexture("DesertDirtWall.bmp", sampler);
	//Resources::UploadTexture("DesertStone.bmp", sampler);
	//Resources::UploadTexture("DesertStoneWall.bmp", sampler);
	//Resources::UploadTexture("Flint.bmp", sampler);
	//Resources::UploadTexture("Grass.bmp", sampler);
	//Resources::UploadTexture("GrasslandDirt.bmp", sampler);
	//Resources::UploadTexture("GrasslandDirtWall.bmp", sampler);
	//Resources::UploadTexture("GrasslandStone.bmp", sampler);
	//Resources::UploadTexture("GrasslandStoneWall.bmp", sampler);
	//Resources::UploadTexture("IronOre.bmp", sampler);
	//Resources::UploadTexture("Items.bmp", sampler);
	//Resources::UploadTexture("JungleDirt.bmp", sampler);
	//Resources::UploadTexture("JungleDirtWall.bmp", sampler);
	//Resources::UploadTexture("JungleStone.bmp", sampler);
	//Resources::UploadTexture("JungleStoneWall.bmp", sampler);
	//Resources::UploadTexture("MarshDirt.bmp", sampler);
	//Resources::UploadTexture("MarshDirtWall.bmp", sampler);
	//Resources::UploadTexture("MarshStone.bmp", sampler);
	//Resources::UploadTexture("MarshStoneWall.bmp", sampler);
	//Resources::UploadTexture("MesaDirt.bmp", sampler);
	//Resources::UploadTexture("MesaDirtWall.bmp", sampler);
	//Resources::UploadTexture("MesaStone.bmp", sampler);
	//Resources::UploadTexture("MesaStoneWall.bmp", sampler);
	//Resources::UploadTexture("Player.bmp", sampler);
	//Resources::UploadTexture("Shrub.bmp", sampler);
	//Resources::UploadTexture("TallGrass.bmp", sampler);
	//Resources::UploadTexture("TinOre.bmp", sampler);
	//Resources::UploadTexture("Torch.bmp", sampler);
	//Resources::UploadTexture("Water.bmp", sampler);
	//Resources::UploadTexture("Wood.bmp", sampler);
	//Resources::UploadTexture("UI.bmp", sampler);

	U32 i = 0;
	for (const String& name : textureNames)
	{
		textureIndices[i++] = (U32)Resources::LoadTexture(name)->handle;
	}

	U32 indices[]{ 0, 2, 1, 2, 3, 1,   4, 6, 5, 6, 7, 5,   8, 10, 9, 10, 11, 9,   12, 14, 13, 14, 15, 13,   16, 18, 17, 18, 19, 17 };

	TileVertex vertices[]{
		{ { 0.0f, TILE_HEIGHT, 1.0f },			{ 0.0f, TILE_TEX_HEIGHT } },
		{ { TILE_WIDTH, TILE_HEIGHT, 1.0f },	{ TILE_TEX_WIDTH, TILE_TEX_HEIGHT } },
		{ { 0.0f, 0.0f, 1.0f },					{ 0.0f, 0.0f } },
		{ { TILE_WIDTH, 0.0f, 1.0f },			{ TILE_TEX_WIDTH, 0.0f } },

		{ { 0.0f, TILE_HEIGHT, 2.0f },			{ 0.0f, TILE_TEX_HEIGHT } },
		{ { TILE_WIDTH, TILE_HEIGHT, 2.0f },	{ TILE_TEX_WIDTH, TILE_TEX_HEIGHT } },
		{ { 0.0f, 0.0f, 2.0f },					{ 0.0f, 0.0f } },
		{ { TILE_WIDTH, 0.0f, 2.0f },			{ TILE_TEX_WIDTH, 0.0f } },

		{ { 0.0f, TILE_HEIGHT, 3.0f },			{ 0.0f, TILE_TEX_HEIGHT } },
		{ { TILE_WIDTH, TILE_HEIGHT, 3.0f },	{ TILE_TEX_WIDTH, TILE_TEX_HEIGHT } },
		{ { 0.0f, 0.0f, 3.0f },					{ 0.0f, 0.0f } },
		{ { TILE_WIDTH, 0.0f, 3.0f },			{ TILE_TEX_WIDTH, 0.0f } },
	};

	tilePushConstant.globalColor = Vector4One;

	//TODO: Create background shader
	PushConstant pushConstants[]
	{
		{ 0, sizeof(TilePushConstant), &tilePushConstant },
		{ sizeof(TilePushConstant), sizeof(CameraData), Renderer::GetCameraData() }
	};
	tileShader = Resources::CreateShader("shaders/Tile.nhshd", CountOf32(pushConstants), pushConstants);

	PipelineInfo info{};
	info.name = "tile_pipeline";
	info.shader = tileShader;
	info.vertexBufferSize = sizeof(TileVertex) * CountOf32(vertices);
	info.indexBufferSize = sizeof(U32) * CountOf32(indices);
	info.drawBufferSize = 60; //VkDrawIndexedIndirectCommand * 3
	tilePipelineGraph.AddPipeline(info);

	tilePipelineGraph.Create("Tiles");

	gameScene = Resources::CreateScene("game_scene", CAMERA_TYPE_ORTHOGRAPHIC);
	Renderer::LoadScene(gameScene); //TODO: Load menu scene first
	Renderer::SetRenderGraph(&tilePipelineGraph);

	tilePipeline = tilePipelineGraph.GetPipeline(0, 0);

	tilePipeline->UploadIndices(sizeof(U32) * CountOf32(indices), indices);
	tilePipeline->UploadVertices(sizeof(TileVertex) * CountOf32(vertices), vertices);
	tilePipeline->UploadDrawCall(6, 0, 0, VIEW_CHUNKS_X * VIEW_CHUNKS_Y * CHUNK_TILE_COUNT, 0);
	tilePipeline->UploadDrawCall(6, 6, 0, VIEW_CHUNKS_X * VIEW_CHUNKS_Y * CHUNK_TILE_COUNT, VIEW_CHUNKS_X * VIEW_CHUNKS_Y * CHUNK_TILE_COUNT);
	tilePipeline->UploadDrawCall(6, 12, 0, VIEW_CHUNKS_X * VIEW_CHUNKS_Y * CHUNK_TILE_COUNT, VIEW_CHUNKS_X * VIEW_CHUNKS_Y * CHUNK_TILE_COUNT * 2);

	World::Initialize(WORLD_SIZE_LARGE);

	return true;
}

void Timeslip::Shutdown()
{
	World::Shutdown();

	tilePipelineGraph.Destroy();

	for (String& string : textureNames)
	{
		string.Destroy();
	}
}

void Timeslip::Update()
{
	World::Update(gameScene->camera);
}

void Timeslip::UploadTiles(U32 size, void* data)
{
	tilePipeline->UploadInstances(size, data);
}

void Timeslip::UpdateTiles(U32 writeCount, BufferCopy* writes, U32 size, void* data)
{
	tilePipeline->UpdateInstances(size, data, writeCount, writes);
}

U32 Timeslip::GetTextureIndex(U32 id)
{
	if (id == 255) { return U16_MAX; }

	return textureIndices[id];
}