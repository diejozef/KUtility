#pragma once
#include <PluginSDK.h>
#include <KDrawing.hpp>
#include <KLib.hpp>
#include "InputManager.hpp"

class RecallTrackerBar
{
public:
	explicit RecallTrackerBar();
	explicit RecallTrackerBar(const Vec2& pos, int width, int height);
	~RecallTrackerBar();

	auto Draw(void) -> void;
	auto SetPos(const Vec2& pos) -> void;
	auto SetWidth(int width) -> void;
	auto SetHeight(int height) -> void;

	inline auto Pos(void) const -> Vec2 {
		return m_vecPos;
	}

	inline auto Width(void) const -> int {
		return m_iWidth;
	}

	inline auto Height(void) const -> int {
		return m_iHeight;
	}

private:
	Vec2 m_vecPos;
	int m_iWidth;
	int m_iHeight;
};

class RecallTracker
{
public:
	explicit RecallTracker(IUnit* hero);
	~RecallTracker();

public:
	auto Draw(float time, bool drawHp, int index, const RecallTrackerBar& bar) const -> void;
	auto OnTeleport(float time, bool printInChat, OnTeleportArgs* data) -> void;

public:
	inline auto Recalling(void) const -> bool {
		return m_bRecalling;
	}

	inline auto Unit(void) const -> IUnit* {
		return m_pUnit;
	}

	inline auto RemainingTime(float time) const -> float {
		return m_flEndTime - time;
	}

private:
	IUnit* m_pUnit;
	bool m_bRecalling;
	bool m_bAborted;
	bool m_bFinished;
	float m_flEndTime;
	float m_flDuration;
};