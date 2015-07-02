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

#include <GeKo_Graphics/Geometry/AntMesh.h>
#include <GeKo_Graphics/Geometry/AntHomeMesh.h>
#include <GeKo_Graphics/Geometry/TreeMesh.h>
#include <GeKo_Graphics/Geometry/GekoMesh.h>
#include <GeKo_Graphics/Geometry/Plane.h>
#include <GeKo_Graphics/Geometry/ForestData.h>

#include <GeKo_Graphics/Geometry/Terrain.h>

#include <GeKo_Physics/CollisionTest.h>

#include <GeKo_Gameplay/Observer/ObjectObserver.h>
#include <GeKo_Gameplay/Observer/CollisionObserver.h>
#include <GeKo_Gameplay/Observer/GravityObserver.h>
#include <GeKo_Gameplay/Observer/SoundObserver.h>
#include <GeKo_Gameplay/Observer/QuestObserver.h>
#include <GeKo_Gameplay/Observer/HighscoreObserver.h>

#include <GeKo_Gameplay/Questsystem/ItemReward.h>
#include <GeKo_Gameplay/Questsystem/ExpReward.h>
#include <GeKo_Gameplay/Questsystem/Goal_Kill.h>
#include <GeKo_Gameplay/Questsystem/Goal_Eaten.h>

#include <GeKo_Gameplay/Object/AntHome.h>

#include <GeKo_Graphics/ParticleSystem/ParticleSystem.h>


//===================================================================//
//==================Things you need globally==========================//
//==================================================================//
Geko geko("Geko", glm::vec3(-10.0, 1.0, 30.0));
static StrategyCamera cam("PlayerViewCam");
static InputHandler iH;

float *reflectionStrength;
float *blurStrength;
float *focusDepth;
float *bloomStrength;
int *useShadowMode;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int HUD_HEIGHT = 100;
const int HUD_WIDTH = 350;
const int QUEST_WIDTH = 325;
const int QUEST_HEIGHT = 300;

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
	Renderer renderer(context);

	//===================================================================//
	//==================Resources like Textures and Geometry============ //
	//==================================================================//
	ResourceManager manager;

	auto gekoHandle = manager.loadStaticMesh(RESOURCES_PATH "/Geko.ply");
	auto gekoGeometry = gekoHandle.get().toGeometry();

	auto treeHandle = manager.loadStaticMesh(RESOURCES_PATH "/Tree.ply");
	auto treeGeometry = treeHandle.get().toGeometry();

	auto antHomeHandler = manager.loadStaticMesh(RESOURCES_PATH "/AntHome.ply");
	auto antHomeGeometry = antHomeHandler.get().toGeometry();

	auto antHandler = manager.loadStaticMesh(RESOURCES_PATH "/Ant.ply");
	auto antGeometry = antHandler.get().toGeometry();

	auto bushHandler = manager.loadStaticMesh(RESOURCES_PATH "/Bush2.obj");
	auto bushGeometry = bushHandler.get().toGeometry();


	Texture terrainTex((char*)RESOURCES_PATH "/Texture/Grass2.jpg");
	Texture texGeko((char*)RESOURCES_PATH "/Texture/Snake.jpg");
	Texture texTree((char*)RESOURCES_PATH "/Texture/cookie.jpg");
	Texture texAnt((char*)RESOURCES_PATH "/Texture/ant.jpg");
	Texture texAntHome((char*)RESOURCES_PATH "/Texture/antHome.jpg");
	Texture texAnt2((char*)RESOURCES_PATH "/Texture/ant2.jpg");
	Texture texStamm((char*)RESOURCES_PATH "/Texture/bark_loo.jpg");
	Texture texLeaf((char*)RESOURCES_PATH "/Texture/Grass.jpg");

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

	//===================================================================//
	//================== Particle System ================================ //
	//===================================================================//


	//===================================================================//
	//==================Object declarations - Geometry, Texture, Node=== //
	//==========================Object: Terrain===========================//

	StaticObject terrainObject;
	terrainObject.setClassType(ClassType::TERRAIN);

	Terrain terrain2((char*)RESOURCES_PATH "/heightmap.jpg", 0.0f, 0.0f);
	Node terrainNode2("Terrain");
	terrainNode2.addGeometry(&terrain2);
	terrainNode2.addTexture(&terrainTex);
	terrainNode2.setObject(&terrainObject);



	//===================================================================//
	//==================Object declarations - Geometry, Texture, Node=== //
	//==========================Object: Player===========================//

	GekoMesh gekoMesh;
	geko.setLevelThreshold(100.0);
	geko.setExp(0.0);
	geko.setLevel(0);

	Node playerNode("Player");

	playerNode.addGeometry(&gekoGeometry);
	playerNode.setObject(&geko);
	playerNode.addTexture(&texGeko);
	playerNode.setCamera(&cam);

	geko.setPosition(glm::vec4(terrain2.getResolutionX() / 2.0f + 10.0f, 5.0f, terrain2.getResolutionY() / 2.0f + 10.0f, 1.0));
	sfh.generateSource(glm::vec3(geko.getPosition()), RESOURCES_PATH "/Sound/Rascheln.wav");
	geko.setSoundHandler(&sfh);
	geko.setSourceName(FIRESOUND, "Fire", RESOURCES_PATH "/Sound/Feuer_kurz.wav");
	geko.setSourceName(MOVESOUND, "SpielerFootsteps", RESOURCES_PATH "/Sound/Rascheln.wav");
	geko.setSourceName(BACKGROUNDMUSIC, "Hintergrund", RESOURCES_PATH "/Sound/cantina.wav");
	geko.setSourceName(FIGHTSOUND, "Kampfsound", RESOURCES_PATH "/Sound/punch.wav");
	geko.setSourceName(EATSOUND, "Essen", RESOURCES_PATH "/Sound/Munching.wav");
	geko.setSourceName(QUESTSOUND, "Quest", RESOURCES_PATH "/Sound/jingle.wav");
	geko.setSourceName(ITEMSOUND, "Item", RESOURCES_PATH "/Sound/itempickup.wav");
	geko.setSourceName(LEVELUP, "Levelup", RESOURCES_PATH "/Sound/Levelup.wav");
	geko.setSourceName(RAIN, "Rain", RESOURCES_PATH "/Sound/lightrain.wav");
	geko.updateSourcesInMap();
	sfh.setGain("Hintergrund", 0.15f);
	sfh.disableLooping("Essen");
	sfh.disableLooping("Levelup");
	sfh.disableLooping("Quest");
	sfh.disableLooping("Item");
	sfh.setGain("Quest", 10.0);
	sfh.setPitch("Quest", 4.0);
	sfh.setGain("Kampfsound", 0.8f);
	sfh.updateListenerPosition(glm::vec3(geko.getPosition()));
	sfh.updateSourcePosition("Lademusik", glm::vec3(geko.getPosition()));

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

	//==================Skybox=======================//

	const char *textureNames[6] = {
		(char*)RESOURCES_PATH "/Skybox_Data/Sky/bluesky_left.jpg",
		(char*)RESOURCES_PATH "/Skybox_Data/Sky/bluesky_right.jpg",
		(char*)RESOURCES_PATH "/Skybox_Data/Sky/bluesky_top.jpg",
		(char*)RESOURCES_PATH "/Skybox_Data/Sky/bluesky_top.jpg",
		(char*)RESOURCES_PATH "/Skybox_Data/Sky/bluesky_back.jpg",
		(char*)RESOURCES_PATH "/Skybox_Data/Sky/bluesky_front.jpg"
	};
	Skybox skybox = Skybox(textureNames);
	Node skyboxNode = Node("skybox");
	skyboxNode.addGeometry(&skybox);
	skyboxNode.addTranslation(glm::vec3(geko.getPosition()));
	skyboxNode.setModelMatrix(glm::scale(skyboxNode.getModelMatrix(), glm::vec3(200, 200, 200)));

	//==================Add Objects to the Scene=======================//
	testScene.setSkyboxNode(&skyboxNode);
	testScene.getScenegraph()->getRootNode()->addChildrenNode(&terrainNode2);
	testScene.getScenegraph()->getRootNode()->addChildrenNode(&playerNode);

	//===================================================================//
	//==================Setting up the Observers========================//
	//==================================================================//

	ObjectObserver playerObserver(&testLevel);
	SoundObserver soundPlayerObserver(&testLevel);
	HighscoreObserver scoreObserver(&testLevel);
	geko.addObserver(&playerObserver);
	geko.addObserver(&scoreObserver);
	geko.addObserver(&soundPlayerObserver);

	// ==============================================================
	// == Object (Forest) ==========================================
	// ==============================================================

	glm::vec3 tmp;
	std::stringstream name;
	for (int j = 0; j < 10; j++)
	{
		for (int i = 0; i < TreeData::forestShort.size(); i++)
		{
			name << "Forest1Tree" << i;
			std::string stringname = name.str();
			StaticObject *treeStatic = new StaticObject();
			treeStatic->setTree(0);
			Node *treeNode = new Node(stringname);
			treeNode->addGeometry(&treeGeometry);
			treeNode->addTexture(&texTree);
			treeNode->setObject(treeStatic);
			tmp.x = TreeData::forestShort[i].x + (j * 20);
			tmp.z = TreeData::forestShort[i].z + (j * 10);
			tmp.y = terrain2.getHeight(glm::vec2(tmp.x, tmp.z));
			treeNode->addTranslation(tmp);
			treeNode->getStaticObject()->setPosition(tmp);
			treeNode->getBoundingSphere()->radius = 0.00001;
			testScene.getScenegraph()->getRootNode()->addChildrenNode(treeNode);
			name.str("");
		}
	}
	std::string stringname;
	for (int i = 0; i<TreeData::foodTrees.size(); i++)
	{
		name << "Forest2Tree" << i;
		stringname = name.str();
		StaticObject *treeStatic = new StaticObject();
		treeStatic->setTree(50 / TreeData::foodTrees.size());
		Node *treeNode = new Node(stringname);
		treeNode->addGeometry(&treeGeometry);
		treeNode->addTexture(&texTree);
		treeNode->setObject(treeStatic);
		name.str("");
		name << "Bush" << i;
		stringname = name.str();
		Node *bushNode = new Node(stringname);
		bushNode->addGeometry(&bushGeometry);
		bushNode->addTexture(&texLeaf);
		treeNode->addChildrenNode(bushNode);
		bushNode->addScale(1.0, 1.0, 1.0);
		tmp.x = TreeData::foodTrees[i].x;
		tmp.z = TreeData::foodTrees[i].z;
		tmp.y = terrain2.getHeight(glm::vec2(tmp.x, tmp.z));
		treeNode->addTranslation(tmp);
		treeNode->getStaticObject()->setPosition(tmp);
		treeNode->getBoundingSphere()->radius = 1.5;
		testScene.getScenegraph()->getRootNode()->addChildrenNode(treeNode);
		name.str("");
	}

	// ==============================================================
	// == Object (Anthome) ==========================================
	// ==============================================================

	glm::vec3 posFood2((terrain2.getResolutionX() / 2.0f) + 10.0, 0.0, (terrain2.getResolutionY() / 2.0f) - 5.0);
	glm::vec3 posSpawn(terrain2.getResolutionX() / 2.0f, 3.0, terrain2.getResolutionY() / 2.0f);
	glm::vec3 posDefaultPlayer(0.0, 0.0, 0.0);
	AntMesh antMesh;

	Graph<AStarNode, AStarAlgorithm>* antAfraidGraph = new Graph<AStarNode, AStarAlgorithm>();
	std::vector<std::vector<glm::vec3>> possFoods;
	possFoods.push_back(TreeData::foodTrees);
	antAfraidGraph->setExampleAntAfraid2(posSpawn, possFoods);

	sfh.generateSource("tst", glm::vec3(geko.getPosition()), RESOURCES_PATH "/Sound/jingle2.wav");
	AntHome antHome(posSpawn, &sfh, antGeometry, &soundPlayerObserver, &playerObserver, &texAnt2, &texAnt, antAfraidGraph);
	antHome.setGrapHighOnTerrain(&terrain2);

	antHome.setAntScale(0.5);
	antHome.generateWorkers(1, testScene.getScenegraph()->getRootNode());
	antHome.generateGuards(1, testScene.getScenegraph()->getRootNode());

	Node homeNode("AntHome");

	homeNode.setObject(&antHome);
	homeNode.addTexture(&texAntHome);
	homeNode.addGeometry(&antHomeGeometry);
	homeNode.addTranslation(posSpawn);
	homeNode.getBoundingSphere()->radius = 0.5;

	testScene.getScenegraph()->getRootNode()->addChildrenNode(&homeNode);


	//===================================================================//
	//==================Setting up the Collision=========================//
	//==================================================================//

	CollisionTest collision;
	collision.collectNodes(testScene.getScenegraph()->getRootNode());

	CollisionObserver colObserver(&testLevel);
	collision.addObserver(&colObserver);
	collision.addObserver(&soundPlayerObserver);

	GravityObserver gravityObserver(&testLevel);
	collision.addObserver(&gravityObserver);

	//===================================================================//
	//==================Setting up the Gravity===========================//
	//==================================================================//

	Gravity gravity;
	playerNode.addGravity(&gravity);

	// ==============================================================
	// == Questsystem ===============================================
	// ==============================================================
	QuestHandler questhandler;

	Quest questKillAnt(1);

	questKillAnt.setDescription("Kill one worker ant.");

	Goal_Kill killAnt(1);

	questKillAnt.addGoal(&killAnt);

	ExpReward expReward(1);
	expReward.setExp(100);

	questKillAnt.addReward(&expReward);

	QuestGraph questGraph;
	QuestGraphNode nodeStart;
	nodeStart.setQuest(&questKillAnt);
	questGraph.addNode(&nodeStart);
	questKillAnt.setActive(true);

	testLevel.getQuestHandler()->addQuest(&questKillAnt);

	testLevel.getQuestHandler()->setGraph(&questGraph);

	QuestObserver questObserver(&testLevel);

	questKillAnt.addObserver(&questObserver);
	questKillAnt.addObserver(&scoreObserver);
	questKillAnt.addObserver(&soundPlayerObserver);

	killAnt.addObserver(&questObserver);
	killAnt.addObserver(&scoreObserver);

	testLevel.getFightSystem()->addObserver(&questObserver);
	testLevel.getFightSystem()->addObserver(&scoreObserver);


	//===================================================================//
	//================== Setting up the playerGUI and Highscore============//
	//==================================================================//


	PlayerGUI playerGUI(HUD_WIDTH, HUD_HEIGHT, WINDOW_HEIGHT, WINDOW_WIDTH, QUEST_HEIGHT, QUEST_WIDTH, playerNode.getPlayer(), testLevel.getQuestHandler(), testLevel.getHighscore());
	testLevel.setGUI(&playerGUI);

	//================== Particles ========================//

	//===================================================================//
	//==================The Graphics and Sun==================================//
	//==================================================================//
	ConeLight slight(glm::vec4(geko.getPosition().x, 40.0f, geko.getPosition().z, 1.0), glm::vec4(0.7, 0.7, 0.7, 1.0), true, glm::vec3(0.0, -1.0, -1.0), 200.0f, 2.0f, 0.0f);

	reflectionStrength = new float(0.25f);
	focusDepth = new float(1.0);
	bloomStrength = new float(0.20f);
	useShadowMode = new int(3);
	bool useReflection = false;

	//Sonne: Version 1.0
	float yoffset = 4.0;
	slight.m_angle = 272.0f;
	slight.m_radius = 14.2f;
	slight.m_exponent = 1.2f;
	slight.m_direction = glm::vec3(0.1, -0.9, 0.0);
	slight.m_position = glm::vec4(posSpawn.x, 20.0f, posSpawn.z, 1.0);
	//Die Lichtposition auf einer Kugel bewegen 
	float theta = 45.0f;
	float phi2 = 0.0f;
	float r = 90.0f;
	slight.m_position = glm::vec4(r * glm::sin(theta) * glm::cos(phi2), r * glm::cos(theta), r * glm::sin(theta) * glm::cos(phi2), 1.0);

	//===================================================================//
	//==================The Render-Loop and Let the Music begin============//
	//==================================================================//
	float lastTime = glfwGetTime();

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

	sfh.stopSource("Lademusik");
	sfh.playSource("Hintergrund");

	while (!glfwWindowShouldClose(testWindow.getWindow()))
	{

		testScene.getScenegraph()->searchNode("Player")->setIdentityMatrix_Rotation();

		float currentTime = glfwGetTime();
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;


		//===================================================================//
		//==================Graphics activate =============//
		//==================================================================//

		renderer.useAntiAliasing(true);
		renderer.useBloom(true, bloomStrength);
		renderer.useDoF(true, focusDepth);
		renderer.useShadowMapping(true, useShadowMode, &slight);
		if (glfwGetKey(testWindow.getWindow(), GLFW_KEY_R))
		{
			useReflection = true;
			sfh.playSource("Rain");
		}
		if (glfwGetKey(testWindow.getWindow(), GLFW_KEY_T))
		{
			useReflection = false;
			sfh.stopSource("Rain");
		}
		if (useReflection)
		{
			renderer.useReflections(true, reflectionStrength);
		}
		else{
			renderer.useReflections(false, reflectionStrength);
		}


		//===================================================================//
		//==================Update your Objects per Frame here =============//
		//==================================================================//
		collision.update();

		//===================================================================//
		//==================Input and update for the Player==================//
		//==================================================================//

		geko.update();
		geko.setDeltaTime(currentTime);

		tmpPos = testScene.getScenegraph()->searchNode("Player")->getPlayer()->getPosition();

		viewDirFromPlayer = glm::normalize(testScene.getScenegraph()->searchNode("Player")->getPlayer()->getViewDirection());

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
		//===================================================================//
		//==================Render your Objects==============================//
		//==================================================================//
		playerGUI.update();
		renderer.renderScene(testScene, testWindow);
		renderer.renderGUI(*playerGUI.getHUD(), testWindow);


		testScene.getScenegraph()->searchNode("Player")->getPlayer()->setPosition(testScene.getScenegraph()->searchNode("Player")->getPlayer()->getPosition() - glm::vec4(normalFromTerrain * 0.2f, 1.0));

		//Für sonne
		slight.m_position = glm::vec4(playerNode.getPlayer()->getPosition().x, slight.m_position.y, playerNode.getPlayer()->getPosition().z, 1.0);

		glfwSwapBuffers(testWindow.getWindow());
		glfwPollEvents();
		std::cout << "R" << slight.m_radius << ", Angle " << slight.m_angle << ", Expo" << slight.m_exponent << std::endl;
	}



	glfwDestroyWindow(testWindow.getWindow());
	glfwTerminate();


	return 0;

}