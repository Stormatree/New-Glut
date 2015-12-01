#include "Movement.hpp"

#include <glm\gtc\quaternion.hpp>
#include <SDL.h>
#include <Component\Camera.hpp>

Movement::Movement(float speed, bool player){
	_speed = speed;
	_player = player;
}

void Movement::load(){
	_transform = getComponent<Transform>();
	_velocity = getComponent<Velocity>();
}

void Movement::update(double dt){
	if (!_player)
		return;

	Input* input = getComponent<Input>();

	if (!input)
		return;

	_velocity = getComponent<Velocity>();

	if (input->isDown("w")){
		forward();

		if (input->isDown("shift"))
			forward();
	}

	if (input->isDown("s")){
		back();

		if (input->isDown("shift"))
			back();
	}
	
	if (input->isDown("a"))
		left();	

	if (input->isDown("d"))
		right();
}

void Movement::forward(){
	if (_velocity)
		_velocity->localPush(glm::vec3(0.f, 0.f, -_speed / 25.f));
}

void Movement::back(){
	if (_velocity)
		_velocity->localPush(glm::vec3(0.f, 0.f, _speed / 25.f));
}

void Movement::left(){
	if (_velocity)
		_velocity->localTorque(glm::quat(glm::vec3(0.f, glm::radians(_speed * 0.000001), 0.f)));
}

void Movement::right(){
	if (_velocity)
		_velocity->localTorque(glm::quat(glm::vec3(0.f, glm::radians(-_speed * 0.000001), 0.f)));
}

void Movement::setSpeed(float speed){
	_speed = speed;
}