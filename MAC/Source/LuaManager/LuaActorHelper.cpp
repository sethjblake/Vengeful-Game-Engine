#include "LuaActorHelper.h"
#include "Lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"
#include "Particles/ParticleSystem.h"
#include "Particles/GlobalParticleSystemsManager.h"

void VengefulGameEngine::safeCallLuaFunction(std::string& actorName, std::string& functionName, std::shared_ptr<luabridge::LuaRef> luaRef, bool ignoreIfDisabled)
{
    if (!(*luaRef)["enabled"] && ignoreIfDisabled) return;

    luabridge::LuaRef particleRef = (*luaRef)["ParticleSystem"];

    if (!particleRef.isNil()) {
        int id = particleRef["uniqueComponentID"];
        if (functionName == "OnDestroy") globalParticleSystemsManager.DestroyParticleSystem(id);
    }

    luabridge::LuaRef luaFunction = (*luaRef)[functionName.c_str()];

    if (luaFunction.isNil()) return;

    try {
        (*luaRef)[functionName.c_str()](*luaRef);
    }
    catch (const luabridge::LuaException& e) {
        std::string errorMessage = e.what();
        std::replace(errorMessage.begin(), errorMessage.end(), '\\', '/');
        std::cout << "\033[31m" << actorName << " : " << errorMessage << "\033[0m" << std::endl;
    }
}
