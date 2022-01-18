#include "MainGame.h"
#include "Camera.h"
#include <iostream>
#include <string>

MainGame::MainGame()
{
	_gameState = GameState::PLAY;
	Display* _gameDisplay = new Display(); //new display
	//Audio* audioDevice();
}

MainGame::~MainGame()
{
}

void MainGame::run()
{
	initSystems(); 
	gameLoop();
}

void MainGame::initSystems()
{
	_gameDisplay.initDisplay(); 
	//whistle = audioDevice.loadSound("..\\res\\bang.wav");
	//backGroundMusic = audioDevice.loadSound("..\\res\\background.wav");
	texture.load("..\\res\\bricks.jpg");
	texture1.load("..\\res\\water.jpg");
	asteroidMesh.loadModel("..\\res\\Rock1.obj");
	shipMesh.loadModel("..\\res\\ship.obj");
	missileMesh.loadModel("..\\res\\bomb.obj");
	monkeyMesh.loadModel("..\\res\\monkey3.obj");
	fogShader.init("..\\res\\fogShader.vert", "..\\res\\fogShader.frag"); //new shader
	toonShader.init("..\\res\\shaderToon.vert", "..\\res\\shaderToon.frag"); //new shader
	rimShader.init("..\\res\\shaderRim.vert", "..\\res\\shaderRim.frag");
	eMapping.init("..\\res\\shaderReflection.vert", "..\\res\\shaderReflection.frag");
	FBOShader.init("..\\res\\FBOShader.vert", "..\\res\\FBOShader.frag");

	gameAudio.addAudioTrack("..\\res\\background.wav");
	bangAudio.addSoundEffect("..\\res\\bang.wav");

	initModels(asteroid);

	geoShader.initGeo();

	myCamera.initCamera(glm::vec3(0, 0, -25), 70.0f, (float)_gameDisplay.getWidth()/_gameDisplay.getHeight(), 0.01f, 2000.0f);

	generateFBO(_gameDisplay.getWidth(), _gameDisplay.getHeight());

	createScreenQuad();

	counter = 1.0f;
	 
	skybox.init(skyboxGalaxy);
}

void MainGame::createScreenQuad()
{
	float quadVertices[] = {
		// vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		/*-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f*/

		// vertex attributes for a quad that fills the half of the screen
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f,  0.25f,  0.0f, 0.0f,
		-0.25f,  0.25f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		-0.25f,  0.25f,  1.0f, 0.0f,
		-0.25f,  1.0f,  1.0f, 1.0f
	};
	// cube VAO
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

}

void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		gameAudio.playMusic();
		processInput();
		currentCamPos = myCamera.getPos();
		drawGame();
		updateDelta();
		collision(asteroidMesh.getSpherePos(), asteroidMesh.getSphereRadius(), shipMesh.getSpherePos(), shipMesh.getSphereRadius());
		setCameraTarget();
		//playAudio(backGroundMusic, glm::vec3(0.0f,0.0f,0.0f));
		//cout << "Delta Time: " << deltaTimeDisplay << deltaTimeString << "\n";
	}
}

void MainGame::processInput()
{
	const Uint8* keys = SDL_GetKeyboardState(NULL);

	SDL_Event evnt;

	while (SDL_PollEvent(&evnt))
	{
		if (evnt.type == SDL_KEYDOWN )
		{
			/////////Camera Controls///////////
			//Move
			if (keys[SDL_SCANCODE_UP])
			{
			}
			if (keys[SDL_SCANCODE_DOWN])
			{
			}
			if (keys[SDL_SCANCODE_RIGHT])
			{
			}
			if (keys[SDL_SCANCODE_LEFT])
			{
			}
			//Rotate Around
			if (keys[SDL_SCANCODE_I])
			{

			}
			if (keys[SDL_SCANCODE_J])
			{
				theta -= cameraRotationSpeed;
			}
			if (keys[SDL_SCANCODE_K])
			{
				
			}
			if (keys[SDL_SCANCODE_L])
			{
				theta += cameraRotationSpeed;
			}
			//Roll
			if (keys[SDL_SCANCODE_U])
			{

			}
			if (keys[SDL_SCANCODE_O])
			{

			}
			//Change camera
			if (keys[SDL_SCANCODE_C])
			{
				cameraType = 1;
			}
			if (keys[SDL_SCANCODE_F])
			{
				cameraType = 2;
			}
			if (keys[SDL_SCANCODE_T])
			{
				cameraType = 3;
			}
			if (keys[SDL_SCANCODE_Q])
			{
				cameraType = 4;
			}
			if (keys[SDL_SCANCODE_E])
			{
				cameraType = 5;
			}
			//Cycle Target
			if (keys[SDL_SCANCODE_X])
			{			
				cout << asteroidCamTarget << "\n";
				asteroidCamTarget += 1;
			}
			if (keys[SDL_SCANCODE_Z])
			{
				cout << asteroidCamTarget << "\n";
				asteroidCamTarget -= 1;
			}

			/////////Ship Controls///////////
			//Forward/Back
			if (keys[SDL_SCANCODE_W])
			{
				ship.moveForward(shipSpeed);
			}
			if (keys[SDL_SCANCODE_S])
			{
				ship.moveForward(-shipSpeed);
			}
			//Yaw
			if (keys[SDL_SCANCODE_A])
			{
				ship.yaw(-shipRotationSpeed);
				//myCamera.Pitch(-shipSpeed);
				//cout << *&ship.getTM().GetRot()->z; ////CLue here
			}
			if (keys[SDL_SCANCODE_D])
			{
				ship.yaw(shipRotationSpeed);
			}
			if (keys[SDL_SCANCODE_SPACE])
			{
				missiles[missileLaunchNumber].transformPositions(*ship.getTM().GetPos() + (ship.rght() * 1.0f), *ship.getTM().GetRot(), glm::vec3(0.05, 0.05, 0.05));
				missiles[missileLaunchNumber].setActive(1);
				missileLaunchNumber++;
			}

			/////////Miscellaneous///////////
			//Cycle delta time denomination
			if (keys[SDL_SCANCODE_TAB])
			{
				deltaInSeconds = !deltaInSeconds;
			}
			//Shake
			if (keys[SDL_SCANCODE_BACKSPACE])
			{
				shake = !shake;
			}
		}

		//Camera Zoom
		if (evnt.type == SDL_MOUSEWHEEL)
		{			
			myCamera.MoveForward(evnt.wheel.y);
		}
	}
}

void MainGame::initModels(GameObject*& asteroid)
{
	for (int i = 0; i < numberOfAsteroids; ++i)
	{
		float rX = randLow + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (randHigh - randLow)));
		float rY = randLow + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (randHigh - randLow)));
		float rZ = randLow + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (randHigh - randLow)));

		asteroid[i].transformPositions(glm::vec3(0.4 * i * rX, 0.4 * i * rY, 0.4 * i * rZ), glm::vec3(rX, rY, rZ), glm::vec3(asteroidScale, asteroidScale, asteroidScale));
		asteroid[i].update(&asteroidMesh);
	}

	ship.transformPositions(glm::vec3(0.0, 0.0, -3.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(shipScale, shipScale, shipScale));

	for (int i = 0; i < numberOfMissiles; i++)
	{
		missiles[i].setActive(0);
	}
}

void MainGame::drawAsteroids()
{
	texture.Bind(0);
	eMapping.Bind();
	linkEmapping();

	glActiveTexture(GL_TEXTURE0 + 0); // Texture unit 0
	glBindTexture(GL_TEXTURE_2D, texture.getID());

	glActiveTexture(GL_TEXTURE0 + 1); // Texture unit 1
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.getID());

	for (int i = 0; i < numberOfAsteroids; ++i)
	{
		if (asteroid[i].getActive())
		{
			asteroid[i].transformPositions(glm::vec3(*asteroid[i].getTM().GetPos()), glm::vec3(asteroid[i].getTM().GetRot()->x + deltaTime, asteroid[i].getTM().GetRot()->y + deltaTime, asteroid[i].getTM().GetRot()->z + deltaTime), glm::vec3(asteroidScale, asteroidScale, asteroidScale));
			asteroid[i].draw(&asteroidMesh);
			asteroid[i].update(&asteroidMesh);
			eMapping.Update(asteroid[i].getTM(), myCamera);
		}
	}
}

void MainGame::drawMissiles()
{
	toonShader.Bind();
	linkToon();

	for (int i = 0; i < numberOfMissiles; i++)
	{
		if (missiles[i].getActive())
		{
			missiles[i].draw(&missileMesh);
			missiles[i].update(&missileMesh);
			toonShader.Update(missiles[i].getTM(), myCamera);
			fireMissiles(i);
		}
	}
}

void MainGame::fireMissiles(int i)
{
	/** CALL THIS FROM processInput()
	* Set the missle transform to the ship transform ONCE (initial conditions)
	* Set the missle to active (check it is not already active)
	* Update the tranform to move the missle along its forward vector(more advanced, use seek to target)
	* check for asteroid collision
	* handle asteroid collision
	*/
	float deg2rad = GameObject().deg2rad;
	float rotAngle = 360.0f * deg2rad;

	glm::vec3 missilePosition = glm::vec3(*missiles[i].getTM().GetPos());
	glm::vec3 missileRotation = glm::vec3(*missiles[i].getTM().GetRot());

	glm::vec3 asteroidPosition = *asteroid[i].getTM().GetPos();

	glm::vec3 targetVector = asteroidPosition - missilePosition;
	glm::vec3 targetVelocity = glm::normalize(targetVector) * missileSpeed;

	glm::vec3 norm = glm::normalize(targetVector);
	glm::vec3 rot = glm::vec3(rotAngle, rotAngle, rotAngle) * norm;

	missiles[i].transformPositions(missilePosition + targetVelocity, rot, (glm::vec3(*missiles[i].getTM().GetScale())));


	if (collision(missilePosition, 0.2 , asteroidPosition, 0.2))
	{
		missiles[i].setActive(0);
		asteroid[i].setActive(0);
		bangAudio.playSoundEffect(0);
	}
}

void MainGame::drawShip()
{
	toonShader.Bind();
	linkToon();

	ship.draw(&shipMesh);
	ship.update(&shipMesh);
	toonShader.Update(ship.getTM(), myCamera);
}

void MainGame::drawSkyBox()
{
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.textureID);

	counter = counter + 0.02f;

	skybox.draw(&myCamera);

	//myCamera.setPos(currentCamPos);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnd();
}

void MainGame::setCameraTarget()
{
	glm::vec3 forwardTemp = ship.fwd();
	glm::vec3 upTemp = ship.uP();
	glm::vec3 rightTemp = ship.rght();
	glm::vec3 shipPos = *ship.getTM().GetPos();

	switch (cameraType)
	{
	case 0:
		return;
		break;
	case 1:
	{	
		float distance = glm::distance(shipPos, myCamera.getPos());

		glm::vec3 posRel2Ship = glm::vec3(cameraOffset * sin(theta), -cameraOffset*cos(theta), 0.0f);

		myCamera.setLook(shipPos + (2.0f * forwardTemp));
		myCamera.setPos(shipPos + posRel2Ship);
	}
		break;
	case 2:
	{
		myCamera.setForward(forwardTemp);
		myCamera.setPos(shipPos + (forwardTemp * cameraOffset));
	}
		break;
	case 3:
	{
		myCamera.setForward(forwardTemp);
		myCamera.setPos(shipPos - (forwardTemp * cameraOffset) + (upTemp * cameraOffset / 5.0f));
	}
		break;
	case 4:
	{
		if (asteroidCamTarget < 0)
		{
			asteroidCamTarget = numberOfAsteroids - 1;
		}
		if (asteroidCamTarget > numberOfAsteroids - 1)
		{
			asteroidCamTarget = 0;
		}
		myCamera.setLook(*asteroid[asteroidCamTarget].getTM().GetPos());
	}
	break;
	case 5:
	{
		myCamera.setPos(*missiles[missileLaunchNumber - 1].getTM().GetPos() + (forwardTemp * cameraOffset) - (upTemp * cameraOffset / 5.0f));
		myCamera.setLook(*missiles[missileLaunchNumber - 1].getTM().GetPos());
	}
	break;
	}

	/*myCamera.setLook(*asteroid[cameraTarget].getTM().GetPos());
	myCamera.setPos(currentCamPos);*/
}

bool MainGame::collision(glm::vec3 m1Pos, float m1Rad, glm::vec3 m2Pos, float m2Rad)
{
	float distance = glm::sqrt((m2Pos.x - m1Pos.x) * (m2Pos.x - m1Pos.x) + (m2Pos.y - m1Pos.y) * (m2Pos.y - m1Pos.y) + (m2Pos.z - m1Pos.z) * (m2Pos.z - m1Pos.z));

	if (distance < (m1Rad + m2Rad))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void MainGame::linkFogShader()
{
	//fogShader.setMat4("u_pm", myCamera.getProjection());
	//fogShader.setMat4("u_vm", myCamera.getProjection());
	fogShader.setFloat("maxDist", 20.0f);
	fogShader.setFloat("minDist", 0.0f);
	fogShader.setVec3("fogColor", glm::vec3(0.0f, 0.0f, 0.0f));
}

void MainGame::linkToon()
{
	toonShader.setVec3("lightDir", glm::vec3(0.5f, 0.5f, 0.5f));
}

void MainGame::linkGeo()
{
	float randX = ((float)rand() / (RAND_MAX));
	float randY = ((float)rand() / (RAND_MAX));
	float randZ = ((float)rand() / (RAND_MAX));
	// Frag: uniform float randColourX; uniform float randColourY; uniform float randColourZ;
	geoShader.setFloat("randColourX", randX);
	geoShader.setFloat("randColourY", randY);
	geoShader.setFloat("randColourZ", randZ);
	// Geom: uniform float time;
	geoShader.setFloat("time", counter);
}

void MainGame::linkRimLighting()
{
	glm::vec3 camDir;
	camDir = monkeyMesh.getSpherePos() - myCamera.getPos();
	camDir = glm::normalize(camDir);
	rimShader.setMat4("u_pm", myCamera.getProjection());
	rimShader.setMat4("u_vm", myCamera.getView());
	rimShader.setMat4("model", transform.GetModel());
	rimShader.setMat4("view", myCamera.getView());
	rimShader.setVec3("lightDir", glm::vec3(0.5f, 0.5f, 0.5f));
}

void MainGame::linkEmapping()
{
	eMapping.setMat4("model", asteroid[0].getModel());
	//eMapping.setVec3("cameraPos", myCamera.getPos());
}

void MainGame::updateDelta()
{
	if (deltaInSeconds)
	{
		deltaTimeString = "s";
		deltaTimeConversion = 1;
	}
	else if (!deltaInSeconds)
	{
		deltaTimeString = "ms";
		deltaTimeConversion = 1000;
	}
	LAST = NOW;
	NOW = SDL_GetPerformanceCounter();

	deltaTime = (float)((NOW - LAST) / (float)SDL_GetPerformanceFrequency());

	deltaTimeDisplay = deltaTime * deltaTimeConversion;

	cameraSpeed = cameraVelocity * deltaTime;
	cameraRotationSpeed = cameraRotationVelocity * deltaTime;

	asteroidSpeed = asteroidVelocity * deltaTime;

	shipSpeed = shipVelocity * deltaTime;
	shipRotationSpeed = shipRotationVelocity * deltaTime;

	missileSpeed = missileVelocity * deltaTime;
}

void MainGame::bindFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void MainGame::unbindFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void MainGame::generateFBO(float w, float h)
{
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// create a colorbuffer for attachment texture
	glGenTextures(1, &CBO);
	glBindTexture(GL_TEXTURE_2D, CBO);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, CBO, 0);

	// create a renderbuffer object for depth and stencil attachment 
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h); // use a single renderbuffer object for both a depth AND stencil buffer.
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO); // now actually attach it


// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
		cout << "FRAMEBUFFER:: Framebuffer is complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void MainGame::renderFBO()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
	glClear(GL_COLOR_BUFFER_BIT);

	FBOShader.Bind();
	if (shake)
		FBOShader.setFloat("time", counter);
	else
		FBOShader.setFloat("time", 1);
	glBindVertexArray(quadVAO);
	glBindTexture(GL_TEXTURE_2D, CBO);	// use the color attachment texture as the texture of the quad plane
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void MainGame::drawGame()
{
	_gameDisplay.clearDisplay(0.8f, 0.8f, 0.8f, 1.0f); //sets our background colour
	
	bindFBO();

	drawAsteroids();
	drawSkyBox();
	drawShip();
	drawMissiles();

	unbindFBO();

	//renderFBO();      //Just not rendering

	//glEnable(GL_DEPTH_TEST);

	drawAsteroids();
	drawShip();
	drawSkyBox();
	drawMissiles();

	_gameDisplay.swapBuffer();	
} 