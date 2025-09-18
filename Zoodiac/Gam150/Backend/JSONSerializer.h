// ======================================================================
// File Name:		JSONSerializer.h
// Project Name:	Zoodiac
// Author(s):		Low Zhi Sheng Kitson(90%), Lim Geng Yang(10%)
// Brief:			Contains Serializer functionality
// 
// All content © 2023 DigiPen Institute of Technology Singapore. All 
// rights reserved.
// ======================================================================

#ifndef JSON_SERIALIZER_H_
#define JSON_SERIALIZER_H_

#include <string>

#include <Backend/ECS/ECS_Prefab.h>
#include <Backend/ECS/Components/PrefabComponent.h>

#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

namespace Backend
{
	class JSONSerializer
	{
	public:
		inline static rapidjson::StringBuffer s_StringBuffer{};
		inline static rapidjson::Writer<rapidjson::StringBuffer> s_Writer{ s_StringBuffer };

		/**
		* \brief	Parses prefab.
		*/
		static auto ParsePrefabs() -> void;

		/**
		* \brief	Generates prefab.
		*/
		static auto GeneratePrefabs() -> void;

		/**
		* \brief	Starts generate scene json file.
		*/
		static auto GenerateScene() -> void;

		/**
		* \brief	Saves scene to json.
		*/
		static auto SaveSceneEntity(const ECS::Component::PrefabComponent& _prefabComponent) -> void;

		/**
		* \brief	Ends generate scene json file.
		*/
		static auto GenerateSceneEnd() -> void;

		/**
		* \brief				Parses scene.
		* \param _scene			Scene to parse.
		* \param _editorMode	Parses scene for level editor if true.
		*/
		static auto ParseScene(std::string _scene, bool _editorMode = false)->void;

		/**
		* \brief				Saves data.
		* \param _lastPet		Last pet unlocked.
		*/
		static auto SaveData(const ECS::Component::PlayerComponent::Pet_Type _lastPet) -> void;

		/**
		* \brief	Loads save data.
		*/
		static auto LoadSaveData() -> void;
	private:
		/**
		* \brief			Generates prefab.
		* \param _writer	Writer.
		* \param _prefab	Prefab to generate.
		*/
		static auto GeneratePrefab(rapidjson::PrettyWriter<rapidjson::StringBuffer>& _writer, const ECS::ECS_Prefab& _prefab) -> void;
	};
}

#endif