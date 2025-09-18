// ======================================================================
// File Name:		DependencyInjector.h
// Project Name:	Zoodiac
// Author(s):		Low Zhi Sheng Kitson(94%), Lim Geng Yang(1%),
//					Lye Pin Liang Xavier(4%), Muhammad Dzulhafiz(1%)
// Brief:			Dependency Injector Information
// 
// All content © 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================

#ifndef DEPENDENCY_INJECTOR_H_
#define DEPENDENCY_INJECTOR_H_

#include <Backend/Singleton.h>
#include <Backend/SceneManagement/SceneCommon.h>

#include <unordered_map>

namespace Backend
{
	/**
	 * \brief Singleton to inject dependencies
	 */
	class DependencyInjector final : public Singleton<DependencyInjector>
	{
	public:
		enum Dependency
		{
			PLAYER_POS,
			SCORE,
			HIT,
			PET,
			PAUSE,
			CURRENT_PET,
			SCENE_PET,
			EXIT,

			NUM_DEPENDENCY
		};
	private:
		using Dependencies = std::unordered_map<Scene, std::unordered_map<Dependency, void*>>;
		Dependencies m_Dependencies;
	public:
		/**
		 * \brief			Registers a new dependency.
		 * \param _scene	The scene it belongs to.
		 * \param _id		The id of the dependency.
		 * \param _data		The pointer to the dependency.
		 */
		auto RegisterDependency(const Scene& _scene, Dependency _id, void* _data) -> void;

		/**
		 * \brief			Returns the dependency in a form of a pointer.
		 * \tparam T		The return type.
		 * \param _scene	The scene it belongs to.
		 * \param _id		The id of the dependency.
		 * \return			The pointer to the dependency.
		 */
		template<typename T>
		auto GetDependency(const Scene& _scene, Dependency _id) -> T*;
	};

	template <typename T>
	auto DependencyInjector::GetDependency(const Scene& _scene, const Dependency _id) -> T*
	{
		if (!m_Dependencies.at(_scene).contains(_id))
			return nullptr;
		return static_cast<T*>(m_Dependencies.at(_scene).at(_id));
	}

	inline auto DependencyInjector::RegisterDependency(const Scene& _scene, const Dependency _id, void* _data) -> void
	{
		m_Dependencies[_scene][_id] = _data;
	}
}

#endif