#pragma once

#include "TimeslipDefines.hpp"

#include "Resources\ResourceDefines.hpp"
#include "Math\Math.hpp"

struct TilePushConstant
{
	Vector4 globalColor;
};

struct Scene;
struct Shader;
struct Pipeline;
struct BufferCopy;

class Timeslip
{
public:
	static bool Initialize();
	static void Shutdown();

	static void Update();

	static void UploadTiles(U32 size, void* data);
	static void UpdateTiles(U32 writeCount, BufferCopy* writes, U32 size, void* data);
	static U32 GetTextureIndex(U32 id);

private:
	static Shader* tileShader;
	static Pipeline* tilePipeline;
	static PipelineGraph tilePipelineGraph;
	static TilePushConstant tilePushConstant;

	static Scene* gameScene;

	STATIC_CLASS(Timeslip);
};