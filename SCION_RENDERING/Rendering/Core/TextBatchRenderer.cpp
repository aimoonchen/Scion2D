#include "TextBatchRenderer.h"

namespace SCION_RENDERING {

	void TextBatchRenderer::Initialize()
	{
		SetVertexAttribute(0, 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, position));
		SetVertexAttribute(1, 4, GL_UNSIGNED_BYTE, sizeof(Vertex), (void*)offsetof(Vertex, color), GL_TRUE);
		SetVertexAttribute(2, 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, uvs));
	}

	void TextBatchRenderer::GenerateBatches()
	{
		GLuint offset{ 0 }, prevFontID{ 0 };
		int currentFont{ 0 };

		size_t total{ 0 }, currentVertex{ 0 };

		// Add up the total characters
		for (const auto& textGlpyh : m_Glyphs)
			total += textGlpyh->textStr.size();

		std::vector<Vertex> vertices;
		vertices.resize(total * 6);

		for (const auto& textGlyph : m_Glyphs)
		{
			std::vector<std::string> textChunks{};
			std::string text_holder{""};
			glm::vec2 temp_pos = textGlyph->position;

			if (textGlyph->wrap > 1.f)
			{
				// Create the text chunks for each line.
				// TODO:
			}
			else
			{
				textChunks.push_back(textGlyph->textStr);
			}

			// Reset the text position
			temp_pos = textGlyph->position;

			// Add new Text Sprite
			for (const auto& textStr : textChunks)
			{
				for (const auto& character : textStr)
				{
					auto glyph = textGlyph->font->GetGlyph(character, temp_pos);

					// First Triangle
					vertices[currentVertex++] = Vertex{
						.position = textGlyph->model * glm::vec4{glyph.min.position.x, glyph.min.position.y, 0.f, 1.f},
						.uvs = glm::vec2{glyph.min.uvs.x, glyph.min.uvs.y},
						.color = textGlyph->color
					};

					vertices[currentVertex++] = Vertex{
						.position = textGlyph->model * glm::vec4{glyph.max.position.x, glyph.min.position.y, 0.f, 1.f},
						.uvs = glm::vec2{glyph.max.uvs.x, glyph.min.uvs.y},
						.color = textGlyph->color
					};

					vertices[currentVertex++] = Vertex{
						.position = textGlyph->model * glm::vec4{glyph.max.position.x, glyph.max.position.y, 0.f, 1.f},
						.uvs = glm::vec2{glyph.max.uvs.x, glyph.max.uvs.y},
						.color = textGlyph->color
					};

					// Second Triangle
					vertices[currentVertex++] = Vertex{
						.position = textGlyph->model * glm::vec4{glyph.min.position.x, glyph.min.position.y, 0.f, 1.f},
						.uvs = glm::vec2{glyph.min.uvs.x, glyph.min.uvs.y},
						.color = textGlyph->color
					};

					vertices[currentVertex++] = Vertex{
						.position = textGlyph->model * glm::vec4{glyph.max.position.x, glyph.max.position.y, 0.f, 1.f},
						.uvs = glm::vec2{glyph.max.uvs.x, glyph.max.uvs.y},
						.color = textGlyph->color
					};

					vertices[currentVertex++] = Vertex{
						.position = textGlyph->model * glm::vec4{glyph.min.position.x, glyph.max.position.y, 0.f, 1.f},
						.uvs = glm::vec2{glyph.min.uvs.x, glyph.max.uvs.y},
						.color = textGlyph->color
					};

					if (currentFont == 0)
						m_Batches.push_back(std::make_shared<TextBatch>(
							TextBatch{ .offset = offset, .numVertices = 6,
							.fontAtlasID = textGlyph->font->GetFontAtlasID() }));
					else if (textGlyph->font->GetFontAtlasID() != prevFontID)
						m_Batches.push_back(std::make_shared<TextBatch>(
							TextBatch{ .offset = offset, .numVertices = 6,
							.fontAtlasID = textGlyph->font->GetFontAtlasID() }));
					else
						m_Batches.back()->numVertices += 6;

					currentFont++;
					prevFontID = textGlyph->font->GetFontAtlasID();
					offset += 6;
				}

				// Move to the next Line
				temp_pos.x = textGlyph->position.x;
				temp_pos.y += textGlyph->font->GetFontSize() + textGlyph->padding;
			}
		}

		glBindBuffer(GL_ARRAY_BUFFER, GetVBO());
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	TextBatchRenderer::TextBatchRenderer()
		: Batcher(false)
	{
		Initialize();
	}

	void TextBatchRenderer::End()
	{
		if (m_Glyphs.empty())
			return;

		GenerateBatches();
	}

	void TextBatchRenderer::Render()
	{
		if (m_Batches.empty())
			return;

		EnableVAO();
		for (const auto& batch : m_Batches)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, batch->fontAtlasID);
			glDrawArrays(GL_TRIANGLES, batch->offset, batch->numVertices);
		}
		DisableVAO();
	}

	void TextBatchRenderer::AddText(const std::string& text, const std::shared_ptr<Font>& font, const glm::vec2& position, int padding, float wrap, Color color, glm::mat4 model)
	{
		if (!font)
			return;

		auto newTextGlyph = std::make_shared<TextGlyph>(
			TextGlyph{
				.textStr = text,
				.position = position,
				.color = color,
				.model = model,
				.font = font,
				.wrap = wrap,
				.padding = padding
			}
		);

		m_Glyphs.push_back(std::move(newTextGlyph));
	}
}