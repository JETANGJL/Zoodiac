// ======================================================================
// File Name:		Utils.h
// Project Name:	Zoodiac
// Author(s):		Ang Jie Le Jet(80%), Lim Geng Yang(10%),
//					Low Zhi Sheng Kitson(10%)
// Brief:			File for utility functions
// 
// All content � 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================

#ifndef UTILS_H_
#define UTILS_H_

#include <AEEngine.h>

namespace Backend
{
	/**
	* \brief	Returns a random integer generated within a range.
	* \param	_min	Min int.
	* \param	_max	Max int.
	* \return	int
	*/
	auto RandomInt(int _min, int _max) -> int;

	/**
	* \brief	Returns a random float generated within a range with precision.
	* \param	_min	Min float.
	* \param	_max	Max float.
	* \return	float
	*/
	auto RandomFloat(float _min, float _max, int _precision = 2) -> float;

	/**
	* \brief	Snaps position to grid.
	* \param	_pos	AEVec2 position.
	*/
	auto SnapToGrid(AEVec2& _pos) -> void;

	/**
	* \brief	Returns a random float generated within a range.
	* \param	_min	Min float.
	* \param	_max	Max float.
	* \return	float
	*/
	auto RandomRange(float _min, float _max) -> float;

	/**
	* \brief	Returns a random AEVec2 Point.
	* \param	_minX	Min float.
	* \param	_minY	Min float.
	* \param	_maxX	Max float.
	* \param	_maxY	Max float.
	* \return	AEVec2
	*/
	auto RandomPoint(float _minX, float _minY, float _maxX, float _maxY) -> AEVec2;

	/**
	* \brief	Destroy font.
	* \param	_font	font.
	*/
	auto DestroyFont(const s8* _font) -> void;
}

#endif