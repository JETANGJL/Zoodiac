// ======================================================================
// File Name:		State.h
// Project Name:	Zoodiac
// Author(s):		Ang Jie Le Jet(50%), Muhammad Dzulhafiz(30%),
//					Low Zhi Sheng Kitson(20%)
// Brief:			Contains State Functionality
// 
// All content © 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================

#ifndef STATE_H_
#define STATE_H_

#include <AEEngine.h>

namespace Backend::ECS::Component
{
    class PetComponent;
}

enum StateType
{
	STATE_IDLE,
	STATE_HAPPY,
	STATE_ANGRY,
	STATE_WILD,
	NUM_STATE
};

class State
{
public:
	/**
	 * \brief Destructor
	 */
	virtual ~State() = default;

	/**
     * \brief					Updates the entity each frame.
     * \param _petComponent		The Entity to update.
     */
    virtual void Update(Backend::ECS::Component::PetComponent* _petComponent) = 0;
};

namespace StateUtils
{
	auto IsPetNear(AEVec2 _playerPosition, AEVec2 _petPosition) -> bool;
	auto MoveAwayFromPoint(AEVec2& _direction, const AEVec2& _position, const AEVec2& _target) -> void;
	auto MoveTowardsPoint(AEVec2& _direction, const AEVec2& _position, const AEVec2& _target) -> void;
	auto CircleState(AEVec2& _direction, float& _angle) -> void;
	auto FollowPlayerState(AEVec2& _direction, const AEVec2& _position, const AEVec2& _target) -> void;
	auto RandomState(AEVec2& _direction, const AEVec2& _position, const AEVec2& _min, const AEVec2& _max) -> void;
	void JumpState(bool& _jump);
}

#endif