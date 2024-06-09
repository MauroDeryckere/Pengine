# Pengine - C++ 2D Game Engine

## Minigin
This projected started with Minigin, provided by my Programming 4 teacher.

Minigin is a very small project using [SDL2](https://www.libsdl.org/) and [glm](https://github.com/g-truc/glm) for 2D c++ game projects. It is in no way a game engine, only a barebone start project where everything sdl related has been set up. It contains glm for vector math, to aleviate the need to write custom vector and matrix classes.

[![Build Status](https://github.com/avadae/minigin/actions/workflows/msbuild.yml/badge.svg)](https://github.com/avadae/msbuild/actions)
[![GitHub Release](https://img.shields.io/github/v/release/avadae/minigin?logo=github&sort=semver)](https://github.com/avadae/minigin/releases/latest)

## Table of Contents

- [Features](#features)
    - [ECS](#ecs)
    - [Event Queue](#event-queue)
    - [Input Manager](#input-manager)
    - [Serialization / Deserialization](#serialization--deserialization)
    - [Sound System](#sound-system)
    - [ImGUI Debugging](#imgui-debugging)
- [Future Work](#future-work)
- [References](#references)


# Features

## ECS
The Entity Component System (ECS) is a design pattern that allows for a flexible and modular approach to building game objects. It decouples data (components) from behaviour (systems), enabling efficient data management and processing.  It also provides great cache locality which can make the program a lot faster.

### Entities and Components
Entities and components are stored by the ECS class in a sparse set. This allows for both efficient lookups/removals and fast iteration for components by maintaining cache locality.

```cpp
//Create an entity with a position and velocity: 
struct Position {
    float x, y;
};

struct Velocity {
    float x, y;
};

ECS ecs{};

EntityId ent = ecs.CreateEntity();
ecs.AddComponent<Position>(ent, 1, 1);
ecs.AddComponent<Velocity>(ent, 20, 30);

//Iterate over all entities with a position: 
for(auto& e : ecs.GetComponents<Position>()){
    //Do something

//Removing components and destroying an entity
ecs.RemoveComponent<Position>(ent);
ecs.DestroyEntity(ent);
}
```

Entity and component destruction calls are placed on a queue to ensure no dependencies cause trouble during the update of a frame.

### Systems
The System Manager allows you to register generic systems which will then be updated, rendered,... in the game loop. There's also an option to add dependencies for a given system if necessary. The Scene class provides an easy-to-use interface for the System Manager.

```cpp
auto pSys = make_shared<TestSys>();
pScene->RegisterSystems([&](SystemManager& sysManager, ECS&) { sysManager.RegisterSystem<TestSys>(pSys); } );

//System with a dependency
pScene->RegisterSystems([&](SystemManager& sysManager, ECS& ecs) { sysManager.RegisterSystem<TestSys>(make_shared<TestSys2>(ecs), { pSys }); } );
```

## Input Manager
### Users and Actions
The Input Manager allows for easy registration of both controller and keyboard users and mapping of actions to keybinds. Every executed action can be polled by name, separating the keybind from the action. It's also possible to request the action (= InputCommand) pointer and modify and/or request data stored in this command.

```cpp
auto& input = InputManager::GetInstance();

//Register user(s)
auto user = input.RegisterUser(UserType::Keyboard);

//Define an action
class Movement final : public InputCommand {
	public:
		Movement(const UserIndex& user, const vec3& direction) :
			InputCommand{ user, "Movement" },
			m_Direction{ direction }
		{ }

		void Execute() {};
		const vec3& GetDirection() { return m_Direction; }
	private:
		const vec3 m_Direction;
};

//Map actions to a key
input.MapKeyboardAction(user, KeyBoardKey::Up, InputState::Pressed, make_shared<Movement>(user, vec3{ 0, -1, 0 }));

//Poll if an action is executed and request the actions pointer
shared_ptr<InputCommand> pCommand;
if (input.IsActionExecuted(user, "Movement", &pCommand)) {
	assert(pCommand);
	Movement* pMovement{ static_cast<Movement*>(pCommand.get()) };

	pMovement->GetDirection(); 
	//Do something with the requested data
}

```

### Combo System
There's also an easy-to-use combo system. This allows the user to create complex behaviour of different actions chained together within a given time frame.

```cpp
auto& input = InputManager::GetInstance();

//Register some actions
auto a1 = input.MapKeyboardAction(user, KeyBoardKey::E, InputState::Pressed, make_shared<A1>(user));
auto a2 = input.MapKeyboardAction(user, KeyBoardKey::F, InputState::Pressed, make_shared<A2>(user));
auto a3 = input.MapKeyboardAction(user, KeyBoardKey::T, InputState::Pressed, make_shared<A3>(user));

//Map a combo
InputCombo combo;
combo.pComboActions.emplace_back(a1);
combo.pComboActions.emplace_back(a2);
combo.pComboActions.emplace_back(a3);

combo.allowedDelay.emplace_back(1.f); //how much time is allowed between different action executions before the combo is invalidated
combo.allowedDelay.emplace_back(0.5f);

combo.pResultingAction = make_shared<RAction>(user);

input.MapCombo(user, combo);
```


## Event Queue
The Event Manager manages game events, allowing for decoupled communication between different parts of the game. Events can be dispatched and handled asynchronously. 

```cpp
//Create an observer and register for an event: 
std::shared_ptr<Observer> pObs = EventManager::GetInstance().CreateObserver();
pObs->RegisterForEvent(pObs, "EnemyKilled", [](const BaseEvent& event){ DoSomething(event); });

//Broadcast an event: 
//Either non-blocking
EventManager::GetInstance().BroadcastEvent(std::make_unique<EnemyKilldEvent>(data));
//Or blocking
EventManager::GetInstance().BroadcastBlockingEvent(std::make_unique<EnemyKilldEvent>(data));

//Handle an event: 
DoSomething(const BaseEvent& event){
    const auto& ev { static_cast<const EnemyKilledEvent&>(event) };

    //GetData from the event
    ev.GetData();
}
```

Observers are unregistered automatically when they go out of scope by the Event Manager whenever a call for an event is made that the observer was listening to.

## Serialization / Deserialization
Serialization and deserialization functionalities allow game states to be saved and loaded efficiently. Scenes can be saved and loaded with a single function call given you defined the necessary functions in the components you wish to deserialize and/or serialize.

### Serializing and deserializing components
You can serialize and deserialize components by adding a static function and registering this function in the serialization registry.

```cpp
#include "SerializationRegistry.h"

struct Component {
	int value = 10;

	static void Serialize(const FieldSerializer& fieldSer, const ECS& ecs, EntityId id, vector<uint8_t>& fieldVector) {
		const auto& comp = ecs.GetComponent<Component>(id);

		fieldSer.SerializeField("ValueName", value, ecs, fieldVector);
	}
	static void Deserialize(const FieldSerializer& fieldSer, ECS& ecs, EntityId id, const unordered_map<string, vector<uint8_t>>& serFields, const unordered_map<GameUUID, EntityId>& entityMap) {
		auto& comp = ecs.AddComponent<Component>(id);
		fieldSer.DeserializeField("ValueName", comp.value, serFields, entityMap);
	}
}


REGISTER_SERIALIZATION_FUNCTION(Component, Component::Serialize);
REGISTER_DESERIALIZATION_FUNCTION(Component, Component::Deserialize);
```

### Struct stored in a component
```cpp
struct Data {
	float val;

	static void Serialize(const FieldSerializer& fieldSer, const Data& serStruct, vector<uint8_t>& fieldVector, const ECS& ecs) {
			fieldSer.SerializeField("Val", serStruct.val, ecs, fieldVector);
	}
	static void Deserialize(const FieldSerializer& fieldSer, Data& deserStruct, const unordered_map<string, vector<uint8_t>>& fields, const unordered_map<GameUUID, EntityId>& entityMap) {
			fieldSer.DeserializeField("val", deserStruct.val, fields, entityMap);
	}
}

//Now you can Serialize/Deserialize Data as a field in any component.
```

## Sound System
Pengin provides a Sound System which can be accessed through the Service Locator. The Sound System uses FMOD under the hood and can be used to stream and play audio files of nearly all available types. All sounds are played and loaded via a SoundData struct, which contains all necessary data/modifiers. Playing an unloaded sound will load the sound automatically.

```cpp
//Load a sound
SoundData s{"Audio/MySound.wav"};
ServiceLocator::GetSoundSystem().LoadSound(s);

//Play a sound
auto channel = ServiceLocator::GetSoundSystem().PlaySound(s);

//A channel handle is returned to allow further modification if necessary
ServiceLocator::GetSoundSystem().SetChannel3DPosition({0,0,0});
ServiceLocator::GetSoundSystem().SetVolume(10),

//Playing a sound as a stream is as easy as changing a boolean in the data
s.isStream = true;
ServiceLocator::GetSoundSystem().PlaySound(s);
```

## ImGUI Debugging
ImGUI is integrated for in-game debugging. It provides a graphical interface for real-time debugging and inspection of game objects, components, and systems.

# Future Work
The engine is by no means finished and 'optimal' in its current state, I will list some of the things I plan to work on in the near future below.

 - ECS optimizations: the ECS implementation can be improved in many ways.
 - Renderer: The renderer is very simple because the focus was on other aspects of engine programming, but the renderer should eventually be replaced with a better system.
 - More audio support: Supporting 3D audio, FSbank files, and audio effects.
 - Physics Engine: Currently I've only written a simple implementation for the physics engine since not more was needed for the arcade game, the physics should be improved as I want to support bigger games.
 - Binary serializer / deserializer
 - Editor 
