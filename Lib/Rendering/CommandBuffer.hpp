#pragma once

#include "Resources\ResourceDefines.hpp"

struct Shader;
struct Pipeline;
struct VkQueue_T;
struct VkSemaphore_T;
struct VkDescriptorSet_T;
struct VkCommandBuffer_T;
struct VkCommandPool_T;
struct VkClearAttachment;
struct VkClearRect;
struct VkViewport;
struct VkRect2D;
struct VkBufferImageCopy;
struct VkBufferCopy;
struct VkImageCopy;
struct VkImageBlit;
struct VkBufferMemoryBarrier2;
struct VkImageMemoryBarrier2;
enum VkFilter;
enum VkResult;

struct CommandBuffer
{
	VkResult Begin();
	VkResult End();

	void ClearAttachments(U32 attachmentCount, VkClearAttachment* attachments, U32 rectCount, VkClearRect* rects);

	void BeginRenderpass(Renderpass* renderpass);
	void NextSubpass();
	void EndRenderpass();

	void SetViewport(const VkViewport& viewport, const VkRect2D& scissor);
	void BindPipeline(const Pipeline* pipeline);
	void BindIndexBuffer(Shader* shader, const Buffer& buffer);
	void BindVertexBuffer(Shader* shader, const Buffer& buffer);
	void BindInstanceBuffer(Shader* shader, const Buffer& buffer);
	void BindDescriptorSets(Shader* shader, U32 setOffset, U32 setCount, VkDescriptorSet_T** sets);

	void PushDescriptors();
	void PushConstants(Shader* shader, U32 offset, U32 size, const void* data);

	void Draw(U32 firstVertex, U32 vertexCount, U32 firstInstance, U32 instanceCount);
	void DrawIndexed(U32 indexCount, U32 instanceCount, U32 firstIndex, I32 vertexOffset, U32 firstInstance);
	void DrawIndirect(Buffer* buffer, U32 offset, U32 stride);
	void DrawIndexedIndirect(const Buffer& buffer, U32 count, U32 offset = 0);
	void DrawIndexedIndirectCount(const Buffer& drawBuffer, const Buffer& countBuffer, U32 drawOffset = 0, U32 countOffset = 0);

	void Dispatch(U32 groupX, U32 groupY, U32 groupZ);
	void DispatchIndirect(Buffer* buffer, U32 offset);

	void BufferToImage(const Buffer& buffer, Texture* texture, U32 regionCount, const VkBufferImageCopy* regions);
	void ImageToBuffer(Texture* texture, const Buffer& buffer, U32 regionCount, const VkBufferImageCopy* regions);
	void BufferToBuffer(const Buffer& src, const Buffer& dst, U32 regionCount, const VkBufferCopy* regions);
	void ImageToImage(Texture* src, Texture* dst, U32 regionCount, const VkImageCopy* regions);
	void Blit(Texture* src, Texture* dst, VkFilter filter, U32 blitCount, const VkImageBlit* blits);

	void PipelineBarrier(I32 dependencyFlags, U32 bufferBarrierCount, const VkBufferMemoryBarrier2* bufferBarriers, U32 imageBarrierCount, const VkImageMemoryBarrier2* imageBarriers);

private:
	VkCommandBuffer_T* vkCommandBuffer{ nullptr };
	bool recorded{ false };

	friend class Renderer;
	friend struct CommandBufferRing;
};