// ======================================================================
// File Name:		StateHappy.h
// Project Name:	Zoodiac
// Author(s):		Ang Jie Le Jet(60%), Low Zhi Sheng Kitson(40%)
// Brief:			Contains Happy State Functionality
// 
// All content © 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================

#ifndef STATE_HAPPY_H_
#define STATE_HAPPY_H_

#include <States/State.h>

class StateHappy : public State
{
public:
    /**
     * \brief					Updates the entity each frame.
     * \param _petComponent		The Entity to update.
     */
    void Update(Backend::ECS::Component::PetComponent* _petComponent) override;
};

#endif
