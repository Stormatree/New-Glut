#pragma once

#include "Helper.hpp"

#include <GL\glew.h>

#include "Component\Transform.hpp"
#include "Component\Phong.hpp"

#include "Render\Cube.hpp"


class Mesh : public HelperComponent{
	Transform* _transform = 0;

public:
	Mesh(){

	}

	void load(){
		_transform = _getComponent<Transform>();
	}

	Component* clone(){
		return new Mesh;
	}

	void render(){
		glPushMatrix();

		// Tranformations
		glTranslatef(_transform->glPosition().x(), 0.f, 0.f);
		glTranslatef(0.f, _transform->glPosition().y(), 0.f);
		glTranslatef(0.f, 0.f, _transform->glPosition().z());

		glRotatef(_transform->euler().x(), 1.f, 0.f, 0.f);
		glRotatef(_transform->euler().y(), 0.f, 1.f, 0.f);
		glRotatef(_transform->euler().z(), 0.f, 0.f, 1.f);

		Phong* phong = _getComponent<Phong>();

		if (phong)
			phong->use();
		else
			glBindTexture(GL_TEXTURE_2D, 0);

		// Testing quad
		glBegin(GL_QUADS);

			glTexCoord2f(0.f, 0.f);
			glVertex3f(-1.0f, -1.0f, 0.0f);

			glTexCoord2f(0.f, 1.f);
			glVertex3f(-1.0f, 1.0f, 0.0f);

			glTexCoord2f(1.f, 1.f);
			glVertex3f(1.0f, 1.0f, 0.0f);

			glTexCoord2f(1.f, 0.f);
			glVertex3f(1.0f, -1.0f, 0.0f);

		glEnd();

		glPopMatrix();
	}
};