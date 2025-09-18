// ======================================================================
// File Name:		State.cpp
// Project Name:	Zoodiac
// Author(s):		Ang Jie Le Jet(50%), Muhammad Dzulhafiz(30%),
//					Low Zhi Sheng Kitson(20%)
// Brief:			Contains State Functionality
// 
// All content © 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================

#include <Backend/Engine_Common.h>
#include <Backend/Utils.h>
#include <States/State.h>	

namespace StateUtils
{
	auto IsPetNear(AEVec2 _playerPosition, AEVec2 _petPosition) -> bool
	{
		return AEVec2Distance(&_playerPosition, &_petPosition) < 100.0f;
	}

	auto MoveAwayFromPoint(AEVec2& _direction, const AEVec2& _position, const AEVec2& _target) -> void
	{
		_direction = _position - _target;
		AEVec2Normalize(&_direction, &_direction);
	}

	auto MoveTowardsPoint(AEVec2& _direction, const AEVec2& _position, const AEVec2& _target) -> void
	{
		_direction = _target - _position;
		AEVec2Normalize(&_direction, &_direction);
	}

	auto CircleState(AEVec2& _direction, float& _angle) -> void
	{
		static constexpr float radius = 30.f;
		_direction = { radius * cos(_angle), radius * sin(_angle) };
		_angle = AEWrap(_angle + Backend::GetDeltaTime(), 0, TWO_PI);
		AEVec2Normalize(&_direction, &_direction);
	}

	auto FollowPlayerState(AEVec2& _direction, const AEVec2& _position, const AEVec2& _target) -> void
	{
		static constexpr float radius = 30.f;
		static constexpr float minDistance = 10.f;
		AEVec2 d = _target - _position;
		const float distance = AEVec2Length(&d);
		if (distance > radius)
			MoveTowardsPoint(_direction, _position, _target);
		if (distance < minDistance)
			MoveAwayFromPoint(_direction, _position, _target);
	}

	auto RandomState(AEVec2& _direction, const AEVec2& _position, const AEVec2& _min, const AEVec2& _max) -> void
	{
		static AEVec2 randomPoint{ _position };
		static unsigned int timeElapsed = 0;
		timeElapsed++;

		if (!(timeElapsed % 100))
		{
			randomPoint.x = Backend::RandomRange(_min.x, _max.x);
			randomPoint.y = Backend::RandomRange(_min.y, _max.y);
			timeElapsed = 0;
		}
		MoveTowardsPoint(_direction, _position, randomPoint);
	}

	auto JumpState(bool& _jump) -> void
	{
		static int timeElapsed = 0;
		timeElapsed++;

		if (timeElapsed % 25 == 0)
		{
			_jump = true;
			timeElapsed = 0;
		}
	}
}