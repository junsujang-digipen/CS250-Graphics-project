/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/

#pragma once

namespace function {
	class Noise {
		static constexpr unsigned valArraySize = 256;
		static constexpr unsigned valArraySizeMask = valArraySize - 1;//0b11111111
		float values[valArraySize][valArraySize];
	public:
		Noise();
		float eval(const float& xoff, const float& yoff);
		float operator()(const float& xoff, const float& yoff);
		float operator()(const float&& xoff, const float&& yoff);
	};

	class GradientNoise {
		static const unsigned tableSize = 256;
		static const unsigned tableSizeMask = tableSize - 1;
		glm::vec3 gradients[tableSize];
		unsigned permutationTable[tableSize * 2];
		int hash(const int& x, const int& y, const int& z)const;
	public:
		GradientNoise();
		float eval(const glm::vec3& p)const ;
		float operator()(const glm::vec3& p);
		float operator()(const glm::vec3&& p);
	};
}
inline function::Noise noise2D{};
inline function::GradientNoise perlinNoise{};
