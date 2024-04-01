#ifndef TEXTSYSTEM
#define TEXTSYSTEM

#include <memory>
#include <string>

#include "EntityId.h"

namespace Pengin
{
	class ECS;
	class Observer;

	class TextSystem final
	{
	public:
		TextSystem(ECS& ecs);
		~TextSystem() = default;

		TextSystem(const TextSystem&) = delete;
		TextSystem(TextSystem&&) = delete;
		TextSystem& operator=(const TextSystem&) = delete;
		TextSystem& operator=(const TextSystem&&) = delete;

		void Update();

		void SetText(const EntityId id, const std::string& text); 

	private:
		ECS& m_ECS;
		std::shared_ptr<Observer> m_pObserver;	
	};
}

#endif
