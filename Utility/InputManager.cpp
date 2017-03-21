#include "InputManager.hpp"

InputManager::InputManager()
{

}

InputManager::~InputManager()
{

}

auto InputManager::IsKeyPressed( uint8_t key ) -> bool
{
	return m_mapKeyStates[ key ];
}

auto InputManager::IsMouseButtonPressed( eMouseButton button ) -> bool
{
	if ( button >= kMouseButtonMax )
		return false;

	return m_mapMouseButtonStates[ button ];
}

auto InputManager::OnWndProc( HWND wnd, UINT message, WPARAM wparam, LPARAM lparam ) -> bool
{
	switch ( message )
	{
	case WM_KEYDOWN:
		if ( wparam > 255 || wparam < 0 )
			break;

		m_mapKeyStates[ static_cast< uint8_t >( wparam ) ] = true;
		break;

	case WM_KEYUP:
		if ( wparam > 255 || wparam < 0 )
			break;

		m_mapKeyStates[ static_cast< uint8_t >( wparam ) ] = false;
		break;
	
	case WM_LBUTTONDOWN:
		m_mapMouseButtonStates[ kLButton ] = true;
		break;

	case WM_LBUTTONUP:
		m_mapMouseButtonStates[ kLButton ] = false;
		break;

	case WM_RBUTTONDOWN:
		m_mapMouseButtonStates[ kRButton ] = true;
		break;

	case WM_RBUTTONUP:
		m_mapMouseButtonStates[ kRButton ] = false;
		break;

	case WM_MBUTTONDOWN:
		m_mapMouseButtonStates[ kMButton ] = true;
		break;

	case WM_MBUTTONUP:
		m_mapMouseButtonStates[ kMButton ] = false;
		break;

	default:
		break;
	}

	return true;
}
