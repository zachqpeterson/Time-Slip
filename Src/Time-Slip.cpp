#include "Time-Slip.hpp"

#include "Resources\ResourceDefines.hpp"
#include "Resources\Resources.hpp"
#include "Resources\Scene.hpp"
#include "Rendering\UI.hpp"
#include "Rendering\Renderer.hpp"
#include "Rendering\Shader.hpp"
#include "Rendering\Pipeline.hpp"

Shader* TimeSlip::tileShader;
Pipeline* TimeSlip::tilePipeline;
RenderGraph TimeSlip::tileRenderGraph;
TilePushConstant TimeSlip::tilePushConstant;

Scene* TimeSlip::gameScene;

bool TimeSlip::Initialize()
{
	tilePushConstant.globalColor = Vector3(1.0f);

	//TODO: Create background shader
	PushConstant tpc = { 0, sizeof(TilePushConstant), &tilePushConstant };
	tileShader = Resources::CreateShader("shaders/Tile.nhshd", 1, &tpc);

	PipelineInfo info{};
	info.name = "tile_pipeline";
	info.shader = tileShader;

	tilePipeline = Resources::CreatePipeline(info);

	tileRenderGraph.AddPipeline(tilePipeline);
	
	gameScene = Resources::CreateScene("game_scene");
	Renderer::LoadScene(gameScene); //TODO: Load menu scene first
	Renderer::SetRenderGraph(&tileRenderGraph);

	return true;
}

void TimeSlip::Shutdown()
{

}

void TimeSlip::Update()
{
	tilePushConstant.vp = gameScene->camera.ViewProjection();
}