// ======================================================================
// File Name:		ResourceLibrary.h
// Project Name:	Zoodiac
// Author(s):		Low Zhi Sheng Kitson(100%)
// Brief:			Contains Resource Library functionality
// 
// All content © 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================

#ifndef RESOURCE_LIBRARY_H_
#define RESOURCE_LIBRARY_H_

#include <map>
#include <Memory>
#include <vector>
#include <ranges>

#include <Backend/Singleton.h>

namespace Backend::Resource
{
	/**
	 * \brief				A resource library.
	 * \tparam Key			Type of the key.
	 * \tparam Type			Type of the resource.
	 * \tparam Destructor	Type of the destructor.
	 */
	template <typename Key, typename Type, typename Destructor>
	class ResourceLibrary : public Singleton<ResourceLibrary<Key, Type, Destructor>>
	{
	private:
		std::map<Key, std::unique_ptr<Type, Destructor>> m_Resources;	//	Resources
		Destructor m_Destructor{};										//	Destructor
	public:
		/**
		 * \brief		Gets a resource.
		 * \param _key	The key.
		 * \return		The resource.
		 */
		Type* GetResource(Key _key);

		/**
		 * \brief		Adds a resource.
		 * \param _key	The key.
		 * \param _val	The value.
		 * \return		True if it succeeds, false if it fails.
		 */
		bool AddResource(Key _key, Type* _val);

		/**
		 * \brief		Removes the resource described by _key.
		 * \param _key	The key.
		 * \return		True if it succeeds, false if it fails.
		 */
		bool RemoveResource(Key _key);

		/**
		 * \brief	Clears the library.
		 */
		void ClearLibrary();

		/**
		 * \brief				Sets a destructor.
		 * \param _destructor	The destructor.
		 */
		void SetDestructor(Destructor _destructor);

		std::vector<Key> GetAllKeys();
	};

	/**
	 * \brief				A resource library.
	 * \tparam Key			Type of the key.
	 * \tparam Type			Type of the resource.
	 */
	template <typename Key, typename Type>
	class ResourceLibrary<Key, Type, std::nullptr_t> : public Singleton<ResourceLibrary<Key, Type, std::nullptr_t>>
	{
	private:
		std::map<Key, std::unique_ptr<Type>> m_Resources;	//	Resources
	public:
		/**
		 * \brief		Gets a resource.
		 * \param _key	The key.
		 * \return		The resource.
		 */
		Type* GetResource(Key _key);

		/**
		 * \brief		Adds a resource.
		 * \param _key	The key.
		 * \param _val	The value.
		 * \return		True if it succeeds, false if it fails.
		 */
		bool AddResource(Key _key, Type* _val);

		/**
		 * \brief		Removes the resource described by _key.
		 * \param _key	The key.
		 * \return		True if it succeeds, false if it fails.
		 */
		bool RemoveResource(Key _key);

		/**
		 * \brief	Clears the library.
		 */
		void ClearLibrary();

		std::vector<Key> GetAllKeys();
	};

	template<typename Key, typename Type, typename Destructor>
	Type* ResourceLibrary<Key, Type, Destructor>::GetResource(Key _key)
	{
		if (m_Resources.count(_key))
			return m_Resources.at(_key).get();
		return nullptr;
	}

	template<typename Key, typename Type, typename Destructor>
	bool ResourceLibrary<Key, Type, Destructor>::AddResource(Key _key, Type* _val)
	{
		if (m_Resources.count(_key))
			return false;
		m_Resources.emplace(_key, std::unique_ptr<Type, Destructor>{_val, m_Destructor});
		return true;
	}

	template<typename Key, typename Type, typename Destructor>
	bool ResourceLibrary<Key, Type, Destructor>::RemoveResource(Key _key)
	{
		if (!m_Resources.count(_key))
			return false;
		m_Resources.erase(_key);
		return true;
	}

	template<typename Key, typename Type, typename Destructor>
	void ResourceLibrary<Key, Type, Destructor>::ClearLibrary()
	{
		m_Resources.clear();
	}

	template<typename Key, typename Type, typename Destructor>
	void ResourceLibrary<Key, Type, Destructor>::SetDestructor(Destructor _destructor)
	{
		m_Destructor = _destructor;
	}

	template <typename Key, typename Type, typename Destructor>
	std::vector<Key> ResourceLibrary<Key, Type, Destructor>::GetAllKeys()
	{
		std::vector<Key> ret;
		for (auto i : m_Resources | std::views::keys)
			ret.emplace_back(i);
		return ret;
	}

	template <typename Key, typename Type>
	Type* ResourceLibrary<Key, Type, std::nullptr_t>::GetResource(Key _key)
	{
		if (m_Resources.count(_key))
			return m_Resources.at(_key).get();
		return nullptr;
	}

	template <typename Key, typename Type>
	bool ResourceLibrary<Key, Type, std::nullptr_t>::AddResource(Key _key, Type* _val)
	{
		if (m_Resources.count(_key))
			return false;
		m_Resources.emplace(_key, std::unique_ptr<Type>{_val});
		return true;
	}

	template <typename Key, typename Type>
	bool ResourceLibrary<Key, Type, std::nullptr_t>::RemoveResource(Key _key)
	{
		if (!m_Resources.count(_key))
			return false;
		m_Resources.erase(_key);
		return true;
	}

	template <typename Key, typename Type>
	void ResourceLibrary<Key, Type, std::nullptr_t>::ClearLibrary()
	{
		m_Resources.clear();
	}

	template <typename Key, typename Type>
	std::vector<Key> ResourceLibrary<Key, Type, std::nullptr_t>::GetAllKeys()
	{
		std::vector<Key> ret;
		for (const auto& i : m_Resources | std::views::keys)
			ret.emplace_back(i);
		return ret;
	}
}
#endif