#ifndef COMPONENTS
#define COMPONENTS

namespace Pengin
{
	struct TestComponent
	{
		int randomintfortesting = 1;

		TestComponent() = default;
		TestComponent(int testConstructorArgs) : randomintfortesting{ testConstructorArgs } {}
	};
}

#endif