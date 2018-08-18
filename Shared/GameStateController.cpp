#include "GameStateController.hpp"
ProgramMode GameStateController::m_mode = ProgramMode::Uninitialized;
GameState GameStateController::m_state = GameState::Uninitialized;

bool GameStateController::is_graphics_loop_ready = false;
bool GameStateController::is_physics_loop_ready = false;
bool GameStateController::is_networking_loop_ready = false;