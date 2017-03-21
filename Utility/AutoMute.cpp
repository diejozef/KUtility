#include "AutoMute.hpp"

AutoMute::AutoMute( IMenu* parentMenu )
{
	m_pMenu = parentMenu->AddMenu( "Auto Mute" );
	m_pAutoMuteAllies = m_pMenu->CheckBox( "Auto Mute Allies", false );
	m_pAutoMuteEnemies = m_pMenu->CheckBox( "Auto Mute Enemies", false );

	if ( m_pAutoMuteAllies->Enabled() && m_pAutoMuteEnemies->Enabled() )
		GGame->Say( "/mute all" );
	else if ( m_pAutoMuteEnemies->Enabled() )
		GGame->Say( "/mute enemy" );
	else if ( m_pAutoMuteAllies->Enabled() )
		GGame->Say( "/mute team" );
}

AutoMute::~AutoMute()
{
	m_pMenu->Remove();
}
