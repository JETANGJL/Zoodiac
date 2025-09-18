// ======================================================================
// File Name:		Singleton.h
// Project Name:	Zoodiac
// Author(s):		Low Zhi Sheng Kitson(100%)
// Brief:			Contains Singleton functionality
// 
// All content © 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================

#ifndef SINGLETON_H_
#define SINGLETON_H_

namespace Backend
{
	/**
	 * \brief			Singleton class
	 * \tparam Type		Type of the singleton
	 */
	template <class Type>
	class Singleton
	{
	protected:
		/**
		 * \brief	Constructor.
		 */
		Singleton<Type>() = default;

		/**
		 * \brief	Destructor.
		 */
		virtual ~Singleton<Type>() = default;
	public:
		/**
		 * \brief	Gets the instance of the singleton.
		 * \return	Instance of the singleton.
		 */
		static Type& GetInstance();
	};

	template<class Type>
	Type& Singleton<Type>::GetInstance()
	{
		static Type instance;
		return instance;
	}
}

#endif