#ifndef PENGO_UILETTER
#define PENGO_UILETTER


namespace Pengo
{
	struct UILetterComponent final
	{
		UILetterComponent() = default;
		~UILetterComponent() = default;
		
		void ScrollLetter(bool up)
		{
			if (up)
			{
				++letter;

				if (letter > 90)
				{
					letter = 65;
				}
			}
			else
			{
				--letter;
				
				if (letter < 65)
				{
					letter = 90;
				}
			}
		}

		char letter{ 65 };

	};
}

#endif