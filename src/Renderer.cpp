#include <iostream>
#include <array>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

#include <Renderer.h>
#include <Debugger.h>

static const size_t MaxQuadCount = 20;
static const size_t MaxVertexCount = MaxQuadCount * 4;
static const size_t MaxIndexCount = MaxQuadCount * 6;
static const size_t MaxTextures = 32;

struct Vertex
{
	glm::vec3 Position;
	glm::vec4 Color;
	glm::vec2 TexCoords;
	float TexIndex;
};

struct RendererData
{
	GLuint QuadVA = 0;
	GLuint QuadVB = 0;
	GLuint QuadIB = 0;

	int32_t NoTextureSlot = -1;

	uint32_t IndexCount = 0;

	Vertex* QuadBuffer = nullptr;
	Vertex* QuadBufferPtr = nullptr;

	std::array<uint32_t, MaxTextures> TextureSlots;
	uint32_t TextureSlotIndex = 1;

	Renderer::Stats RendererStats;
};

static RendererData s_Data;

void Renderer::Init()
{
	s_Data.QuadBuffer = new Vertex[MaxVertexCount];

	glCall(glCreateVertexArrays(1, &s_Data.QuadVA));
	glCall(glBindVertexArray(s_Data.QuadVA));

	glCall(glCreateBuffers(1, &s_Data.QuadVB));
	glCall(glBindBuffer(GL_ARRAY_BUFFER, s_Data.QuadVB));
	glCall(glBufferData(GL_ARRAY_BUFFER, MaxVertexCount * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW));

	glCall(glEnableVertexArrayAttrib(s_Data.QuadVA, 0));
	glCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Position)));

	glCall(glEnableVertexArrayAttrib(s_Data.QuadVA, 1));
	glCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Color)));

	glCall(glEnableVertexArrayAttrib(s_Data.QuadVA, 2));
	glCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexCoords)));

	glCall(glEnableVertexArrayAttrib(s_Data.QuadVA, 3));
	glCall(glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexIndex)));

	uint32_t indices[MaxIndexCount];
	uint32_t offset = 0;
	for (int i = 0; i < MaxIndexCount; i += 6)
	{
		indices[i + 0] = 0 + offset;
		indices[i + 1] = 1 + offset;
		indices[i + 2] = 2 + offset;

		indices[i + 3] = 2 + offset;
		indices[i + 4] = 3 + offset;
		indices[i + 5] = 0 + offset;

		offset += 4;
	}

	glCall(glCreateBuffers(1, &s_Data.QuadIB));

	glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_Data.QuadIB));
	glCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

	for (size_t i = 0; i < MaxTextures; i++)
		s_Data.TextureSlots[i] = 0;

}

void Renderer::Shutdown()
{
	glCall(glDeleteVertexArrays(1, & s_Data.QuadVA));
	glCall(glDeleteBuffers(1, &s_Data.QuadVB));
	glCall(glDeleteBuffers(1, &s_Data.QuadIB));

	delete[] s_Data.QuadBuffer;
}

void Renderer::BeginBatch()
{
	s_Data.QuadBufferPtr = s_Data.QuadBuffer;
}

void Renderer::EndBatch()
{
	GLsizeiptr size = (uint8_t*)s_Data.QuadBufferPtr - (uint8_t*)s_Data.QuadBuffer;
	glCall(glBindBuffer(GL_ARRAY_BUFFER, s_Data.QuadVB));
	glCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, s_Data.QuadBuffer));
}

void Renderer::Flush()
{
	for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
		glBindTextureUnit(i, s_Data.TextureSlots[i]);

	glCall(glBindVertexArray(s_Data.QuadVA));
	glCall(glCall(glDrawElements(GL_TRIANGLES, s_Data.IndexCount, GL_UNSIGNED_INT, nullptr)));
	s_Data.RendererStats.DrawCount++;

	s_Data.IndexCount = 0;
	s_Data.TextureSlotIndex = 1;
}

void Renderer::ResetStats()
{
	s_Data.RendererStats.DrawCount = 0;
	s_Data.RendererStats.QuadCount = 0;
}

const Renderer::Stats& Renderer::GetStats()
{
	return s_Data.RendererStats;
}

void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
{
	if (s_Data.IndexCount >= MaxIndexCount)
	{
		EndBatch();
		Flush();
		BeginBatch();
	}

	float textureIndex = -1.0f;

	s_Data.QuadBufferPtr->Position = { position.x, position.y, 0.0f };
	s_Data.QuadBufferPtr->Color = color;
	s_Data.QuadBufferPtr->TexCoords = { 0.0f, 0.0f };
	s_Data.QuadBufferPtr->TexIndex = textureIndex;
	s_Data.QuadBufferPtr++;

	s_Data.QuadBufferPtr->Position = { position.x + size.x, position.y, 0.0f };
	s_Data.QuadBufferPtr->Color = color;
	s_Data.QuadBufferPtr->TexCoords = { 1.0f, 0.0f };
	s_Data.QuadBufferPtr->TexIndex = textureIndex;
	s_Data.QuadBufferPtr++;

	s_Data.QuadBufferPtr->Position = { position.x + size.x, position.y + size.y, 0.0f };
	s_Data.QuadBufferPtr->Color = color;
	s_Data.QuadBufferPtr->TexCoords = { 1.0f, 1.0f };
	s_Data.QuadBufferPtr->TexIndex = textureIndex;
	s_Data.QuadBufferPtr++;

	s_Data.QuadBufferPtr->Position = { position.x, position.y + size.y, 0.0f };
	s_Data.QuadBufferPtr->Color = color;
	s_Data.QuadBufferPtr->TexCoords = { 0.0f, 1.0f };
	s_Data.QuadBufferPtr->TexIndex = textureIndex;
	s_Data.QuadBufferPtr++;

	s_Data.IndexCount += 6;
	s_Data.RendererStats.QuadCount++;
}

void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, uint32_t textureID)
{
	if (s_Data.IndexCount >= MaxIndexCount || s_Data.TextureSlotIndex > MaxTextures)
	{
		EndBatch();
		Flush();
		BeginBatch();
	}

	glm::vec4 color = { 0.0f, 0.0f, 0.0f, 1.0f };

	float textureIndex = 0.0f;
	for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
	{
		if (s_Data.TextureSlots[i] == textureID)
		{
			textureIndex = (float)i;
			break;
		}
	}
	if (textureIndex == 0.0f)
	{
		textureIndex = (float)s_Data.TextureSlotIndex;
		s_Data.TextureSlots[s_Data.TextureSlotIndex] = textureID;
		s_Data.TextureSlotIndex++;
	}

	s_Data.QuadBufferPtr->Position = { position.x, position.y, 0.0f };
	s_Data.QuadBufferPtr->Color = color;
	s_Data.QuadBufferPtr->TexCoords = { 0.0f, 0.0f };
	s_Data.QuadBufferPtr->TexIndex = textureIndex;
	s_Data.QuadBufferPtr++;

	s_Data.QuadBufferPtr->Position = { position.x + size.x, position.y, 0.0f };
	s_Data.QuadBufferPtr->Color = color;
	s_Data.QuadBufferPtr->TexCoords = { 1.0f, 0.0f };
	s_Data.QuadBufferPtr->TexIndex = textureIndex;
	s_Data.QuadBufferPtr++;

	s_Data.QuadBufferPtr->Position = { position.x + size.x, position.y + size.y, 0.0f };
	s_Data.QuadBufferPtr->Color = color;
	s_Data.QuadBufferPtr->TexCoords = { 1.0f, 1.0f };
	s_Data.QuadBufferPtr->TexIndex = textureIndex;
	s_Data.QuadBufferPtr++;

	s_Data.QuadBufferPtr->Position = { position.x, position.y + size.y, 0.0f };
	s_Data.QuadBufferPtr->Color = color;
	s_Data.QuadBufferPtr->TexCoords = { 0.0f, 1.0f };
	s_Data.QuadBufferPtr->TexIndex = textureIndex;
	s_Data.QuadBufferPtr++;

	s_Data.IndexCount += 6;
	s_Data.RendererStats.QuadCount++;
}
