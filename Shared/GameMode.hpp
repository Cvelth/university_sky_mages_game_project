#pragma once
enum class GameMode {
	Initialization, Normal, Pause
};

class GameModeController {
private:
	static GameMode current_game_mode;
	static bool physics_loop_ready;
	static bool graphics_loop_ready;
public:
	static GameMode const getCurrentGameMode();

	static void startInitialization();
	static void startGame();
	static void pauseGame();
	static void physicsLoopIsReady(bool state = true);
	static void graphicsLoopIsReady(bool state = true);
	static void clearState();
};