// ======================================================================
// File Name:		VecOverloads.h
// Project Name:	Zoodiac
// Author(s):		Lim Geng Yang(80%), Lye Pin Liang Xavier(20%)
// Brief:			Contains AEVec2 Overloads
// 
// All content © 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================

#include <Backend/VecOperators.h>

AEVec2& operator-(AEVec2& _rhs)
{
	return _rhs *= -1;
}

AEVec2 operator+(const AEVec2& _lhs, const AEVec2& _rhs)
{
	return AEVec2{_lhs.x + _rhs.x, _lhs.y + _rhs.y};
}

AEVec2& operator+=(AEVec2& _lhs, const AEVec2& _rhs)
{
	_lhs.x += _rhs.x;
	_lhs.y += _rhs.y;
	return _lhs;
}

AEVec2 operator-(const AEVec2& _lhs, const AEVec2& _rhs)
{
	return AEVec2{ _lhs.x - _rhs.x, _lhs.y - _rhs.y };
}

AEVec2& operator-=(AEVec2& _lhs, const AEVec2& _rhs)
{
	_lhs.x -= _rhs.y;
	_lhs.y -= _rhs.y;
	return _lhs;
}

AEVec2 operator*(const AEVec2& _lhs, const float& _rhs)
{
	return AEVec2{ _lhs.x * _rhs, _lhs.y * _rhs };
}

AEVec2& operator*=(AEVec2& _lhs, const float& _rhs)
{
	_lhs.x *= _rhs;
	_lhs.y *= _rhs;
	return _lhs;
}

AEVec2 operator*(const AEVec2& _lhs, const AEMtx33& _rhs)
{
	return { _rhs.m[0][0] * _lhs.x + _rhs.m[0][1] * _lhs.y,
		_rhs.m[1][0] * _lhs.x + _rhs.m[1][1] * _lhs.y };
}

AEVec2& operator*=(AEVec2& _lhs, const AEMtx33& _rhs)
{
	_lhs = { _rhs.m[0][0] * _lhs.x + _rhs.m[0][1] * _lhs.y,
		_rhs.m[1][0] * _lhs.x + _rhs.m[1][1] * _lhs.y };
	return _lhs;
}

bool operator==(const AEVec2& _lhs, const AEVec2& _rhs)
{
	return (static_cast<int>(_lhs.x * 100000.0f) == static_cast<int>(_rhs.x * 100000.0f) && static_cast<int>(_lhs.y * 100000.0f) == static_cast<int>(_rhs.y * 100000.0f));
}

bool operator!=(const AEVec2& _lhs, const AEVec2& _rhs)
{
	return (static_cast<int>(_lhs.x * 100000.0f) != static_cast<int>(_rhs.x * 100000.0f) && static_cast<int>(_lhs.y * 100000.0f) != static_cast<int>(_rhs.y * 100000.0f));
}

AEVec2& NormalizeVec2(AEVec2& _vec)
{
	AEVec2Normalize(&_vec, &_vec);
	return _vec;
}

AEVec2& operator+(AEVec2& _lhs, const float& _rhs)
{
	_lhs.x += _rhs;
	_lhs.y += _rhs;
	return _lhs;
}
