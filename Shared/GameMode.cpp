#include "GameMode.hpp"
GameMode GameModeController::current_game_mode = GameMode::Initialization;
bool GameModeController::physics_loop_ready = false;
bool GameModeController::graphics_loop_ready = false;

GameMode const GameModeController::getCurrentGameMode() {
	return current_game_mode;
}

void GameModeController::startInitialization() {
	current_game_mode = GameMode::Initialization;
}

void GameModeController::startGame() {
	if (physics_loop_ready && graphics_loop_ready)
		current_game_mode = GameMode::Normal;
}

void GameModeController::pauseGame() {
	if (current_game_mode == GameMode::Normal)
		current_game_mode = GameMode::Pause;
	else if (current_game_mode == GameMode::Pause)
		current_game_mode = GameMode::Normal;
}

void GameModeController::physicsLoopIsReady(bool state) {
	physics_loop_ready = state;
	startGame();
}

void GameModeController::graphicsLoopIsReady(bool state) {
	graphics_loop_ready = state;
	startGame();
}

void GameModeController::clearState() {
	physics_loop_ready = false;
	graphics_loop_ready = false;
}
