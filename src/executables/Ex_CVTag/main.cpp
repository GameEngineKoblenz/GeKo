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

#include <GeKo_Gameplay/Questsystem/ItemReward.h>
#include <GeKo_Gameplay/Questsystem/ExpReward.h>
#include <GeKo_Gameplay/Questsystem/Goal_Kill.h>
#include <GeKo_Gameplay/Questsystem/Goal_Eaten.h>

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

	//===================================================================//
	//================== PArticle System ================================ //
	//===================================================================//

	/*ParticleSystem* particle = new ParticleSystem(glm::vec3(0, 0, 0), (char*)RESOURCES_PATH "/XML/Effect_ComicCloud.xml");
	particle->m_type = ParticleType::FIGHT;
	ParticleSystem* particle2 = new ParticleSystem(glm::vec3(0, 0, 0), (char*)RESOURCES_PATH "/XML/SwarmOfFliesEffect.xml");
	particle2->m_type = ParticleType::SWARMOFFLIES;
	ParticleSystem* particleFire = new ParticleSystem(glm::vec3(0, 0, 0), (char*)RESOURCES_PATH "/XML/Effect_Fire.xml");
	particleFire->m_type = ParticleType::FIRE;

	Node particleNode("ParticleNode");
	particleNode.addParticleSystem(particle2);
	particleNode.setCamera(&cam);
	particleNode.setParticleActive(true);

	Node particleNodeFire("ParticleNodeFire");
	particleNodeFire.addParticleSystem(particleFire);
	particleNodeFire.setCamera(&cam);
	particleNodeFire.setParticleActive(true);*/

	//===================================================================//
	//==================Object declarations - Geometry, Texture, Node=== //
	//==========================Object: Terrain===========================//

	StaticObject terrainObject;
	terrainObject.setClassType(ClassType::TERRAIN);

	Texture terrainTex((char*)RESOURCES_PATH "/Texture/Grass2.jpg");

	Terrain terrain2((char*)RESOURCES_PATH "/heightmap.jpg", 0.0f, 0.0f);
	Node terrainNode2("Terrain");
	terrainNode2.addGeometry(&terrain2);
	terrainNode2.addTexture(&terrainTex);
	terrainNode2.setObject(&terrainObject);


	//===================================================================//
	//==================Object declarations - Geometry, Texture, Node=== //
	//==========================Object: Player===========================//

	Texture texGeko((char*)RESOURCES_PATH "/Texture/Snake.jpg");

	auto gekoHandle = manager.loadStaticMesh(RESOURCES_PATH "/Geko.ply");
	auto gekoGeometry = gekoHandle.get().toGeometry();

	GekoMesh gekoMesh;
	geko.setLevelThreshold(100.0);
	geko.setExp(0.0);
	geko.setLevel(0);

	Node playerNode("Player");

	playerNode.addGeometry(&gekoGeometry);
	playerNode.setObject(&geko);
	playerNode.addTexture(&texGeko);

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
	//sfh.playSource("Levelup");
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
	//sfh.generateSource("Feuer",posFood, RESOURCES_PATH "/Sound/Feuer kurz.wav");
	playerNode.setCamera(&cam);

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
	/*testScene.getScenegraph()->addParticleSystem(particle);
	testScene.getScenegraph()->addParticleSystem(particle2);
	testScene.getScenegraph()->addParticleSystem(particleFire);*/

	//===================================================================//
	//==================Setting up the Observers========================//
	//==================================================================//

	ObjectObserver playerObserver(&testLevel);
	SoundObserver soundPlayerObserver(&testLevel);
	geko.addObserver(&playerObserver);
	geko.addObserver(&soundPlayerObserver);


	// ==============================================================
	// == Object (Forest) ==========================================
	// ==============================================================

	Texture texTree((char*)RESOURCES_PATH "/Texture/cookie.jpg");

	auto treeHandle = manager.loadStaticMesh(RESOURCES_PATH "/Tree.ply");
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
		tmp.z = TreeData::forest1[i].z;
		TreeData::forest1[i].y = terrain2.getHeight(glm::vec2(tmp.x, tmp.z));
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
		tmp.z = TreeData::forest2[i].z;
		TreeData::forest2[i].y = terrain2.getHeight(glm::vec2(tmp.x, tmp.z));
		treeNode->addTranslation(tmp);
		treeNode->getStaticObject()->setPosition(tmp);
		treeNode->getBoundingSphere()->radius = 2.5;
		testScene.getScenegraph()->getRootNode()->addChildrenNode(treeNode);
		name.str("");
	}

	// ==============================================================
	// == Object (Anthome) ==========================================
	// ==============================================================

	Texture texAntHome((char*)RESOURCES_PATH "/Texture/antHome.jpg");

	auto antHomeHandler = manager.loadStaticMesh(RESOURCES_PATH "/AntHome.ply");
	auto antHomeGeometry = antHomeHandler.get().toGeometry();

	glm::vec3 posFood(10.0, 0.0, -5.0);
	glm::vec3 posFood2((terrain2.getResolutionX() / 2.0f) + 10.0, 0.0, (terrain2.getResolutionY() / 2.0f) - 5.0);
	glm::vec3 posSpawn(terrain2.getResolutionX() / 2.0f, 3.0, terrain2.getResolutionY() / 2.0f);
	glm::vec3 posDefaultPlayer(0.0, 0.0, 0.0);
	AntMesh antMesh;

	Graph<AStarNode, AStarAlgorithm>* antAfraidGraph = new Graph<AStarNode, AStarAlgorithm>();
	std::vector<std::vector<glm::vec3>> possFoods;
	possFoods.push_back(TreeData::forest1);
	possFoods.push_back(TreeData::forest2);
	antAfraidGraph->setExampleAntAfraid2(posSpawn, possFoods);

	Texture texAnt((char*)RESOURCES_PATH "/Texture/ant.jpg");
	Texture texAnt2((char*)RESOURCES_PATH "/Texture/ant2.jpg");
	auto antHandler = manager.loadStaticMesh(RESOURCES_PATH "/Ant.ply");
	auto antGeometry = antHandler.get().toGeometry();
	sfh.generateSource("tst", glm::vec3(geko.getPosition()), RESOURCES_PATH "/Sound/jingle2.wav");
	AntHome antHome(posSpawn, &sfh, antGeometry, &soundPlayerObserver, &playerObserver, &texAnt2, &texAnt, antAfraidGraph);
	antHome.setGrapHighOnTerrain(&terrain2);
	
	antHome.setGrapHighOnTerrain(&terrain2);
	antHome.setAntScale(0.5);
	antHome.generateWorkers(5, testScene.getScenegraph()->getRootNode());
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
	Quest questEatAnt(2);
	Quest questCollectBranch(3);

	questKillAnt.setDescription("Kill one worker ant.");
	questEatAnt.setDescription("Eat two ants.");
	questCollectBranch.setDescription("Collect ten branches.");

	Goal_Kill killAnt(1);
	Goal_Eaten eatAnt(2);
	Goal_Collect collectBranch(3);

	questKillAnt.addGoal(&killAnt);
	questEatAnt.addGoal(&eatAnt);
	questCollectBranch.addGoal(&collectBranch);

	killAnt.setGoalCount(2);
	eatAnt.setGoalCount(2);
	collectBranch.setGoalCount(10);

	collectBranch.setItemType(ItemType::BRANCH);

	ExpReward expReward(1);
	expReward.setExp(100);

	questKillAnt.addReward(&expReward);
	questEatAnt.addReward(&expReward);
	questCollectBranch.addReward(&expReward);

	QuestGraph questGraph;
	QuestGraphNode nodeStart;
	nodeStart.setQuest(&questKillAnt);
	questGraph.addNode(&nodeStart);
	questKillAnt.setActive(true);

	QuestGraphNode nodeSecond;
	nodeSecond.setQuest(&questEatAnt);
	nodeSecond.setParent(&nodeStart);
	questGraph.addNode(&nodeSecond);

	QuestGraphNode nodeThird;
	nodeThird.setQuest(&questCollectBranch);
	nodeThird.setParent(&nodeSecond);
	questGraph.addNode(&nodeThird);
	questCollectBranch.setActive(true);

	testLevel.getQuestHandler()->addQuest(&questKillAnt);
	testLevel.getQuestHandler()->addQuest(&questEatAnt);
	testLevel.getQuestHandler()->addQuest(&questCollectBranch);

	testLevel.getQuestHandler()->setGraph(&questGraph);

	QuestObserver questObserver(&testLevel);

	questKillAnt.addObserver(&questObserver);
	questKillAnt.addObserver(&soundPlayerObserver);
	questEatAnt.addObserver(&questObserver);
	questEatAnt.addObserver(&soundPlayerObserver);

	questCollectBranch.addObserver(&questObserver);
	questCollectBranch.addObserver(&soundPlayerObserver);

	killAnt.addObserver(&questObserver);
	eatAnt.addObserver(&questObserver);
	collectBranch.addObserver(&questObserver);

	testLevel.getFightSystem()->addObserver(&questObserver);


	//===================================================================//
	//================== Setting up the playerGUI ========================//
	//==================================================================//

	PlayerGUI playerGUI(HUD_WIDTH, HUD_HEIGHT, WINDOW_HEIGHT, WINDOW_WIDTH, QUEST_HEIGHT, QUEST_WIDTH, playerNode.getPlayer(), testLevel.getQuestHandler());
	testLevel.setGUI(&playerGUI);

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

	sfh.stopSource("Lademusik");
	sfh.playSource("Hintergrund");

	while (!glfwWindowShouldClose(testWindow.getWindow()))
	{

		testScene.getScenegraph()->searchNode("Player")->setIdentityMatrix_Rotation();
		//testScene.getScenegraph()->searchNode("Player")->setIdentityMatrix_Translate();

		float currentTime = glfwGetTime();
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;


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

		//ToDo calculate Normal funktioniert evtl falsch
		normalFromTerrain = glm::normalize(terrain2.calculateNormal(tmpPos.x, tmpPos.z));
		rotateAxis = glm::cross(glm::vec3(normalFromTerrain), up);
		lengthFromNormal = glm::length(normalFromTerrain);
		lengthFromUp = glm::length(up);
		up = glm::normalize(up);
		dot = glm::dot(normalFromTerrain, up);

		phi = glm::acos(dot / (lengthFromNormal * lengthFromUp));
		phi = phi * (180 / glm::pi<float>());
		//std::cout << phi << std::endl;

		if (dot <0.99)
			testScene.getScenegraph()->searchNode("Player")->addRotation(-phi, rotateAxis);

		testScene.getScenegraph()->searchNode("Player")->getPlayer()->setPosition(testScene.getScenegraph()->searchNode("Player")->getPlayer()->getPosition() + glm::vec4(normalFromTerrain * 0.2f, 1.0));
		antHome.updateAnts();

		testScene.getScenegraph()->searchNode("Player")->addRotation(testScene.getScenegraph()->searchNode("Player")->getPlayer()->getPhi(), glm::vec3(0, -1, 0));
		//===================================================================//
		//==================Render your Objects==============================//
		//==================================================================//
		//renderer.renderScene(testScene, testWindow);
		playerGUI.update();
		renderer.renderScene(testScene, testWindow);
		//renderer.renderGUI(*playerGUI.getHUD(), testWindow);


		testScene.getScenegraph()->searchNode("Player")->getPlayer()->setPosition(testScene.getScenegraph()->searchNode("Player")->getPlayer()->getPosition() - glm::vec4(normalFromTerrain * 0.2f, 1.0));
		//std::cout << "FPS " << 1 / deltaTime << std::endl;

		glfwSwapBuffers(testWindow.getWindow());
		glfwPollEvents();

	}



	glfwDestroyWindow(testWindow.getWindow());
	glfwTerminate();


	return 0;

}

