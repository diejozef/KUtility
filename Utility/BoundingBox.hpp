#pragma once
#include <PluginSDK.h>

class BoundingBox
{
public:
	explicit BoundingBox( IMenu* parentMenu );
	~BoundingBox();

	auto OnRenderEnemy( IUnit* hero ) -> void;

private:
	IMenu* m_pMenu;
	IMenuOption* m_pEnable;
};