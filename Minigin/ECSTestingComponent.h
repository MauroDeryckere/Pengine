#pragma once
#include <iostream>
#include <functional>

namespace Pengin
{
	struct TestComponent
	{
		TestComponent(int testInt_) : 
			testInt{ testInt_ }
		{
			InitCallback();
		};

		void InitCallback()
		{
			m_std_funcTest = [this]()
				{
					Test();
				};
		}

		void Test()
		{
			std::cout << " testInt:" << testInt << "\n";

		}

		int testInt;
		//Using std function ptr
		std::function<void()> m_std_funcTest;

	};
}
