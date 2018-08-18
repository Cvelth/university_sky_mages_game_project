#pragma once
enum class ProgramMode {
	Uninitialized = 0, Server = 1, Client = 2
};
enum class GameState {
	Uninitialized = 0, Initialization = 1, Normal = 2, Pause = 3
};
#include "AbstractException.hpp"
DefineNewException(GameStateException);

class GameStateController {
private:
	static ProgramMode m_mode;
	static GameState m_state;

	static bool is_graphics_loop_ready;
	static bool is_physics_loop_ready;
	static bool is_networking_loop_ready;
protected:
	static void change_state(bool &old_state, bool new_state) {
		if (m_mode == ProgramMode::Uninitialized || m_state == GameState::Uninitialized)
			throw Exceptions::GameStateException("GameState wasn't properly initialized.");
		old_state = new_state;
		start();
	}
public:
	static ProgramMode const mode() { return m_mode; }
	static GameState const state() { return m_state; }
	static void initialize(ProgramMode mode) { 
		if (m_mode != ProgramMode::Uninitialized || m_state != GameState::Uninitialized)
			throw Exceptions::GameStateException("Program mode can only be defined once.");
		m_mode = mode; m_state = GameState::Initialization;
		is_graphics_loop_ready = false; is_physics_loop_ready = false; is_networking_loop_ready = false;
	}
	static void initialization() {
		if (m_mode == ProgramMode::Uninitialized)
			throw Exceptions::GameStateException("ProgramMode has to be defined. Please use another initialization method."); 
		m_state = GameState::Initialization;
	}
	static void start() {
		if (m_mode == ProgramMode::Uninitialized)
			throw Exceptions::GameStateException("Game cannot be started if program mode wasn't initialized.");
		if (is_networking_loop_ready && is_physics_loop_ready && (m_mode == ProgramMode::Server || (m_mode == ProgramMode::Client && is_graphics_loop_ready)))
			m_state = GameState::Normal;
		else if (m_state == GameState::Normal)
			m_state = GameState::Initialization;
	}
	static void pause() {
		if (m_state == GameState::Normal)
			m_state = GameState::Pause;
		else if (m_state == GameState::Pause)
			start();
	}

	static void change_physics_loop_state(bool state) { change_state(is_physics_loop_ready, state); }
	static void change_networking_loop_state(bool state) { change_state(is_networking_loop_ready, state); }
	static void change_graphics_loop_state(bool state) { 
		if (m_mode == ProgramMode::Server)
			throw Exceptions::GameStateException("Graphics loop cannot be started on ServerMode application.");
		change_state(is_graphics_loop_ready, state); 
	}

	static bool is_server() { return m_mode == ProgramMode::Server; }
	static bool is_client() { return m_mode == ProgramMode::Client; }
};