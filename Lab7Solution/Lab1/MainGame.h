#pragma once
#include <SDL\SDL.h>
#include <SDL/SDL_mixer.h>
#include <GL/glew.h>
#include "Display.h" 
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "transform.h"
#include "SkyBox.h"
#include "GameObject.h"
#include "SDL__Audio.h"

enum class GameState{PLAY, EXIT};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:

	void initSystems();
	void processInput();
	void gameLoop();
	void drawGame();
	void linkFogShader();
	void linkToon();
	void linkRimLighting();
	void linkGeo();
	void linkEmapping();

	void drawSkyBox();
	void drawAsteroids();
	void drawShip();
	void drawMissiles(); 

	void fireMissiles(int i);
	
	void updateDelta();
	void moveCamera();

	void createScreenQuad();

	void generateFBO(float w, float h);
	void bindFBO();
	void unbindFBO();
	void renderFBO();
	
	void initModels(GameObject*& asteroid);
	void setCameraTarget();
	bool collision(glm::vec3 m1Pos, float m1Rad, glm::vec3 m2Pos, float m2Rad);

	//void playAudio(unsigned int Source, glm::vec3 pos);

	Display _gameDisplay;
	GameState _gameState;

	Mesh asteroidMesh;
	Mesh shipMesh;
	Mesh missileMesh;
	Mesh monkeyMesh;

	Transform transform;
	Transform transform1;

	GameObject asteroid1;
	GameObject asteroid2;
	GameObject* asteroid = new GameObject[numberOfAsteroids];
	GameObject* missiles = new GameObject[numberOfMissiles];
	GameObject ship;

	Texture texture;
	Texture texture1;

	Camera myCamera;

	Shader fogShader;
	Shader toonShader;
	Shader rimShader;
	Shader geoShader;
	Shader shaderSkybox;
	Shader eMapping;
	Shader FBOShader;

	SDL__Audio gameAudio;
	SDL__Audio bangAudio;

	GLuint FBO;
	GLuint RBO;
	GLuint CBO;

	GLuint quadVAO;
	GLuint quadVBO;

	glm::vec3 currentCamPos;

	Skybox skybox;

	Uint64 NOW = SDL_GetPerformanceCounter(), LAST = 0;

	int numberOfAsteroids = 50;
	int numberOfMissiles = 20;
	int missileLaunchNumber = 0;
	

	float deltaTime = 0; 
	float deltaTimeConversion = 1;
	float deltaTimeDisplay = 0;
	float counter;

	float missileVelocity = 2;
	float missileSpeed;
	float asteroidVelocity = 15;
	float asteroidSpeed;
	float shipVelocity =20;
	float shipSpeed;
	float shipRotationVelocity = 3;
	float shipRotationSpeed;

	int cameraType = 0;
	int asteroidCamTarget = 0;
	float cameraVelocity = 15;
	float cameraSpeed;
	float cameraRotationVelocity = 20;
	float cameraRotationSpeed;
	float cameraOffset = 10;
	float theta = 0;

	float randLow = -1;
	float randHigh = 1;

	float asteroidScale = 0.05;
	float shipScale = 0.5;
	float missileScale = 0.1;

	string deltaTimeString = "s";
	string controls;

	//Audio audioDevice;

	bool look = true;
	bool shake = false;
	bool deltaInSeconds = true;

	unsigned int whistle;
	unsigned int backGroundMusic;

	vector<std::string> skyboxGalaxy
	{"..\\res\\skybox\\right.png","..\\res\\skybox\\left.png","..\\res\\skybox\\top.png","..\\res\\skybox\\bottom.png","..\\res\\skybox\\front.png","..\\res\\skybox\\back.png" };
	vector<std::string> skyboxDefault
	{"..\\res\\skybox\\right1.jpg","..\\res\\skybox\\left1.jpg","..\\res\\skybox\\top1.jpg","..\\res\\skybox\\bottom1.jpg","..\\res\\skybox\\front1.jpg","..\\res\\skybox\\back1.jpg" };
};


