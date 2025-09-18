// ======================================================================
// File Name:		VecOverloads.h
// Project Name:	Zoodiac
// Author(s):		Lim Geng Yang(80%), Lye Pin Liang Xavier(20%)
// Brief:			Contains AEVec2 Overloads
// 
// All content © 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================

#ifndef VEC_OPERATORS_H_
#define VEC_OPERATORS_H_

#include <AEEngine.h>

//AEVec2Neg
/**
* \brief		Operator overload for AEVec2, negates value of AEVec2 object.
* \param _rhs	The AEVec2 object to be Neg.
*/
AEVec2& operator-(AEVec2& _rhs);
//AEVec2Add
/**
* \brief		Operator overload for AEVec2, for AEVec2 + AEVec2.
*				Returns a copy of the object.
* \param _lhs	The AEVec2 object to be added to.
* \param _rhs	The AEVec2 object to add.
*/
AEVec2 operator+(const AEVec2& _lhs, const AEVec2& _rhs);
/**
* \brief		Operator overload for AEVec2, for AEVec2 += AEVec2.
* \param _lhs	The AEVec2 object to be added to.
* \param _rhs	The AEVec2 object to add.
*/
AEVec2& operator+=(AEVec2& _lhs, const AEVec2& _rhs);
//AEVec2Sub
/**
* \brief		Operator overload for AEVec2, for AEVec2 - AEVec2.
* \param _lhs	The AEVec2 object to be subtracted.
* \param _rhs	The AEVec2 object to subtract.
*/
AEVec2 operator-(const AEVec2& _lhs, const AEVec2& _rhs);
/**
* \brief		Operator overload for AEVec2, for AEVec2 -= AEVec2.
* \param _lhs	The AEVec2 object to be subtracted.
* \param _rhs	The AEVec2 object to subtract.
*/
AEVec2& operator-=(AEVec2& _lhs, const AEVec2& _rhs);
//AEVec2Scale
/**
* \brief		Operator overload for AEVec2 scaling, for AEVec2 * float.
* \param _lhs	The AEVec2 object to be multiplied.
* \param _rhs	The float value to multiply.
*/
AEVec2 operator*(const AEVec2& _lhs, const float& _rhs);
/**
* \brief		Operator overload for AEVec2 scaling, for AEVec2 * float.
* \param _lhs	The AEVec2 object to be multiplied.
* \param _rhs	The float value to multiply.
*/
AEVec2& operator*=(AEVec2& _lhs, const float& _rhs);
//Multiply Vec
/**
* \brief		Operator overload for AEVec2, for AEVec2 * AEVec2.
* \param _lhs	The AEVec2 object to be multiplied.
* \param _rhs	The AEVec2 object to multiply.
*/
AEVec2 operator*(const AEVec2& _lhs, const AEMtx33& _rhs);
/**
* \brief		Operator overload for AEVec2, for AEVec2 *= AEVec2.
* \param _lhs	The AEVec2 object to be multiplied.
* \param _rhs	The AEVec2 object to multiply.
*/
AEVec2& operator*=(AEVec2& _lhs, const AEMtx33& _rhs);
//== check
/**
* \brief		Operator overload for AEVec2, for AEVec2 == AEVec2.
* \param _lhs	The first AEVec2 object to check.
* \param _rhs	The second AEVec2 object to check.
*/
bool operator==(const AEVec2& _lhs, const AEVec2& _rhs);
/**
* \brief		Operator overload for AEVec2, for AEVec2 != AEVec2.
* \param _lhs	The first AEVec2 object to check.
* \param _rhs	The second AEVec2 object to check.
*/
bool operator!=(const AEVec2& _lhs, const AEVec2& _rhs);
/**
* \brief		Normalizes valule of AEVec2 object.
* \param _vec	The object to be normalized.
*/
AEVec2& NormalizeVec2(AEVec2& _vec);
//AEVec2+int
/**
* \brief		Operator overload for AEVec2, for AEVec2 + int.
* \param _lhs	The object to be added to.
* \param _rhs	The value to be added to the object.
*/
AEVec2& operator+(AEVec2& _lhs, int _rhs);

#endif