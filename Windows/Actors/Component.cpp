//#include "Component.h"
//#include <cstdlib>
//#include <string>
//#include <algorithm>
//
//void VengefulGameEngine::Component::OnDestroy()
//{
//    luabridge::LuaRef onDestroyFunction = (*luaRef)["OnDestroy"];
//
//    if (onDestroyFunction.isNil()) {
//        return;
//    }
//
//    try {
//        (*luaRef)["OnDestroy"](*luaRef);
//    }
//    catch (const luabridge::LuaException& e) {
//        std::string errorMessage = e.what();
//        std::replace(errorMessage.begin(), errorMessage.end(), '\\', '/');
//        std::cout << "\033[31m" << name << " : " << errorMessage << "\033[0m" << std::endl;
//    }
//}
//}
