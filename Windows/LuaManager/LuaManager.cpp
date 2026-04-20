#include "LuaManager.h"
#include "LuaState.h"
#include "../../ThirdParty/Lua/Lua.hpp"
#include "../../ThirdParty/LuaBridge/LuaBridge.h"
#include "LuaDebug.h"
#include <filesystem>
#include <iostream>
#include <cstdlib>
#include "../Actors/Actor.h"
#include "LuaActorInterface.h"
#include "../Actors/Component.h"
#include "LuaApplicationInterface.h"
#include "../ThirdParty/glm/glm/glm.hpp"
#include "LuaInput.h"
#include "LuaText.h"
#include "LuaAudio.h"
#include "LuaImage.h"
#include "LuaCamera.h"
#include "LuaScene.h"
#include "box2d/b2_math.h"
#include "Physics/Rigidbody.h"
#include "Physics/RigidbodyConstants.h"
#include "Physics/GlobalRigidbodyConstants.h"
#include "Physics/Collision.h"
#include "Physics/HitResult.h"
#include "Physics/RayCast.h"
#include "LuaEvents.h"
#include "Particles/ParticleSystem.h"
#include "LuaManager/LuaProgress.h"

void VengefulGameEngine::LuaManager::init()
{
	luaState = luaL_newstate();
	luaL_openlibs(luaState);

	std::string resourcesFolder = RESOURCES_FOLDER;

	std::string filePath = resourcesFolder + "/component_types";

	if (std::filesystem::exists(filePath)) {
		for (const auto& templateLuaRef : std::filesystem::directory_iterator(filePath)) {

			if (templateLuaRef.is_regular_file() && templateLuaRef.path().extension() == ".lua")
			{
				if (luaL_dofile(luaState, templateLuaRef.path().string().c_str()) != LUA_OK) {
					std::cout << "problem with lua file " << templateLuaRef.path().stem().string();
					exit(0);
				}
			}
		}
	}

	luabridge::getGlobalNamespace(luaState)
		.beginNamespace("Debug")
		.addFunction("Log", &log)
		.addFunction("LogError", &log)
		.endNamespace();

	luabridge::getGlobalNamespace(luaState)
		.beginNamespace("Application")
		.addFunction("Quit", &LuaApplicationInterface::Quit)
		.addFunction("GetFrame", &LuaApplicationInterface::GetFrameNumber)
		.addFunction("Sleep", &LuaApplicationInterface::Sleep)
		.endNamespace();

	luabridge::getGlobalNamespace(luaState)
		.beginClass<Actor>("Actor")
		.addFunction("GetName", &Actor::GetName)
		.addFunction("GetID", &Actor::GetID)
		.addFunction("GetComponentByKey", &Actor::GetComponentByKey)
		.addFunction("GetComponent", &Actor::GetComponent)
		.addFunction("GetComponents", &Actor::GetComponents)
		.addStaticFunction("Find", &Find)
		.addStaticFunction("FindAll", &FindAll)
		.addStaticFunction("Instantiate", &Instantiate)
		.addStaticFunction("Destroy", &Destroy)
		.addFunction("AddComponent", &Actor::AddComponent)
		.addFunction("RemoveComponent", &Actor::RemoveComponent)
		.endClass();

	luabridge::getGlobalNamespace(luaState)
		.beginClass<glm::vec2>("vec2")
		.addProperty("x", &glm::vec2::x)
		.addProperty("y", &glm::vec2::y)
		.endClass();

	luabridge::getGlobalNamespace(luaState)
		.beginNamespace("Input")
		.addFunction("GetKey", &LuaCpp::getKey)
		.addFunction("GetKeyDown", &LuaCpp::getKeyDown)
		.addFunction("GetKeyUp", &LuaCpp::getKeyUp)
		.addFunction("GetMousePosition", &LuaCpp::getMousePosition)
		.addFunction("GetMouseButton", &LuaCpp::getMouseButton)
		.addFunction("GetMouseButtonDown", &LuaCpp::getMouseButtonDown)
		.addFunction("GetMouseButtonUp", &LuaCpp::getMouseButtonUp)
		.addFunction("GetMouseScrollDelta", &LuaCpp::getMouseScrollDelta)
		.addFunction("HideCursor", &LuaCpp::hideCursor)
		.addFunction("ShowCursor", &LuaCpp::showCursor)
		.endNamespace();

	luabridge::getGlobalNamespace(luaState)
		.beginNamespace("Text")
		.addFunction("Draw", &Draw)
		.endNamespace();

	luabridge::getGlobalNamespace(luaState)
		.beginNamespace("Audio")
		.addFunction("Play", &LuaAudio::Play)
		.addFunction("Halt", &LuaAudio::Halt)
		.addFunction("SetVolume", &LuaAudio::SetVolume)
		.endNamespace();

	luabridge::getGlobalNamespace(luaState)
		.beginNamespace("Image")
		.addFunction("DrawUI", &LuaImage::DrawUI)
		.addFunction("DrawUIEx", &LuaImage::DrawUIEx)
		.addFunction("Draw", &LuaImage::Draw)
		.addFunction("DrawEx", &LuaImage::DrawEx)
		.addFunction("DrawPixel", &LuaImage::DrawPixel)
		.endNamespace();
	
	luabridge::getGlobalNamespace(luaState)
		.beginNamespace("Camera")
		.addFunction("SetPosition", &LuaCamera::SetPosition)
		.addFunction("GetPositionX", &LuaCamera::GetPositionX)
		.addFunction("GetPositionY", &LuaCamera::GetPositionY)
		.addFunction("SetZoom", &LuaCamera::SetZoom)
		.addFunction("GetZoom", &LuaCamera::GetZoom)
		.endNamespace();

	luabridge::getGlobalNamespace(luaState)
		.beginNamespace("Scene")
		.addFunction("Load", &LuaScene::Load)
		.addFunction("GetCurrent", &LuaScene::GetCurrent)
		.addFunction("DontDestroy", &LuaScene::DontDestroy)
		.endNamespace();

	luabridge::getGlobalNamespace(luaState)
		.beginClass<b2Vec2>("Vector2")
		.addConstructor<void(*) (float, float)>()
		.addProperty("x", &b2Vec2::x)
		.addProperty("y", &b2Vec2::y)
		.addFunction("Normalize", &b2Vec2::Normalize)
		.addFunction("Length", &b2Vec2::Length)
		.addFunction("__add", &b2Vec2::operator_add)
		.addFunction("__sub", &b2Vec2::operator_sub)
		.addFunction("__mul", &b2Vec2::operator_mul)
		.addStaticFunction("Distance", &b2Distance)
		.addStaticFunction("Dot", &b2Dot_wrapped)
		.endClass();

	luabridge::getGlobalNamespace(luaState)
		.beginClass<Collision>("Collision")
		.addProperty("other", &Collision::other)
		.addProperty("point", &Collision::point)
		.addProperty("relative_velocity", &Collision::relativeVelocity)
		.addProperty("normal", &Collision::normal)
		.endClass();

	luabridge::getGlobalNamespace(luaState)
		.beginClass<HitResult>("HitResult")
		.addProperty("actor", &HitResult::actor)
		.addProperty("point", &HitResult::point)
		.addProperty("normal", &HitResult::normal)
		.addProperty("is_trigger", &HitResult::isTrigger)
		.endClass();

	luabridge::getGlobalNamespace(luaState)
		.beginNamespace("Physics")
		.addFunction("Raycast", &RayCast)
		.addFunction("RaycastAll", &RayCastAll)
		.endNamespace();

	luabridge::getGlobalNamespace(luaState)
		.beginClass<Rigidbody>("Rigidbody")
		.addConstructor<void (*) (void)>()
		.addFunction(globalRigidbodyFunctionNames.ADD_FORCE.c_str(), &Rigidbody::addForce)
		.addFunction(globalRigidbodyFunctionNames.SET_POSITION.c_str(), &Rigidbody::setPosition)
		.addFunction(globalRigidbodyFunctionNames.GET_POSITION.c_str(), &Rigidbody::getPosition)
		.addFunction(globalRigidbodyFunctionNames.SET_ROTATION.c_str(), &Rigidbody::setRotation)
		.addFunction(globalRigidbodyFunctionNames.GET_ROTATION.c_str(), &Rigidbody::getRotation)
		.addFunction(globalRigidbodyFunctionNames.SET_BODY_TYPE.c_str(), &Rigidbody::setBodyType)
		.addFunction(globalRigidbodyFunctionNames.SET_ANGULAR_VELOCITY.c_str(), &Rigidbody::setAngularVelocity)
		.addFunction(globalRigidbodyFunctionNames.GET_ANGULAR_VELOCITY.c_str(), &Rigidbody::getAngularVelocity)
		.addFunction(globalRigidbodyFunctionNames.SET_VELOCITY.c_str(), &Rigidbody::setVelocity)
		.addFunction(globalRigidbodyFunctionNames.GET_VELOCITY.c_str(), &Rigidbody::getVelocity)
		.addFunction(globalRigidbodyFunctionNames.SET_GRAVITY_SCALE.c_str(), &Rigidbody::setGravityScale)
		.addFunction(globalRigidbodyFunctionNames.GET_GRAVITY_SCALE.c_str(), &Rigidbody::getGravityScale)
		.addFunction(globalRigidbodyFunctionNames.SET_UP_DIRECTION.c_str(), &Rigidbody::setUpDirection)
		.addFunction(globalRigidbodyFunctionNames.GET_UP_DIRECTION.c_str(), &Rigidbody::getUpDirection)
		.addFunction(globalRigidbodyFunctionNames.SET_RIGHT_DIRECTION.c_str(), &Rigidbody::setRightDirection)
		.addFunction(globalRigidbodyFunctionNames.GET_RIGHT_DIRECTION.c_str(), &Rigidbody::getRightDirection)
		.addProperty(globalRigidbodyConstants.GRAVITY_SCALE.c_str(), &Rigidbody::getGravityScale, &Rigidbody::setGravityScale)
		.addProperty(globalRigidbodyConstants.BODY_TYPE.c_str(), &Rigidbody::getBodyType, &Rigidbody::setBodyType)
		.addProperty(globalRigidbodyConstants.ROTATION.c_str(), &Rigidbody::getRotation, &Rigidbody::setRotation)
		.addProperty(globalRigidbodyConstants.X.c_str(), &Rigidbody::getX, &Rigidbody::setX)
		.addProperty(globalRigidbodyConstants.Y.c_str(), &Rigidbody::getY, &Rigidbody::setY)
		.addProperty(globalRigidbodyConstants.ANGULAR_FRICTION.c_str(), &Rigidbody::getAngularFriction, &Rigidbody::setAngularFruction)
		.addProperty(globalRigidbodyConstants.DENSITY.c_str(), &Rigidbody::getDensity, &Rigidbody::setDensity)
		.addProperty(globalRigidbodyConstants.HAS_COLLIDER.c_str(), &Rigidbody::getHasCollider, &Rigidbody::setHasCollider)
		.addProperty(globalRigidbodyConstants.HAS_TRIGGER.c_str(), &Rigidbody::getHasTrigger, &Rigidbody::setHasTrigger)
		.addProperty(globalRigidbodyConstants.PRECICE.c_str(), &Rigidbody::getPrecise, &Rigidbody::setPrecise)
		.addProperty(globalRigidbodyConstants.COLLIDER_TYPE.c_str(), &Rigidbody::getColliderType, &Rigidbody::setColliderType)
		.addProperty(globalRigidbodyConstants.WIDTH.c_str(), &Rigidbody::getColliderWidth, &Rigidbody::setColliderWidth)
		.addProperty(globalRigidbodyConstants.HEIGHT.c_str(), &Rigidbody::getColliderHeight, &Rigidbody::setColliderHeight)
		.addProperty(globalRigidbodyConstants.RADIUS.c_str(), &Rigidbody::getColliderRadius, &Rigidbody::setColliderRadius)
		.addProperty(globalRigidbodyConstants.BOUNCINESS.c_str(), &Rigidbody::getBounciness, &Rigidbody::setBounciness)
		.addProperty(globalRigidbodyConstants.RIGIDBODY_ID.c_str(), &Rigidbody::rigidbodyID)
		.addProperty(globalRigidbodyConstants.PARENT_ACTOR_ID.c_str(), &Rigidbody::parentActorID)
		.addProperty(globalRigidbodyConstants.FRICTION.c_str(), &Rigidbody::getFriction, &Rigidbody::setFriction)
		.addProperty(globalRigidbodyConstants.TRIGGER_WIDTH.c_str(), &Rigidbody::getTriggerWidth, &Rigidbody::setTriggerWidth)
		.addProperty(globalRigidbodyConstants.TRIGGER_HEIGHT.c_str(), &Rigidbody::getTriggerHeight, &Rigidbody::setTriggerHeight)
		.addProperty(globalRigidbodyConstants.TRIGGER_RADIUS.c_str(), &Rigidbody::getTriggerRadius, &Rigidbody::setTriggerRadius)
		.addProperty(globalRigidbodyConstants.TRIGGER_TYPE.c_str(), &Rigidbody::getTriggerType, &Rigidbody::setTriggerType)
		.addProperty("key", &Rigidbody::key)
		.addProperty("actor",&Rigidbody::actor)
		.addProperty("enabled", &Rigidbody::enabled)
		.endClass();

	luabridge::getGlobalNamespace(luaState)
		.beginNamespace("Event")
		.addFunction("Subscribe", &Subscribe)
		.addFunction("Unsubscribe", &Unsubscribe)
		.addFunction("Publish", &Publish)
		.endNamespace();

	luabridge::getGlobalNamespace(luaState)
		.beginClass<ParticleSystem>("ParticleSystem")
		.addConstructor<void (*) (void)>()
		.addFunction("OnStart", &ParticleSystem::OnStart)
		.addFunction("OnUpdate", &ParticleSystem::OnUpdate)
		.addProperty("enabled", &ParticleSystem::enabled)
		.addProperty("uniqueComponentID", &ParticleSystem::uniqueComponentID)
		.addProperty("actor", &ParticleSystem::actor)
		.addProperty("key", &ParticleSystem::key)
		.addProperty("x", &ParticleSystem::x)
		.addProperty("y", &ParticleSystem::y)
		.addProperty("frames_between_bursts", &ParticleSystem::getFramesBetweenBursts, &ParticleSystem::setFramesBetweenBursts)
		.addProperty("burst_quantity", &ParticleSystem::getBurstQuantity, &ParticleSystem::setBurstQuantity)
		.addProperty("start_scale_min", &ParticleSystem::startScaleMin)
		.addProperty("start_scale_max", &ParticleSystem::startScaleMax)
		.addProperty("start_speed_min", &ParticleSystem::startSpeedMin)
		.addProperty("start_speed_max", &ParticleSystem::startSpeedMax)
		.addProperty("rotation_min", &ParticleSystem::rotationMin)
		.addProperty("rotation_max", &ParticleSystem::rotationMax)
		.addProperty("rotation_speed_min", &ParticleSystem::rotationSpeedMin)
		.addProperty("rotation_speed_max", &ParticleSystem::rotationSpeedMax)
		.addProperty("emit_radius_min", &ParticleSystem::emitRadiusMin)
		.addProperty("emit_radius_max", &ParticleSystem::emitRadiusMax)
		.addProperty("emit_angle_min", &ParticleSystem::emitAngleMin)
		.addProperty("emit_angle_max", &ParticleSystem::emitAngleMax)
		.addProperty("image", &ParticleSystem::image)
		.addProperty("sorting_order", &ParticleSystem::sortingOrder)
		.addProperty("start_color_r", &ParticleSystem::startColorR)
		.addProperty("start_color_g", &ParticleSystem::startColorG)
		.addProperty("start_color_b", &ParticleSystem::startColorB)
		.addProperty("start_color_a", &ParticleSystem::startColorA)
		.addProperty("duration_frames", &ParticleSystem::getDurationFrames, &ParticleSystem::setDurationFrames)
		.addProperty("gravity_scale_x", &ParticleSystem::gravityScaleX)
		.addProperty("gravity_scale_y", &ParticleSystem::gravityScaleY)
		.addProperty("drag_factor", &ParticleSystem::dragFactor)
		.addProperty("angular_drag_factor", &ParticleSystem::angularDragFactor)
		.addProperty("end_scale", &ParticleSystem::endScale)
		.addProperty("end_color_r", &ParticleSystem::endColorR)
		.addProperty("end_color_g", &ParticleSystem::endColorG)
		.addProperty("end_color_b", &ParticleSystem::endColorB)
		.addProperty("end_color_a", &ParticleSystem::endColorA)
		.addFunction("Play", &ParticleSystem::Play)
		.addFunction("Stop", &ParticleSystem::Stop)
		.addFunction("Burst", &ParticleSystem::Burst)
		.addFunction("OnUpdate", &ParticleSystem::OnUpdate)
		.endClass();

	luabridge::getGlobalNamespace(luaState)
		.beginNamespace("Progress")
		.addFunction("SetInt", &SetInt)
		.addFunction("SetFloat", &SetFloat)
		.addFunction("SetString", &SetString)
		.addFunction("SetBool", &SetBool)
		.addFunction("GetInt", &GetInt)
		.addFunction("GetFloat", &GetFloat)
		.addFunction("GetString", &GetString)
		.addFunction("GetBool", &GetBool)
		.addFunction("Save", &Save)
		.endNamespace();
}

void VengefulGameEngine::LuaManager::reportError(const std::string& actorName, const luabridge::LuaException& e)
{
	std::string errorMessage = e.what();

	std::replace(errorMessage.begin(), errorMessage.end(), '\\', '/');

	std::cout << "\033[31m" << actorName << " : " << errorMessage << "\033[0m" << std::endl;
}

void VengefulGameEngine::LuaManager::closeLua()
{
	lua_close(luaState);
}
