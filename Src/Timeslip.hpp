#pragma once

#include "TimeslipDefines.hpp"

#include "Resources\ResourceDefines.hpp"

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

	static void UploadTiles();
	static void UpdateTiles(U32 writeCount, BufferCopy* writes);
	static U32 GetTextureIndex(U32 type, U32 id);
	static U32 GetMaskIndex(U32 type);

private:
	static Shader* tileShader;
	static Pipeline* tilePipeline;
	static PipelineGraph tilePipelineGraph;
	static TilePushConstant tilePushConstant;
	static Buffer stagingBuffer;

	static Scene* gameScene;

	STATIC_CLASS(Timeslip);
};