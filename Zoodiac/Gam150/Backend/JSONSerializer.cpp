// ======================================================================
// File Name:		Engine_Common.h
// Project Name:	Zoodiac
// Author(s):		Low Zhi Sheng Kitson(90%), Lim Geng Yang(10%)
// Brief:			Contains Serializer functionality
// 
// All content © 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================

#include <Backend/JSONSerializer.h>
#include <Backend/ECS/ECS_IncludeComponents.h>
#include <rapidjson/document.h>
#include <fstream>
#include <Backend/ECS/ECS_Factory.h>
#include <Backend/Engine_Common.h>
#include <iostream>

namespace Backend
{
	auto JSONSerializer::ParsePrefabs() -> void
	{
		std::ifstream ifs{ "Assets/json/prefabs.json" };
		const std::string json((std::istreambuf_iterator<char>(ifs)),
		                       std::istreambuf_iterator<char>());
		rapidjson::Document document;
		ifs.close();
		document.Parse(json.c_str());
		for(const auto& i : document["Prefabs"].GetArray())
		{
			ECS::ECS_Prefab prefab;
			prefab.m_Name = i["Name"].GetString();
			for(const auto& j : i["Components"].GetArray())
			{
				prefab.AddComponent(j["ID"].GetInt(), j);
			}
			PREFAB_LIBRARY.AddResource(prefab.m_Name, new ECS::ECS_Prefab{ prefab });
		}
	}

	auto JSONSerializer::GeneratePrefabs() -> void
	{
		rapidjson::StringBuffer s;
		rapidjson::PrettyWriter writer(s);

		ECS::ECS_Prefab prefab1;
		prefab1.m_Name = "Tile";	
		prefab1.AddComponent<ECS::Component::IsometricTransformComponent>();
		auto& sprite = prefab1.AddComponent<ECS::Component::SpriteComponent>();
		sprite.m_TextureKey = "Assets/tiles/rtile.png";
		sprite.m_Layer = ECS::Component::SpriteComponent::STATIC;
		sprite.m_Red = 1;
		sprite.m_Green = 1;
		sprite.m_Blue = 1;
		sprite.m_Alpha = 1;
		auto& tile = prefab1.AddComponent<ECS::Component::TileComponent>();
		tile.m_Red = 1;
		tile.m_Green = 1;
		tile.m_Blue = 1;
		tile.m_Alpha = 1;
		auto& collider = prefab1.AddComponent<ECS::Component::ColliderComponent>();
		collider.m_Size = { 50,60 };
		collider.m_Tag = ECS::Component::ColliderComponent::TAG_TILE;

		writer.StartObject();
		writer.Key("Prefabs");
		writer.StartArray();
		GeneratePrefab(writer, prefab1);
		writer.EndArray();
		writer.EndObject();

		std::ofstream ofs{ "prefabs.json" };
		ofs << s.GetString();
		ofs.close();
	}

	auto JSONSerializer::GenerateScene() -> void
	{
		s_Writer.StartObject();
		s_Writer.Key("Name");
		s_Writer.String("Name");
		s_Writer.Key("Entities");
		s_Writer.StartArray();
	}

	auto JSONSerializer::SaveSceneEntity(const ECS::Component::PrefabComponent& _prefabComponent) -> void
	{
		s_Writer.StartObject();
		s_Writer.Key("Prefab");
		s_Writer.String(_prefabComponent.m_Key.c_str());
		s_Writer.Key("Position");
		s_Writer.StartArray();
		s_Writer.Double(_prefabComponent.m_Pos.x);
		s_Writer.Double(_prefabComponent.m_Pos.y);
		s_Writer.EndArray();
		s_Writer.Key("Height");
		s_Writer.Double(_prefabComponent.m_Height);
		s_Writer.EndObject();
	}

	auto JSONSerializer::GenerateSceneEnd() -> void
	{
		s_Writer.EndArray();
		s_Writer.EndObject();
		std::ofstream ofs{ "Assets/json/scene.json" };
		ofs << s_StringBuffer.GetString();
		ofs.close();
		s_StringBuffer.Clear();
		s_Writer.Reset(s_StringBuffer);
	}

	auto JSONSerializer::ParseScene(std::string _scene, bool _editorMode)->void
	{
		std::ifstream ifs{ _scene };
		const std::string json((std::istreambuf_iterator<char>(ifs)),
			std::istreambuf_iterator<char>());
		ifs.close();
		rapidjson::Document document;
		document.Parse(json.c_str());
		for(const auto& i : document["Entities"].GetArray())
		{
			const auto& entity = ECS::Factory::ECS_Factory::CreatePrefabEntity(i["Prefab"].GetString());
			auto& transform = ECS_WORLD.GetComponent<ECS::Component::IsometricTransformComponent>(entity);
			auto positionArray = i["Position"].GetArray();
			transform.m_Position = AEVec2{ static_cast<float>(positionArray[0].GetDouble()),
												static_cast<float>(positionArray[1].GetDouble()) };
			transform.m_Height = static_cast<float>(i["Height"].GetDouble());
			if(_editorMode)
			{
				auto& prefab = ECS_WORLD.AddComponent<ECS::Component::PrefabComponent>(entity);
				prefab.m_Key = i["Prefab"].GetString();
				prefab.m_Pos = transform.m_Position;
				prefab.m_Height = transform.m_Height;
				ECS_WORLD.RegisterEntityIntoSystems(entity);
			}
		}
	}

	auto JSONSerializer::GeneratePrefab(rapidjson::PrettyWriter<rapidjson::StringBuffer>& _writer,
	                                    const ECS::ECS_Prefab& _prefab) -> void
	{
		_prefab.Serialize(_writer);

	}

	auto JSONSerializer::SaveData(const ECS::Component::PlayerComponent::Pet_Type _lastPet) -> void
	{
		s_Writer.StartObject();
		s_Writer.Key("SaveData");
		s_Writer.Uint(_lastPet);
		s_Writer.EndObject();
		std::ofstream ofs{ "Assets/json/savedata.json" };
		ofs << s_StringBuffer.GetString();
		ofs.close();
		s_StringBuffer.Clear();
		s_Writer.Reset(s_StringBuffer);
	}

	auto JSONSerializer::LoadSaveData() -> void
	{
		std::ifstream ifs{ "Assets/json/savedata.json" };
		const std::string json((std::istreambuf_iterator<char>(ifs)),
			std::istreambuf_iterator<char>());
		ifs.close();
		rapidjson::Document document;
		document.Parse(json.c_str());
		auto& data = ECS_WORLD.GetComponent<ECS::Component::PlayerComponent>();
		data.m_CurrentPet = static_cast<ECS::Component::PlayerComponent::Pet_Type>(document["SaveData"].GetUint());
		data.m_Pet = static_cast<ECS::Component::PlayerComponent::Pet_Type>(document["SaveData"].GetUint() + 1);
	}
}
