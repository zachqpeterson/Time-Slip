#include "Time-Slip.hpp"

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
	Vector2 position;
	Vector2 texcoord;
};

Shader* TimeSlip::tileShader;
Pipeline* TimeSlip::tilePipeline;
PipelineGraph TimeSlip::tilePipelineGraph;
TilePushConstant TimeSlip::tilePushConstant;

Scene* TimeSlip::gameScene;

constexpr F32 TILE_WIDTH = 3.0f;
constexpr F32 TILE_HEIGHT = 3.0f;
constexpr F32 TILE_TEX_WIDTH = 1.0f / 6.0f;
constexpr F32 TILE_TEX_HEIGHT = 1.0f / 8.0f;

String textureNames[]{
	"textures/GrasslandDirt.nhtex",
	"textures/GrasslandDirtWall.nhtex",
	"textures/GrasslandStone.nhtex",
	"textures/GrasslandDirtWall.nhtex",
};

U32 textureIndices[CountOf(textureNames)];

bool TimeSlip::Initialize()
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

	U32 indices[]{ 0, 2, 1, 2, 3, 1,   4, 5, 6, 6, 7, 5,   8, 9, 10, 10, 11, 9,   12, 13, 14, 14, 15, 13,   16, 17, 18, 18, 19, 17 };

	TileVertex vertices[]{
		{ { 0.0f, TILE_HEIGHT },		{ 0.0f, TILE_TEX_HEIGHT } },
		{ { TILE_WIDTH, TILE_HEIGHT },	{ TILE_TEX_WIDTH, TILE_TEX_HEIGHT } },
		{ { 0.0f, 0.0f },				{ 0.0f, 0.0f } },
		{ { TILE_WIDTH, 0.0f },			{ TILE_TEX_WIDTH, 0.0f } },
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
	info.drawBufferSize = 20; //VkDrawIndexedIndirectCommand
	tilePipelineGraph.AddPipeline(info);

	tilePipelineGraph.Create("Tiles");

	gameScene = Resources::CreateScene("game_scene", CAMERA_TYPE_ORTHOGRAPHIC);
	Renderer::LoadScene(gameScene); //TODO: Load menu scene first
	Renderer::SetRenderGraph(&tilePipelineGraph);

	tilePipeline = tilePipelineGraph.GetPipeline(0, 0);

	tilePipeline->UploadIndices(sizeof(U32) * CountOf32(indices), indices);
	tilePipeline->UploadVertices(sizeof(TileVertex) * CountOf32(vertices), vertices);
	tilePipeline->UploadDrawCall(6, 0, 0, 12 * 8 * 192, 0);

	World::Initialize(WORLD_SIZE_LARGE);

	return true;
}

void TimeSlip::Shutdown()
{
	World::Shutdown();

	tilePipelineGraph.Destroy();

	for (String& string : textureNames)
	{
		string.Destroy();
	}
}

void TimeSlip::Update()
{
	World::Update(gameScene->camera);
}

void TimeSlip::UploadTiles(U32 size, void* data)
{
	tilePipeline->UploadInstances(size, data);
}

void TimeSlip::UpdateTiles(U32 size, U32 offset, void* data)
{
	BufferCopy copy{};
	copy.size = size;
	copy.srcOffset = 0;
	copy.dstOffset = offset;

	tilePipeline->UpdateInstances(size, data, 1, &copy);
}

U32 TimeSlip::GetTextureIndex(U32 id)
{
	if (id == 255) { return U16_MAX; }

	return textureIndices[id];
}