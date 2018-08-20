#pragma once
enum class MessageType {
	//Server to Client
	Connection = 0x01,
	Map = 0x02,
	ActorQueue = 0x03,
	ActorQueueUpdate = 0x04,
	ProjectileQueueUpdate = 0x05,

	//Client to Server
	ControlEvent = 0x11,
	AimEvent = 0x12,
};