#pragma once
#include <iostream>
#include <functional>

namespace Pengin
{
	struct TestComponent
	{
		TestComponent(int testInt_) : 
			testInt{ testInt_ },
			t{0}
		{
			InitCallback();
		};

		void InitCallback()
		{
		}

		void Test()
		{
			std::cout << " testInt:" << testInt << "\n";
		}

		int testInt;
		const int t;
		//Using std function ptr
		//std::function<void()> m_std_funcTest;

	};
}
