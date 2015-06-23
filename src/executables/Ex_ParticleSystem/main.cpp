#include "GeKo_Graphics/ParticleSystem/ParticleSystem.h"
#include <GL/glew.h>
#include <GeKo_Graphics/InputInclude.h>
#include <GeKo_Graphics/MaterialInclude.h>
#include <GeKo_Graphics/GeometryInclude.h>
#include <GeKo_Graphics/ShaderInclude.h>
#include <GeKo_Graphics/ScenegraphInclude.h>
#include "GeKo_Graphics/GUI/GUI.h"

//GUI
GUI *gui;
GuiElement::Checkbox *useSnowButton;
GuiElement::Checkbox *useStrongSnowButton;
GuiElement::Checkbox *useRainButton;
GuiElement::Checkbox *useFruitFliesButton;
GuiElement::Checkbox *useMaximumParticle;

//CAM
static InputHandler iH;
static Pilotview cam("Pilotview");
static Pilotview screen("Screen");

//RENDERER FOR GUI
OpenGL3Context context;
Renderer *renderer;

//include data?
enum FLOW { UNUSED = -1, CONSTANT = 0, ONCE = 1 } output;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
	iH.getActiveInputMap()->checkKeys(key, *window);
}

void initGUI()
{
	gui = new GUI("Particlesystem Settings - Hide [F10]", 300, 250);

	GuiElement::Header *moveableParticleHeader = new GuiElement::Header("Weather");

	useSnowButton = new GuiElement::Checkbox("Firework");
	moveableParticleHeader->addElement(useSnowButton);
	moveableParticleHeader->addElement(new GuiElement::Spacing);
	
	useStrongSnowButton = new GuiElement::Checkbox("Strong Snow");
	moveableParticleHeader->addElement(useStrongSnowButton);
	moveableParticleHeader->addElement(new GuiElement::Spacing);

	useRainButton = new GuiElement::Checkbox("Rain");
	moveableParticleHeader->addElement(useRainButton);
	moveableParticleHeader->addElement(new GuiElement::Spacing);

	GuiElement::Header *screenParticleHeader = new GuiElement::Header("Screen Emitter");

	useFruitFliesButton = new GuiElement::Checkbox("Fruitflies");
	screenParticleHeader->addElement(useFruitFliesButton);
	screenParticleHeader->addElement(new GuiElement::Spacing);

	GuiElement::Header *maxParticleHeader = new GuiElement::Header("Maximum Particlenumber");
	useMaximumParticle = new GuiElement::Checkbox("300 000 Particles without Texture");
	maxParticleHeader->addElement(useMaximumParticle);
	maxParticleHeader->addElement(new GuiElement::Spacing);

	gui->addElement(moveableParticleHeader);
	//gui->addElement(screenParticleHeader);
	//gui->addElement(maxParticleHeader);

	renderer = new Renderer(context);
	renderer->addGui(gui);
}

int main()
{
	//////////////////////////////////////////BASICS///////////////////////////////////////////

	srand(time(NULL));

	glfwInit();

	//old

	/*//WINDOW
	Window window(50, 50, 800, 600, "ParticleSystem");
	glfwMakeContextCurrent(window.getWindow());

	//CAM
	cam.setPosition(glm::vec4(0.0, 0.0, 7.0, 1.0));
	cam.setNearFar(0.1f, 100.0f);
	cam.setLookAt(glm::vec3(cam.getPosition().x, cam.getPosition().y, cam.getPosition().z - 1.0));
	cam.setKeySpeed(8.0);
	iH.setAllInputMaps(cam);
	iH.changeActiveInputMap("Pilotview");

	//Callback
	glfwSetKeyCallback(window.getWindow(), key_callback);*/

	//More then just 60 fps, vsync off
	//glfwSwapInterval(0);


	//new
	Window window(500, 50, 800, 600, "ParticleSystem");
	glfwMakeContextCurrent(window.getWindow());

	// Callback
	glfwSetKeyCallback(window.getWindow(), key_callback);

	cam.setKeySpeed(4.0);
	cam.setNearFar(0.1, 100);
	cam.setPosition(glm::vec4(-13, 0, 5, 0));

	glewInit();

	//SHADER
	VertexShader vsSkybox(loadShaderSource(SHADERS_PATH + std::string("/SkyboxShader/SkyboxShader.vert")));
	FragmentShader fsSkybox(loadShaderSource(SHADERS_PATH + std::string("/SkyboxShader/SkyboxShader.frag")));
	ShaderProgram shaderSkybox(vsSkybox, fsSkybox);

	VertexShader vsObject(loadShaderSource(SHADERS_PATH + std::string("/TextureShader3D/TextureShader3D.vert")));
	FragmentShader fsObject(loadShaderSource(SHADERS_PATH + std::string("/TextureShader3D/TextureShader3D.frag")));
	ShaderProgram shaderObject(vsObject, fsObject);

	//////////////////////////////////////OUR SCENE///////////////////////////////////////////////

	//SKYBOX
	Cube cube;
	const char *textureNames[6] = {
		/*(char*)RESOURCES_PATH "/PereaBeach1/posx.jpg",
		(char*)RESOURCES_PATH "/PereaBeach1/negx.jpg",
		(char*)RESOURCES_PATH "/PereaBeach1/posy.jpg",
		(char*)RESOURCES_PATH "/PereaBeach1/negy.jpg",
		(char*)RESOURCES_PATH "/PereaBeach1/posz.jpg",
		(char*)RESOURCES_PATH "/PereaBeach1/negz.jpg"*/
		/*(char*)RESOURCES_PATH "/Color/testTex.png",
		(char*)RESOURCES_PATH "/Color/testTex.png",
		(char*)RESOURCES_PATH "/Color/testTex.png",
		(char*)RESOURCES_PATH "/Color/testTex.png",
		(char*)RESOURCES_PATH "/Color/testTex.png",
		(char*)RESOURCES_PATH "/Color/testTex.png"*/
		(char*)RESOURCES_PATH "/Color/hellblau.png",
		(char*)RESOURCES_PATH "/Color/hellblau.png",
		(char*)RESOURCES_PATH "/Color/hellblau.png",
		(char*)RESOURCES_PATH "/Color/hellblau.png",
		(char*)RESOURCES_PATH "/Color/hellblau.png",
		(char*)RESOURCES_PATH "/Color/hellblau.png" };
	Skybox skybox(textureNames);
	Node skyboxNode("skybox");
	skyboxNode.addGeometry(&cube);

	//OBJECTS
	Rect plane;
	plane.loadBufferData();

	//TEXTURES
	Texture chrome((char*)RESOURCES_PATH "/Metal/chrome.jpg");
	Texture marble((char*)RESOURCES_PATH "/Wall/seamless_marble_long.png");
	Texture cvLogo((char*)RESOURCES_PATH "/Symbol/cv_logo.bmp");
	Texture bricks((char*)RESOURCES_PATH "/Wall/bricks_diffuse_long.png");

	//CREATING NODES
	Level psLevel("psLevel");
	Scene psScene("psScene");
	psLevel.addScene(&psScene);
	psLevel.changeScene("psScene");

	Node wallNode1("wall1");
	wallNode1.addGeometry(&plane);
	wallNode1.addTexture(&marble);
	wallNode1.setModelMatrix(glm::translate(wallNode1.getModelMatrix(), glm::vec3(0.0, -1.0, 1.0)));
	wallNode1.setModelMatrix(glm::rotate(wallNode1.getModelMatrix(), 90.0f, glm::vec3(1.0, 0.0, 0.0)));
	wallNode1.setModelMatrix(glm::scale(wallNode1.getModelMatrix(), glm::vec3(15.0, 1.0, 1.0)));

	Node wallNode2("wall2");
	wallNode2.addGeometry(&plane);
	wallNode2.addTexture(&bricks);
	wallNode2.setModelMatrix(glm::translate(wallNode2.getModelMatrix(), glm::vec3(0.0, 0.0, 0.0)));
	wallNode2.setModelMatrix(glm::scale(wallNode2.getModelMatrix(), glm::vec3(15.0, 1.0, 1.0)));

	//CREATING A SCENEGRAPH
	psScene.getScenegraph()->getRootNode()->addChildrenNode(&wallNode1);
	psScene.getScenegraph()->getRootNode()->addChildrenNode(&wallNode2);

	//ADD CAMERA TO SCENEGRAPH
	psScene.getScenegraph()->addCamera(&cam);
	psScene.getScenegraph()->getCamera("Pilotview");
	psScene.getScenegraph()->setActiveCamera("Pilotview");

	//Set Input-Maps and activate one
	iH.setAllInputMaps(*(psScene.getScenegraph()->getActiveCamera()));
	iH.changeActiveInputMap(MapType::CAMPILOTVIEW);
	iH.getActiveInputMap()->update(cam);

	/////////////////////////////////////////TEXTURES//////////////////////////////////////////

	//COMIC CLOUD
	Texture* comicBalloonTex1 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/comicCloud/balloon01.png"); 
	Texture* comicBalloonTex2 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/comicCloud/balloon02.png"); 
	Texture* comicBalloonTex3 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/comicCloud/balloon03.png"); 
	Texture* comicBalloonTex4 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/comicCloud/balloon04.png"); 
	Texture* comicCloudTex1 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/comicCloud/cloud01.png"); 
	Texture* comicCloudTex2 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/comicCloud/cloud02.png");
	Texture* comicCloudTex3 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/comicCloud/cloud03.png");
	Texture* comicCloudTex4 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/comicCloud/cloud04.png");
	Texture* comicExclamationMarkTex1 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/comicCloud/exclamationMark01.png");
	Texture* comicExclamationMarkTex2 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/comicCloud/exclamationMark02.png");
	Texture* comicExclamationMarkTex3 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/comicCloud/exclamationMark03.png");
	Texture* comicExclamationMarkTex4 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/comicCloud/exclamationMark04.png");
	Texture* comicLightningTex1 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/comicCloud/lightning01.png");
	Texture* comicLightningTex2 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/comicCloud/lightning02.png");
	Texture* comicLightningTex3 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/comicCloud/lightning03.png");
	Texture* comicLightningTex4 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/comicCloud/lightning04.png");
	Texture* comicSpiralTex1 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/comicCloud/spiral01.png");
	Texture* comicSpiralTex2 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/comicCloud/spiral02.png");
	Texture* comicSpiralTex3 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/comicCloud/spiral03.png");
	Texture* comicSpiralTex4 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/comicCloud/spiral04.png");
	Texture* comicSpiralTex5 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/comicCloud/spiral05.png");
	Texture* comicSpiralTex6 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/comicCloud/spiral06.png");
	Texture* comicStarTex1 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/comicCloud/star01.png");
	Texture* comicStarTex2 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/comicCloud/star02.png");

	//FIRE
	Texture* fireTex1 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/fire/flame01_L.png");
	Texture* fireTex2 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/fire/flame02_L.png");
	Texture* fireTex3 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/fire/flame03_L.png");

	//PARTICLE
	Texture* particleBlackTex = new Texture((char*)RESOURCES_PATH "/ParticleSystem/particle/virus.png");
	Texture* particleWhiteTex = new Texture((char*)RESOURCES_PATH "/ParticleSystem/particle/particle.png");
	Texture* snowTex = new Texture((char*)RESOURCES_PATH "/ParticleSystem/particle/snowflake.png"); //TODO better Resolution
	Texture* glowwormTex = new Texture((char*)RESOURCES_PATH "/ParticleSystem/particle/glowworm.png");
	Texture* energyTex = new Texture((char*)RESOURCES_PATH "/ParticleSystem/particle/energy.png");
	Texture* fireflyTex = new Texture((char*)RESOURCES_PATH "/ParticleSystem/particle/firefly.png");
	Texture* fireSparkTex1 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/particle/sparkOrange.png");
	Texture* fireSparkTex2 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/particle/sparkRed.png");

	//RAIN
	Texture* rainTex = new Texture((char*)RESOURCES_PATH "/ParticleSystem/rain/rain.png"); //TODO darker

	//SMOKE
	Texture* drawSmokeTex1 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/smoke/smokeDraw/smoke01_L.png");
	Texture* drawSmokeTex2 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/smoke/smokeDraw/smoke02_L.png");
	Texture* drawSmokeTex3 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/smoke/smokeDraw/smoke03_L.png");
	Texture* drawSmokeTex4 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/smoke/smokeDraw/smoke04_L.png");
	Texture* smokeWhiteTex1 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/smoke/smokeWhite/smokeWhite01.png");
	Texture* smokeWhiteTex2 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/smoke/smokeWhite/smokeWhite02.png");
	Texture* smokeBlackTex1 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/smoke/smokeBlack/smokeBlack01.png");
	Texture* smokeBlackTex2 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/smoke/smokeBlack/smokeBlack02.png");

	//STAR
	Texture* fireWorkYellowTex = new Texture((char*)RESOURCES_PATH "/ParticleSystem/star/starYellow.png"); //TODO Work?
	Texture* fireWorkOrangeTex = new Texture((char*)RESOURCES_PATH "/ParticleSystem/star/starPink.png"); //TODO Work?
	Texture* fireWorkRedTex = new Texture((char*)RESOURCES_PATH "/ParticleSystem/star/starRed.png"); //TODO Work?
	Texture* fireWorkBlueTex = new Texture((char*)RESOURCES_PATH "/ParticleSystem/star/starBlue.png"); //TODO Work?
	Texture* fireWorkGreenTex = new Texture((char*)RESOURCES_PATH "/ParticleSystem/star/starGreen.png"); //TODO Work?

	///////////////////////////////////////FINAL EMITTER///////////////////////////////////////

	//FINAL EMITTER SNOW
	Emitter* snow = new Emitter(0, glm::vec3(0.0, 5.0, 0.0), 0.0, 0.166, 100, 30.0, true);
	snow->setVelocity(0);
	snow->usePhysicDirectionGravity(glm::vec4(0.0, -1.0, 0.0, 1.0), 0.5f);
	snow->setAreaEmitting(false,true, 10.0, 10000);
	snow->addTexture(snowTex, 0.0);
	snow->defineLook(true, 0.04, 2.0);
	snow->setMovable(true);

	//FINAL EMITTER STRONG SNOW
	Emitter* snowStrong = new Emitter(0, glm::vec3(-3.5, 4.0, 0.0), 0.0, 0.166, 100, 15.0, true);
	snowStrong->setVelocity(2);
	snowStrong->usePhysicDirectionGravity(glm::vec4(0.6, -1.5, 0.0, 1.0), 2.6f);
	snowStrong->setAreaEmitting(false, true, 8.0, 10000);
	snowStrong->addTexture(snowTex, 0.0);
	snowStrong->defineLook(true, 0.03, 1.0, 3.0);
	snowStrong->setMovable(true);

	//FINAL EMITTER WHITE SMOKE
	Emitter* smokeWhite = new Emitter(0, glm::vec3(0.0, 0.0, 0.0), 0.0, 0.4, 1, 8.0, true);
	smokeWhite->setVelocity(2);
	smokeWhite->usePhysicDirectionGravity(glm::vec4(0.0, -1.0, 0.0, -0.8), 0.3f);
	smokeWhite->addTexture(smokeWhiteTex1, 1.0);
	smokeWhite->addTexture(smokeWhiteTex2, 0.25);
	std::vector<float> smokeWhiteSize{ 0.05f, 0.5f, 0.75f, 1.2f };
	std::vector<float> smokeWhiteTime{ 0.0f, 0.4f, 0.75f, 1.0f };
	smokeWhite->defineLook(true, smokeWhiteSize, smokeWhiteTime, 1.0, 4.0, 3.0, false, 0.3);
	smokeWhite->switchToGeometryShader();

	//FINAL EMITTER BLACK SMOKE
	Emitter* smokeBlack = new Emitter(0, glm::vec3(0.0, 0.0, 0.0), 0.0, 0.6, 1, 8.0, true);
	smokeBlack->setVelocity(2);
	smokeBlack->usePhysicDirectionGravity(glm::vec4(0.0, -1.0, 0.0, -0.8), 0.3f);
	smokeBlack->addTexture(smokeBlackTex1, 1.0);
	smokeBlack->addTexture(smokeBlackTex2, 0.2);
	std::vector<float> smokeBlackSize{ 0.1f, 0.4f, 0.8f, 1.2f };
	std::vector<float> smokeBlackTime{ 0.0f, 0.2f, 0.75f, 1.0f };
	smokeBlack->defineLook(true, smokeBlackSize, smokeBlackTime, 1.0, 5.0, 3.0, false, 0.3);
	smokeBlack->switchToGeometryShader();

	//FINAL EMITTER CLOUD SMOKE
	Emitter* smokeCloud = new Emitter(0, glm::vec3(0.0, 0.0, 0.0), 0.0, 0.3, 1, 10.0, true);
	smokeCloud->setVelocity(3);
	smokeCloud->usePhysicDirectionGravity(glm::vec4(0.0, -1.0, 0.0, -0.1), 0.15f);
	smokeCloud->addTexture(smokeWhiteTex1, 1.0);
	smokeCloud->addTexture(smokeWhiteTex2, 0.08);
	std::vector<float> smokeCloudSize{ 0.1f, 0.4f, 0.8f, 1.2f };
	std::vector<float> smokeCloudTime{ 0.0f, 0.2f, 0.75f, 1.0f };
	smokeCloud->defineLook(true, smokeBlackSize, smokeBlackTime, 1.0, 2.0, 4.0, false, 0.3);
	smokeCloud->switchToGeometryShader();

	//FINAL EMITTER RAIN
	Emitter* rain = new Emitter(0, glm::vec3(0.0, 3.0, 0.0), 0.0, 0.02, 50, 5.0, true);
	rain->setVelocity(0);
	rain->usePhysicDirectionGravity(glm::vec4(0.0, -1.0, 0.0, 1.0), 5.0f);
	rain->setAreaEmitting(false, true, 8.0, 10000);
	rain->addTexture(rainTex, 0.0);
	rain->defineLook(true, 0.03, 1.0, 0.0);
	rain->setMovable(true);

	//FINAL EMITTER FONTAINE
	Emitter* fontaine = new Emitter(0, glm::vec3(0.0, 0.0, 0.0), 0.0, 0.05, 1, 2.0, true);
	fontaine->setVelocity(0);
	fontaine->usePhysicTrajectory(glm::vec4(0.0, 0.0, 0.0, 1.0), 1.0);
	//fontaine->usePhysicDirectionGravity(glm::vec4(0.0, -1.0, 0.0, -0.0), 0.7f);
	fontaine->defineLook(false, 0.05, 0.0, 0.5, 0.0, true, 1.0);

	//FINAL EMITTER CIRCLE
	Emitter* circle = new Emitter(0, glm::vec3(0.0, 0.0, 0.0), 0.0, 0.2, 50, 4.0, true);
	circle->setVelocity(4);
	circle->addTexture(fireSparkTex1, 1.0);
	circle->usePhysicDirectionGravity(glm::vec4(0.0, -1.0, 0.0, -0.0), 0.3f);
	circle->addTexture(fireSparkTex2 ,0.1);
	circle->defineLook(true, 0.03, 0.0, 2.0, 1.0, true, 0.0);
	circle->switchToGeometryShader();

	//FINAL EMITTER QUAD
	Emitter* quad = new Emitter(0, glm::vec3(0.0, 0.0, 0.0), 0.0, 1.0, 100, 1.0, true);
	quad->setVelocity(0);
	quad->setAreaEmitting(true, true, 0.5, 1);
	quad->defineLook(false, 0.1, 0.0, 0.0, 0.0, false, 0.0);

	//FINAL EMITTER FRUITFLIES
	Emitter* fruitFlies = new Emitter(0, glm::vec3(0.0, 0.0, 0.0), 0.0, 0.166, 2, 10.0, true);
	fruitFlies->setVelocity(0);
	fruitFlies->usePhysicSwarmCircleMotion(true, true, true, 3.0);
	fruitFlies->setAreaEmitting(true, true, 0.5, 100);
	fruitFlies->addTexture(particleBlackTex, 0.0);
	//fruitFlies->defineLook(true, 0.022, 3.0, 3.0, 0.0, true, 1.0);
	fruitFlies->defineLook(true, 0.07, 3.0, 3.0, 0.0);
	//fruitFlies->switchToGeometryShader();

	//FINAL SCREEN EMITTER FRUITFLIES
	Emitter* screenFruitFlies = new Emitter(0, glm::vec3(-0.2, 0.2, 7.0), 0.0, 0.1, 2, 10.0, true);
	screenFruitFlies->setVelocity(0);
	screenFruitFlies->usePhysicSwarmCircleMotion(true, true, true, 3.0);
	screenFruitFlies->setAreaEmitting(true, false, 2.5, 1000);
	screenFruitFlies->addTexture(particleBlackTex, 0.0);
	screenFruitFlies->defineLook(true, 0.02, 1.0, 3.0, 0.0, true, 1.0);
	screenFruitFlies->switchToGeometryShader();

	//FINAL EMITTER GLOWWORM
	Emitter* glowworm = new Emitter(0, glm::vec3(0.0, 0.0, 0.0), 0.0, 0.2, 1, 10.0, true);
	glowworm->setVelocity(0);
	glowworm->usePhysicSwarmCircleMotion(true, true, true, 3.0);
	glowworm->setAreaEmitting(true, false, 0.4, 1000);
	glowworm->addTexture(glowwormTex, 0.0);
	glowworm->defineLook(true, 0.1, 1.0, 1.5);

	//FINAL EMITTER ENERGYBALL
	Emitter* energyBall = new Emitter(0, glm::vec3(0.0, 0.0, 1.0), 0.0, 0.1, 5, 3.0, true);
	energyBall->setVelocity(5);
	energyBall->usePhysicPointGravity(glm::vec3(0.0, 0.0, 0.0), 2.0, 8.0, 2, 2.8, true);
	energyBall->addTexture(energyTex, 1.0);
	energyBall->defineLook(true, 0.1);

	//FINAL EMITTER COMIC CLOUD
	Emitter* cloud01 = new Emitter(0, glm::vec3(0.0, 0.5, 0.0), 0.0, 1.6, 1, 10.0, true);
	cloud01->setVelocity(4);
	//cloud01->usePhysicPointGravity(glm::vec4(0.0, -1.0, 0.0, -0.0), 0.0, 0, 0.72);
	cloud01->addTexture(comicCloudTex1, 0.0);
	std::vector<float> cloudSize1{ 0.8f, 1.6f };
	std::vector<float> cloudTime1{ 0.0f, 1.0f };
	cloud01->defineLook(true, cloudSize1, cloudTime1, 0.4, 4.0, 0.0, false, 0.3);
	cloud01->switchToGeometryShader();

	Emitter* cloud02 = new Emitter(0, glm::vec3(0.0, 0.5, 0.0), 0.0, 2.3, 1, 10.0, true);
	cloud02->setVelocity(4);
	//cloud02->usePhysicPointGravity(glm::vec4(0.0, -1.0, 0.0, -0.0), 0.0, 0, 0.6);
	cloud02->addTexture(comicCloudTex3, 0.0);
	std::vector<float> cloudSize2{ 0.8f, 1.6f };
	std::vector<float> cloudTime2{ 0.0f, 1.0f };
	cloud02->defineLook(true, cloudSize2, cloudTime2, 0.4, 4.0, 0.0, false, 0.3);
	cloud02->switchToGeometryShader();

	Emitter* cloud03 = new Emitter(0, glm::vec3(-1.0, 1.0, 0.0), 0.0, 0.0, 1, 0.0, false);
	cloud03->setVelocity(0);
	cloud03->addTexture(comicBalloonTex4, 1.0);
	cloud03->defineLook(true, 0.8, 2.0);
	cloud03->switchToGeometryShader();

	Emitter* cloud04 = new Emitter(0, glm::vec3(1.2, 2.0, 0.0), 0.0, 0.0, 1, 0.0, false);
	cloud04->setVelocity(0);
	cloud04->addTexture(comicStarTex2, 1.0);
	cloud04->defineLook(true, 0.3, 2.0, 0.0, 0.0, false, 0.6);
	cloud04->switchToGeometryShader();

	Emitter* cloud05 = new Emitter(0, glm::vec3(-1.0, -1.0, 0.0), 0.0, 0.0, 1, 0.0, false);
	cloud05->setVelocity(0);
	cloud05->addTexture(comicStarTex2, 1.0);
	cloud05->defineLook(true, 0.5, 2.0, 0.0, 0.0, true, 0.4);
	cloud05->switchToGeometryShader();

	Emitter* cloud06 = new Emitter(0, glm::vec3(1.7, 0.2, 0.0), 0.0, 0.0, 1, 0.0, false);
	cloud06->setVelocity(0);
	cloud06->addTexture(comicSpiralTex5, 1.0);
	cloud06->defineLook(true, 0.3, 2.0);
	cloud06->switchToGeometryShader();

	Emitter* cloud07 = new Emitter(0, glm::vec3(-2.0, 0.7, 0.0), 0.0, 0.0, 1, 0.0, false);
	cloud07->setVelocity(0);
	cloud07->addTexture(comicSpiralTex3, 1.0);
	cloud07->defineLook(true, 0.3, 2.0);
	cloud07->switchToGeometryShader();

	Emitter* cloud08 = new Emitter(0, glm::vec3(-1.9, 2.1, 0.0), 0.0, 0.0, 1, 0.0, false);
	cloud08->setVelocity(0);
	cloud08->addTexture(comicLightningTex1, 1.0);
	cloud08->defineLook(true, 0.6, 2.0);
	cloud08->switchToGeometryShader();

	Emitter* cloud09 = new Emitter(0, glm::vec3(2.0, 1.4, 0.0), 0.0, 0.0, 1, 0.0, false);
	cloud09->setVelocity(0);
	cloud09->addTexture(comicLightningTex4, 1.0);
	cloud09->defineLook(true, 0.5, 2.0);
	cloud09->switchToGeometryShader();

	Emitter* cloud10 = new Emitter(0, glm::vec3(2.7, 0.8, 0.0), 0.0, 0.0, 1, 0.0, false);
	cloud10->setVelocity(0);
	cloud10->addTexture(comicExclamationMarkTex3, 1.0);
	cloud10->defineLook(true, 0.3, 2.0);
	cloud10->switchToGeometryShader();

	Emitter* cloud11 = new Emitter(0, glm::vec3(-0.5, 2.4, 0.0), 0.0, 0.0, 1, 0.0, false);
	cloud11->setVelocity(0);
	cloud11->addTexture(comicExclamationMarkTex2, 1.0);
	cloud11->defineLook(true, 0.5, 2.0);
	cloud11->switchToGeometryShader();

	//////////////////Presentation/////////////////////////
	Texture* texFire1 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/fire/fire1_M.png");
	Texture* texFire2 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/fire/fire3_M.png");
	Texture* texfireSparkleTex1 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/fire/fireSparkle1_S.png");
	Texture* texsmokeBlack2 = new Texture((char*)RESOURCES_PATH "/ParticleSystem/smoke/smokeBlack/smokeBlack02.png");
	//Emitter fire smoke
	Emitter* fire = new Emitter(0, glm::vec3(0.0, 0.0, 0.0), 0.0, 0.2, 2, 5.0, true);
	fire->setVelocity(5);
	fire->usePhysicDirectionGravity(glm::vec4(0.0, 1.0, 0.0, 5.0), 0.5f);
	fire->addTexture(texFire1, 1.0);
	fire->addTexture(texFire2, 0.7);
	fire->addTexture(texsmokeBlack2, 0.1);
	//fire->addTexture(smokeBlack2, 0.1);
	//fire->addTexture(smokeWhiteTex2, 0.25);
	std::vector<float> sizeF{ 0.05f, 0.5f, 0.75f, 1.0f };
	std::vector<float> timeF{ 0.0f, 0.4f, 0.75f, 1.0f };
	fire->defineLook(true, sizeF, timeF, 0.5, 4.0, 3.0, true, 0.3);
	fire->switchToGeometryShader();

	Emitter* fireSparkle = new Emitter(0, glm::vec3(0.0, 0.1, 0.0), 0.0, 0.05, 3, 2.5, true);
	fireSparkle->setVelocity(5);
	fireSparkle->usePhysicDirectionGravity(glm::vec4(0.0, 1.0, 0.0, 0.8), 0.5f);
	fireSparkle->addTexture(texfireSparkleTex1, 1.0);
	fireSparkle->defineLook(true, 0.05, 0.5, 0.5);

	Effect* efFire = new Effect();
	efFire->addEmitter(fire);
	efFire->addEmitter(fireSparkle);



	////////////////////////////////NOT FINAL EMITTER////////////////////////////////

	//FINAL EMITTER MAXIMUM; SET POSITION
	Emitter* maximumParticle = new Emitter(0, glm::vec3(0.0, 0.0, 0.0), 0.0, 0.2, 5000, 12.0, true);
	maximumParticle->setVelocity(3);
	maximumParticle->usePhysicDirectionGravity(glm::vec4(0.0, 1.0, 0.0, 0.2), 0.5);
	maximumParticle->defineLook(false, 0.01);

	//particleMax with Texture

	//!EMITTER FIRE
	Emitter* fireMiddle = new Emitter(0, glm::vec3(0.0, 0.0, 0.0), 0.0, 1.0, 3.0, 4.0, true);
	fireMiddle->setVelocity(3);
	fireMiddle->usePhysicDirectionGravity(glm::vec4(0.0, -1.0, 0.0, -0.5), 0.2);
	fireMiddle->addTexture(fireTex1, 1.0);
	//fireMiddle->addTexture(fireTex2, 0.5);
	fireMiddle->addTexture(fireTex3, 0.0);
	fireMiddle->defineLook(true, 0.5, 1.0, 2.0);
	fireMiddle->switchToGeometryShader();

	Emitter* fireSmoke = new Emitter(0, glm::vec3(0.0, 0.0, 0.0), 0.0, 0.4, 1, 8.0, true);
	fireSmoke->setVelocity(2);
	fireSmoke->usePhysicDirectionGravity(glm::vec4(0.0, -1.0, 0.0, -0.8), 0.3f);
	fireSmoke->addTexture(smokeBlackTex1, 1.0);
	fireSmoke->addTexture(smokeBlackTex2, 0.08);
	std::vector<float> fireSmokeSize{ 0.1f, 0.4f, 0.8f, 1.2f };
	std::vector<float> fireSmokeTime{ 0.0f, 0.2f, 0.75f, 1.0f };
	fireSmoke->defineLook(true, fireSmokeSize, fireSmokeTime, 1.0, 4.0, 1.0, false, 0.3);
	fireSmoke->switchToGeometryShader();

	Emitter* firefly = new Emitter(0, glm::vec3(0.0, 0.0, 0.0), 0.0, 1.3, 1, 9.0, true);
	firefly->setVelocity(3);
	firefly->usePhysicDirectionGravity(glm::vec4(0.0, -1.0, 0.0, -1.5), 0.13f);
	firefly->addTexture(fireflyTex, 1.0);
	firefly->defineLook(true, 0.04, 2.0, 2.0, 0.0, false, 0.3);

	Emitter* fireSparkOrange = new Emitter(0, glm::vec3(0.1, 0.0, 1.0), 0.0, 0.1, 1, 0.8, true);
	fireSparkOrange->setVelocity(3);
	fireSparkOrange->usePhysicDirectionGravity(glm::vec4(0.0, -1.0, 0.0, -1.0), 1.5f);
	fireSparkOrange->addTexture(fireSparkTex1, 1.0);
	fireSparkOrange->defineLook(true, 0.02, 0.4, 0.3, 0.0, false, 0.3);

	Emitter* fireSparkRed = new Emitter(0, glm::vec3(0.1, 0.0, 1.0), 0.0, 0.1, 1, 1.0, true);
	fireSparkRed->setVelocity(3);
	fireSparkRed->usePhysicDirectionGravity(glm::vec4(0.0, -1.0, 0.0, -1.5), 1.5f);
	fireSparkRed->addTexture(fireSparkTex2, 1.0);
	fireSparkRed->defineLook(true, 0.02, 0.4, 0.3, 0.0, false, 0.3);

	//!EMITTER FIREWORK
	Emitter* firework = new Emitter(0, glm::vec3(0.6, 0.6, 0.0), 0.0, 0.5, 5, 1.0, true);
	firework->setVelocity(4);
	firework->usePhysicDirectionGravity(glm::vec4(0.0,-1.0,0.0,0.1), 1.0f);
	//firework->addTexture(*fireWorkYellowTex, 0.0);
	//firework->defineLook(true, 0.8, 0.1, 0.2);
	Emitter* firework2 = new Emitter(0, glm::vec3(-0.6, -0.6, 0.0), 0.0, 0.5, 5, 1.0, true);
	firework2->setVelocity(4);
	firework2->usePhysicDirectionGravity(glm::vec4(0.0, -1.0, 0.0, 0.1), 1.0f);
	//firework2->addTexture(*fireWorkOrangeTex, 0.0);
	//firework2->defineLook(true, 0.8, 0.1, 0.2);

	//////////////////////////////////////Effect//////////////////////////////////////////////////
	
	Effect* effect = new Effect();
	
	effect->addEmitter(snow);
	effect->saveEffect(RESOURCES_PATH "/XML/Effect_Snow.xml");
	effect->removeEmitter(0);

	effect->addEmitter(snowStrong);
	effect->saveEffect(RESOURCES_PATH "/XML/Effect_SnowStrong.xml");
	effect->removeEmitter(0);
	
	effect->addEmitter(smokeWhite);
	effect->saveEffect(RESOURCES_PATH "/XML/Effect_SmokeWhite.xml");
	effect->removeEmitter(0);

	effect->addEmitter(smokeBlack);
	effect->saveEffect(RESOURCES_PATH "/XML/Effect_SmokeBlack.xml");
	effect->removeEmitter(0);

	effect->addEmitter(smokeCloud);
	effect->saveEffect(RESOURCES_PATH "/XML/Effect_SmokeCloud.xml");
	effect->removeEmitter(0);

	effect->addEmitter(rain);
	effect->saveEffect(RESOURCES_PATH "/XML/Effect_Rain.xml");
	effect->removeEmitter(0);

	effect->addEmitter(fontaine);
	effect->saveEffect(RESOURCES_PATH "/XML/Effect_Fontaine.xml");
	effect->removeEmitter(0);

	effect->addEmitter(circle);
	effect->saveEffect(RESOURCES_PATH "/XML/Effect_Circle.xml");
	effect->removeEmitter(0);

	effect->addEmitter(quad);
	effect->saveEffect(RESOURCES_PATH "/XML/Effect_Quad.xml");
	effect->removeEmitter(0);

	effect->addEmitter(fruitFlies);
	effect->saveEffect(RESOURCES_PATH "/XML/Effect_FruitFlies.xml");
	effect->removeEmitter(0);

	effect->addEmitter(screenFruitFlies);
	effect->saveEffect(RESOURCES_PATH "/XML/Effect_ScreenFruitFlies.xml");
	effect->removeEmitter(0);

	effect->addEmitter(glowworm);
	effect->saveEffect(RESOURCES_PATH "/XML/Effect_Glowworm.xml");
	effect->removeEmitter(0);

	effect->addEmitter(energyBall);
	effect->saveEffect(RESOURCES_PATH "/XML/Effect_EnergyBall.xml");
	effect->removeEmitter(0);

	//////////////////////////////////////Particle System//////////////////////////////////////////

	ParticleSystem* psSnow = new ParticleSystem(glm::vec3(0, 0, 0), RESOURCES_PATH "/XML/Effect_Snow.xml");
	psScene.getScenegraph()->addParticleSystem(psSnow);

	ParticleSystem* psRain = new ParticleSystem(glm::vec3(0, 0, 0), RESOURCES_PATH "/XML/Effect_Rain.xml");
	psScene.getScenegraph()->addParticleSystem(psRain);
	//psRain->start();

	ParticleSystem* psSnowStrong = new ParticleSystem(glm::vec3(0, 0, 0), RESOURCES_PATH "/XML/Effect_SnowStrong.xml");
	psScene.getScenegraph()->addParticleSystem(psSnowStrong);
	//psSnow->start();

	ParticleSystem* psSmokeBlack = new ParticleSystem(glm::vec3(3, -0.5, 1), RESOURCES_PATH "/XML/Effect_SmokeBlack.xml");
	psScene.getScenegraph()->addParticleSystem(psSmokeBlack);
	psSmokeBlack->start();
	
	ParticleSystem* psSmokeWhite = new ParticleSystem(glm::vec3(6, -0.5, 1), RESOURCES_PATH "/XML/Effect_SmokeWhite.xml");
	psScene.getScenegraph()->addParticleSystem(psSmokeWhite);
	psSmokeWhite->start();

	ParticleSystem* psSmokeCloud = new ParticleSystem(glm::vec3(9, -0.5, 1), RESOURCES_PATH "/XML/Effect_SmokeCloud.xml");
	psScene.getScenegraph()->addParticleSystem(psSmokeCloud);
	psSmokeCloud->start();

	ParticleSystem* psFontaine = new ParticleSystem(glm::vec3(-13, 0, 1), RESOURCES_PATH "/XML/Effect_Fontaine.xml");
	psScene.getScenegraph()->addParticleSystem(psFontaine);
	psFontaine->start();

	ParticleSystem* psCircle = new ParticleSystem(glm::vec3(0, 0, 1), RESOURCES_PATH "/XML/Effect_Circle.xml");
	psScene.getScenegraph()->addParticleSystem(psCircle);
	psCircle->start();

	ParticleSystem* psQuad = new ParticleSystem(glm::vec3(-10, 0, 1), RESOURCES_PATH "/XML/Effect_Quad.xml");
	psScene.getScenegraph()->addParticleSystem(psQuad);
	psQuad->start();

	ParticleSystem* psFruitFlies = new ParticleSystem(glm::vec3(-7, 0, 1), RESOURCES_PATH "/XML/Effect_FruitFlies.xml");
	psScene.getScenegraph()->addParticleSystem(psFruitFlies);
	psFruitFlies->start();

	ParticleSystem* psGlowworm = new ParticleSystem(glm::vec3(-6, 0, 1), RESOURCES_PATH "/XML/Effect_Glowworm.xml");
	psScene.getScenegraph()->addParticleSystem(psGlowworm);
	psGlowworm->start();

	ParticleSystem* psEnergyBall = new ParticleSystem(glm::vec3(-3, 0, 1), RESOURCES_PATH "/XML/Effect_EnergyBall.xml");
	psScene.getScenegraph()->addParticleSystem(psEnergyBall);
	psEnergyBall->start();

	ParticleSystem* psComicCloud = new ParticleSystem(glm::vec3(20, -2, -6), RESOURCES_PATH "/XML/Effect_ComicCloud.xml");
	psScene.getScenegraph()->addParticleSystem(psComicCloud);
	psComicCloud->start();

	//ParticleSystem* psFire = new ParticleSystem(glm::vec3(4, -0.5, 1), efFire);
	ParticleSystem* psFire = new ParticleSystem(glm::vec3(12, -0.5, 1), RESOURCES_PATH "/XML/Effect_Fire.xml");
	psScene.getScenegraph()->addParticleSystem(psFire);
	psFire->start();


	ParticleSystem* psFireworkRed = new ParticleSystem(glm::vec3(-6, 1, -2), RESOURCES_PATH "/XML/Effect_FireworkRed.xml");
	psScene.getScenegraph()->addParticleSystem(psFireworkRed);
	psFireworkRed->start();
	ParticleSystem* psFireworkBlue = new ParticleSystem(glm::vec3(-2, 1, -2), RESOURCES_PATH "/XML/Effect_FireworkBlue.xml");
	psScene.getScenegraph()->addParticleSystem(psFireworkBlue);
	psFireworkBlue->start();
	ParticleSystem* psFireworkGreen = new ParticleSystem(glm::vec3(2, 1, -2), RESOURCES_PATH "/XML/Effect_FireworkGreen.xml");
	psScene.getScenegraph()->addParticleSystem(psFireworkGreen);
	psFireworkGreen->start();
	ParticleSystem* psFireworkGold = new ParticleSystem(glm::vec3(6, 1, -2), RESOURCES_PATH "/XML/Effect_FireworkGold.xml");
	psScene.getScenegraph()->addParticleSystem(psFireworkGold);
	psFireworkGold->start();

	bool isStarted = false;
	bool bfirework = false;
	//////////////////////////////////////Other Things//////////////////////////////////////////

	//GUI
	initGUI();

	double startCamTime = glfwGetTime();

	int outputFrames = 0;
	float dTime;

	while (!glfwWindowShouldClose(window.getWindow()))
	{
		//CAM
		dTime = glfwGetTime() - startCamTime;
		cam.setSensitivity(dTime);
		startCamTime = glfwGetTime();

		if (useSnowButton->isActive()){
			//psSnow->start();
			bfirework = true;
		}
		else{
			psSnow->stop();
		}

		if (useStrongSnowButton->isActive()){
			psSnowStrong->start();
		}
		else{
			psSnowStrong->stop();
		}

		if (useRainButton->isActive()){
			psRain->start();
		}
		else{
			psRain->stop();
		}

		//update firework
		if (((int)glfwGetTime() % 7) < 1 && !isStarted && bfirework){
			isStarted = true;
			psFireworkRed->stop();
			psFireworkBlue->stop();
			psFireworkGreen->stop();
			psFireworkGold->stop();

			glm::vec3 pos = psFireworkRed->getPosition();
			psFireworkRed->setPosition(glm::vec3(pos.x, 1, pos.z));
			
			pos = psFireworkBlue->getPosition();
			psFireworkBlue->setPosition(glm::vec3(pos.x, 1, pos.z));

			pos = psFireworkGreen->getPosition();
			psFireworkGreen->setPosition(glm::vec3(pos.x, 1, pos.z));

			pos = psFireworkGold->getPosition();
			psFireworkGold->setPosition(glm::vec3(pos.x, 1, pos.z));

			psFireworkRed->start();
			psFireworkBlue->start();
			psFireworkGreen->start();
			psFireworkGold->start();
		}
		else if (((int)glfwGetTime() % 5) > 3 && isStarted){
			isStarted = false;
		}

		glm::vec3 pos = psFireworkRed->getPosition();
		psFireworkRed->setPosition(glm::vec3(pos.x, pos.y + (2 * dTime), pos.z));

		pos = psFireworkBlue->getPosition();
		psFireworkBlue->setPosition(glm::vec3(pos.x, pos.y + (2 * dTime), pos.z));

		pos = psFireworkGreen->getPosition();
		psFireworkGreen->setPosition(glm::vec3(pos.x, pos.y + (2 * dTime), pos.z));

		pos = psFireworkGold->getPosition();
		psFireworkGold->setPosition(glm::vec3(pos.x, pos.y + (2 * dTime), pos.z));


		//Maximum Particle
		if (!useMaximumParticle->isActive()){

			//SKYBOX
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			shaderSkybox.bind();
			glDisable(GL_DEPTH_TEST);
			shaderSkybox.sendMat4("viewMatrix", cam.getViewMatrix());
			shaderSkybox.sendMat4("projectionMatrix", cam.getProjectionMatrix());
			shaderSkybox.sendSkyboxTexture("testTexture", skybox.getSkyboxTexture());
			//skyboxNode.render();
			shaderSkybox.unbind();
			glEnable(GL_DEPTH_TEST);

			shaderObject.bind();
			shaderObject.sendMat4("viewMatrix", cam.getViewMatrix());
			shaderObject.sendMat4("projectionMatrix", cam.getProjectionMatrix());
			shaderObject.sendInt("useTexture", 1);
			psScene.render(shaderObject);
			shaderObject.unbind();

			//render ParticleSystems
			psScene.renderParticleSystems();
		}
		////////////////////////////////FINAL SCREEN EMITTER//////////////////////////////////////////////////
		else{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			//maximumParticle->update();
			//maximumParticle->render(cam);
		}

		//if (useFruitFliesButton->isActive()){
		//	//screenFruitFlies->update();
		//	//screenFruitFlies->render(screen);
		//}
		//else{
		//	//screenFruitFlies->startTime();
		//}

		////////////////////////////////BASICS///////////////////////////////////////////////////////

		renderer->renderGUI(*gui, window);

		//WINDOW
		window.swapAndPoll();

		//FPS
		if (!(outputFrames % 30)){
			outputFrames = 1;
			//std::cout << "FPS: " << static_cast<int> (1 / dTime) << "; Up to " << particleCount << " particles"<<std::endl;
		}
		outputFrames++;
	}
	glfwDestroyWindow(window.getWindow());
	glfwTerminate();

	return 0;
}