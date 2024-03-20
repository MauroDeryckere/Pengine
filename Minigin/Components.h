#ifndef COMPONENTS
#define COMPONENTS

#include "ECS.h"

#include "Texture2D.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "EventManager.h"
#include "SceneManager.h"
#include "Time.h"
#include "Font.h"
#include <string>
#include <format>

#include "glm/glm.hpp"

#include <stdexcept>
#include <SDL_ttf.h>

#include <iostream>
#include <functional>
#include <memory>

//TODO - move to separate files

namespace Pengin
{
	struct TransformComponent 
	{
		TransformComponent() = default;
		TransformComponent(const glm::vec3& pos) : m_Position{ pos } 
		{}
		
		glm::vec3 m_Position{ 0, 0, 0};
	};

	class TextureComponent final
	{
	public:
		TextureComponent(EntityId id) :
			m_Id{ id }
		{}
		TextureComponent(const std::string& texturePath, EntityId id) :
			m_pTexture{ dae::ResourceManager::GetInstance().LoadTexture(texturePath) },
			m_Id{ id }
		{}
		~TextureComponent() = default;

		void SetTexture(const std::string& texturePath) { m_pTexture = dae::ResourceManager::GetInstance().LoadTexture(texturePath); }
		void SetTexture(std::shared_ptr<dae::Texture2D> texture) { m_pTexture = texture; }

		void Render() const
		{
			//In future: move ID out, create view for comp types
			if (m_pTexture)
			{
				auto& ecs = ECS::GetInstance();
				const auto& tranform{ ecs.GetComponent<TransformComponent>(m_Id) };

				dae::Renderer::GetInstance().RenderTexture(*m_pTexture, tranform.m_Position.x, tranform.m_Position.y);
			}
		}
	private:
		std::shared_ptr<dae::Texture2D> m_pTexture{nullptr};
		const EntityId m_Id;
	};

	class TextComponent final 
	{
	public:
		TextComponent(EntityId id, const std::string& fontPath, unsigned fontSize, const std::string& text = "<EMPTY STRING>") :
			m_Id{ id },
			m_pFont{ dae::ResourceManager::GetInstance().LoadFont(fontPath, fontSize) },
			m_Text{ text }
		{}

		~TextComponent() = default;

		void Update()
		{
			if (m_NeedsUpdate)
			{
				const SDL_Color color = { 255,255,255,255 }; // only white text is supported now

				const auto surf = TTF_RenderText_Blended(m_pFont->GetFont(), m_Text.c_str(), color);
				if (surf == nullptr)
				{
					throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
				}
				auto texture = SDL_CreateTextureFromSurface(dae::Renderer::GetInstance().GetSDLRenderer(), surf);
				if (texture == nullptr)
				{
					throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
				}
				SDL_FreeSurface(surf);
				auto pTexture = std::make_shared<dae::Texture2D>(texture);
				m_NeedsUpdate = false;

				auto& textureComp{ Pengin::ECS::GetInstance().GetComponent<TextureComponent>(m_Id) };
				textureComp.SetTexture(pTexture);
			}
		}

		void SetText(const std::string& text)
		{
			m_Text = text;
			m_NeedsUpdate = true;
		}

	private:
		bool m_NeedsUpdate{true};
		std::string m_Text;

		std::shared_ptr<dae::Font> m_pFont;
		const EntityId m_Id;
	};

	class FPSComponent final 
	{
	public:
		FPSComponent(EntityId id) :
			m_Id{ id }
		{}

		~FPSComponent() = default;

		void Update()
		{
			m_AccumulatedTime += Pengin::Time::GetInstance().GetElapsedSec();

			++m_FrameCount;

			if (m_AccumulatedTime >= 1.0f)
			{
				const auto txt{ std::format("{:.1f}", m_FrameCount / m_AccumulatedTime) };

				auto& textComp{ Pengin::ECS::GetInstance().GetComponent<TextComponent>(m_Id) };
				textComp.SetText(txt);

				m_FrameCount = 0;
				m_AccumulatedTime -= 1.f;
			}
		}

	private:
		unsigned m_FrameCount{0};
		float m_AccumulatedTime{0.f};
		
		const EntityId m_Id;
	};

	class CharacterMovementComponent final
	{
	public:
		CharacterMovementComponent(float movementSpeed, EntityId id) :
			m_MovementSpeed{ movementSpeed },
			m_Id{ id }
		{}

		void Move(const glm::vec3& dir) const
		{
			//In future: move ID out, create view for comp types
			auto& tranform{ Pengin::ECS::GetInstance().GetComponent<TransformComponent>(m_Id) };
			auto vel = dir * m_MovementSpeed;
			tranform.m_Position += (vel * Time::GetInstance().GetElapsedSec());
		}

	private:
		const float m_MovementSpeed;
		const EntityId m_Id;
	};

	struct TestingAnEvent
	{
		TestingAnEvent():
			pObserver{ std::make_shared<Observer>("testObs") }
		{
			auto functioCallback = [](const void*)
				{
					std::cout << "Callback executed\n";
				};

			pObserver->RegisterForEvent("test", functioCallback);
		}


		std::shared_ptr<Observer> pObserver;

		std::string test{ "testing the evet class" };
	};
}

#endif