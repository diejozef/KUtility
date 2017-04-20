#include "KLib.hpp"

auto KLib::ChatNotification(float time, const char* text) -> void
{
	auto minutes = static_cast<int>(std::floorf(time / 60));
	auto seconds = static_cast<int>(time - minutes * 60);

	char buffer[1024];
	sprintf_s(buffer, "<font color=\"#dddddd\">[%02d:%02d] KUtility:</font> %s", minutes, seconds, text);

	GGame->PrintChat(buffer);
}

auto KLib::InRange(IUnit* Local, IUnit* Unit, float Range, bool IncludeBounds) -> bool
{
	auto distanceSqr = (Unit->ServerPosition() - Local->ServerPosition()).Length2DSqr();
	auto range = IncludeBounds ? Unit->BoundingRadius() + Range : Range;

	return (distanceSqr <= _sqr(range));
}
