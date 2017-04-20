#pragma once
#include <PluginSDK.h>

/* KUtility version. */

namespace KLib
{
	auto InRange(IUnit* Local, IUnit* Unit, float Range, bool IncludeBounds) -> bool;
	auto ChatNotification(float time, const char* text) -> void;
}