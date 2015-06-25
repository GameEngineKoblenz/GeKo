#include <GL/glew.h>

#include <GeKo_Graphics/InputInclude.h>
#include <GeKo_Graphics/MaterialInclude.h>
#include <GeKo_Graphics/GeometryInclude.h>
#include <GeKo_Graphics/ShaderInclude.h>
#include <GeKo_Graphics/ScenegraphInclude.h>

#include <GeKo_Graphics/AIInclude.h>

#include <GeKo_Graphics/Scenegraph/BoundingSphere.h>
#include <GeKo_Physics/CollisionTest.h>

#include "GeKo_Graphics/Camera/Trackball.h"
#include "GeKo_Graphics/Camera/Playerview.h"
#include <GeKo_Graphics/Camera/StrategyCamera.h>

#include <GeKo_Gameplay/Object/Object.h>
#include <GeKo_Gameplay/Object/AI.h>
#include <GeKo_Gameplay/Object/Geko.h>
#include <GeKo_Gameplay/Object/StaticObject.h>

#include <GeKo_Graphics/Geometry/TreeMesh.h>
#include <GeKo_Graphics/Geometry/AntMesh.h>
#include <GeKo_Graphics/Geometry/AntHomeMesh.h>
#include <GeKo_Graphics/Geometry/GekoMesh.h>

#include <GeKo_Gameplay/AI_Decisiontree/DecisionTree.h>
#include <GeKo_Gameplay/FightSystem/FightSystem.h>

#include <GeKo_Gameplay/Item/Item.h>

#include <GeKo_Gameplay/Questsystem/Quest.h>
#include <GeKo_Gameplay/Questsystem/Goal_Collect.h>

#include <GeKo_Gameplay/Observer/ObjectObserver.h>
#include <GeKo_Gameplay/Observer/CollisionObserver.h>
#include <GeKo_Gameplay/Observer/GravityObserver.h>

#include <GeKo_Gameplay/Object/AntHome.h>

#include <list>
#include <queue>
#include <stack>


static InputHandler iH;
static StrategyCamera cam("PlayerViewCam");

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

int main()
{
	Player geko("Geko", glm::vec3(10.0, 10.0, 10.0));


	// Start Konifguration - Window, Context, Camera and Callbacks //
	glfwInit();

	Window testWindow(500, 50, 800, 600, "testWindow");
	glfwMakeContextCurrent(testWindow.getWindow());

	cam.setKeySpeed(2.0);
	cam.setNearFar(0.01, 100);

	//Callback for Camera and Player
	glfwSetKeyCallback(testWindow.getWindow(), key_callback);

	glewInit();

	// Shader  and Renderer Initialization //
	//Shader for Objects
	VertexShader vs(loadShaderSource(SHADERS_PATH + std::string("/Vertex-Shaders/TextureShader3D.vert")));
	FragmentShader fs(loadShaderSource(SHADERS_PATH + std::string("/Fragment-Shaders/TextureShader3D.frag")));
	ShaderProgram shader(vs, fs);

	//Shader for a Skybox
	VertexShader vsSkybox(loadShaderSource(SHADERS_PATH + std::string("/SkyboxShader/SkyboxShader.vert")));
	FragmentShader fsSkybox(loadShaderSource(SHADERS_PATH + std::string("/SkyboxShader/SkyboxShader.frag")));
	ShaderProgram shaderSkybox(vsSkybox, fsSkybox);

	//Renderer with context
	OpenGL3Context context;
	Renderer renderer(context);

	//Level and Scene
	Level testLevel("testLevel");
	Scene testScene("testScene");
	testLevel.addScene(&testScene);
	testLevel.changeScene("testScene");

	//Add Camera to scenegraph
	testScene.getScenegraph()->addCamera(&cam);
	testScene.getScenegraph()->getCamera("PlayerViewCam");
	testScene.getScenegraph()->setActiveCamera("PlayerViewCam");

	//Set all InputMaps and set one InputMap active
	iH.setAllInputMaps(*(testScene.getScenegraph()->getActiveCamera()));
	iH.changeActiveInputMap("Object");
	iH.getActiveInputMap()->update(geko);

	// ==============================================================
	// == Object (ant, afraid) ======================================
	// ==============================================================
	Teapot teaAnt;
	Texture texCV((char*)RESOURCES_PATH "/cv_logo.bmp");
	//AntMesh antMesh;
	//Node aiNodeFlick("Flick");

	//aiNodeFlick.addGeometry(&teaAnt);
	//aiNodeFlick.addTexture(&texCV);

	//testScene.getScenegraph()->getRootNode()->addChildrenNode(&aiNodeFlick);

	//AI ant_Flick;
	//ant_Flick.setAntAfraid();
	//aiNodeFlick.ssetObject(&ant_Flick);

	// ==============================================================
	// == Object (ant, aggressiv) ===================================
	// ==============================================================
	AntMesh antMesh;
	//Node aiNodeFlack("Flack");

	//aiNodeFlack.addGeometry(&teaAnt);
	//aiNodeFlack.addGeometry(&antMesh);
	//aiNodeFlack.addTexture(&texCV);

	//testScene.getScenegraph()->getRootNode()->addChildrenNode(&aiNodeFlack);

	//AI ant_Flack;
	//ant_Flack.setAntAggressiv();
	//aiNodeFlack.setObject(&ant_Flack);

	// ==============================================================
	// == Object (Tree) =============================================
	// ==============================================================

	Texture texBrick((char*)RESOURCES_PATH "/brick.bmp");
	//TreeMesh treeMesh;
	Teapot teaTree;
	TreeMesh treeMesh;

	Node treeNode("CookieTree");
	StaticObject tree;
	tree.setTree();

	treeNode.setObject(&tree);
	treeNode.addTexture(&texCV);
	treeNode.addGeometry(&treeMesh);
	glm::vec3 posFood(10.0, 0.0, -5.0);
	treeNode.addTranslation(posFood);
	treeNode.getBoundingSphere()->radius = 2.0;

	testScene.getScenegraph()->getRootNode()->addChildrenNode(&treeNode);

	// ==============================================================
	// == Player ====================================================
	// ==============================================================

	Teapot teaPlayer;
	GekoMesh gekomesh;
	//	Texture texBrick((char*)RESOURCES_PATH "/brick.bmp");

	Node playerNode("Player");
	playerNode.addTexture(&texBrick);
	playerNode.setObject(&geko);
	//playerNode.addGeometry(&teaPlayer);
	playerNode.addGeometry(&gekomesh);
	playerNode.setCamera(&cam);
	geko.setPosition(glm::vec3(10.0, 0.0, 10.0));

	//Add the node to the Scene
	testScene.getScenegraph()->getRootNode()->addChildrenNode(&playerNode);

	// ==============================================================
	// == Questsystem ====================================================
	// ==============================================================
	//QuestHandler questhandler;

	Quest questCollect(1);
	questCollect.setActive(true);
	Goal_Collect goalCollect(1);
	Goal_Collect goalCollect3(3);
	questCollect.addGoal(&goalCollect);
	questCollect.addGoal(&goalCollect3);

	Quest questCollect2(2);
	questCollect2.setActive(true);
	Goal_Collect goalCollect2(2);
	questCollect2.addGoal(&goalCollect2);

	goalCollect.setGoalCount(50);
	goalCollect2.setGoalCount(50);
	goalCollect3.setGoalCount(50);

	goalCollect.setItemType(ItemType::BRANCH);
	goalCollect2.setItemType(ItemType::BRANCH);
	goalCollect3.setItemType(ItemType::COOKIE);

	testLevel.getQuestHandler()->addQuest(&questCollect);
	testLevel.getQuestHandler()->addQuest(&questCollect2);
	/*questhandler.addQuest(&questCollect);
	questhandler.addQuest(&questCollect2);*/

	//std::cout << "GRÖßE DES QUESTHANDLERS : " << questhandler.getQuests().size() << std::endl;


	// ==============================================================
	// == Collision, FightSystem=====================================
	// ==============================================================

	// Bounding Box creation for the objects and collision test //
	CollisionTest collision;


	FightSystem fight;

	//===================================================================//
	//==================Setting up the Observers========================//
	//==================================================================//
	ObjectObserver aiObserver(&testLevel);
	//ant_Flick.addObserver(&aiObserver);
	//ant_Flack.addObserver(&aiObserver);

	ObjectObserver playerObserver(&testLevel);
	geko.addObserver(&playerObserver);

	CollisionObserver colObserver(&testLevel);
	collision.addObserver(&colObserver);

	GravityObserver gravityObserver(&testLevel);
	collision.addObserver(&gravityObserver);

	SoundObserver soundObserver(&testLevel);

	// ==============================================================
	// == Object (Anthome) ==========================================
	// ==============================================================

	//TreeMesh treeMesh;
	AntHomeMesh homeMesh;

	Node homeNode("AntHome");
	StaticObject home;
	//home.setTree();

	homeNode.setObject(&home);
	homeNode.addTexture(&texBrick);
	homeNode.addGeometry(&homeMesh);
	glm::vec3 posHome(1.0, 0.0, 1.0);
	homeNode.addTranslation(posHome);
	homeNode.getBoundingSphere()->radius = 0.5;

	testScene.getScenegraph()->getRootNode()->addChildrenNode(&homeNode);

	glm::vec3 posFood2(10.0, 0.0, -5.0);
	glm::vec3 posSpawn(3.0, 0.0, 3.0);
	glm::vec3 posDefaultPlayer(0.0, 0.0, 0.0);

	DecisionTree *aggressivedecisionTree = new DecisionTree();
	aggressivedecisionTree->setAntTreeAggressiv();

	DecisionTree *afraidDecisionTree = new DecisionTree();
	afraidDecisionTree->setAntTreeAfraid();

	Graph<AStarNode, AStarAlgorithm>* antAfraidGraph = new Graph<AStarNode, AStarAlgorithm>();
	antAfraidGraph->setExampleAntAfraid(posSpawn, posFood, posDefaultPlayer);

	//AntHome antHome(posSpawn, antMesh, &texCV, &texCV, aggressivedecisionTree, antAggressiveGraph, afraidDecisionTree, antAfraidGraph);
	////antHome.generateGuards(5, &aiObserver);
	//antHome.generateWorkers(1, &aiObserver);
	SoundFileHandler sfh(1000);
	AntHome antHome(posSpawn, &sfh, antMesh, &soundObserver, &playerObserver, &texCV, &texCV, aggressivedecisionTree, afraidDecisionTree, antAfraidGraph);
	antHome.generateWorkers(5, testScene.getScenegraph()->getRootNode());
	antHome.addAntsToSceneGraph(testScene.getScenegraph()->getRootNode());

	//antHome.setGrapHighOnTerrain(&terrain);

	collision.collectNodes(testScene.getScenegraph()->getRootNode());
	float lastTime = glfwGetTime();
	int i = 0;
	while (!glfwWindowShouldClose(testWindow.getWindow()))
	{
		i++;
		if (i == 30){
			i++;
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		float currentTime = glfwGetTime();
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		collision.update();

		antHome.updateAnts();
		antHome.printPosWorkers();

		/*	ant_Flick.update();
		ant_Flack.update();*/

		geko.update();
		geko.setDeltaTime(currentTime);

		renderer.renderScene(testScene, testWindow);

	}

	glfwDestroyWindow(testWindow.getWindow());
	glfwTerminate();

	return 0;
}
