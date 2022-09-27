#pragma once
namespace Dragonite
{
	struct Resolution
	{
		unsigned int myWidth, myHeight;
	};


	struct Color
	{
		float r, g, b, a;

		Color() = default;
		Color(float anR, float anG, float anB, float anA) : r(anR), g(anG), b(anB), a(anA) {}
		Color(const Color& aColor) : r(aColor.r), g(aColor.g), b(aColor.b), a(aColor.a) {}

		void operator=(const Color& aColor)
		{
			r = aColor.r;
			g = aColor.g;
			b = aColor.b;
			a = aColor.a;
		}

		float* operator&() {
			return (float*)this;
		}
	};


}