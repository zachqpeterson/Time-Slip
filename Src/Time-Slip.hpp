#pragma once

#include "Defines.hpp"

#include "Resources\ResourceDefines.hpp"
#include "Math\Math.hpp"

struct TilePushConstant
{
	Matrix4 vp;
	Vector3 globalColor;
};

struct Scene;
struct Shader;
struct Pipeline;

class TimeSlip
{
public:
	static bool Initialize();
	static void Shutdown();

	static void Update();

private:
	static Shader* tileShader;
	static Pipeline* tilePipeline;
	static RenderGraph tileRenderGraph;
	static TilePushConstant tilePushConstant;

	static Scene* gameScene;

	STATIC_CLASS(TimeSlip);
};