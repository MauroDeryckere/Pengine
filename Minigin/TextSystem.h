#ifndef TEXTSYSTEM
#define TEXTSYSTEM

#include "BaseSystem.h"

#include "EntityId.h"

#include <memory>
#include <string>

namespace Pengin
{
	class ECS;
	class Observer;

	class TextSystem final : public BaseSystem
	{
	public:
		TextSystem(ECS& ecs);
		virtual ~TextSystem() override = default;

		TextSystem(const TextSystem&) = delete;
		TextSystem(TextSystem&&) = delete;
		TextSystem& operator=(const TextSystem&) = delete;
		TextSystem& operator=(const TextSystem&&) = delete;

		virtual void Update() override;

		void SetText(const EntityId id, const std::string& text); 

	private:
		ECS& m_ECS;
		std::shared_ptr<Observer> m_pObserver;	
	};
}

#endif
