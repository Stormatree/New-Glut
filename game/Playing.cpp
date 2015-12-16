#include "Playing.hpp"

#include <Static\Renderer.hpp>

#include <Component\Box2d.hpp>
#include <Component\Camera.hpp>
#include <Component\Debug\Axis.hpp>
#include <Component\Debug\Grid.hpp>

#include "Movement.hpp"
#include "Velocity.hpp"
#include "Input.hpp"

#include <glm\vec3.hpp>

void Playing::on(){
	Entity* origin = EntityManager::createEntity("main");
	origin->addComponent(new Grid(512, 8, 8, Grid::AxisZ));
	
	Entity* model = EntityManager::createEntity("model");
	model->getComponent<Transform>()->setPosition(glm::vec3(0, 0, 0));
	model->getComponent<Transform>()->setScale(glm::vec3(2, 2, 2));
	model->getComponent<Transform>()->setRotation(glm::quat(glm::vec3(glm::radians(180.f), 0, glm::radians(180.f))));
	model->addComponent(new Model("star.obj", "star.png", "main"));
	model->getComponent<Model>()->setUnlit(true);
	
	Entity* player = EntityManager::createEntity("player");
	player->getComponent<Transform>()->setPosition(glm::vec3(-256, 0, -1024));
	player->getComponent<Transform>()->setRotation(glm::quat(glm::vec3(glm::radians(90.f), 0, 0)));
	player->addComponent(new Velocity(1.f));
	player->addComponent(new Input);
	player->addComponent(new Movement(25000.f, 0.1f));
	player->addComponent(new Camera);
	player->getComponent<Camera>()->set2d(false);
	player->getComponent<Camera>()->setFov(90);
	player->addComponent(new Box2d(32, 64));
	
	Entity* box0 = EntityManager::createEntity("box");
	box0->addComponent(new Model("cube.obj", "test.mtl", "main"));
	box0->getComponent<Model>()->setUnlit(true);
	box0->getComponent<Transform>()->setScale(glm::vec3(4, 4, 4));
	box0->getComponent<Transform>()->setPosition(glm::vec3(512, 512, -2));
	box0->getComponent<Transform>()->setRotation(glm::quat(glm::vec3(0, 0, glm::radians(10.f))));
	box0->addComponent(new Box2d(68, 68));

	player->getComponent<Transform>()->setParent(box0->getComponent<Transform>());

	EntityManager::invokeAll(Entity::TRIGGER_LOAD);
}

void Playing::off(){
	EntityManager::destroyAll();
	EntityManager::deleteDestroyed();
}