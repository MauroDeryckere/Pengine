#pragma once
#include <iostream>
#include <functional>
#include <memory>

#include "TransformComponent.h"

namespace Pengin
{
	struct TestComponent
	{
		TestComponent(int testInt_) : 
			testInt{ testInt_ },
			t{0},
			m_T{std::make_shared<TransformComponent>()}
		{
			InitCallback();
		};

		TestComponent(TestComponent&& other) noexcept :
			testInt(other.testInt),
			t(other.t),
			m_T(std::move(other.m_T))
		{
			// Reset other's shared pointer to nullptr
			std::cout << "MOVE CONSTRUCTOR: \n";
			std::cout << "m_usect : " << m_T.use_count() << "\n";
			std::cout << "other.use_ct : " << other.m_T.use_count() << "\n";
		}

		TestComponent(const TestComponent& other) = default;
		//TestComponent(TestComponent&& other) = default;
		TestComponent& operator=(const TestComponent& other) = default;
		TestComponent& operator=(TestComponent&& other) = default;

		~TestComponent()
		{
			m_T.reset();
		}

		void InitCallback()
		{
		}

		void Test()
		{
			std::cout << " testInt:" << testInt << "\n";
		}

		int testInt;
		const int t;

		std::shared_ptr<TransformComponent> m_T;
		//Using std function ptr
		//std::function<void()> m_std_funcTest;

	};
}
