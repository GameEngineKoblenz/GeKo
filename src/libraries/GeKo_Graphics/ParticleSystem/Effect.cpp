#include "Effect.h"

using namespace tinyxml2;

Effect::Effect()
{
	emitterVec.clear();
	notStartedEmitters.clear();
	m_isStarted = false;
	setShader();
}


Effect::Effect(const char*filepath)
{
	emitterVec.clear();
	notStartedEmitters.clear();
	m_isStarted = false;
	setShader();
	loadEffect(filepath);
}

Effect::~Effect()
{
	glDeleteShader(emitterShader->handle);
	glDeleteProgram(emitterShader->handle);
	delete emitterShader;

	glDeleteShader(emitterShaderGeom->handle);
	glDeleteProgram(emitterShaderGeom->handle);
	delete emitterShader;

	glDeleteShader(compute->handle);
	glDeleteProgram(compute->handle);
	delete emitterShader;

	emitterVec.~vector();
}


void Effect::start(){
	if (!m_isStarted) {
		m_isStarted = true;
		m_startTime = glfwGetTime();
		for (auto emitter : emitterVec){
			if (emitter->getStartTime() == 0.0) {
				emitter->start();
			}
			else {
				notStartedEmitters.push_back(emitter);
			}
		}
	}
}

void Effect::stop(){
	for (auto emitter : emitterVec){
		emitter->stop();
	}
	m_isStarted = false;
	notStartedEmitters.clear();
}

void Effect::addEmitter(Emitter* emitter)
{
	emitterVec.push_back(emitter);
}

void Effect::removeEmitter(int arrayPosition)
{
	emitterVec.erase(emitterVec.begin() + arrayPosition);
}

void Effect::updateEmitters(Camera &cam)
{
	//start remaining emitters if their startTime is lower than the passed time
	if (m_isStarted) {
		double timePassed = glfwGetTime() - m_startTime;
		int i = 0;
		std::vector<int> toDelete;
		for (auto emitter : notStartedEmitters) {
			if (emitter->getStartTime() < timePassed) {
				emitter->start();
				toDelete.push_back(i);
			}
			i++;
		}
		//delete started emitters from vector
		int k = 0;
		for (auto j : toDelete) {
			notStartedEmitters.erase(notStartedEmitters.begin() + j - k);
			k++;
		}

	}

	//this cannot be in the block above because there may be living particles when the ParticleSystem gets stopped.
	for (auto emitter : emitterVec) {
		if (emitter->getMovable()) {
			emitter->update(compute, glm::vec3(cam.getPosition().x, cam.getPosition().y, cam.getPosition().z));
		}
		else {
			emitter->update(compute);
		}
	}
}

void Effect::renderEmitters(Camera &cam)
{
	for (auto emitter : emitterVec){
		if (emitter->getUseGeometryShader()){
			emitter->render(emitterShaderGeom, cam);
		}
		else {
			emitter->render(emitterShader, cam);
		}
	}
}

void Effect::setPosition(glm::vec3 newPosition)
{
	for (auto emitter : emitterVec){
		glm::vec3 pos = emitter->getLocalPosition();
		glm::vec3 result(0.0, 0.0, 0.0);
		result = pos + newPosition;
		/*result.x = pos.x + newPosition.x;
		result.y = pos.y + newPosition.y;
		result.z = pos.z + newPosition.z;*/

		emitter->setPosition(result);
	}
}

void Effect::setShader(){
	//Point Sprites shader
	VertexShader vsParticle(loadShaderSource(SHADERS_PATH + std::string("/ParticleSystem/ParticleSystemPointSprites.vert")));
	FragmentShader fsParticle(loadShaderSource(SHADERS_PATH + std::string("/ParticleSystem/ParticleSystemPointSprites.frag")));
	emitterShader = new ShaderProgram(vsParticle, fsParticle);
	//glDeleteShader(emitterShader->handle);

	//...with Geometry Shader
	VertexShader vsParticleGeom(loadShaderSource(SHADERS_PATH + std::string("/ParticleSystem/ParticleSystemGeometryShader.vert")));
	GeometryShader gsParticleGeom(loadShaderSource(SHADERS_PATH + std::string("/ParticleSystem/ParticleSystemGeometryShader.geom")));
	FragmentShader fsParticleGeom(loadShaderSource(SHADERS_PATH + std::string("/ParticleSystem/ParticleSystemGeometryShader.frag")));
	emitterShaderGeom = new ShaderProgram(vsParticleGeom, gsParticleGeom, fsParticleGeom);
	//glDeleteShader(emitterShader->handle);

	//our default compute shader
	ComputeShader csParticle(loadShaderSource(SHADERS_PATH + std::string("/ParticleSystem/ParticleSystem.comp")));
	compute = new ShaderProgram(csParticle);
	//glDeleteShader(compute->handle);
}

int Effect::loadEffect(const char* filepath)
{
	//load file
	XMLDocument doc;
	XMLError error = doc.LoadFile(filepath);
	XMLCheckResult(error);

	XMLNode* effectNode = doc.FirstChild();
	if (effectNode == nullptr) return XML_ERROR_FILE_READ_ERROR;

	XMLElement* emitterNode = effectNode->FirstChildElement("Emitter");
	while (emitterNode != nullptr)
	{
	/****Constructor stuff****/
		int outputType;
		XMLElement* item = emitterNode->FirstChildElement("OutputType");
		if (item == nullptr) return XML_ERROR_PARSING_ELEMENT;
		error = item->QueryIntText(&outputType);
		XMLCheckResult(error);

		glm::vec3 position;
		item = emitterNode->FirstChildElement("Position");
		if (item == nullptr) return XML_ERROR_PARSING_ELEMENT;
		error = item->QueryFloatAttribute("x", &position.x);
		XMLCheckResult(error);
		error = item->QueryFloatAttribute("y", &position.y);
		XMLCheckResult(error);
		error = item->QueryFloatAttribute("z", &position.z);
		XMLCheckResult(error);

		double emitterLifetime;
		item = emitterNode->FirstChildElement("EmitterLifetime");
		if (item == nullptr) return XML_ERROR_PARSING_ELEMENT;
		error = item->QueryDoubleText(&emitterLifetime);
		XMLCheckResult(error);

		double emitFrequency;
		item = emitterNode->FirstChildElement("EmitFrequency");
		if (item == nullptr) return XML_ERROR_PARSING_ELEMENT;
		error = item->QueryDoubleText(&emitFrequency);
		XMLCheckResult(error);

		int particlesPerEmit;
		item = emitterNode->FirstChildElement("ParticlesPerEmit");
		if (item == nullptr) return XML_ERROR_PARSING_ELEMENT;
		error = item->QueryIntText(&particlesPerEmit);
		XMLCheckResult(error);

		double particleLifetime;
		item = emitterNode->FirstChildElement("ParticleLiftime");
		if (item == nullptr) return XML_ERROR_PARSING_ELEMENT;
		error = item->QueryDoubleText(&particleLifetime);
		XMLCheckResult(error);

		bool particleMortal;
		item = emitterNode->FirstChildElement("ParticleMortal");
		if (item == nullptr) return XML_ERROR_PARSING_ELEMENT;
		error = item->QueryBoolText(&particleMortal);
		XMLCheckResult(error);

		//create an Emitter
		Emitter* emitter = new Emitter(outputType, position, emitterLifetime, 
			emitFrequency, particlesPerEmit, particleLifetime, particleMortal);

	/****other necessary stuff****/
		//Geometry Shader
		item = emitterNode->FirstChildElement("UseGeometryShader");
		if (item != nullptr){
			bool geom;
			error = item->QueryBoolText(&geom);
			XMLCheckResult(error);
			if (geom) emitter->switchToGeometryShader();
		}

		//Movable
		item = emitterNode->FirstChildElement("Movable");
		if (item != nullptr){
			bool mov;
			error = item->QueryBoolText(&mov);
			XMLCheckResult(error);
			if (mov) emitter->setMovable(mov);
		}

		//Start time
		item = emitterNode->FirstChildElement("StartTime");
		if (item != nullptr){
			double st;
			error = item->QueryDoubleText(&st);
			XMLCheckResult(error);
			emitter->setStartTime(st);
		}
			
		//Velocity
		item = emitterNode->FirstChildElement("Velocity");
		if (item != nullptr){
			int velocity;
			error = item->QueryIntText(&velocity);
			XMLCheckResult(error);
			emitter->setVelocity(velocity);
		}

		//Physic
		item = emitterNode->FirstChildElement("Physic");
		if (item != nullptr) 
		{
			XMLElement* physicType = item->FirstChildElement("Trajectory");
			if (physicType != nullptr){
				XMLElement* physicElement = physicType->FirstChildElement("Gravity");
				if (physicElement == nullptr) return XML_ERROR_PARSING_ELEMENT;
				glm::vec4 gravity;
				error = physicElement->QueryFloatAttribute("x", &gravity.x);
				XMLCheckResult(error);
				error = physicElement->QueryFloatAttribute("y", &gravity.y);
				XMLCheckResult(error);
				error = physicElement->QueryFloatAttribute("z", &gravity.z);
				XMLCheckResult(error);
				error = physicElement->QueryFloatAttribute("w", &gravity.w);
				XMLCheckResult(error);

				physicElement = physicType->FirstChildElement("Speed");
				if (physicElement == nullptr) return XML_ERROR_PARSING_ELEMENT;
				float speed;
				error = physicElement->QueryFloatText(&speed);
				XMLCheckResult(error);

				emitter->usePhysicTrajectory(gravity, speed);
			} 
			physicType = item->FirstChildElement("DirectionGravity");
			if (physicType != nullptr){
				XMLElement* physicElement = physicType->FirstChildElement("Gravity");
				if (physicElement == nullptr) return XML_ERROR_PARSING_ELEMENT;
				glm::vec4 gravity;
				error = physicElement->QueryFloatAttribute("x", &gravity.x);
				XMLCheckResult(error);
				error = physicElement->QueryFloatAttribute("y", &gravity.y);
				XMLCheckResult(error);
				error = physicElement->QueryFloatAttribute("z", &gravity.z);
				XMLCheckResult(error);
				error = physicElement->QueryFloatAttribute("w", &gravity.w);
				XMLCheckResult(error);

				physicElement = physicType->FirstChildElement("Speed");
				if (physicElement == nullptr) return XML_ERROR_PARSING_ELEMENT;
				float speed;
				error = physicElement->QueryFloatText(&speed);
				XMLCheckResult(error);

				emitter->usePhysicDirectionGravity(gravity, speed);
			}
			physicType = item->FirstChildElement("PointGravity");
			if (physicType != nullptr){
				XMLElement* physicElement = physicType->FirstChildElement("Point");
				if (physicElement == nullptr) return XML_ERROR_PARSING_ELEMENT;
				glm::vec3 point;
				error = physicElement->QueryFloatAttribute("x", &point.x);
				XMLCheckResult(error);
				error = physicElement->QueryFloatAttribute("y", &point.y);
				XMLCheckResult(error);
				error = physicElement->QueryFloatAttribute("z", &point.z);
				XMLCheckResult(error);

				physicElement = physicType->FirstChildElement("GravityImpact");
				if (physicElement == nullptr) return XML_ERROR_PARSING_ELEMENT;
				float gravityImpact;
				error = physicElement->QueryFloatText(&gravityImpact);
				XMLCheckResult(error);

				physicElement = physicType->FirstChildElement("Speed");
				if (physicElement == nullptr) return XML_ERROR_PARSING_ELEMENT;
				float speed;
				error = physicElement->QueryFloatText(&speed);
				XMLCheckResult(error);
			
				physicElement = physicType->FirstChildElement("GravityRange");
				if (physicElement == nullptr) return XML_ERROR_PARSING_ELEMENT;
				float gravityRange;
				error = physicElement->QueryFloatText(&gravityRange);
				XMLCheckResult(error);

				physicElement = physicType->FirstChildElement("GravityFunction");
				if (physicElement == nullptr) return XML_ERROR_PARSING_ELEMENT;
				int gravityFunction;
				error = physicElement->QueryIntText(&gravityFunction);
				XMLCheckResult(error);

				physicElement = physicType->FirstChildElement("BackToSource");
				if (physicElement == nullptr) return XML_ERROR_PARSING_ELEMENT;
				bool backToSource;
				error = physicElement->QueryBoolText(&backToSource);
				XMLCheckResult(error);
				
				emitter->usePhysicPointGravity(point, gravityImpact, gravityRange, gravityFunction, speed, backToSource);
			}
			physicType = item->FirstChildElement("SwarmCircleMotion");
			if (physicType != nullptr){
				XMLElement* physicElement = physicType->FirstChildElement("MovementVertical");
				if (physicElement == nullptr) return XML_ERROR_PARSING_ELEMENT;
				bool movementVertical;
				error = physicElement->QueryBoolText(&movementVertical);
				XMLCheckResult(error);
			
				physicElement = physicType->FirstChildElement("MovementHorizontalX");
				if (physicElement == nullptr) return XML_ERROR_PARSING_ELEMENT;
				bool movementHorizontalX;
				error = physicElement->QueryBoolText(&movementHorizontalX);
				XMLCheckResult(error);

				physicElement = physicType->FirstChildElement("MovementHorizontalZ");
				if (physicElement == nullptr) return XML_ERROR_PARSING_ELEMENT;
				bool movementHorizontalZ;
				error = physicElement->QueryBoolText(&movementHorizontalZ);
				XMLCheckResult(error);

				physicElement = physicType->FirstChildElement("Speed");
				if (physicElement == nullptr) return XML_ERROR_PARSING_ELEMENT;
				float speed;
				error = physicElement->QueryFloatText(&speed);
				XMLCheckResult(error);

				emitter->usePhysicSwarmCircleMotion(movementVertical, movementHorizontalX, movementHorizontalZ, speed);
			}
		}

		//Area Emitter
		item = emitterNode->FirstChildElement("AreaEmitter");
		if (item != nullptr){
			bool on;
			error = item->QueryBoolAttribute("on", &on);
			XMLCheckResult(error);
			if (on) {
				XMLElement* area = item->FirstChildElement("AreaXY");
				if (area == nullptr) return XML_ERROR_PARSING_ELEMENT;
				bool areaXY;
				error = area->QueryBoolText(&areaXY);
				XMLCheckResult(error);
				
				area = item->FirstChildElement("AreaXZ");
				bool areaXZ;
				error = area->QueryBoolText(&areaXZ);
				XMLCheckResult(error);

				area = item->FirstChildElement("Size");
				float size;
				error = area->QueryFloatText(&size);
				XMLCheckResult(error);

				area = item->FirstChildElement("Accuracy");
				int accuracy;
				error = area->QueryIntText(&accuracy);
				XMLCheckResult(error);

				emitter->setAreaEmitting(areaXY, areaXZ, size, accuracy);
			}
		}
		//Texures
		item = emitterNode->FirstChildElement("Texture");
		if (item != nullptr){
			//addTexture
			XMLElement* tex = item->FirstChildElement("Tex");
			while (tex != nullptr){
				std::string filepath = tex->GetText();
				std::string spath = RESOURCES_PATH + filepath;
				char* cpath = new char[spath.length() + 1];
				strcpy(cpath, spath.c_str());

				Texture* texture = new Texture(cpath);
				
				float time;
				error = tex->QueryFloatAttribute("time", &time);
				XMLCheckResult(error);
				
				emitter->addTexture(texture, time);

				tex = tex->NextSiblingElement("Tex");
			}

			//defineLook
			XMLElement* scaling = item->FirstChildElement("Scaling");
			if (scaling != nullptr) {
				bool useTexture;
				XMLElement* tex = scaling->FirstChildElement("UseTexture");
				error = tex->QueryBoolText(&useTexture);
				XMLCheckResult(error);

				std::vector<float> scalingSize;
				tex = scaling->FirstChildElement("ScalingSize");
				if (tex == nullptr) return XML_ERROR_PARSING_ELEMENT;
				XMLElement* temp = tex->FirstChildElement("Item");
				while (temp != nullptr){
					float value;
					error = temp->QueryFloatText(&value);
					XMLCheckResult(error);
					scalingSize.push_back(value);

					temp = temp->NextSiblingElement("Item");
				}
				
				std::vector<float> scalingMoment;
				tex = scaling->FirstChildElement("ScalingMoment");
				if (tex == nullptr) return XML_ERROR_PARSING_ELEMENT;
				temp = tex->FirstChildElement("Item");
				while (temp != nullptr){
					float value;
					error = temp->QueryFloatText(&value);
					XMLCheckResult(error);
					scalingMoment.push_back(value);

					temp = temp->NextSiblingElement("Item");
				}

				float birthTime;
				tex = scaling->FirstChildElement("BirthTime");
				error = tex->QueryFloatText(&birthTime);
				XMLCheckResult(error);
				
				float deathTime;
				tex = scaling->FirstChildElement("DeathTime");
				error = tex->QueryFloatText(&deathTime);
				XMLCheckResult(error);

				float blendingTime;
				tex = scaling->FirstChildElement("BlendingTime");
				error = tex->QueryFloatText(&blendingTime);
				XMLCheckResult(error);

				bool rotateLeft;
				tex = scaling->FirstChildElement("RotateLeft");
				error = tex->QueryBoolText(&rotateLeft);
				XMLCheckResult(error);

				float rotationSpeed;
				tex = scaling->FirstChildElement("RotationSpeed");
				error = tex->QueryFloatText(&rotationSpeed);
				XMLCheckResult(error);

				emitter->defineLook(useTexture, scalingSize, scalingMoment, 
					birthTime, deathTime, blendingTime, rotateLeft, rotationSpeed);
			}
			else {
				scaling = item->FirstChildElement("Size");
				if (scaling == nullptr) return XML_ERROR_PARSING_ELEMENT;
				
				bool useTexture;
				XMLElement* tex = scaling->FirstChildElement("UseTexture");
				error = tex->QueryBoolText(&useTexture);
				XMLCheckResult(error);

				float particleSize;
				tex = scaling->FirstChildElement("ParticleSize");
				error = tex->QueryFloatText(&particleSize);
				XMLCheckResult(error);

				float birthTime;
				tex = scaling->FirstChildElement("BirthTime");
				error = tex->QueryFloatText(&birthTime);
				XMLCheckResult(error);

				float deathTime;
				tex = scaling->FirstChildElement("DeathTime");
				error = tex->QueryFloatText(&deathTime);
				XMLCheckResult(error);

				float blendingTime;
				tex = scaling->FirstChildElement("BlendingTime");
				error = tex->QueryFloatText(&blendingTime);
				XMLCheckResult(error);

				bool rotateLeft;
				tex = scaling->FirstChildElement("RotateLeft");
				error = tex->QueryBoolText(&rotateLeft);
				XMLCheckResult(error);

				float rotationSpeed;
				tex = scaling->FirstChildElement("RotationSpeed");
				error = tex->QueryFloatText(&rotationSpeed);
				XMLCheckResult(error);

				emitter->defineLook(useTexture, particleSize,
					birthTime, deathTime, blendingTime, rotateLeft, rotationSpeed);
			}
			
		}

		//next Emitter
		emitterVec.push_back(emitter);
		emitterNode = emitterNode->NextSiblingElement("Emitter");
	}

	printf("EFFECT: loading Effect successfully\n");
	return XML_SUCCESS;
}

int Effect::saveEffect(char* filepath)
{
	XMLDocument doc;
	XMLNode* effect = doc.NewElement("Effect");
	doc.InsertFirstChild(effect);
	for (auto emitter : emitterVec)
	{
	/****Constructor stuff****/
		XMLNode* emitterNode = doc.NewElement("Emitter");
		effect->InsertEndChild(emitterNode);

		XMLElement* element = doc.NewElement("OutputType");
		element->SetText(emitter->getOutputType());
		emitterNode->InsertEndChild(element);

		element = doc.NewElement("Position");
		glm::vec3 position = emitter->getLocalPosition();
		element->SetAttribute("x", position.x);
		element->SetAttribute("y", position.y);
		element->SetAttribute("z", position.z);
		emitterNode->InsertEndChild(element);

		element = doc.NewElement("EmitterLifetime");
		element->SetText(emitter->getEmitterLifetime());
		emitterNode->InsertEndChild(element);

		element = doc.NewElement("EmitFrequency");
		element->SetText(emitter->getEmitFrequency());
		emitterNode->InsertEndChild(element);

		element = doc.NewElement("ParticlesPerEmit");
		element->SetText(emitter->getParticlesPerEmit());
		emitterNode->InsertEndChild(element);

		element = doc.NewElement("ParticleLiftime");
		element->SetText(emitter->getParticleLifetime());
		emitterNode->InsertEndChild(element);

		element = doc.NewElement("ParticleMortal");
		element->SetText(emitter->getParticleMortality());
		emitterNode->InsertEndChild(element);

	/****other necessary stuff****/
		//Geometry Shader
		element = doc.NewElement("UseGeometryShader");
		element->SetText(emitter->getUseGeometryShader());
		emitterNode->InsertEndChild(element);

		//Movable
		element = doc.NewElement("Movable");
		element->SetText(emitter->getMovable());
		emitterNode->InsertEndChild(element);

		//Start time
		element = doc.NewElement("StartTime");
		element->SetText(emitter->getStartTime());
		emitterNode->InsertEndChild(element);
		
		//Velocity
		element = doc.NewElement("Velocity");
		element->SetText(emitter->getVelocityType());
		emitterNode->InsertEndChild(element);

		//Physic
		element = doc.NewElement("Physic");
		if (emitter->getPhysicTrajectory()){
			XMLElement* physic = doc.NewElement("Trajectory");
			XMLElement* gravity = doc.NewElement("Gravity");
			glm::vec4 gravityVec = emitter->getGravity();
			gravity->SetAttribute("x", gravityVec.x);
			gravity->SetAttribute("y", gravityVec.y);
			gravity->SetAttribute("z", gravityVec.z);
			gravity->SetAttribute("w", gravityVec.w);
			physic->InsertEndChild(gravity);

			XMLElement* speed = doc.NewElement("Speed");
			speed->SetText(emitter->getSpeed());
			physic->InsertEndChild(speed);
			element->InsertEndChild(physic);
		}
		else if (emitter->getPhysicDirectionGravity()){
			XMLElement* physic = doc.NewElement("DirectionGravity");
			XMLElement* temp = doc.NewElement("Gravity");
			glm::vec4 gravityVec = emitter->getGravity();
			temp->SetAttribute("x", gravityVec.x);
			temp->SetAttribute("y", gravityVec.y);
			temp->SetAttribute("z", gravityVec.z);
			temp->SetAttribute("w", gravityVec.w);
			physic->InsertEndChild(temp);

			temp = doc.NewElement("Speed");
			temp->SetText(emitter->getSpeed());
			physic->InsertEndChild(temp);
			element->InsertEndChild(physic);
		}
		else if (emitter->getPhysicPointGravity()){
			XMLElement* physic = doc.NewElement("PointGravity");
			XMLElement* temp = doc.NewElement("Point");
			glm::vec4 gravityVec = emitter->getGravity();
			temp->SetAttribute("x", gravityVec.x);
			temp->SetAttribute("y", gravityVec.y);
			temp->SetAttribute("z", gravityVec.z);
			physic->InsertEndChild(temp);

			//TODO impact + backtosource
			temp = doc.NewElement("GravityImpact");
			temp->SetText(emitter->getPhysicAttGravityImpact());
			physic->InsertEndChild(temp);

			temp = doc.NewElement("Speed");
			temp->SetText(emitter->getSpeed());
			physic->InsertEndChild(temp);

			temp = doc.NewElement("GravityRange");
			temp->SetText(emitter->getPhysicAttGravityRange());
			physic->InsertEndChild(temp);

			temp = doc.NewElement("GravityFunction");
			temp->SetText(emitter->getPhysicAttGravityFunction());
			physic->InsertEndChild(temp);
			element->InsertEndChild(physic);

			temp = doc.NewElement("BackToSource");
			temp->SetText(emitter->getPhysicAttUseLocalCoordinates());
			physic->InsertEndChild(temp);
		}
		else if (emitter->getPhysicSwarmCircleMotion()){
			XMLElement* physic = doc.NewElement("SwarmCircleMotion");
			XMLElement* temp = doc.NewElement("MovementVertical");
			temp->SetText(emitter->getPhysicAttMovementVertical());
			physic->InsertEndChild(temp);

			temp = doc.NewElement("MovementHorizontalX");
			temp->SetText(emitter->getPhysicAttMovementHorizontalX());
			physic->InsertEndChild(temp);

			temp = doc.NewElement("MovementHorizontalZ");
			temp->SetText(emitter->getPhysicAttMovementHorizontalZ());
			physic->InsertEndChild(temp);

			temp = doc.NewElement("Speed");
			temp->SetText(emitter->getSpeed());
			physic->InsertEndChild(temp);
			element->InsertEndChild(physic);
		}
		emitterNode->InsertEndChild(element);

		//Area emitting
		element = doc.NewElement("AreaEmitter");
		bool on = emitter->getAreaEmittingXY() | emitter->getAreaEmittingXZ();
		element->SetAttribute("on", on);
		if (on){
			XMLElement* temp = doc.NewElement("AreaXY");
			temp->SetText(emitter->getAreaEmittingXY());
			element->InsertEndChild(temp);

			temp = doc.NewElement("AreaXZ");
			temp->SetText(emitter->getAreaEmittingXZ());
			element->InsertEndChild(temp);

			temp = doc.NewElement("Size");
			temp->SetText(emitter->getAreaSize());
			element->InsertEndChild(temp);

			temp = doc.NewElement("Accuracy");
			temp->SetText(emitter->getAreaAccuracy());
			element->InsertEndChild(temp);
		}
		emitterNode->InsertEndChild(element);

		//Textures
		element = doc.NewElement("Texture");
		int k = 0;
		for (auto texture : emitter->m_textureList)
		{
			XMLElement* tex = doc.NewElement("Tex");
			tex->SetText(texture->getFilepath());
			tex->SetAttribute("time", emitter->blendingTime[k]);
			element->InsertEndChild(tex);
			k++;
		}

		if (emitter->getTexUseScaling()) {
			XMLElement* scaling = doc.NewElement("Scaling");
			
			XMLElement* tex = doc.NewElement("UseTexture");
			tex->SetText(emitter->getUseTexture());
			scaling->InsertEndChild(tex);

			tex = doc.NewElement("ScalingSize");
			for (int i = 1; i < emitter->getTexScalingCount(); i = i + 2){
				XMLElement* item = doc.NewElement("Item");
				item->SetText(emitter->m_scalingData[i]);
				tex->InsertEndChild(item);
			}
			scaling->InsertEndChild(tex);
			
			tex = doc.NewElement("ScalingMoment");
			for (int i = 0; i < emitter->getTexScalingCount(); i = i + 2){
				XMLElement* item = doc.NewElement("Item");
				item->SetText(emitter->m_scalingData[i]);
				tex->InsertEndChild(item);
			}
			scaling->InsertEndChild(tex);			

			tex = doc.NewElement("BirthTime");
			tex->SetText(emitter->getTexBirthTime());
			scaling->InsertEndChild(tex);

			tex = doc.NewElement("DeathTime");
			tex->SetText(emitter->getTexDeathTime());
			scaling->InsertEndChild(tex);

			tex = doc.NewElement("BlendingTime");
			tex->SetText(emitter->getTexBlendingTime());
			scaling->InsertEndChild(tex);

			tex = doc.NewElement("RotateLeft");
			tex->SetText(emitter->getTexRotateLeft());
			scaling->InsertEndChild(tex);

			tex = doc.NewElement("RotationSpeed");
			tex->SetText(emitter->getRotationSpeed());
			scaling->InsertEndChild(tex);

			element->InsertEndChild(scaling);
		}
		else {
			XMLElement* scaling = doc.NewElement("Size");

			XMLElement* tex = doc.NewElement("UseTexture");
			tex->SetText(emitter->getUseTexture());
			scaling->InsertEndChild(tex);

			tex = doc.NewElement("ParticleSize");
			tex->SetText(emitter->getTexParticleDefaultSize());
			scaling->InsertEndChild(tex);

			tex = doc.NewElement("BirthTime");
			tex->SetText(emitter->getTexBirthTime());
			scaling->InsertEndChild(tex);

			tex = doc.NewElement("DeathTime");
			tex->SetText(emitter->getTexDeathTime());
			scaling->InsertEndChild(tex);

			tex = doc.NewElement("BlendingTime");
			tex->SetText(emitter->getTexBlendingTime());
			scaling->InsertEndChild(tex);

			tex = doc.NewElement("RotateLeft");
			tex->SetText(emitter->getTexRotateLeft());
			scaling->InsertEndChild(tex);

			tex = doc.NewElement("RotationSpeed");
			tex->SetText(emitter->getRotationSpeed());
			scaling->InsertEndChild(tex);

			element->InsertEndChild(scaling);
		}
		emitterNode->InsertEndChild(element);
	}

	//save the file
	XMLError error = doc.SaveFile(filepath);
	XMLCheckResult(error);
	printf("EFFECT: saving Effect successfully\n");
	return XML_SUCCESS;
}

int Effect::XMLCheckResult(int result)
{
	if (result != XML_SUCCESS) 
	{
		switch (result)
		{
		case tinyxml2::XML_NO_ATTRIBUTE:
			printf("XMLError: %i - XML_NO_ATTRIBUTE\n", result);
			break;
		case tinyxml2::XML_WRONG_ATTRIBUTE_TYPE:
			printf("XMLError: %i - XML_WRONG_ATTRIBUTE_TYPE\n", result);
			break;
		case tinyxml2::XML_ERROR_FILE_NOT_FOUND:
			printf("XMLError: %i - XML_ERROR_FILE_NOT_FOUND\n", result);
			break;
		case tinyxml2::XML_ERROR_FILE_COULD_NOT_BE_OPENED:
			printf("XMLError: %i - XML_ERROR_FILE_COULD_NOT_BE_OPENED\n", result);
			break;
		case tinyxml2::XML_ERROR_FILE_READ_ERROR:
			printf("XMLError: %i - XML_ERROR_FILE_READ_ERROR\n", result);
			break;
		case tinyxml2::XML_ERROR_ELEMENT_MISMATCH:
			printf("XMLError: %i - XML_ERROR_ELEMENT_MISMATCH\n", result);
			break;
		case tinyxml2::XML_ERROR_PARSING_ELEMENT:
			printf("XMLError: %i - XML_ERROR_PARSING_ELEMENT\n", result);
			break;
		case tinyxml2::XML_ERROR_PARSING_ATTRIBUTE:
			printf("XMLError: %i - XML_ERROR_PARSING_ATTRIBUTE\n", result);
			break;
		case tinyxml2::XML_ERROR_IDENTIFYING_TAG:
			printf("XMLError: %i - XML_ERROR_IDENTIFYING_TAG\n", result);
			break;
		case tinyxml2::XML_ERROR_PARSING_TEXT:
			printf("XMLError: %i - XML_ERROR_PARSING_TEXT\n", result);
			break;
		case tinyxml2::XML_ERROR_PARSING_CDATA:
			printf("XMLError: %i - XML_ERROR_PARSING_CDATA\n", result);
			break;
		case tinyxml2::XML_ERROR_PARSING_COMMENT:
			printf("XMLError: %i - XML_ERROR_PARSING_COMMENT\n", result);
			break;
		case tinyxml2::XML_ERROR_PARSING_DECLARATION:
			printf("XMLError: %i - XML_ERROR_PARSING_DECLARATION\n", result);
			break;
		case tinyxml2::XML_ERROR_PARSING_UNKNOWN:
			printf("XMLError: %i - XML_ERROR_PARSING_UNKNOWN\n", result);
			break;
		case tinyxml2::XML_ERROR_EMPTY_DOCUMENT:
			printf("XMLError: %i - XML_ERROR_EMPTY_DOCUMENT\n", result);
			break;
		case tinyxml2::XML_ERROR_MISMATCHED_ELEMENT:
			printf("XMLError: %i - XML_ERROR_MISMATCHED_ELEMENT\n", result);
			break;
		case tinyxml2::XML_ERROR_PARSING:
			printf("XMLError: %i - XML_ERROR_PARSING\n", result);
			break;
		case tinyxml2::XML_CAN_NOT_CONVERT_TEXT:
			printf("XMLError: %i - XML_CAN_NOT_CONVERT_TEXT\n", result);
			break;
		case tinyxml2::XML_NO_TEXT_NODE:
			printf("XMLError: %i - XML_NO_TEXT_NODE\n", result);
			break;
		case tinyxml2::XML_ERROR_COUNT:
			printf("XMLError: %i - XML_ERROR_COUNT\n", result);
			break;
		default:
			printf("XMLError: %i\n", result); 
			break;
		}	
		return result; 
	}
}