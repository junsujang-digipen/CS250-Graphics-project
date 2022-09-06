/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/

#include <glm/glm.hpp>
#include "Noise.h"
#include "GraphicEngine.h"


unsigned min(const unsigned& a, const unsigned& b) { return (a < b ? a : b); }
template <typename T = float>
T lerp(const T& lo, const T& hi, const T& t) { return lo * (1 - t) + hi * t; }
float cosineSmooth(const float& t) { return (1 - std::cosf(t * 3.14159f)) * 0.5f; }
float smoothStep(const float& t)
{
	float t3 = t*t*t;
	return 6 * t3 * t * t - 15.f * t3 * t + 10 * t3;
}

function::Noise::Noise()
{
	for (auto&arr:values) {
		for (auto&v: arr) {
			v = static_cast<float>(GraphicEngine::GetRandomNum(0.,1.));
		}
	}
}

float function::Noise::eval(const float& xoff, const float& yoff)
{
	int xi = static_cast<int>(std::floor(xoff));
	int yi = static_cast<int>(std::floor(yoff));

	int xi0 = xi & valArraySizeMask;
	int xi1 = (xi0 + 1) & valArraySizeMask;
	int yi0 = yi & valArraySizeMask;
	int yi1 = (yi0 + 1) & valArraySizeMask;

	const float& ran00 = values[xi0][yi0];
	const float& ran10 = values[xi1][yi0];
	const float& ran01 = values[xi0][yi1];
	const float& ran11 = values[xi1][yi1];

	float xf = xoff - xi;
	float yf = yoff - yi;
	float sx = smoothStep(xf);
	float sy = smoothStep(yf);

	float nx0 = lerp(ran00, ran10, sx);
	float nx1 = lerp(ran01, ran11, sx);
	return lerp(nx0, nx1, sy);
}

float function::Noise::operator()(const float& xoff, const float& yoff)
{
	return eval(xoff,yoff);
}

float function::Noise::operator()(const float&& xoff, const float&& yoff)
{
	return operator()(xoff, yoff);
}

function::GradientNoise::GradientNoise()
{
	float gradientLen2;
	for (unsigned i = 0; i < tableSize; ++i)
	{
		do
		{
			gradients[i] = glm::vec3(2 * static_cast<float>(GraphicEngine::GetRandomNum(0., 1.)) - 1, 2 * static_cast<float>(GraphicEngine::GetRandomNum(0., 1.)) - 1, 2 * static_cast<float>(GraphicEngine::GetRandomNum(0., 1.)) - 1);
			gradientLen2 = glm::dot(gradients[i], gradients[i]);
		} while (gradientLen2 > 1);
		gradients[i] /= sqrtf(gradientLen2); // normalize gradient
		permutationTable[i] = i;
	}
	// create permutation table
	for (unsigned i = 0; i < tableSize; ++i)
	{//permutationTable[static_cast<int>(GraphicEngine::GetRandomNum(0., 1.))
		std::swap(permutationTable[i], permutationTable[static_cast<int>(GraphicEngine::GetRandomNum(0., 2*tableSize)) & tableSizeMask]);
		permutationTable[i + tableSize] = permutationTable[i];
	}
}

float function::GradientNoise::operator()(const glm::vec3& p)
{
	return eval(p);
}

float function::GradientNoise::operator()(const glm::vec3&& p)
{
	return operator()(p);
}

int function::GradientNoise::hash(const int& x, const int& y, const int& z) const
{
	return permutationTable[permutationTable[permutationTable[x] + y] + z];
}

float function::GradientNoise::eval(const glm::vec3& p) const
{
	int xi0 = ((int)std::floor(p.x)) & tableSizeMask;
	int yi0 = ((int)std::floor(p.y)) & tableSizeMask;
	int zi0 = ((int)std::floor(p.z)) & tableSizeMask;

	int xi1 = (xi0 + 1) & tableSizeMask;
	int yi1 = (yi0 + 1) & tableSizeMask;
	int zi1 = (zi0 + 1) & tableSizeMask;

	// gradients at the corner of the cell
	const glm::vec3& c000 = gradients[hash(xi0, yi0, zi0)];
	const glm::vec3& c100 = gradients[hash(xi1, yi0, zi0)];
	const glm::vec3& c010 = gradients[hash(xi0, yi1, zi0)];
	const glm::vec3& c110 = gradients[hash(xi1, yi1, zi0)];

	const glm::vec3& c001 = gradients[hash(xi0, yi0, zi1)];
	const glm::vec3& c101 = gradients[hash(xi1, yi0, zi1)];
	const glm::vec3& c011 = gradients[hash(xi0, yi1, zi1)];
	const glm::vec3& c111 = gradients[hash(xi1, yi1, zi1)];

	float tx = p.x - ((int)std::floor(p.x));
	float ty = p.y - ((int)std::floor(p.y));
	float tz = p.z - ((int)std::floor(p.z));

	float x0 = tx, x1 = tx - 1;
	float y0 = ty, y1 = ty - 1;
	float z0 = tz, z1 = tz - 1;

	glm::vec3 p000 = glm::vec3(x0, y0, z0);
	glm::vec3 p100 = glm::vec3(x1, y0, z0);
	glm::vec3 p010 = glm::vec3(x0, y1, z0);
	glm::vec3 p110 = glm::vec3(x1, y1, z0);

	glm::vec3 p001 = glm::vec3(x0, y0, z1);
	glm::vec3 p101 = glm::vec3(x1, y0, z1);
	glm::vec3 p011 = glm::vec3(x0, y1, z1);
	glm::vec3 p111 = glm::vec3(x1, y1, z1);

	float u = smoothStep(tx);
	float v = smoothStep(ty);
	float w = smoothStep(tz);

	// linear interpolation
	float a = lerp(glm::dot(c000, p000), glm::dot(c100, p100), u);
	float b = lerp(glm::dot(c010, p010), glm::dot(c110, p110), u);
	float c = lerp(glm::dot(c001, p001), glm::dot(c101, p101), u);
	float d = lerp(glm::dot(c011, p011), glm::dot(c111, p111), u);

	float e = lerp(a, b, v);
	float f = lerp(c, d, v);

	return lerp(e, f, w);
}