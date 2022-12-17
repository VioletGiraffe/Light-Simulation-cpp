#pragma once

#include <array>
#include <utility>
#include <vector>

template <typename T, size_t w, size_t h>
class Array2D
{
public:
	Array2D()
	{
		_array.resize(w * h, {});
	}

	[[nodiscard]] constexpr T* operator[](size_t x)
	{
		return _array.data() + x * h;
	}

	[[nodiscard]] constexpr const T* operator[](size_t x) const
	{
		return _array.data() + x * h;
	}

	constexpr void fill(const T& value)
	{
		for (auto& item: _array)
			item = value;
	}

private:
	std::vector<T> _array;
};

template <size_t w, size_t h>
using Array2Df = Array2D<float, w, h>;

template <size_t w, size_t h, size_t z>
using Array3Df = Array2D<std::array<float, z>, w, h>;

class WaveField
{
public:
	static constexpr size_t width = 640, height = 480;

	WaveField();
	void resetSimulation();

	void update(float dt);

	std::array<uint8_t, 3> pixelColor(size_t x, size_t y) const;

private:
	Array3Df<width, height, 3> _waveHeight;
	Array3Df<width, height, 3> _waveVelocity;
	Array3Df<width, height, 3> _accumulatedLight;
	Array2Df<width, height>    _pixelMass;

	float _time = 0.0f;
};

