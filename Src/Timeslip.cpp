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

Shader* Timeslip::tileShader;
Pipeline* Timeslip::tilePipeline;
PipelineGraph Timeslip::tilePipelineGraph;
TilePushConstant Timeslip::tilePushConstant;
Buffer Timeslip::stagingBuffer;

Scene* Timeslip::gameScene;

String textureNames[][3]{
	{
		"textures/GrasslandDirtWall.nhtex",
		"textures/GrasslandStoneWall.nhtex",
	},
	{
		"textures/GrasslandDirt.nhtex",
		"textures/GrasslandStone.nhtex",
	},
	{
		"textures/Grass.nhtex",
	}
};

String maskNames[]{
	"textures/TileMask.nhtex"
};

//TODO: Type Traits to automate this
U32 textureIndices[Math::Max(CountOf(textureNames[0]), Math::Max(CountOf(textureNames[1]), CountOf(textureNames[2])))][CountOf(textureNames)];
U32 maskIndices[CountOf(maskNames)];

bool Timeslip::Initialize()
{
	TextureUpload upload{};
	upload.samplerInfo.minFilter = FILTER_TYPE_NEAREST;
	upload.samplerInfo.magFilter = FILTER_TYPE_NEAREST;
	upload.samplerInfo.mipFilter = SAMPLER_MIPMAP_MODE_NEAREST;
	upload.usage = TEXTURE_USAGE_COLOR;

	//Resources::UploadTexture("CoalOre.bmp", upload);
	//Resources::UploadTexture("CopperOre.bmp", upload);
	//Resources::UploadTexture("DesertDirt.bmp", upload);
	//Resources::UploadTexture("DesertDirtWall.bmp", upload);
	//Resources::UploadTexture("DesertStone.bmp", upload);
	//Resources::UploadTexture("DesertStoneWall.bmp", upload);
	//Resources::UploadTexture("Flint.bmp", upload);
	//Resources::UploadTexture("Grass.bmp", upload);
	//Resources::UploadTexture("GrasslandDirt.bmp", upload);
	//Resources::UploadTexture("GrasslandDirtWall.bmp", upload);
	//Resources::UploadTexture("GrasslandStone.bmp", upload);
	//Resources::UploadTexture("GrasslandStoneWall.bmp", upload);
	//Resources::UploadTexture("IronOre.bmp", upload);
	//Resources::UploadTexture("Items.bmp", upload);
	//Resources::UploadTexture("JungleDirt.bmp", upload);
	//Resources::UploadTexture("JungleDirtWall.bmp", upload);
	//Resources::UploadTexture("JungleStone.bmp", upload);
	//Resources::UploadTexture("JungleStoneWall.bmp", upload);
	//Resources::UploadTexture("MarshDirt.bmp", upload);
	//Resources::UploadTexture("MarshDirtWall.bmp", upload);
	//Resources::UploadTexture("MarshStone.bmp", upload);
	//Resources::UploadTexture("MarshStoneWall.bmp", upload);
	//Resources::UploadTexture("MesaDirt.bmp", upload);
	//Resources::UploadTexture("MesaDirtWall.bmp", upload);
	//Resources::UploadTexture("MesaStone.bmp", upload);
	//Resources::UploadTexture("MesaStoneWall.bmp", upload);
	//Resources::UploadTexture("Player.bmp", upload);
	//Resources::UploadTexture("Shrub.bmp", upload);
	//Resources::UploadTexture("TallGrass.bmp", upload);
	//Resources::UploadTexture("TinOre.bmp", upload);
	//Resources::UploadTexture("Torch.bmp", upload);
	//Resources::UploadTexture("Water.bmp", upload);
	//Resources::UploadTexture("Wood.bmp", upload);
	//Resources::UploadTexture("UI.bmp", upload);

	//upload.usage = TEXTURE_USAGE_MASK;

	Resources::UploadTexture("TileMask.bmp", upload);

	for (U32 i = 0; i < CountOf(textureNames); ++i)
	{
		U32 j = 0;
		for (const String& name : textureNames[i])
		{
			if (!name) { break; }
			textureIndices[i][j++] = (U32)Resources::LoadTexture(name)->handle;
		}
	}

	U32 i = 0;
	for (const String& name : maskNames)
	{
		maskIndices[i++] = (U32)Resources::LoadTexture(name)->handle;
	}
	
	U32 indices[]{ 0, 2, 1, 2, 3, 1,   4, 6, 5, 6, 7, 5,   8, 10, 9, 10, 11, 9,   12, 14, 13, 14, 15, 13,   16, 18, 17, 18, 19, 17 };

	TileVertex vertices[]{
		{ { 0.0f, TILE_HEIGHT, 1.0f },			{ 0.0f, 0.0f },							{ 0.0f, 0.0f } },
		{ { TILE_WIDTH, TILE_HEIGHT, 1.0f },	{ TILE_TEX_WIDTH, 0.0f },				{ MASK_TEX_WIDTH, 0.0f } },
		{ { 0.0f, 0.0f, 1.0f },					{ 0.0f, TILE_TEX_HEIGHT },				{ 0.0f, MASK_TEX_HEIGHT } },
		{ { TILE_WIDTH, 0.0f, 1.0f },			{ TILE_TEX_WIDTH, TILE_TEX_HEIGHT },	{ MASK_TEX_WIDTH, MASK_TEX_HEIGHT }	},

		{ { 0.0f, TILE_HEIGHT, 2.0f },			{ 0.0f, 0.0f },							{ 0.0f, 0.0f } },
		{ { TILE_WIDTH, TILE_HEIGHT, 2.0f },	{ TILE_TEX_WIDTH, 0.0f },				{ MASK_TEX_WIDTH, 0.0f } },
		{ { 0.0f, 0.0f, 2.0f },					{ 0.0f, TILE_TEX_HEIGHT },				{ 0.0f, MASK_TEX_HEIGHT } },
		{ { TILE_WIDTH, 0.0f, 2.0f },			{ TILE_TEX_WIDTH, TILE_TEX_HEIGHT },	{ MASK_TEX_WIDTH, MASK_TEX_HEIGHT }	},

		{ { 0.0f, TILE_HEIGHT, 3.0f },			{ 0.0f, 0.0f },							{ 0.0f, 0.0f } },
		{ { TILE_WIDTH, TILE_HEIGHT, 3.0f },	{ TILE_TEX_WIDTH, 0.0f },				{ MASK_TEX_WIDTH, 0.0f } },
		{ { 0.0f, 0.0f, 3.0f },					{ 0.0f, TILE_TEX_HEIGHT },				{ 0.0f, MASK_TEX_HEIGHT } },
		{ { TILE_WIDTH, 0.0f, 3.0f },			{ TILE_TEX_WIDTH, TILE_TEX_HEIGHT },	{ MASK_TEX_WIDTH, MASK_TEX_HEIGHT }	},
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

	stagingBuffer = Renderer::CreateBuffer(sizeof(TileInstance) * CHUNK_INSTANCE_COUNT * VIEW_CHUNKS_X * VIEW_CHUNKS_Y, BUFFER_USAGE_TRANSFER_SRC, BUFFER_MEMORY_TYPE_CPU_VISIBLE | BUFFER_MEMORY_TYPE_CPU_COHERENT);

	World::Initialize((TileInstance*)stagingBuffer.data, WORLD_SIZE_LARGE);

	return true;
}

void Timeslip::Shutdown()
{
	World::Shutdown();

	tilePipelineGraph.Destroy();

	for (U32 i = 0; i < CountOf(textureNames); ++i)
	{
		for (String& string : textureNames[i])
		{
			string.Destroy();
		}
	}
}

void Timeslip::Update()
{
	World::Update(gameScene->camera);
}

void Timeslip::UploadTiles()
{
	tilePipeline->UploadInstances(stagingBuffer.size, stagingBuffer.data);
}

void Timeslip::UpdateTiles(U32 writeCount, BufferCopy* writes)
{
	tilePipeline->UpdateInstances(stagingBuffer, writeCount, writes);
}

U32 Timeslip::GetTextureIndex(U32 type, U32 id)
{
	if (id == 255) { return U16_MAX; }

	return textureIndices[type][id];
}

U32 Timeslip::GetMaskIndex(U32 type)
{
	if (type == 255) { return U16_MAX; }

	return maskIndices[type];
}