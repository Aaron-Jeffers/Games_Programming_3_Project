#include "GameObject.h"
#include <iostream>
using namespace std;

GameObject::GameObject()
{
	active = true;
}

void GameObject::draw(Mesh* mesh)
{
	mesh->draw();
}

void GameObject::setActive(bool set)
{
	active = set;
}

void GameObject::update(Mesh* mesh)
{
	mesh->updateSphereData(*tObject.GetPos(), 0.62f);
}

void GameObject::transformPositions(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale)
{
	tObject.SetPos(pos);
	tObject.SetRot(rot);
	tObject.SetScale(scale);
}

void GameObject::moveForward(float amt)
{
	*tObject.GetPos() += forward * amt;
	
	//cout << "Forward: " << forward.x << " " << forward.y << " " << forward.z << " " << glm::length(forward) << "\n";
}

void GameObject::yaw(float yaw)
{
	*tObject.GetRot() += glm::vec3(0, 0, yaw);

	calculateForward(yaw);
}

void GameObject::calculateForward(float angle)
{
	static const glm::vec3 UP(0.0f, 0.0f, 1.0f);

	glm::mat4 rotation = glm::rotate(angle, UP);

	/**tObject.GetRot() += glm::vec3(0.0, 0.0, 0.0);*/

	forward = glm::vec3(glm::normalize(rotation * glm::vec4(forward, 0.0)));
	right = glm::vec3(glm::normalize(rotation * glm::vec4(right, 0.0)));
	up = glm::cross(forward, right);

	//cout << "Forward: " << forward.x << " " << forward.y << " " << forward.z << " " << glm::length(forward) << "\n";
	//cout << "Right: " << right.x << " " << right.y << " " << right.z << " " << glm::length(right) << "\n";
	//cout << "Up: " << up.x << " " << up.y << " " << up.z << " " << glm::length(up) <<"\n";
	//cout << "Rotation: " << tObject.GetRot()->x * rad2deg << " " << tObject.GetRot()->y * rad2deg << " " << tObject.GetRot()->z * rad2deg << "\n";
	//cout << "\n";
}

void GameObject::setForward(glm::vec3 modelPos)
{
	glm::vec3 pos = *tObject.GetPos();
	forward = glm::vec3(glm::normalize(modelPos - pos));
	glm::lookAt(pos, pos + forward, up);
}