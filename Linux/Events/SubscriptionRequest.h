#ifndef SUBSCRIPTION_REQUEST_H
#define SUBSCRIPTION_REQUEST_H

namespace VengefulGameEngine {
	struct SubscriptionRequest {
		std::string eventType;
		std::shared_ptr<luabridge::LuaRef> luaTable;
		std::shared_ptr<luabridge::LuaRef> luaFunction;
	};
}

#endif
