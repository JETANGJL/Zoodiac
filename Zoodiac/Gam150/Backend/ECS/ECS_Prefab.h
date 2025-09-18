// ======================================================================
// File Name:		ECS_Prefab.h
// Project Name:	Zoodiac
// Author(s):		Low Zhi Sheng Kitson(70%), Lim Geng Yang(15%),
//					Lye Pin Liang Xavier(15%)
// Brief:			Contains ECS Prefab functionality
// 
// All content © 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================

#ifndef ECS_PREFAB_H_
#define ECS_PREFAB_H_

#include <tuple>
#include <typeinfo>
#include <variant>
#include <optional>

#include <Backend/ECS/ECS_Components.h>
#include <Backend/ECS/ECS_IncludeComponents.h>
#include <Backend/SceneManagement/SceneCommon.h>

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

namespace Backend::ECS
{
	class ECS_Prefab
	{
	private:
		using Reader = rapidjson::GenericValue<rapidjson::UTF8<>>;
		using Writer = rapidjson::PrettyWriter<rapidjson::StringBuffer>;
		using ComponentPointer = std::optional<std::variant<ECS_ALL_COMPONENT_POINTERS>>;
	public:
		Component::AllComponents m_Components;
		ECS_Signature m_Signature;
		std::string m_Name;

		/**
		 * \brief					Recursively Gets the Component.
		 * \tparam Component1		Current recursion of component type.
		 * \tparam Component2		Next recursion of component type.
		 * \tparam ComponentOthers	The rest of the components.
		 * \param _id				Component ID to get.
		 * \return					Pointer to the component data.
		 */
		template<typename Component1, typename Component2, typename ...ComponentOthers>
		auto GetComponent(uint32_t _id)->ComponentPointer;

		/**
		 * \brief				Attempts to Get the component if the id matches.
		 * \tparam Component	Component type to compare.
		 * \param _id			Component ID to get.
		 * \return				Pointer to the component data, nullptr if otherwise.
		 */
		template<typename Component>
		auto GetComponent(uint32_t _id)->ComponentPointer;

		/**
		 * \brief				Adds a component to the prefab, deserialization.
		 * \tparam Component	Component type to add.
		 * \return				Reference to the added component.
		 */
		template<typename Component>
		auto AddComponent() ->Component&;

		/**
		 * \brief					Adds a component to the prefab, serialization
		 * \param _componentType	ID of the component
		 * \param _reader			JSON Reader
		 * \return					True if successful
		 */
		auto AddComponent(std::uint32_t _componentType, const Reader& _reader) -> bool;

		/**
		 * \brief					Recursively Inits the Component
		 * \tparam Component		Current recursion of component type.
		 * \tparam Component2		Next recursion of component type.
		 * \tparam ComponentOthers	The rest of the components.
		 * \param _componentType	ID of Component to init.
		 * \param _component		Pointer to Component Data.
		 * \param _reader			JSON Reader.
		 */
		template<typename Component, typename Component2, typename ...ComponentOthers>
		auto InitComponent(std::uint32_t _componentType, ComponentPointer& _component, const Reader& _reader) -> void;

		/**
		 * \brief					Inits the component using the reader.
		 * \tparam Component		Component type.
		 * \param _componentType	ID of the component.
		 * \param _component		Pointer to the Component Data.
		 * \param _reader			JSON Reader.
		 */
		template<typename Component>
		auto InitComponent(std::uint32_t _componentType, ComponentPointer& _component, const Reader& _reader) -> void;
		
		/**
		 * \brief				Removes a component from the prefab.
		 * \tparam Component	Component Type.
		 * \return				True if successful.
		 */
		template<typename Component>
		auto RemoveComponent() -> bool;

		/**
		 * \brief				Checks if the prefab has a component.
		 * \tparam Component	Component Type
		 * \return				True if it exists.
		 */
		template<typename Component>
		[[nodiscard]] auto HasComponent() const -> bool;

		/**
		 * \brief			Serializes the Prefab.
		 * \param _writer	JSON Writer.
		 * \return			True on success.
		 */
		auto Serialize(Writer& _writer) const -> bool;

		/**
		 * \brief				Serializes a component.
		 * \tparam Component	Component Type.
		 * \param _writer		JSON Writer.
		 * \param _component	Component Data to serialize.
		 * \return				True on success.
		 */
		template<typename Component>
		auto SerializeComponent(Writer& _writer, const Component& _component) const -> bool;

		/**
		 * \brief				Serializes a component.
		 * \param _writer 		JSON Writer.
		 * \param _component 	Component Data to serialize.
		 * \return 				True on success.
		 */
		template<>
		auto SerializeComponent<Component::SpriteComponent>(Writer& _writer, const Component::SpriteComponent& _component) const -> bool;

		/**
		 * \brief				Serializes a component.
		 * \param _writer 		JSON Writer.
		 * \param _component 	Component Data to serialize.
		 * \return 				True on success.
		 */
		template<>
		auto SerializeComponent<Component::TileComponent>(Writer& _writer, const Component::TileComponent& _component) const -> bool;

		/**
		 * \brief				Serializes a component.
		 * \param _writer 		JSON Writer.
		 * \param _component 	Component Data to serialize.
		 * \return 				True on success.
		 */
		template<>
		auto SerializeComponent<Component::ColliderComponent>(Writer& _writer, const Component::ColliderComponent& _component) const -> bool;

		/**
		 * \brief				Serializes a component.
		 * \param _writer 		JSON Writer.
		 * \param _component 	Component Data to serialize.
		 * \return 				True on success.
		 */
		template<>
		auto SerializeComponent<Component::IsometricTransformComponent>(Writer& _writer, const Component::IsometricTransformComponent& _component) const ->bool;

		/**
		 * \brief				Serializes a component.
		 * \param _writer 		JSON Writer.
		 * \param _component 	Component Data to serialize.
		 * \return 				True on success.
		 */
		template<>
		auto SerializeComponent<Component::PortalComponent>(Writer& _writer, const Component::PortalComponent& _component) const -> bool;

		/**
		 * \brief				Serializes a component.
		 * \param _writer 		JSON Writer.
		 * \param _component 	Component Data to serialize.
		 * \return 				True on success.
		 */
		template<>
		auto SerializeComponent<Component::EmitterComponent>(Writer& _writer, const Component::EmitterComponent& _component) const -> bool;

	};

	inline auto ECS_Prefab::Serialize(Writer& _writer) const -> bool
	{
		_writer.StartObject();
		_writer.Key("Name");
		_writer.String(m_Name.c_str());
		_writer.Key("Components");
		_writer.StartArray();
		std::apply([&](auto&&... _val) {(SerializeComponent(_writer, _val),...); }, m_Components);
		_writer.EndArray();
		_writer.EndObject();
		return true;
	}

	template <typename Component1, typename Component2, typename ...ComponentOthers>
	auto ECS_Prefab::GetComponent(const uint32_t _id) ->ComponentPointer
	{
		if (auto ret = GetComponent<Component1>(_id))
			return ret;
		return GetComponent<Component2, ComponentOthers...>(_id);
	}

	template <typename Component>
	auto ECS_Prefab::GetComponent(const uint32_t _id) ->ComponentPointer
	{
		if (_id != ECS_Component::GetComponentID<Component>())
			return std::nullopt;
		return std::variant<ECS_ALL_COMPONENT_POINTERS>(&std::get<Component>(m_Components));
	}

	template <typename Component>
	auto ECS_Prefab::AddComponent() -> Component&
	{
		if (!HasComponent<Component>())
			m_Signature.set(ECS_Component::GetBitsetIndex<Component>(), true);
		return std::get<Component>(m_Components);
	}

	template <typename Component, typename Component2, typename ... ComponentOthers>
	auto ECS_Prefab::InitComponent(const std::uint32_t _componentType, ComponentPointer& _component, const Reader& _reader) -> void
	{
		if (_componentType == ECS_Component::GetComponentID<Component>())
			InitComponent<Component>(_componentType, _component, _reader);
		else
			InitComponent<Component2, ComponentOthers...>(_componentType, _component, _reader);
	}

	template <typename Component>
	auto ECS_Prefab::InitComponent(const std::uint32_t, ComponentPointer&, const Reader&) -> void
	{
	}

	inline auto ECS_Prefab::AddComponent(const std::uint32_t _componentType, const Reader& _reader) -> bool
	{
		if (!m_Signature.test(_componentType - 1))
			m_Signature.set(_componentType - 1, true);
		auto component = GetComponent<ECS_ALL_COMPONENTS>(_componentType);
		InitComponent<ECS_ALL_COMPONENTS>(_componentType, component, _reader);
		return true;
	}

	template <>
	inline auto ECS_Prefab::InitComponent<Component::SpriteComponent>(const std::uint32_t, ComponentPointer& _component,
		const Reader& _reader) -> void
	{
		const auto component = std::get<Component::SpriteComponent*>(_component.value());

		component->m_TextureKey = _reader["Sprite"].GetString();
		component->m_Red = static_cast<float>(_reader["r"].GetDouble());
		component->m_Green = static_cast<float>(_reader["g"].GetDouble());
		component->m_Blue = static_cast<float>(_reader["b"].GetDouble());
		component->m_Alpha = static_cast<float>(_reader["a"].GetDouble());
	}

	template <>
	inline auto ECS_Prefab::InitComponent<Component::TileComponent>(const std::uint32_t, ComponentPointer& _component,
		const Reader& _reader) -> void
	{
		assert(_component.has_value());
		const auto component = std::get<Component::TileComponent*>(_component.value());
		component->m_Red = static_cast<float>(_reader["r"].GetDouble());
		component->m_Green = static_cast<float>(_reader["g"].GetDouble());
		component->m_Blue = static_cast<float>(_reader["b"].GetDouble());
		component->m_Alpha = static_cast<float>(_reader["a"].GetDouble());
	}

	template <>
	inline auto ECS_Prefab::InitComponent<Component::ColliderComponent>(const std::uint32_t, ComponentPointer& _component,
		const Reader& _reader) -> void
	{
		assert(_component.has_value());
		const auto component = std::get<Component::ColliderComponent*>(_component.value());
		component->m_Tag = static_cast<Component::ColliderComponent::Tag>(_reader["Tag"].GetUint());
		const auto& size = _reader["Size"].GetArray();
		component->m_Size.x = static_cast<float>(size[0].GetDouble());
		component->m_Size.y = static_cast<float>(size[1].GetDouble());
	}

	template <>
	inline auto ECS_Prefab::InitComponent<Component::PortalComponent>(const std::uint32_t, ComponentPointer& _component,
		const Reader& _reader) -> void
	{
		assert(_component.has_value());
		const auto component = std::get<Component::PortalComponent*>(_component.value());
		component->scene = static_cast<Scene>(_reader["scene"].GetUint());
		component->pet = static_cast<Component::PlayerComponent::Pet_Type>(_reader["pet"].GetUint());
	}

	template <>
	inline auto ECS_Prefab::InitComponent<Component::IsometricTransformComponent>(const std::uint32_t, ComponentPointer& _component,
		const Reader& _reader) -> void
	{
		const auto component = std::get<Component::IsometricTransformComponent*>(_component.value());
		const auto& scale = _reader["Scale"].GetArray();
		component->m_Scale.x = static_cast<float>(scale[0].GetDouble());
		component->m_Scale.y = static_cast<float>(scale[1].GetDouble());
	}

	template <>
	inline auto ECS_Prefab::InitComponent<Component::EmitterComponent>(const std::uint32_t, ComponentPointer& _component,
		const Reader& _reader) -> void
	{
		const auto component = std::get<Component::EmitterComponent*>(_component.value());
		component->m_Type = static_cast<Component::EmitterComponent::Type>(_reader["Type"].GetUint());
		component->m_Emit = _reader["Emit"].GetInt();
	}

	template <typename Component>
	auto ECS_Prefab::RemoveComponent() -> bool
	{
		if (!HasComponent<Component>())
			return false;
		m_Signature.set(ECS_Component::GetBitsetIndex<Component>(), false);
		return true;
	}

	template <typename Component>
	auto ECS_Prefab::HasComponent() const -> bool
	{
		return m_Signature.test(ECS_Component::GetBitsetIndex<Component>());
	}

	template <typename Component>
	auto ECS_Prefab::SerializeComponent(Writer& _writer,
		const Component& _component) const -> bool
	{
		if (!HasComponent<Component>())
			return false;
		_writer.StartObject();
		_writer.Key("Name");
		_writer.String(std::string{ typeid(_component).name() }.substr(std::string{ typeid(_component).name() }.find_last_of(':') + 1).c_str());
		_writer.Key("ID");
		_writer.Uint(ECS_Component::GetComponentID<Component>());
		_writer.EndObject();
		return true;
	}

	template <>
	inline auto ECS_Prefab::SerializeComponent<Component::SpriteComponent>(
		Writer& _writer,
		const Component::SpriteComponent& _component) const -> bool
	{
		if (!HasComponent<Component::SpriteComponent>())
			return false;
		_writer.StartObject();
		_writer.Key("Name");
		_writer.String(std::string{ typeid(_component).name() }.substr(std::string{ typeid(_component).name() }.find_last_of(':') + 1).c_str());
		_writer.Key("ID");
		_writer.Uint(ECS_Component::GetComponentID<Component::SpriteComponent>());
		_writer.Key("Sprite");
		_writer.String(_component.m_TextureKey.c_str());
		_writer.Key("r");
		_writer.Double(static_cast<double>(_component.m_Red));
		_writer.Key("g");
		_writer.Double(static_cast<double>(_component.m_Green));
		_writer.Key("b");
		_writer.Double(static_cast<double>(_component.m_Blue));
		_writer.Key("a");
		_writer.Double(static_cast<double>(_component.m_Alpha));
		_writer.EndObject();
		return true;
	}

	template <>
	inline auto ECS_Prefab::SerializeComponent<Component::TileComponent>(
		Writer& _writer,
		const Component::TileComponent& _component) const -> bool
	{
		if (!HasComponent<Component::TileComponent>())
			return false;
		_writer.StartObject();
		_writer.Key("Name");
		_writer.String(std::string{ typeid(_component).name() }.substr(std::string{ typeid(_component).name() }.find_last_of(':') + 1).c_str());
		_writer.Key("ID");
		_writer.Uint(ECS_Component::GetComponentID<Component::TileComponent>());
		_writer.Key("r");
		_writer.Double(static_cast<double>(_component.m_Red));
		_writer.Key("g");
		_writer.Double(static_cast<double>(_component.m_Green));
		_writer.Key("b");
		_writer.Double(static_cast<double>(_component.m_Blue));
		_writer.Key("a");
		_writer.Double(static_cast<double>(_component.m_Alpha));
		_writer.EndObject();
		return true;
	}

	template <>
	inline auto ECS_Prefab::SerializeComponent<Component::ColliderComponent>(
		Writer& _writer,
		const Component::ColliderComponent& _component) const -> bool
	{
		if (!HasComponent<Component::ColliderComponent>())
			return false;
		_writer.StartObject();
		_writer.Key("Name");
		_writer.String(std::string{ typeid(_component).name() }.substr(std::string{ typeid(_component).name() }.find_last_of(':') + 1).c_str());
		_writer.Key("ID");
		_writer.Uint(ECS_Component::GetComponentID<Component::ColliderComponent>());
		_writer.Key("Size");
		_writer.StartArray();
		_writer.Double(static_cast<double>(_component.m_Size.x));
		_writer.Double(static_cast<double>(_component.m_Size.y));
		_writer.EndArray();

		_writer.Key("Tag");
		_writer.Uint(_component.m_Tag);
		_writer.EndObject();
		return true;
	}

	template <>
	inline auto ECS_Prefab::SerializeComponent<Component::PortalComponent>(
		Writer& _writer,
		const Component::PortalComponent& _component) const -> bool
	{
		if (!HasComponent<Component::PortalComponent>())
			return false;
		_writer.StartObject();
		_writer.Key("Name");
		_writer.String(std::string{ typeid(_component).name() }.substr(std::string{ typeid(_component).name() }.find_last_of(':') + 1).c_str());
		_writer.Key("ID");
		_writer.Uint(ECS_Component::GetComponentID<Component::PortalComponent>());
		_writer.Key("scene");
		_writer.Uint(_component.scene);
		_writer.Key("pet");
		_writer.Uint(_component.pet);
		_writer.EndObject();
		return true;
	}

	template <>
	inline auto ECS_Prefab::SerializeComponent<Component::IsometricTransformComponent>(
		Writer& _writer,
		const Component::IsometricTransformComponent& _component) const -> bool
	{
		if (!HasComponent<Component::IsometricTransformComponent>())
			return false;
		_writer.StartObject();
		_writer.Key("Name");
		_writer.String(std::string{ typeid(_component).name() }.substr(std::string{ typeid(_component).name() }.find_last_of(':') + 1).c_str());
		_writer.Key("ID");
		_writer.Uint(ECS_Component::GetComponentID<Component::IsometricTransformComponent>());
		_writer.Key("Scale");
		_writer.StartArray();
		_writer.Double(_component.m_Scale.x);
		_writer.Double(_component.m_Scale.y);
		_writer.EndArray();
		_writer.EndObject();
		return true;
	}

	template <>
	inline auto ECS_Prefab::SerializeComponent<Component::EmitterComponent>(
		Writer& _writer,
		const Component::EmitterComponent& _component) const -> bool
	{
		if (!HasComponent<Component::EmitterComponent>())
			return false;
		_writer.StartObject();
		_writer.Key("Name");
		_writer.String(std::string{ typeid(_component).name() }.substr(std::string{ typeid(_component).name() }.find_last_of(':') + 1).c_str());
		_writer.Key("ID");
		_writer.Uint(ECS_Component::GetComponentID<Component::EmitterComponent>());
		_writer.Key("Type");
		_writer.Uint(_component.m_Type);
		_writer.Key("Emit");
		_writer.Int(_component.m_Emit);
		_writer.EndObject();
		return true;
	}
}

#endif