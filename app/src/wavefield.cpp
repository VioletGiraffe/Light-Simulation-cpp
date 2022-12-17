#include "wavefield.h"

#include <cmath>

template <typename T>
[[nodiscard]] constexpr T sqr(T v)
{
	return v * v;
}

WaveField::WaveField()
{
	resetSimulation();
}

void WaveField::resetSimulation()
{
	_time = 0.0f;
	_waveHeight.fill(std::array<float, 3>{0.0f, 0.0f, 0.0f});
	_waveVelocity.fill(std::array<float, 3>{0.0f, 0.0f, 0.0f});
	_accumulatedLight.fill(std::array<float, 3>{0.0f, 0.0f, 0.0f});

	_pixelMass.fill(1.0f);
	for (size_t i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			static constexpr size_t circleRadius = 100;
			if (sqr(i - height / 2) + sqr(j - width / 2) < sqr(circleRadius))
				_pixelMass[i][j] = 0.75f;
		}
	}
}

void WaveField::update(const float dt)
{
	_time += dt;

	if (_time < 2.0f)
	{
		for (int j = height / 2 - 50; j < height / 2; j++) {
			_waveHeight[j - 50][165].fill(std::sin(_time * 100.0f * 0.8f) * 12.0f);
		}
	}

	static constexpr float ACCUMULATED_EXPOSURE = 0.0005f;
	static constexpr float COLOR_SHIFT[] {0.02f, 0.0f, -0.04f};

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			for (int k = 0; k < 3; k++) {
				_waveHeight[i][j][k] += _waveVelocity[i][j][k];
				_accumulatedLight[i][j][k] += std::abs(_waveHeight[i][j][k]) * ACCUMULATED_EXPOSURE;
			}
		}
	}

	for (int i = 1; i < height - 1; i++) {
		for (int j = 1; j < width - 1; j++) {
			for (int k = 0; k < 3; k++) {
				float speed = _pixelMass[i][j] - COLOR_SHIFT[k];
				float force = _waveHeight[i - 1][j][k] + _waveHeight[i + 1][j][k] + _waveHeight[i][j - 1][k] + _waveHeight[i][j + 1][k];
				_waveVelocity[i][j][k] += (force / 4.0f - _waveHeight[i][j][k]) * speed;
			}
		}
	}
}

std::array<uint8_t, 3> WaveField::pixelColor(size_t x, size_t y) const
{
	static constexpr size_t GLASS_COLORS[] {50, 60, 70};

	std::array<uint8_t, 3> color;
	for (int k = 0; k < 3; k++)
	{
		size_t colorValue = static_cast<size_t>(sqr(std::min(_accumulatedLight[y][x][k], 1.0f)) * 255.0f);
		if (_pixelMass[y][x] < 1.0f)
			colorValue = std::min(colorValue + GLASS_COLORS[k], size_t{255});

		color[k] = static_cast<uint8_t>(colorValue);
	}

	return color;
}
