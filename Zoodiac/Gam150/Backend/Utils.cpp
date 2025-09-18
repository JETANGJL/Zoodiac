#include <cstdlib>

#include <Backend/Engine_Common.h>
#include <Backend/Utils.h>

namespace Backend
{
	auto RandomInt(const int _min, const int _max) -> int
	{
		return _min == _max ? _max : rand() % (_max - _min) + _min;
	}

	auto RandomFloat(const float _min, const float _max, const int _precision) -> float
	{
		const auto precisionMultiplier = static_cast<float>(pow(10, _precision));
		const auto maxVal = static_cast<int>(_max * precisionMultiplier);
		const auto minVal = static_cast<int>(_min * precisionMultiplier);
		const auto val = static_cast<float>(rand() % (maxVal - minVal));
		return val / precisionMultiplier + _min;
	}

	auto RandomRange(const float _min, const float _max) -> float
	{
		return _min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / (_max - _min));
	}

	auto RandomPoint(const float _minX, const float _minY, const float _maxX, const float _maxY) -> AEVec2
	{
		return AEVec2{ Backend::RandomFloat(_minX,_maxX), Backend::RandomFloat(_minY, _maxY) };
	}

	auto DestroyFont(const s8* _font) -> void
	{
		AEGfxDestroyFont(*_font);
		delete _font;
	}

	auto SnapToGrid(AEVec2& _pos) -> void
	{
		if (_pos.x >= 0.0f)
			_pos.x = static_cast<float>(static_cast<int>(_pos.x) / 30) * 30;
		else
			_pos.x = (static_cast<float>(static_cast<int>(_pos.x) / 30) - 1) * 30;

		if (_pos.y >= 0.0f)
			_pos.y = static_cast<float>(static_cast<int>(_pos.y) / 30) * 30;
		else
			_pos.y = (static_cast<float>(static_cast<int>(_pos.y) / 30) - 1) * 30;
	}
}