#include <vector>
#include <GL/glew.h>
#include "GeKo_Graphics/ParticleSystem/ParticleSystem.h"
#include <GeKo_Graphics/InputInclude.h>
#include <GeKo_Graphics/ScenegraphInclude.h>
#include <GeKo_Graphics/ShaderInclude.h>

static InputHandler iH;
static Pilotview cam("Pilotview");

static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos){
	if (iH.getActiveInputMap()->getType() == MapType::CAMPLAYERVIEW){
		cam.turn(xpos, ypos);
	}
	if (iH.getActiveInputMap()->getType() == MapType::CAMSTRATEGY){
		if (glfwGetMouseButton(window, 0) == GLFW_PRESS){
			cam.turn(xpos, ypos);
		}
		else{
			cam.updateCursor(window);
		}
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
	iH.getActiveInputMap()->checkKeys(key, *window);
}

int main()
{
	glfwInit();

	////our window
	//GLFWwindow* window;
	//window = glfwCreateWindow(800, 600, "ParticleSystemXML", NULL, NULL);
	//glfwMakeContextCurrent(window);
	//
	////CAM
	//cam.setKeySpeed(4.0);
	//iH.setAllInputMaps(cam);
	//glfwSetKeyCallback(window, key_callback);
	//cam.setFOV(50);
	//cam.setNearFar(1, 100);

	Window testWindow(500, 50, 800, 600, "ParticleSystem");
	glfwMakeContextCurrent(testWindow.getWindow());

	// Callback
	glfwSetKeyCallback(testWindow.getWindow(), key_callback);

	cam.setKeySpeed(4.0);
	cam.setNearFar(0.1, 100);
	cam.setPosition(glm::vec4(0, 0, 8, 0));

	glewInit();

	//our renderer
	OpenGL3Context context;
	Renderer *renderer;
	renderer = new Renderer(context);


	//////////////////////Textures//////////////////////

	Texture* fireTex = new Texture((char*)RESOURCES_PATH "/ParticleSystem/fire/Fire2_M.png");
	Texture* fireTex1 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/fire/fire1_M.png");
	Texture* fireTex2 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/fire/fire3_M.png");
	Texture* fireTex3 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/fire/flame02_L.png");
	//Texture* fireFlickering1 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/fire/fire_flickering_1.png");
	//Texture* fireFlickering2 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/fire/fire_flickering_2.png");
	//Texture* fireFlickering3 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/fire/fire_flickering_3.png");
	//Texture* fireFlickering4 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/fire/fire_flickering_4.png");
	Texture* fireSparkleTex1 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/fire/fireSparkle1_S.png");
	Texture* fireSparkleTex2 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/fire/fireSparkle2.png");

	Texture* texFireworkBlue = new Texture((char*)RESOURCES_PATH "/ParticleSystem/firework/firework_blue.png");
	Texture* texFireworkRed = new Texture((char*)RESOURCES_PATH "/ParticleSystem/firework/firework_red.png");
	Texture* texFireworkGreen = new Texture((char*)RESOURCES_PATH "/ParticleSystem/firework/firework_green.png");
	Texture* texFireworkGold = new Texture((char*)RESOURCES_PATH "/ParticleSystem/firework/firework_gold.png");
	Texture* texFireworkTail = new Texture((char*)RESOURCES_PATH "/ParticleSystem/firework/firework_tail.png");

	Texture* smokeWhiteTex1 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/smoke/smokeWhite/smokeWhite01.png");
	Texture* smokeWhiteTex2 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/smoke/smokeWhite/smokeWhite02.png");
	Texture* smokeBlack1 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/smoke/smokeBlack/smokeBlack01.png");
	Texture* smokeBlack2 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/smoke/smokeBlack/smokeBlack02.png");

	//////////////////////Emitter//////////////////////
	
	//Emitter explosion sparkle
	Emitter* explosionSparkle = new Emitter(0, glm::vec3(0.0, 0.0, 0.0), 0.25, 0.01, 80, 1.25, true);
	explosionSparkle->setVelocity(5);
	//explosionSparkle->usePhysicDirectionGravity(glm::vec4(0.0, -1.0, 0.0, 0.3), 3.0f);
	explosionSparkle->usePhysicPointGravity(glm::vec3(0.0, 0.0, 0.0), 0.6, 10.0, 2, 4.0f, true);
	explosionSparkle->addTexture(fireSparkleTex1, 1.0);
	explosionSparkle->defineLook(true, 0.01, 0.0, 0.1);
	
	//Emitter explosion fire
	//TODO

	//Emitter fire smoke
	Emitter* fire = new Emitter(0, glm::vec3(0.0, 0.0, 0.0), 0.0, 0.2, 2, 5.0, true);
	fire->setVelocity(5);
	fire->usePhysicDirectionGravity(glm::vec4(0.0, 1.0, 0.0, 5.0), 0.5f);
	fire->addTexture(fireTex1, 1.0);
	fire->addTexture(fireTex2, 0.7);
	fire->addTexture(smokeBlack2, 0.1);
	//fire->addTexture(smokeBlack2, 0.1);
	//fire->addTexture(smokeWhiteTex2, 0.25);
	std::vector<float> sizeF{ 0.05f, 0.5f, 0.75f, 1.0f };
	std::vector<float> timeF{ 0.0f, 0.4f, 0.75f, 1.0f };
	fire->defineLook(true, sizeF, timeF, 0.5, 4.0, 3.0, true, 0.3);
	fire->switchToGeometryShader();

	//Emitter fire flickering
	Emitter* fireFlickering = new Emitter(0, glm::vec3(0.0, 0.1, 0.0), 0.0, 0.2, 1, 2.0, true);
	fireFlickering->setVelocity(5);
	fireFlickering->usePhysicDirectionGravity(glm::vec4(0.0, 1.0, 0.0, 0.5), 0.3f);
	/*fireFlickering->addTexture(fireFlickering1, 1.0);
	fireFlickering->addTexture(fireFlickering2, 0.7);
	fireFlickering->addTexture(fireFlickering3, 0.5);
	fireFlickering->addTexture(fireFlickering4, 0.3);*/
	fireFlickering->defineLook(true, 0.1, 0.5, 1.0, 0.5, true, 0.3);
	fireFlickering->switchToGeometryShader();

	Emitter* fireSparkle = new Emitter(0, glm::vec3(0.0, 0.1, 0.0), 0.0, 0.05, 3, 2.5, true);
	fireSparkle->setVelocity(5);
	fireSparkle->usePhysicDirectionGravity(glm::vec4(0.0, 1.0, 0.0, 0.8), 0.5f);
	fireSparkle->addTexture(fireSparkleTex1, 1.0);
	fireSparkle->defineLook(true, 0.05, 0.5, 0.5);
	
	
	//Emitter firework explosion
	Emitter* fireworkExplosion = new Emitter(0, glm::vec3(0.0, 0.0, 0.0), 0.1, 0.01, 80, 2.0, true);
	fireworkExplosion->setVelocity(6);
	//fireworkExplosion->usePhysicDirectionGravity(glm::vec4(0.0, -1.0, 0.0, 0.6), 3.0f);
	fireworkExplosion->usePhysicPointGravity(glm::vec3(0.0, -2.0, 0.0), 0.9, 5.0, 2, 2.0f, true);
	fireworkExplosion->addTexture(texFireworkRed, 1.0);
	fireworkExplosion->defineLook(true, 0.04, 0.0, 0.5);
	fireworkExplosion->setStartTime(2.0);
	
	//Emitter firework tail
	Emitter* fireworkTail = new Emitter(0, glm::vec3(0.0, 0.0, 0.0), 2.0, 0.01, 20, 0.5, true);
	fireworkTail->setVelocity(5);
	//fireworkTail->usePhysicDirectionGravity(glm::vec4(0.0, -1.0, 0.0, 2.9), 0.2f);
	fireworkTail->usePhysicPointGravity(glm::vec3(0.0, -4.0, 0.0), 30.6, 10.0, 2, 0.2f, true);
	fireworkTail->addTexture(texFireworkTail, 1.0);
	fireworkTail->defineLook(true, 0.001, 0.0, 0.1);


	//FINAL EMITTER WHITE SMOKE
	Emitter* smokeWhite = new Emitter(0, glm::vec3(0.0, 0.0, 0.0), 0.0, 0.4, 1, 8.0, true);
	smokeWhite->setVelocity(2);
	smokeWhite->usePhysicDirectionGravity(glm::vec4(0.0, -1.0, 0.0, -0.8), 0.3f);
	smokeWhite->addTexture(smokeWhiteTex1, 1.0);
	smokeWhite->addTexture(smokeWhiteTex2, 0.25);
	std::vector<float> smokeWhiteSize{ 0.05f, 0.5f, 0.75f, 1.0f };
	std::vector<float> smokeWhiteTime{ 0.0f, 0.4f, 0.75f, 1.0f };
	smokeWhite->defineLook(true, smokeWhiteSize, smokeWhiteTime, 1.0, 2.0, 1.0, false, 0.3);
	smokeWhite->switchToGeometryShader();

	//////////////////////Effect//////////////////////
	Effect* efExplosion = new Effect();
	efExplosion->addEmitter(explosionSparkle);
	
	Effect* efFire = new Effect();
	efFire->addEmitter(fire);
	//efFire->addEmitter(fireFlickering);
	efFire->addEmitter(fireSparkle);
	efFire->saveEffect(RESOURCES_PATH "/XML/Effect_Fire.xml");
	
	Effect* efFirework = new Effect();
	efFirework->addEmitter(fireworkTail);
	efFirework->addEmitter(fireworkExplosion);
	//efFirework->saveEffect(RESOURCES_PATH "/XML/Effect_Firework.xml");

	Effect* efFireworkTail = new Effect();
	efFireworkTail->addEmitter(fireworkTail);

	Effect* efFireworkExplosion = new Effect();
	efFireworkExplosion->addEmitter(fireworkExplosion);

	Effect* efSmWhi = new Effect();
	efSmWhi->addEmitter(smokeWhite);
	efSmWhi->saveEffect(RESOURCES_PATH "/XML/Effect_SmokeWhite.xml");
	

	//////////////////////ParticleSystem//////////////////////
	//ParticleSystem* psExplosion = new ParticleSystem(glm::vec3(0, -1, 0), efExplosion);
	//ParticleSystem* psFire = new ParticleSystem(glm::vec3(-2, 0, 3), efFire);
	ParticleSystem* psFire = new ParticleSystem(glm::vec3(-2, 0, 3), RESOURCES_PATH "/XML/Effect_Fire.xml");
	ParticleSystem* psFirework = new ParticleSystem(glm::vec3(0, 0, 5), efFirework);
	ParticleSystem* psFireworkTail = new ParticleSystem(glm::vec3(0, 0, 5), efFireworkTail);
	ParticleSystem* psFireworkExplosion = new ParticleSystem(glm::vec3(0, 2, 5), efFireworkExplosion);
	ParticleSystem* psSmokeWhite = new ParticleSystem(glm::vec3(2, 0, 3), efSmWhi);
	//ParticleSystem* psSmokeWhite = new ParticleSystem(glm::vec3(2, 0, 3), RESOURCES_PATH "/XML/Effect_SmokeWhite.xml");

	ParticleSystem* psFireworkRed = new ParticleSystem(glm::vec3(-6, -1, -3), RESOURCES_PATH "/XML/Effect_FireworkRed.xml");
	ParticleSystem* psFireworkBlue = new ParticleSystem(glm::vec3(-2, -1, -3), RESOURCES_PATH "/XML/Effect_FireworkBlue.xml");
	ParticleSystem* psFireworkGreen = new ParticleSystem(glm::vec3(2, -1, -3), RESOURCES_PATH "/XML/Effect_FireworkGreen.xml");
	ParticleSystem* psFireworkGold = new ParticleSystem(glm::vec3(6, -1, -3), RESOURCES_PATH "/XML/Effect_FireworkGold.xml");

	ParticleSystem* psComicCloud = new ParticleSystem(glm::vec3(0, -1, 3), RESOURCES_PATH "/XML/Effect_ComicCloud.xml");


	//////////////////////Node//////////////////////
	//Node nodeExplosion("nodeExplosion");
	//nodeExplosion.setCamera(&cam);
	//nodeExplosion.addParticleSystem(psExplosion);
	//nodeExplosion.setParticleActive(true);
	//
	//Node fireNode("fireNode");
	//fireNode.setCamera(&cam);
	//fireNode.addParticleSystem(psFire);
	//fireNode.setParticleActive(true);
	//
	//Node nodeFirework("fireworkNode");
	//nodeFirework.setCamera(&cam);
	//nodeFirework.addParticleSystem(psFirework);
	//nodeFirework.setParticleActive(true);
	//
	//Node whiteSmokeNode("whiteSmokeNode");
	//whiteSmokeNode.setCamera(&cam);
	//whiteSmokeNode.addParticleSystem(psSmokeWhite);
	//whiteSmokeNode.setParticleActive(true);

	////Firework
	//Node nodeFireworkRed("fireworkRedNode");
	//nodeFireworkRed.setCamera(&cam);
	//nodeFireworkRed.addParticleSystem(psFireworkRed);
	//nodeFireworkRed.setParticleActive(true);

	//Node nodeFireworkBlue("fireworBlueNode");
	//nodeFireworkBlue.setCamera(&cam);
	//nodeFireworkBlue.addParticleSystem(psFireworkBlue);
	//nodeFireworkBlue.setParticleActive(true);

	//Node nodeFireworkGreen("fireworkGreenNode");
	//nodeFireworkGreen.setCamera(&cam);
	//nodeFireworkGreen.addParticleSystem(psFireworkGreen);
	//nodeFireworkGreen.setParticleActive(true);

	//Node nodeFireworkGold("fireworkGoldNode");
	//nodeFireworkGold.setCamera(&cam);
	//nodeFireworkGold.addParticleSystem(psFireworkGold);
	//nodeFireworkGold.setParticleActive(true);


	// Shader
	VertexShader vs(loadShaderSource(SHADERS_PATH + std::string("/ColorShader3D/ColorShader3D.vert")));
	FragmentShader fs(loadShaderSource(SHADERS_PATH + std::string("/ColorShader3D/ColorShader3D.frag")));
	ShaderProgram shader(vs, fs);
	
	//need scene here mainly because of input
	Level testLevel("testLevel");
	Scene testScene("testScene");
	testLevel.addScene(&testScene);
	testLevel.changeScene("testScene");

	//Add Camera to Scene
	testScene.getScenegraph()->addCamera(&cam);
	testScene.getScenegraph()->setActiveCamera("Pilotview");

	//Set Input-Maps and activate one
	iH.setAllInputMaps(*(testScene.getScenegraph()->getActiveCamera()));
	iH.changeActiveInputMap(MapType::CAMPILOTVIEW);
	iH.getActiveInputMap()->update(cam);

	//Object
	Cube cube;
	Texture bricks((char*)RESOURCES_PATH "/Wall/bricks_diffuse.png");
	Node cube1("cube");
	cube1.addGeometry(&cube);
	cube1.addTexture(&bricks);
	cube1.setModelMatrix(glm::translate(cube1.getModelMatrix(), glm::vec3(0.0, 0.0, 0.0)));
	//cube1.setModelMatrix(glm::scale(cube1.getModelMatrix(), glm::vec3(0.5, 0.5, 0.5)));
	testScene.getScenegraph()->getRootNode()->addChildrenNode(&cube1);
	
	//add nodes to the scenegraph
	//testScene.getScenegraph()->getRootNode()->addChildrenNode(&nodeExplosion);
	//testScene.getScenegraph()->getRootNode()->addChildrenNode(&fireNode);
	//testScene.getScenegraph()->getRootNode()->addChildrenNode(&whiteSmokeNode);
	//testScene.getScenegraph()->getRootNode()->addChildrenNode(&nodeFirework);
	/*testScene.getScenegraph()->getRootNode()->addChildrenNode(&nodeFireworkRed);
	testScene.getScenegraph()->getRootNode()->addChildrenNode(&nodeFireworkBlue);
	testScene.getScenegraph()->getRootNode()->addChildrenNode(&nodeFireworkGreen);
	testScene.getScenegraph()->getRootNode()->addChildrenNode(&nodeFireworkGold);*/

	//using this, the ParticleSystems get rendered in order of their distance to the camera
	testScene.getScenegraph()->addParticleSystem(psFire);
	testScene.getScenegraph()->addParticleSystem(psSmokeWhite);
	testScene.getScenegraph()->addParticleSystem(psFireworkBlue);
	testScene.getScenegraph()->addParticleSystem(psFireworkRed);
	testScene.getScenegraph()->addParticleSystem(psFireworkGreen);
	testScene.getScenegraph()->addParticleSystem(psFireworkGold);
	//testScene.getScenegraph()->addParticleSystem(psComicCloud);

	////TEST
	//Emitter* etest = new Emitter(0, glm::vec3(0, 0, 0), 0, 1, 1000000, 1, true);
	//etest->setVelocity(6);
	//etest->usePhysicPointGravity(glm::vec3(0, 0, 0), 0.5, 20, 1, -2.5, true);
	//etest->defineLook(false, 0.005);
	//Effect* eftest = new Effect();
	//eftest->addEmitter(etest);
	//ParticleSystem* pstest = new ParticleSystem(glm::vec3(0, 2, 0), eftest);
	//testScene.getScenegraph()->addParticleSystem(pstest);
	//pstest->start();


	//start the ParticleSystems
	psFire->start();
	psSmokeWhite->start();
	//psExplosion->start();
	//psFirework->start();
	psFireworkRed->start();
	psFireworkBlue->start();
	psFireworkGreen->start();
	psFireworkGold->start();
	//psComicCloud->start();

	double startTime = glfwGetTime();
	
	double lastTime = glfwGetTime();
	int nbFrames = 0;

	while (!glfwWindowShouldClose(testWindow.getWindow()))
	{
		// Measure speed
		double currentTime = glfwGetTime();
		nbFrames++;
		if (currentTime - lastTime >= 1.0){ // If last prinf() was more than 1 sec ago
			// printf and reset timer
			//printf("%f ms/frame\n", 1000.0 / double(nbFrames));
			nbFrames = 0;
			lastTime += 1.0;
		}

		double dt = glfwGetTime() - startTime;
		cam.setSensitivity(dt);
		startTime = glfwGetTime();

		/*glEnable(GL_DEPTH);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);

		glClearColor(0.5, 0.5, 0.5, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.bind();
		shader.sendMat4("viewMatrix", cam.getViewMatrix());
		shader.sendMat4("projectionMatrix", cam.getProjectionMatrix());
		testScene.render(shader);
		testScene.renderParticleSystems();
		shader.unbind();*/

		//renderer->useBloom(true);
		renderer->renderScene(testScene, testWindow);

		//update Positions of firework ParticleSystems
		glm::vec3 pos = psFireworkRed->getPosition();
		psFireworkRed->setPosition(glm::vec3(pos.x, pos.y + (2 * dt), pos.z));
		
		pos = psFireworkBlue->getPosition();
		psFireworkBlue->setPosition(glm::vec3(pos.x, pos.y + (2 * dt), pos.z));
		
		pos = psFireworkGreen->getPosition();
		psFireworkGreen->setPosition(glm::vec3(pos.x, pos.y + (2 * dt), pos.z));
		
		pos = psFireworkGold->getPosition();
		psFireworkGold->setPosition(glm::vec3(pos.x, pos.y + (2 * dt), pos.z));


		/*glfwSwapBuffers(testWindow.getWindow());
		glfwPollEvents();*/
	}
	glfwDestroyWindow(testWindow.getWindow());
	glfwTerminate();
	
	return 0;
}