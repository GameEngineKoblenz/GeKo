//===================================================================//
//==================Your Includes===================================//
//==================================================================//
#include <GL/glew.h>

#include <GeKo_Graphics/InputInclude.h>
#include <GeKo_Graphics/GeometryInclude.h>
#include <GeKo_Graphics/Camera/StrategyCamera.h>
#include <GeKo_Graphics/Camera/Pilotview.h>
#include <GeKo_Graphics/ShaderInclude.h>

#include <GeKo_Graphics/AIInclude.h>
#include "GeKo_Gameplay/AI_Decisiontree/DecisionTree.h"
#include <GeKo_Gameplay/Object/Geko.h>
#include <GeKo_Gameplay/Object/AI.h>
#include "GeKo_Gameplay/Object/Coin.h"

#include <GeKo_Graphics/Geometry/AntMesh.h>
#include <GeKo_Graphics/Geometry/AntHomeMesh.h>
#include <GeKo_Graphics/Geometry/TreeMesh.h>
#include <GeKo_Graphics/Geometry/GekoMesh.h>
#include <GeKo_Graphics/Geometry/Plane.h>
#include <GeKo_Graphics/Geometry/ForestData.h>
#include <GeKo_Graphics/Geometry/FireplaceData.h>

#include <GeKo_Graphics/Geometry/Terrain.h>

#include <GeKo_Physics/CollisionTest.h>

#include <GeKo_Gameplay/Observer/ObjectObserver.h>
#include <GeKo_Gameplay/Observer/CollisionObserver.h>
#include <GeKo_Gameplay/Observer/GravityObserver.h>
#include <GeKo_Gameplay/Observer/SoundObserver.h>
#include <GeKo_Gameplay/Observer/QuestObserver.h>
#include <GeKo_Gameplay/Observer/HighscoreObserver.h>

#include <GeKo_Gameplay/Questsystem/QuestHandler_CVTag.h>
#include <Geko_Gameplay/Questsystem/Counter.h>

#include <GeKo_Gameplay/Object/AntHome.h>

#include <GeKo_Graphics/ParticleSystem/ParticleSystem.h>

//#include <GeKo_Graphics/GUI/GUI.h>
//#include <GeKo_Graphics/GUI/GUIComponents.hpp>
//#include <GeKo_Graphics/GUI/PlayerGUI.h>

//===================================================================//
//==================Things you need globally==========================//
//==================================================================//
Geko geko("Geko", glm::vec3(-10.0, 1.0, 30.0));
static StrategyCamera cam("PlayerViewCam");
static InputHandler iH;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int HUD_HEIGHT = 100;
const int HUD_WIDTH = 350;
const int QUEST_WIDTH = 325;
const int QUEST_HEIGHT = 300;
//
//GUI *hud;

//===================================================================//
//==================Callbacks for the Input==========================//
//==================================================================//

// As we do not use the Node class, we can't set the teapot into the middle of the window, therefore is the change of the direction improperly limited
// just needed in the player mode
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
	// The active InputMap is fetched
	std::map<int, std::function<void()>> activeMap = iH.getActiveInputMap()->getMap();
	// You go over the active InputMap, if it's the key that is pressed, a method is called and the mapped action is executed else the key is ignored
	for (std::map<int, std::function<void()>>::iterator it = activeMap.begin(); it != activeMap.end(); it++){
		if (it->first == key)
			activeMap.at(key)();
		if (it == activeMap.end())
			std::cout << "Key is not mapped to an action" << std::endl;
	}
}

//===================================================================//
//==================Main Method=====================================//
//==================================================================//
int main()
{

	std::cout << "STARTING DEMO: CVTag_Ants" << std::endl;

	SoundFileHandler sfh = SoundFileHandler(1000);
	sfh.generateSource("Lademusik", glm::vec3(0.0, 0.0, 0.0), RESOURCES_PATH "/Sound/jingle2.wav");
	sfh.setGain("Lademusik", 0.4f);
	sfh.playSource("Lademusik");
	//===================================================================//
	//==================Things you need to start with====================//
	//==================================================================//
	glfwInit();

	Window testWindow(50, 50, 900, 600, "Demo");
	glfwMakeContextCurrent(testWindow.getWindow());

	// Callback
	glfwSetKeyCallback(testWindow.getWindow(), key_callback);

	cam.setKeySpeed(2.0);
	cam.setNearFar(0.01, 10000);

	glewInit();

	OpenGL3Context context;
	//renderer = new Renderer(context);
	Renderer renderer(context);

	ResourceManager manager;

	//===================================================================//
	//==================Shaders for your program========================//
	//==================================================================//
	VertexShader vs(loadShaderSource(SHADERS_PATH + std::string("/Vertex-Shaders/TextureShader3D.vert")));
	FragmentShader fs(loadShaderSource(SHADERS_PATH + std::string("/Fragment-Shaders/TextureShader3D.frag")));
	ShaderProgram shader(vs, fs);

	VertexShader vsGBuffer(loadShaderSource(SHADERS_PATH + std::string("/GBuffer/GBuffer.vert")));
	FragmentShader fsGBuffer(loadShaderSource(SHADERS_PATH + std::string("/GBuffer/GBuffer.frag")));
	ShaderProgram shaderGBuffer(vsGBuffer, fsGBuffer);

	VertexShader vsSfq(loadShaderSource(SHADERS_PATH + std::string("/ScreenFillingQuad/ScreenFillingQuad.vert")));
	FragmentShader fsSfq(loadShaderSource(SHADERS_PATH + std::string("/ScreenFillingQuad/ScreenFillingQuad.frag")));
	ShaderProgram shaderSFQ(vsSfq, fsSfq);

	VertexShader vsSkybox(loadShaderSource(SHADERS_PATH + std::string("/SkyboxShader/SkyboxShader.vert")));
	FragmentShader fsSkybox(loadShaderSource(SHADERS_PATH + std::string("/SkyboxShader/SkyboxShader.frag")));
	ShaderProgram shaderSkybox(vsSkybox, fsSkybox);

	FBO fboGBuffer(WINDOW_WIDTH, WINDOW_HEIGHT, 3, true, false);


	Rect screenFillingQuad;
	screenFillingQuad.loadBufferData();

	std::cout << "SUCCESS: Load Shader & Buffer" << std::endl;

	//===================================================================//
	//================== PArticle System ================================ //
	//===================================================================//

	ParticleSystem* particle = new ParticleSystem(glm::vec3(0, 0, 0), (char*)RESOURCES_PATH "/XML/Effect_ComicCloud.xml");
	particle->m_type = ParticleType::FIGHT;
	ParticleSystem* particle2 = new ParticleSystem(glm::vec3(0, 0, 0), (char*)RESOURCES_PATH "/XML/SwarmOfFliesEffect.xml");
	particle2->m_type = ParticleType::SWARMOFFLIES;
	ParticleSystem* particleFire = new ParticleSystem(glm::vec3(0, 0, 0), (char*)RESOURCES_PATH "/XML/Effect_Fire.xml");
	particleFire->m_type = ParticleType::FIRE;
	ParticleSystem* particleFireWork = new ParticleSystem(glm::vec3(0, 0, 0), (char*)RESOURCES_PATH "/XML/Effect_FireworkBlue.xml");
	particleFireWork->m_type = ParticleType::FIREWORK;
	ParticleSystem* particleRain = new ParticleSystem(glm::vec3(0, 0, 0), (char*)RESOURCES_PATH "/XML/Effect_Rain.xml");
	particleFireWork->m_type = ParticleType::RAIN;

	/*Node particleNode("ParticleNode");
	particleNode.addParticleSystem(particle2);
	particleNode.setCamera(&cam);
	particleNode.setParticleActive(true);

	Node particleNodeFire("ParticleNodeFire");
	particleNodeFire.addParticleSystem(particleFire);
	particleNodeFire.setCamera(&cam);
	particleNodeFire.setParticleActive(true);*/

	std::cout << "SUCCESS: Load Particle" << std::endl;

	//===================================================================//
	//==================Object declarations - Geometry, Texture, Node=== //
	//==========================Object: Terrain===========================//

	StaticObject terrainObject;
	terrainObject.setClassType(ClassType::TERRAIN);

	Texture terrainTex((char*)RESOURCES_PATH "/Texture/Grass2.jpg");

	Terrain terrain2((char*)RESOURCES_PATH "/Texture/heightmap.jpg", 0.0f, 0.0f);
	Node terrainNode2("Terrain");
	terrainNode2.addGeometry(&terrain2);
	terrainNode2.addTexture(&terrainTex);
	terrainNode2.setObject(&terrainObject);

	std::cout << "SUCCESS: Load Terrain" << std::endl;

	//===================================================================//
	//==================Object declarations - Geometry, Texture, Node=== //
	//==========================Object: Player===========================//

	Texture texGeko((char*)RESOURCES_PATH "/Texture/Snake.jpg");

	auto gekoHandle = manager.loadStaticMesh(RESOURCES_PATH "/Geometry/Gecko.obj");
	auto gekoGeometry = gekoHandle.get().toGeometry();

	Node playerNode("Player");

	playerNode.addGeometry(&gekoGeometry);
	playerNode.setObject(&geko);
	playerNode.addTexture(&texGeko);

	geko.setPosition(glm::vec4(terrain2.getResolutionX() / 2.0f + 10.0f, 5.0f, terrain2.getResolutionY() / 2.0f + 10.0f, 1.0));

	playerNode.setCamera(&cam);

	std::cout << "SUCCESS: Load Player" << std::endl;


	//===================================================================//
	//==================Setting up the Sound==================//
	//==================================================================//
	sfh.generateSource(glm::vec3(geko.getPosition()), RESOURCES_PATH "/Sound/Rascheln.wav");
	geko.setSoundHandler(&sfh);
	geko.setSourceName(FIRESOUND, "Fire", RESOURCES_PATH "/Sound/Feuer_kurz.wav");
	geko.setSourceName(MOVESOUND, "SpielerFootsteps", RESOURCES_PATH "/Sound/Rascheln.wav");
	geko.setSourceName(BACKGROUNDMUSIC, "Hintergrund", RESOURCES_PATH "/Sound/Village.wav");
	geko.setSourceName(FIGHTSOUND, "Kampfsound", RESOURCES_PATH "/Sound/punch.wav");
	geko.setSourceName(EATSOUND, "Essen", RESOURCES_PATH "/Sound/Munching.wav");
	geko.setSourceName(QUESTSOUND, "Quest", RESOURCES_PATH "/Sound/jingle.wav");
	geko.setSourceName(ITEMSOUND, "Item", RESOURCES_PATH "/Sound/itempickup.wav");
	geko.setSourceName(LEVELUP, "Levelup", RESOURCES_PATH "/Sound/Levelup.wav");
	geko.setSourceName(RAIN, "Rain", RESOURCES_PATH "/Sound/lightrain.wav");
	geko.setSourceName(COIN, "Coin", RESOURCES_PATH "/Sound/CollectCoin.wav");
	geko.setSourceName(SECRET, "Secret", RESOURCES_PATH "/Sound/secretfound.wav");
	geko.setSourceName(CHOP, "Chop", RESOURCES_PATH "/Sound/ChopChop.wav");
	geko.setSourceName(BATTLEMUSIC, "Battle", RESOURCES_PATH "/Sound/Battle.wav");
	geko.updateSourcesInMap();
	sfh.setGain("Hintergrund", 0.15f);
	sfh.disableLooping("Chop");
	sfh.setGain("Chop", 7.0f);
	sfh.setPitch("Chop", 2.0f);
	sfh.disableLooping("Essen");
	sfh.disableLooping("Coin");
	sfh.disableLooping("Levelup");
	sfh.disableLooping("Quest");
	sfh.disableLooping("Item");
	sfh.disableLooping("Secret");
	sfh.disableLooping("Battle");
	sfh.setGain("Quest", 10.0);
	sfh.setPitch("Quest", 4.0);
	sfh.setGain("Kampfsound", 0.8f);
	sfh.updateListenerPosition(glm::vec3(geko.getPosition()));
	sfh.updateSourcePosition("Lademusik", glm::vec3(geko.getPosition()));

	std::cout << "SUCCESS: Load Sound" << std::endl;

	//===================================================================//
	//==================Setting up the Level and Scene==================//
	//==================================================================//

	Level testLevel("testLevel");
	Scene testScene("testScene");
	testLevel.addScene(&testScene);
	testLevel.changeScene("testScene");
	testLevel.setTerrain(&terrain2);

	//==================Add Camera to Scene============================//
	testScene.getScenegraph()->addCamera(&cam);
	testScene.getScenegraph()->setActiveCamera("PlayerViewCam");

	//==================Set Input-Maps and activate one================//
	iH.setAllInputMaps(*(testScene.getScenegraph()->getActiveCamera()));
	iH.changeActiveInputMap(MapType::OBJECT);
	iH.getActiveInputMap()->update(geko);

	//==================Add Objects to the Scene=======================//
	//==================Update the Bounding-Sphere 1st time============//
	/*Node translateNode("Translate");
	translateNode.addTranslation(glm::vec3(terrain2.getResolutionX() / 2.0f, 0.0, terrain2.getResolutionY() / 2.0f));*/

	testScene.getScenegraph()->getRootNode()->addChildrenNode(&terrainNode2);
	testScene.getScenegraph()->getRootNode()->addChildrenNode(&playerNode);
	//testScene.getScenegraph()->getRootNode()->addChildrenNode(&translateNode);
	//testScene.getScenegraph()->getRootNode()->getChildrenNode("Translate")->addChildrenNode(&playerNode);


	const char *textureNames[6] = {
		(char*)RESOURCES_PATH "/Skybox_Data/Sky/bluesky_left.jpg",
		(char*)RESOURCES_PATH "/Skybox_Data/Sky/bluesky_right.jpg",
		(char*)RESOURCES_PATH "/Skybox_Data/Sky/bluesky_top.jpg",
		(char*)RESOURCES_PATH "/Skybox_Data/Sky/bluesky_top.jpg",
		(char*)RESOURCES_PATH "/Skybox_Data/Sky/bluesky_back.jpg",
		(char*)RESOURCES_PATH "/Skybox_Data/Sky/bluesky_front.jpg"
		/*(char*)RESOURCES_PATH "/Color/testTex.png",
		(char*)RESOURCES_PATH "/Color/testTex.png",
		(char*)RESOURCES_PATH "/Color/testTex.png",
		(char*)RESOURCES_PATH "/Color/testTex.png",
		(char*)RESOURCES_PATH "/Color/testTex.png",
		(char*)RESOURCES_PATH "/Color/testTex.png" */
	};
	Skybox skybox = Skybox(textureNames);
	Node skyboxNode = Node("skybox");
	skyboxNode.addGeometry(&skybox);
	skyboxNode.addTranslation(glm::vec3(geko.getPosition()));
	skyboxNode.setModelMatrix(glm::scale(skyboxNode.getModelMatrix(), glm::vec3(200, 200, 200)));
	//skyboxNode.addTranslation(glm::vec3(terrain2.getResolutionX() / 2.0f, 0.0, terrain2.getResolutionY() / 2.0f));

	testScene.setSkyboxNode(&skyboxNode);

	//================== Particles ========================//
	testScene.getScenegraph()->addParticleSystem(particle);
	testScene.getScenegraph()->addParticleSystem(particle2);
	testScene.getScenegraph()->addParticleSystem(particleFire);
	testScene.getScenegraph()->addParticleSystem(particleFireWork);

	std::cout << "SUCCESS: Load Scene" << std::endl;

	//===================================================================//
	//==================Setting up the Observers========================//
	//==================================================================//

	ObjectObserver playerObserver(&testLevel);
	SoundObserver soundPlayerObserver(&testLevel);
	HighscoreObserver scoreObserver(&testLevel);
	geko.addObserver(&playerObserver);
	geko.addObserver(&soundPlayerObserver);
	geko.addObserver(&scoreObserver);

	std::cout << "SUCCESS: Load Observer" << std::endl;

	// ==============================================================
	// == Object (Forest) ==========================================
	// ==============================================================

	Texture texTree((char*)RESOURCES_PATH "/Texture/cookie.jpg");

	auto treeHandle = manager.loadStaticMesh(RESOURCES_PATH "/Geometry/Tree.ply");
	auto treeGeometry = treeHandle.get().toGeometry();

	glm::vec3 tmp;
	std::stringstream name;
	for (int i = 0; i<TreeData::forest1.size(); i++)
	{
		name << "Forest1Tree" << i;
		std::string stringname = name.str();
		StaticObject *treeStatic = new StaticObject();
		treeStatic->setTree(50 / TreeData::forest1.size());
		Node *treeNode = new Node(stringname);
		treeNode->addGeometry(&treeGeometry);
		treeNode->addTexture(&texTree);
		treeNode->setObject(treeStatic);
		tmp.x = TreeData::forest1[i].x;
		tmp.z = TreeData::forest1[i].z;
		TreeData::forest1[i].y = terrain2.getHeight(glm::vec2(tmp.x, tmp.z));
		tmp.y = TreeData::forest1[i].y;
		treeNode->addTranslation(tmp);
		treeNode->getStaticObject()->setPosition(tmp);
		treeNode->getBoundingSphere()->radius = 2.5;
		testScene.getScenegraph()->getRootNode()->addChildrenNode(treeNode);
		name.str("");
	}
	for (int i = 0; i<TreeData::forest2.size(); i++)
	{
		name << "Forest2Tree" << i;
		std::string stringname = name.str();
		StaticObject *treeStatic = new StaticObject();
		treeStatic->setTree(50 / TreeData::forest2.size());
		Node *treeNode = new Node(stringname);
		treeNode->addGeometry(&treeGeometry);
		treeNode->addTexture(&texTree);
		treeNode->setObject(treeStatic);
		tmp.x = TreeData::forest2[i].x;
		tmp.z = TreeData::forest2[i].z;
		TreeData::forest2[i].y = terrain2.getHeight(glm::vec2(tmp.x, tmp.z));
		tmp.y = TreeData::forest2[i].y;
		treeNode->addTranslation(tmp);
		treeNode->getStaticObject()->setPosition(tmp);
		treeNode->getBoundingSphere()->radius = 2.5;
		testScene.getScenegraph()->getRootNode()->addChildrenNode(treeNode);
		name.str("");
	}
	for (int i = 0; i<TreeData::foodTrees.size(); i++)
	{
		name << "FoodTree" << i;
		std::string stringname = name.str();
		StaticObject *treeStatic = new StaticObject();
		treeStatic->setTree();
		Node *treeNode = new Node(stringname);
		treeNode->addGeometry(&treeGeometry);
		treeNode->addTexture(&texTree);
		treeNode->setObject(treeStatic);
		tmp.x = TreeData::foodTrees[i].x;
		tmp.z = TreeData::foodTrees[i].z;
		TreeData::foodTrees[i].y = terrain2.getHeight(glm::vec2(tmp.x, tmp.z));
		tmp.y = TreeData::foodTrees[i].y;
		treeNode->addTranslation(tmp);
		treeNode->getStaticObject()->setPosition(tmp);
		treeNode->getBoundingSphere()->radius = 2.5;
		testScene.getScenegraph()->getRootNode()->addChildrenNode(treeNode);
		name.str("");
	}

	std::cout << "SUCCESS: Load ForestData" << std::endl;

	// ==============================================================
	// == Object (Anthome) ==========================================
	// ==============================================================

	Texture texAntHome((char*)RESOURCES_PATH "/Texture/antHome.jpg");

	auto antHomeHandler = manager.loadStaticMesh(RESOURCES_PATH "/Geometry/AntHome.ply");
	auto antHomeGeometry = antHomeHandler.get().toGeometry();

	glm::vec3 posAntHome(terrain2.getResolutionX() / 2.0f, 3.0, terrain2.getResolutionY() / 2.0f);

	std::vector<std::vector<glm::vec3>> posFoods;
	posFoods.push_back(TreeData::forest1);
	posFoods.push_back(TreeData::forest2);

	auto antHandler = manager.loadStaticMesh(RESOURCES_PATH "/Geometry/Ant.ply");
	auto antGeometry = antHandler.get().toGeometry();
	sfh.generateSource("tst", glm::vec3(geko.getPosition()), RESOURCES_PATH "/Sound/jingle2.wav");

	AntHome antHome(posAntHome, &sfh, antGeometry, &soundPlayerObserver, &playerObserver, testScene.getScenegraph()->getRootNode(), posFoods, &terrain2);

	Node homeNode("AntHome");
	
	homeNode.setObject(&antHome);
	homeNode.addTexture(&texAntHome);
	homeNode.addGeometry(&antHomeGeometry);
	homeNode.addTranslation(posAntHome);
	homeNode.getBoundingSphere()->radius = 0.5;

	testScene.getScenegraph()->getRootNode()->addChildrenNode(&homeNode);

	std::cout << "SUCCESS: Load AntHome" << std::endl;


	//===================================================================//
	//==================Setting up the Fireplace=========================//
	//==================================================================//	

	auto fireHandler = manager.loadStaticMesh(RESOURCES_PATH "/Geometry/fireplace.obj");
	auto fireGeometry = fireHandler.get().toGeometry();

	Texture texFire((char*)RESOURCES_PATH "/Texture/fireplace.png");

	std::string stringname;

	for (int i = 0; i<FireplaceData::fireplace.size(); i++)
	{
		name << "Fireplace" << i;
		stringname = name.str();
		StaticObject *fireStatic = new StaticObject();
		fireStatic->setObjectType(ObjectType::FIREPLACE);
		Node *fireplaceNode = new Node(stringname);
		fireplaceNode->addGeometry(&fireGeometry);
		fireplaceNode->addTexture(&texFire);
		fireplaceNode->setObject(fireStatic);
		name.str("");
		tmp.x = FireplaceData::fireplace[i].x;
		tmp.z = FireplaceData::fireplace[i].z;
		FireplaceData::fireplace[i].y = terrain2.getHeight(glm::vec2(tmp.x, tmp.z));
		tmp.y = FireplaceData::fireplace[i].y;
		fireplaceNode->addTranslation(tmp);
		fireplaceNode->addScale(0.4, 0.4, 0.4);
		fireplaceNode->getStaticObject()->setPosition(tmp);
		fireplaceNode->getBoundingSphere()->radius = 0.5;
		testScene.getScenegraph()->getRootNode()->addChildrenNode(fireplaceNode);
		name.str("");
	}

	//===================================================================//
	//========================= Set the coins ===========================//
	//==================================================================//

	auto coinHandler = manager.loadStaticMesh(RESOURCES_PATH "/Geometry/Goldcoin2.obj");
	auto coinGeometry = coinHandler.get().toGeometry();

	Texture texCoin((char*)RESOURCES_PATH "/Texture/Goldcoin.png");

	Coin *coinFactory = new Coin(&terrain2, &coinGeometry, &texCoin, testLevel.getActiveScene()->getScenegraph()->getRootNode());
	Coin *coinFactory2 = new Coin(&terrain2, &coinGeometry, &texCoin, testLevel.getActiveScene()->getScenegraph()->getRootNode());
	Coin *coinFactory3 = new Coin(&terrain2, &coinGeometry, &texCoin, testLevel.getActiveScene()->getScenegraph()->getRootNode());
	Coin *coinFactory4 = new Coin(&terrain2, &coinGeometry, &texCoin, testLevel.getActiveScene()->getScenegraph()->getRootNode());
	Coin *coinFactory5 = new Coin(&terrain2, &coinGeometry, &texCoin, testLevel.getActiveScene()->getScenegraph()->getRootNode());
	Coin *coinFactory6 = new Coin(&terrain2, &coinGeometry, &texCoin, testLevel.getActiveScene()->getScenegraph()->getRootNode());

	//===================================================================//
	//======================== Set the Flowers ==========================//
	//==================================================================//

	auto flowerHandler = manager.loadStaticMesh(RESOURCES_PATH "/Geometry/RedFlower.obj");
	auto flowerGeometry = flowerHandler.get().toGeometry();

	Texture texFlower((char*)RESOURCES_PATH "/Texture/RedFlowers.png");

	for (int i = 0; i<TreeData::Flower.size(); i++)
	{
		name << "Flower2Wiese" << i;
		stringname = name.str();
		StaticObject *flowerStatic = new StaticObject();
		flowerStatic->setTree(50 / TreeData::Flower.size());
		flowerStatic->setObjectType(ObjectType::FLOWER);
		Node *flowerNode = new Node(stringname);
		flowerNode->addGeometry(&flowerGeometry);
		flowerNode->addTexture(&texFlower);
		flowerNode->setObject(flowerStatic);
		name.str("");
		tmp.x = TreeData::Flower[i].x;
		tmp.z = TreeData::Flower[i].z;
		TreeData::Flower[i].y = terrain2.getHeight(glm::vec2(tmp.x, tmp.z));
		tmp.y = TreeData::Flower[i].y;
		flowerNode->addTranslation(tmp);
		flowerNode->getStaticObject()->setPosition(tmp);
		flowerNode->getBoundingSphere()->radius = 0.5;
		testScene.getScenegraph()->getRootNode()->addChildrenNode(flowerNode);
		name.str("");
	}

	//===================================================================//
	//==================Setting up the Collision=========================//
	//==================================================================//

	testLevel.getCollision()->collectNodes(testScene.getScenegraph()->getRootNode());

	CollisionObserver colObserver(&testLevel);
	testLevel.getCollision()->addObserver(&colObserver);
	testLevel.getCollision()->addObserver(&soundPlayerObserver);

	GravityObserver gravityObserver(&testLevel);
	testLevel.getCollision()->addObserver(&gravityObserver);

	//===================================================================//
	//==================Setting up the Gravity===========================//
	//==================================================================//

	Gravity gravity;
	playerNode.addGravity(&gravity);

	std::cout << "SUCCESS: Load Physic (Collision & Gravity)" << std::endl;

	// ==============================================================
	// == Questsystem ===============================================
	// ==============================================================
	//Reihenfolge wichtig! Erst questHandler zu level hinzufügen, dann generaten
	QuestObserver questObserver(&testLevel);
	QuestHandler_CVTag questhandler;
	testLevel.setQuestHandler(&questhandler);
	questhandler.generateQuests(&questObserver, TreeData::Flower);
	testLevel.getFightSystem()->addObserver(&questObserver);

	std::cout << "SUCCESS: Load Questsystem" << std::endl;

	//===================================================================//
	//================== Setting up the playerGUI ========================//
	//==================================================================//

	PlayerGUI playerGUI(HUD_WIDTH+50, HUD_HEIGHT, WINDOW_HEIGHT, WINDOW_WIDTH, QUEST_HEIGHT, QUEST_WIDTH, playerNode.getPlayer(), testLevel.getQuestHandler(), testLevel.getHighscore());
	testLevel.setGUI(&playerGUI);

	std::cout << "SUCCESS: Load GUI" << std::endl;

	//===================================================================//
	//==================The Render-Loop==================================//
	//==================================================================//
	float lastTime = glfwGetTime();


	//TODO adjust the Rotation,to match the Terrain
	glm::vec4 tmpPos;
	glm::vec3 normalFromTerrain;
	glm::vec3 rotateAxis;
	glm::vec4 viewDirFromPlayer;
	glm::vec3 up(0.0, 1.0, 0.0);
	float lengthFromNormal;
	float lengthFromUp;
	float phi;
	glm::vec4 tangente;
	float dot;
	
	Counter counter;
	counter.setTime(60* 30);
	bool fireWorkStarted = false;
	bool isRaining = false;


	sfh.stopSource("Lademusik");
	sfh.playSource("Hintergrund");

	//std::vector<ParticleSystem*>* ps = testLevel.getActiveScene()->getScenegraph()->getParticleSet();
	//for (auto particle : *ps)
	//{
	//	if (particle->m_type == ParticleType::FIREWORK)
	//	{
	//		particle->setPosition(glm::vec3(geko.getPosition()));
	//		particle->start();
	//		particle->update(*testLevel.getActiveScene()->getScenegraph()->getActiveCamera());
	//		particle->render(*testLevel.getActiveScene()->getScenegraph()->getActiveCamera());
	//	}
	//}

	counter.start();
	sfh.playSource("Rain");
	while (!glfwWindowShouldClose(testWindow.getWindow()))
	{

		if (counter.getTime() > 0)
		{
			counter.update();
			if (!geko.getStates(States::HEALTH)){
			
				counter.end();
			}

		if (geko.getStates(States::HEALTH)){
			//===================================================================//
			//==================Update your Objects per Frame here =============//
			//==================================================================//

			testScene.getScenegraph()->searchNode("Player")->setIdentityMatrix_Rotation();
			//testScene.getScenegraph()->searchNode("Player")->setIdentityMatrix_Translate();

			float currentTime = glfwGetTime();
			float deltaTime = currentTime - lastTime;
			lastTime = currentTime;

			testLevel.getCollision()->update();

			coinFactory->update();
			coinFactory2->update();
			coinFactory3->update();
			coinFactory4->update();
			coinFactory5->update();
			coinFactory6->update();

			geko.update();
			geko.setDeltaTime(currentTime);

			tmpPos = testScene.getScenegraph()->searchNode("Player")->getPlayer()->getPosition();

			viewDirFromPlayer = glm::normalize(testScene.getScenegraph()->searchNode("Player")->getPlayer()->getViewDirection());

			//ToDo calculate Normal funktioniert evtl falsch
			normalFromTerrain = glm::normalize(terrain2.calculateNormal(tmpPos.x, tmpPos.z));
			rotateAxis = glm::cross(glm::vec3(normalFromTerrain), up);
			lengthFromNormal = glm::length(normalFromTerrain);
			lengthFromUp = glm::length(up);
			up = glm::normalize(up);
			dot = glm::dot(normalFromTerrain, up);

			phi = glm::acos(dot / (lengthFromNormal * lengthFromUp));
			phi = phi * (180 / glm::pi<float>());

			if (dot <0.99)
				testScene.getScenegraph()->searchNode("Player")->addRotation(-phi, rotateAxis);

			testScene.getScenegraph()->searchNode("Player")->getPlayer()->setPosition(testScene.getScenegraph()->searchNode("Player")->getPlayer()->getPosition() + glm::vec4(normalFromTerrain * 0.2f, 1.0));
			antHome.updateAnts();
		
			testScene.getScenegraph()->searchNode("Player")->addRotation(testScene.getScenegraph()->searchNode("Player")->getPlayer()->getPhi(), glm::vec3(0, -1, 0));
		}

		}
		//===================================================================//
		//==================Render your Objects==============================//
		//==================================================================//
		//renderer.renderScene(testScene, testWindow);
		if (counter.getTime() <= 0)
		{
			if (!fireWorkStarted){
				fireWorkStarted = true;
				playerGUI.setTimeOver();
				particleFireWork->setPosition(glm::vec3(geko.getPosition()));
				particleFireWork->start();
				particleFireWork->update(*testLevel.getActiveScene()->getScenegraph()->getActiveCamera());
				particleFireWork->render(*testLevel.getActiveScene()->getScenegraph()->getActiveCamera());
			}
		}
		/*if (((int)glfwGetTime() % 7) < 1 && !fireWorkStarted && counter.getTime() <= 0){
			playerGUI.setTimeOver();

			fireWorkStarted = true;

			particleFireWork->setPosition(glm::vec3(geko.getPosition()));

			particleFireWork->start();
			particleFireWork->update(*testLevel.getActiveScene()->getScenegraph()->getActiveCamera());
			particleFireWork->render(*testLevel.getActiveScene()->getScenegraph()->getActiveCamera());

		}
		else if (((int)glfwGetTime() % 5) > 3 && fireWorkStarted && counter.getTime() <= 0){
			fireWorkStarted = false;
			particleFireWork->stop();
		}*/
		if (sfh.sourceIsPlaying("Rain"))
		{
			if (!isRaining){
				//renderer.useReflections(true, reflectionStrength);
				isRaining = true;
				particleRain->setPosition(glm::vec3(geko.getPosition()));
				particleRain->start();
				particleRain->update(*testLevel.getActiveScene()->getScenegraph()->getActiveCamera());
				particleRain->render(*testLevel.getActiveScene()->getScenegraph()->getActiveCamera());
			}
		}
		playerGUI.receiveFramesLeft(counter.getTime());
		playerGUI.update();
		renderer.renderScene(testScene, testWindow);
		renderer.renderGUI(*playerGUI.getHUD(), testWindow);


		testScene.getScenegraph()->searchNode("Player")->getPlayer()->setPosition(testScene.getScenegraph()->searchNode("Player")->getPlayer()->getPosition() - glm::vec4(normalFromTerrain * 0.2f, 1.0));
		//std::cout << "FPS " << 1 / deltaTime << std::endl;

		glfwSwapBuffers(testWindow.getWindow());
		if (counter.getTime() > 0)
			glfwPollEvents();

	}



	glfwDestroyWindow(testWindow.getWindow());
	glfwTerminate();


	return 0;

}

