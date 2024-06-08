#include "PengoInputCommands.h"

#include "GameManager.h"
#include "SceneManager.h"
#include "EventManager.h"
#include "ServiceLocator.h"
#include "LevelWonEvent.h"
#include "PlayerComponent.h"
#include "UISelectorComponent.h"
#include "TextComponent.h"
#include "UILetterComponent.h"

void Pengo::PengoPlayGame::Execute()
{
	Pengin::EventManager::GetInstance().BroadcoastEvent(std::make_unique<Pengin::BaseEvent>("PlayGame"));
}

void Pengo::PengoSkipLevel::Execute()
{
	Pengin::EventManager::GetInstance().BroadcoastEvent(std::make_unique<LevelWonEvent>(GetUserIdx()));
}

void Pengo::MuteSounds::Execute()
{
	auto& pSoundSys = Pengin::ServiceLocator::GetSoundSystem();

	pSoundSys.IsMuted() ? pSoundSys.Unmute() : pSoundSys.Mute();
}

void Pengo::SelectLetter::Execute()
{	
	using namespace Pengin;

	auto player = SceneManager::GetInstance().GetActiveScene()->GetPlayer(GetUserIdx());
	assert(player.HasComponent<UISelectorComponent>());	

	auto& uiSelector = player.GetComponent<UISelectorComponent>();

	if (m_Direction.x > 0)
	{
		uiSelector.ScrollIdx(true);
	}
	else if (m_Direction.x < 0)
	{
		uiSelector.ScrollIdx(false);
	}
	else if (m_Direction.y > 0)
	{
		const auto id = uiSelector.letters[uiSelector.currHorIdx];

		auto letterEnt = Entity{ id, SceneManager::GetInstance().GetActiveScene() };
		auto& letterComp = letterEnt.GetComponent<UILetterComponent>();
		letterComp.ScrollLetter(true);
		letterEnt.GetComponent<TextComponent>().SetText({ letterComp.letter });
	}
	else if (m_Direction.y < 0)
	{
		const auto id = uiSelector.letters[uiSelector.currHorIdx];

		auto letterEnt = Entity{ id, SceneManager::GetInstance().GetActiveScene() };
		auto& letterComp = letterEnt.GetComponent<UILetterComponent>();
		letterComp.ScrollLetter(false);
		letterEnt.GetComponent<TextComponent>().SetText({ letterComp.letter });
	}
}

void Pengo::Continue::Execute()
{
	Pengin::EventManager::GetInstance().BroadcoastEvent(std::make_unique<Pengin::BaseEvent>("BackToMainMenu"));
}
