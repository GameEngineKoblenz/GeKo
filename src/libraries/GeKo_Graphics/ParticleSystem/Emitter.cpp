#include "Emitter.h"

//TODO: COMMENTS & VAR RENAMING

Emitter::Emitter(const int OUTPUT, glm::vec3 position, double emitterLifetime, double emitFrequency,
	int particlesPerEmit, double particleLifeTime, bool particleMortal)
{
	//set default
	setAttributes();

	m_output = static_cast<FLOW> (-1); //set if won't generate (-1), constant (0) or just once (1)
	m_outputType = OUTPUT;

	//set Emitter properties
	setPosition(position);
	setLocalPosition(position);
	setEmitterLifetime(emitterLifetime);
	this->emitterLifetime = emitterLifetime;
	//setEmitterMortality(emitterLifetime);

	//set properties for the emitting
	setEmitFrequency(emitFrequency);
	setParticlesPerEmit(particlesPerEmit);

	//set properties for the particles
	setParticleLifetime(particleLifeTime);
	setParticleMortality(particleMortal);

	updateSize(); //update the number of max particle and create new buffer for it.

	m_scalingData[32] = { 0 };
	blendingTime[4] = { 0 };
}
//TODO: Memory?
Emitter::~Emitter()
{
	m_textureList.clear();
}

void Emitter::startTime(){
	updateTime = glfwGetTime();
	deltaTime = updateTime;
	generateTime = deltaTime;
}

void Emitter::start(){
	if (m_output == UNUSED){
		m_isStarted = true;
		startTime();
		setOutputMode(m_outputType);
	}
}

void Emitter::stop(){
	m_isStarted = false;
	setOutputMode(-1);
	m_emitLifetime = emitterLifetime;
}

void Emitter::loadBuffer(){
	//create and fill a buffer with positions
	glGenBuffers(1, &position_ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, position_ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, numMaxParticle * sizeof(glm::vec4), NULL, GL_STREAM_DRAW);
	glm::vec4* positions = (glm::vec4*) glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
	for (int i = 0; i < numMaxParticle; i++)
	{
		positions[i] = glm::vec4(getPosition(), -1.0f);
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	//create and fill a buffer with velocitys
	glGenBuffers(1, &velocity_ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, velocity_ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, numMaxParticle * sizeof(glm::vec4), NULL, GL_STREAM_DRAW);
	glm::vec4* velocity = (glm::vec4*) glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
	for (int i = 0; i < numMaxParticle; i++)
	{
		velocity[i] = glm::vec4(0.0, 0.0, 0.0, 0.0);
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	//create and fill a buffer with angles
	glGenBuffers(1, &angle_ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, angle_ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, numMaxParticle * sizeof(glm::vec4), NULL, GL_STREAM_DRAW);
	glm::vec4* angle = (glm::vec4*) glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
	for (int i = 0; i < numMaxParticle; i++)
	{
		angle[i] = glm::vec4(0.0, 0.0, 0.0, 0.0);
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void Emitter::update(ShaderProgram* compute, glm::vec3 playerPosition){

	generateParticle(playerPosition);

	deltaTime = glfwGetTime() - updateTime; //time remain since last update
	updateTime = glfwGetTime();

	if (m_isStarted && m_emitLifetime > 0) {
		m_emitLifetime -= deltaTime;
	}
	if (m_emitLifetime <= 0 && m_emitterMortal){ //it's only for dying emitters relevant
		m_output = UNUSED;
	}
	compute->bind();
	//Bind CS and all SSBO's
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, position_ssbo);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, velocity_ssbo);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, angle_ssbo);

	//Uniform Vars
	compute->sendFloat("deltaTime", (float)deltaTime);
	compute->sendVec3("emitterPos", getPosition()); //can be saved position, or parameter
	compute->sendFloat("fullLifetime", m_particleLifetime);
	compute->sendInt("particleMortal", m_particleMortal);

	if (m_usePointGravity && m_useLocalCoordinates){
		glm::vec3 newPosition(0,0,0);
		newPosition.x = m_emitterPosition.x + m_gravity.x;
		newPosition.y = m_emitterPosition.y + m_gravity.y;
		newPosition.z = m_emitterPosition.z + m_gravity.z;
		compute->sendVec4("gravity", glm::vec4(newPosition, m_gravityImpact));
		
	}
	else{
		compute->sendVec4("gravity", m_gravity);
		
	}
	compute->sendFloat("gravityRange", m_gravityRange);
	compute->sendInt("gravityFunc", m_gravityFunction);

	compute->sendInt("useTrajectory", m_useTrajectory);
	compute->sendInt("useDirectionGravity", m_useDirectionGravity);
	compute->sendInt("usePointGravity", m_usePointGravity);
	compute->sendInt("useChaoticSwarmMotion", m_useChaoticSwarmMotion);

	compute->sendInt("useMovementVertical", m_movementVertical);
	compute->sendInt("useMovementHorizontalX", m_movementHorizontalX);
	compute->sendInt("useMovementHorizontalZ", m_movementHorizontalZ);

	//Unbind CD and all SSBO's
	glDispatchCompute(computeGroupCount, 1, 1); //runs the compute shader
	glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT); //essential for memory synchronisation
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0); //unbind SSBOs
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, 0);
	compute->unbind();
}
void Emitter::pushParticle(int numberNewParticle, glm::vec3 playerPosition){
	auto emitPosition = getPosition() + playerPosition;
	auto areaEmittingXY = getAreaEmittingXY();
	auto areaEmittingXZ = getAreaEmittingXZ();

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, position_ssbo);

	glm::vec4* positions = (glm::vec4*) glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
	if (areaEmittingXY && areaEmittingXZ){
		auto accuracy = getAreaAccuracy(); //how near it will be generated
		auto areaSize = getAreaSize(); //how big the area is
		float randomNumber;
		glm::vec3 pos;

		for (int i = 0; i < numberNewParticle; i++)
		{
			int index = (indexBuffer + i) % numMaxParticle;

			randomNumber = (rand() % (2 * accuracy + 1) - accuracy) / (float)accuracy; //-1 .. 1 with a certain comma accuracy
			pos.x = emitPosition.x + areaSize * randomNumber;
			randomNumber = (rand() % (2 * accuracy + 1) - accuracy) / (float)accuracy; //-1 .. 1
			pos.y = emitPosition.y + areaSize * randomNumber;
			randomNumber = (rand() % (2 * accuracy + 1) - accuracy) / (float)accuracy; //-1 .. 1
			pos.z = emitPosition.z + areaSize * randomNumber;

			positions[index] = glm::vec4(pos, m_particleLifetime);
		}
	}
	else if (areaEmittingXZ){ //emits in a xz area, like rain
		auto accuracy = getAreaAccuracy(); //how near it will be generated
		auto areaSize = getAreaSize(); //how big the area is
		float randomNumber;
		glm::vec3 pos;

		for (int i = 0; i < numberNewParticle; i++)
		{
			int index = (indexBuffer + i) % numMaxParticle;

			randomNumber = (rand() % (2 * accuracy + 1) - accuracy) / (float)accuracy; //-1 .. 1 with a certain comma accuracy
			pos.x = emitPosition.x + areaSize * randomNumber;
			randomNumber = (rand() % (2 * accuracy + 1) - accuracy) / (float)accuracy; //-1 .. 1
			pos.z = emitPosition.z + areaSize * randomNumber;
			pos.y = emitPosition.y;

			positions[index] = glm::vec4(pos, m_particleLifetime);
		}
	}
	else if (areaEmittingXY){ //will be emitted in xy area
		auto accuracy = getAreaAccuracy();
		auto areaSize = getAreaSize();
		float randomNumber;
		glm::vec3 pos;

		for (int i = 0; i < numberNewParticle; i++)
		{
			int index = (indexBuffer + i) % numMaxParticle;

			randomNumber = (rand() % (2 * accuracy + 1) - accuracy) / (float)accuracy; //-1 .. 1 with a certain comma accuracy
			pos.x = emitPosition.x + areaSize * randomNumber;
			randomNumber = (rand() % (2 * accuracy + 1) - accuracy) / (float)accuracy; //-1 .. 1
			pos.y = emitPosition.y + areaSize * randomNumber;
			pos.z = emitPosition.z;

			positions[index] = glm::vec4(pos, m_particleLifetime);
		}
	}
	else{ //will be emitted like a jet
		for (int i = 0; i < numberNewParticle; i++)
		{
			int index = (indexBuffer + i) % numMaxParticle;
			positions[index] = glm::vec4(emitPosition, m_particleLifetime);
		}
	}

	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	auto speed = getSpeed();

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, velocity_ssbo);

	glm::vec4* velocity = (glm::vec4*) glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY); //direction of movement
	for (int i = 0; i < numberNewParticle; i++)
	{
		glm::vec3 vel = this->m_pfunc(); //gets a random vector from the desired vector space
		if (glm::length(vel) != 0.0)
			vel = glm::normalize(vel);
		int index = (indexBuffer + i) % numMaxParticle;
		velocity[index] = glm::vec4(vel, speed);
	}

	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, angle_ssbo);

	glm::vec4* angle = (glm::vec4*) glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY); //angle of movement as option to velocity
	for (int i = 0; i < numberNewParticle; i++)
	{
		int index = (indexBuffer + i) % numMaxParticle;
		int phi = 50.0; // (rand() % 360); //x&z axis
		int theta = 50; //y axis
		angle[index] = glm::vec4(phi, theta, 0.0, 0.0);
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	indexBuffer = (indexBuffer + numberNewParticle) % numMaxParticle;
}
void Emitter::generateParticle(glm::vec3 playerPosition)
{
	switch (m_output)
	{
	case CONSTANT: //we generate constant new particle
		if (glfwGetTime() - generateTime >= m_emitFrequency){
			deltaTime = glfwGetTime() - generateTime;
			while (deltaTime >= m_emitFrequency) {
				pushParticle(particlesPerEmit, playerPosition);
				deltaTime -= m_emitFrequency;
			}
			generateTime = glfwGetTime();
		}
		break;
	case ONCE: //we generate only one time particle
		if (glfwGetTime() - generateTime >= m_emitFrequency){
			pushParticle(particlesPerEmit, playerPosition);
			m_output = UNUSED;
		}
		break;
	case UNUSED: // we dont generate new particle
		break;
	}
}

void Emitter::render(ShaderProgram* emitterShader, Camera &cam)
{
	auto useTexture = getUseTexture();
	glDepthMask(GL_FALSE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //how we calculate transperancy

	if (getUsePointSprites()){ //if we dont use a geometry shader..
		emitterShader->bind();
		glBindBuffer(GL_ARRAY_BUFFER, position_ssbo);

		if (useTexture){
			glEnable(GL_POINT_SPRITE);
			glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);	//the fragment color gets interpolated
			glEnable(GL_PROGRAM_POINT_SIZE);
			std::string s = "tex";
			for (int i = 0; i < m_textureCount; i++){
				std::string i2 = std::to_string(i);
				s += i2;
				emitterShader->sendSampler2D(s, m_textureList.at(i)->getTexture(), i + 1);
				s = "tex";
			}
			emitterShader->sendInt("textureCount", m_textureCount);
		}
		//Uniform Vars
		emitterShader->sendFloat("fullLifetime", (float)m_particleLifetime);
		emitterShader->sendInt("particleMortal", m_particleMortal);
		emitterShader->sendFloat("birthTime", m_birthTime);
		emitterShader->sendFloat("deathTime", m_deathTime);
		emitterShader->sendMat4("viewMatrix", cam.getViewMatrix());
		emitterShader->sendMat4("projectionMatrix", cam.getProjectionMatrix());
		emitterShader->sendInt("useTexture", useTexture);
		emitterShader->sendFloat("blendingTime", m_blendingTime);
		emitterShader->sendFloatArray("time", 4, blendingTime);
		if (m_useScaling){
			emitterShader->sendInt("useScaling", 1);
			emitterShader->sendInt("scalingCount", m_scalingCount);
			emitterShader->sendFloatArray("scalingData", m_scalingCount, m_scalingData);
			emitterShader->sendFloat("fullLifetime", (float)m_particleLifetime);
			emitterShader->sendInt("particleMortal", m_particleMortal);
		}
		else{
			emitterShader->sendInt("useScaling", 0);
			emitterShader->sendFloat("size", m_particleDefaultSize);
		}
		glVertexPointer(4, GL_FLOAT, 0, (void*)0);
		glEnableClientState(GL_VERTEX_ARRAY);
		glDrawArrays(GL_POINTS, 0, numMaxParticle);
		glDisableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		emitterShader->unbind();
	}
	else if (getUseGeometryShader() && useTexture){
		emitterShader->bind();
		glBindBuffer(GL_ARRAY_BUFFER, position_ssbo);

		//Uniform Vars
		emitterShader->sendMat4("viewMatrix", cam.getViewMatrix());
		emitterShader->sendMat4("projectionMatrix", cam.getProjectionMatrix());
		emitterShader->sendFloat("birthTime", m_birthTime);
		emitterShader->sendFloat("deathTime", m_deathTime);
		emitterShader->sendFloat("fullLifetime", (float)m_particleLifetime);
		emitterShader->sendVec4("camPos", cam.getPosition());
		emitterShader->sendInt("rotateLeft", m_rotateLeft);
		emitterShader->sendFloat("rotationSpeed", m_rotationSpeed);
		emitterShader->sendInt("useTexture", useTexture);
		emitterShader->sendFloat("blendingTime", m_blendingTime);
		emitterShader->sendFloatArray("time", 4, blendingTime);
		if (m_useTexture){
			std::string s = "tex";
			for (int i = 0; i < m_textureCount; i++){
				std::string i2 = std::to_string(i);
				s += i2;
				emitterShader->sendSampler2D(s, m_textureList.at(i)->getTexture(), i + 1);
				s = "tex";
			}
			emitterShader->sendInt("textureCount", m_textureCount);
		}
		if (m_useScaling){
			emitterShader->sendInt("useScaling", 1);
			emitterShader->sendInt("scalingCount", m_scalingCount);
			emitterShader->sendFloatArray("scalingData", m_scalingCount, m_scalingData);
			emitterShader->sendInt("particleMortal", m_particleMortal);
		}
		else{
			emitterShader->sendInt("useScaling", 0);
			emitterShader->sendFloat("size", m_particleDefaultSize);
		}
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glDrawArrays(GL_POINTS, 0, numMaxParticle);
		glDisableVertexAttribArray(0);

		glDisableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		emitterShader->unbind();
	}
	else{
		perror("Problem in Emitter.cpp: Geometry Shader maybe miss a texture");
	}
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}

void Emitter::updateSize()
{
	if (!m_particleMortal){
		numMaxParticle = particlesPerEmit;
		m_deathTime = 0.0;
	}
	else{
		numMaxParticle = (int)((particlesPerEmit * m_particleLifetime) / m_emitFrequency) + 1; //+1 for comma numbers
	}
	computeGroupCount = (int)(numMaxParticle / 16) + 1; //+1 to make sure every particle gets updated. 16 is the local_size_x in the CS

	loadBuffer();
}

//GS & PS swichting
void Emitter::switchToGeometryShader(){
	m_usePointSprites = false;
	m_useGeometryShader = true;
}
void Emitter::switchToPointSprites(){
	m_usePointSprites = true;
	m_useGeometryShader = false;
}

//physic:

void Emitter::usePhysicTrajectory(glm::vec4 gravity, float speed){
	 setGravity(gravity);
	 m_useTrajectory = true;
	 m_useDirectionGravity = false;
	 m_usePointGravity = false;
	 m_useChaoticSwarmMotion = false;

	 setSpeed(speed);
}
void Emitter::usePhysicDirectionGravity(glm::vec4 gravity, float speed){
	setGravity(gravity);
	m_useTrajectory = false;
	m_useDirectionGravity = true;
	m_usePointGravity = false;
	m_useChaoticSwarmMotion = false;

	setSpeed(speed);
}
//gravityFunc: 1=constant 2=x^4 3=cosinus
void Emitter::usePhysicPointGravity(glm::vec3 point, float gravityImpact, float gravityRange, int gravityFunction, float speed, bool useLocalCoordinates){
	m_useTrajectory = false;
	m_useDirectionGravity = false;
	m_usePointGravity = true;
	m_useChaoticSwarmMotion = false;

	m_gravityImpact = gravityImpact;
	m_useLocalCoordinates = useLocalCoordinates;
	setGravity(glm::vec4(point, gravityImpact));
	
	setSpeed(speed);
	m_gravityFunction = gravityFunction;
	m_gravityRange = gravityRange;
}
void Emitter::usePhysicSwarmCircleMotion(bool movementVertical, bool movementHorizontalX, bool movementHorizontalZ, float speed){
	m_useTrajectory = false;
	m_useDirectionGravity = false;
	m_usePointGravity = false;
	m_useChaoticSwarmMotion = true;

	m_movementVertical = movementVertical;
	m_movementHorizontalX = movementHorizontalX;
	m_movementHorizontalZ = movementHorizontalZ;

	setSpeed(speed);
}

//setters:

void Emitter::setOutputMode(const int OUTPUT)
{
	m_output = static_cast<FLOW> (OUTPUT);
}
void Emitter::setOutputType(int outputType)
{
	m_outputType = outputType;
}
void Emitter::setPosition(glm::vec3 newPosition)
{
	m_emitterPosition = newPosition;
}
void Emitter::setLocalPosition(glm::vec3 newLocalPosition)
{
	glm::vec3 psPosition(
		m_emitterPosition.x - m_localPosition.x,
		m_emitterPosition.y - m_localPosition.y,
		m_emitterPosition.z - m_localPosition.z);
	m_localPosition = newLocalPosition;
	setPosition(glm::vec3(
		psPosition.x + m_localPosition.x,
		psPosition.y + m_localPosition.y, 
		psPosition.z + m_localPosition.z));
}
void Emitter::movePosition(glm::vec3 playerPosition)
{
	setPosition(getPosition()+playerPosition);
}
//void Emitter::setEmitterMortality(double emitterLifetime)
//{
//	if (emitterLifetime < 0.001)
//		m_emitterMortal = false;
//	else
//		m_emitterMortal = true;
//}
void Emitter::setEmitterLifetime(double emitterLifetime)
{
	if (emitterLifetime < 0.001)
		m_emitterMortal = false;
	else
		m_emitterMortal = true;
	m_emitLifetime = emitterLifetime;
}
void Emitter::setEmitFrequency(float newEmitFrequency)
{
	m_emitFrequency = newEmitFrequency;
	updateSize();
}
void Emitter::setParticlesPerEmit(int numberParticles)
{
	particlesPerEmit = numberParticles;
	updateSize();
}
void Emitter::setParticleLifetime(float newLifetime)
{
	m_particleLifetime = newLifetime;
	updateSize();
}
void Emitter::setParticleMortality(bool particleMortality)
{
	m_particleMortal = particleMortality;
	if (!m_particleMortal || m_emitFrequency == 0.0 || m_particleLifetime == 0.0){
		m_particleMortal = false;
		m_emitFrequency = 0.0;
		m_deathTime = 0.0;
		m_output = static_cast<FLOW> (1); //set output to once
	}
}
void Emitter::setGravity(glm::vec4 newGravity)
{
	m_gravity = newGravity;
}
//void Emitter::setComputeShader(std::string address){
//	glDeleteProgram(compute.handle);
//	delete compute;
//	ComputeShader csParticle(loadShaderSource(SHADERS_PATH + address));
//	compute = new ShaderProgram(csParticle);
//	glDeleteShader(compute.handle);
//}
void Emitter::setSpeed(float speed){
	m_speed = speed;
}
void Emitter::setAreaEmitting(bool areaEmittingXY, bool areaEmittingXZ, float size, int accuracy){
	m_areaEmittingXY = areaEmittingXY; //emits in a XY area
	m_areaEmittingXZ = areaEmittingXZ; //emits in XZ, like Rain
	m_areaSize = size;
	if (accuracy > 10000)
		m_areaAccuracy = 10000;
	else m_areaAccuracy = accuracy;

}
void Emitter::setMovable(bool on){
	m_movable = on;
}
void Emitter::setStartTime(double time)
{
	m_startTime = time;
}

void Emitter::addTexture(Texture* texture, float time){
	if (m_textureCount == 0){
		blendingTime[0] = 1.0f; //bei wie viel prozent der lebenszeit es eingeblendet wird
		m_textureList.push_back(texture);
		m_textureCount++;

	}  // first added texture has always the blending time 1
	else if (m_textureCount > 0 && m_textureCount < 4 && blendingTime[m_textureCount - 1] > time && 0 <= time <= 1.0){
		blendingTime[m_textureCount] = time;
		m_textureList.push_back(texture);
		m_textureCount++;

	}
	else if (m_textureCount == 4){
		perror("just 4 textures per emitter possible");
	}
	else if (blendingTime[m_textureCount - 1] < time){
		perror("the textures must be added ordered descending by their blending time");
	}
}

void Emitter::defineLook(bool useTexture, float particleSize, float birthTime, float deathTime, float blendingTime, bool rotateLeft, float rotationSpeed){
	m_useTexture = useTexture;

	m_birthTime = birthTime;
	m_deathTime = deathTime;
	m_blendingTime = blendingTime;

	m_rotateLeft = rotateLeft;
	m_rotationSpeed = rotationSpeed;

	m_particleDefaultSize = particleSize;
}
void Emitter::defineLook(bool useTexture, std::vector<float> scalingSize, std::vector<float> scalingMoment,
	float birthTime, float deathTime, float blendingTime, bool rotateLeft, float rotationSpeed){
	m_useTexture = useTexture;
	
	m_birthTime = birthTime;
	m_deathTime = deathTime;
	m_blendingTime = blendingTime;

	m_rotateLeft = rotateLeft;
	m_rotationSpeed = rotationSpeed;

	if (scalingSize.empty() == false && scalingMoment.empty() == false){
		if (scalingSize.size() > 16 || scalingMoment.size() > 16)
			perror("Error in Emitter: We just support a maximum of 16 scaling-steps");
		else{
			if (scalingSize.size() != scalingMoment.size())
				perror("Error in Emitter: Size of scalingSize & scalingMoment is not equal");
			else {
				for (int i = 0; i < scalingSize.size(); i++){
					m_scalingData[2 * i] = scalingMoment[i];
					m_scalingData[2 * i + 1] = scalingSize[i];
					m_scalingCount = m_scalingCount + 2;
				}
				m_useScaling = true;
			}
		}
	}
}

//Velocity Getter
glm::vec3 Emitter::useVelocityZero(){
	return glm::vec3(0.0, 0.0, 0.0);
}
glm::vec3 Emitter::useVelocityLeftQuarterCircle(){
	return glm::vec3(((rand() % 100) / 100.0f) - 1.0f,
					((rand() % 100) / 100.0f),
					0.0f);
}
glm::vec3 Emitter::useVelocityRightQuarterCircle(){
	return glm::vec3(((rand() % 100) / 100.0f),
					((rand() % 100) / 100.0f),
					0.0f);
}
glm::vec3 Emitter::useVelocitySemiCircle(){
	return glm::vec3(((rand() % 200) / 100.0f) - 1.0f,
					((rand() % 100) / 100.0f),
					0.0f);
}
glm::vec3 Emitter::useVelocityCircle(){
	return	glm::vec3(((rand() % 200) / 100.0f) - 1.0f,
					((rand() % 200) / 100.0f) - 1.0f,
					0.0f);
}
glm::vec3 Emitter::useVelocitySemiSphere(){
	return glm::vec3(((rand() % 200) / 100.0f) - 1.0f,
					((rand() % 100) / 100.0f),
					((rand() % 200) / 100.0f) - 1.0f);
}
glm::vec3 Emitter::useVelocitySphere(){
	return glm::vec3(((rand() % 200) / 100.0f) - 1.0f,
					((rand() % 200) / 100.0f) - 1.0f,
					((rand() % 200) / 100.0f) - 1.0f);
}
void Emitter::setVelocity(int velocityType){
	switch (velocityType)
	{
	case 0:
		m_velocityType = 0;
		m_pfunc = &Emitter::useVelocityZero;
		break;
	case 1:
		m_velocityType = 1;
		m_pfunc = &Emitter::useVelocityLeftQuarterCircle;
		break;
	case 2:
		m_velocityType = 2;
		m_pfunc = &Emitter::useVelocityRightQuarterCircle;
		break;
	case 3:
		m_velocityType = 3;
		m_pfunc = &Emitter::useVelocitySemiCircle;
		break;
	case 4:
		m_velocityType = 4;
		m_pfunc = &Emitter::useVelocityCircle;
		break;
	case 5:
		m_velocityType = 5;
		m_pfunc = &Emitter::useVelocitySemiSphere;
		break;
	case 6:
		m_velocityType = 6;
		m_pfunc = &Emitter::useVelocitySphere;
		break;
	default:
		m_velocityType = -1;
		perror("Error: setVelocity_Ungültige Eingabe");
		break;
	}
}
int Emitter::getVelocityType()
{
	return m_velocityType;
}

//getters:
int Emitter::getOutputMode()
{
	return m_output;
}
int Emitter::getOutputType()
{
	return m_outputType;
}
glm::vec3 Emitter::getPosition()
{
	return m_emitterPosition;
}
glm::vec3 Emitter::getLocalPosition()
{
	return m_localPosition;
}
bool Emitter::getEmitterMortality()
{
	return m_emitterMortal;
}
double Emitter::getEmitterLifetime()
{
	return m_emitLifetime;
}
double Emitter::getEmitFrequency()
{
	return m_emitFrequency;
}
int Emitter::getParticlesPerEmit()
{
	return particlesPerEmit;
}
float Emitter::getParticleLifetime()
{
	return m_particleLifetime;
}
bool Emitter::getParticleMortality(){
	return m_particleMortal;
}
glm::vec4 Emitter::getGravity()
{
	return m_gravity;
}
float Emitter::getSpeed(){
	return m_speed;
}
bool Emitter::getAreaEmittingXY(){
	return m_areaEmittingXY;
}
bool Emitter::getAreaEmittingXZ(){
	return m_areaEmittingXZ;
}
float Emitter::getAreaSize(){
	return m_areaSize;
}
int Emitter::getAreaAccuracy(){
	return m_areaAccuracy;
}
bool Emitter::getUseTexture(){
	return m_useTexture;
}
bool Emitter::getUseGeometryShader(){
	return m_useGeometryShader;
}
bool Emitter::getUsePointSprites(){
	return m_usePointSprites;
}
float Emitter::getRotationSpeed(){
	return m_rotationSpeed;
}
bool Emitter::getMovable(){
	return m_movable;
}
double Emitter::getStartTime(){
	return m_startTime;
}

bool Emitter::getPhysicTrajectory(){
	return m_useTrajectory;
}
bool Emitter::getPhysicDirectionGravity(){
	return m_useDirectionGravity;
}
bool Emitter::getPhysicPointGravity(){
	return m_usePointGravity;
}
bool Emitter::getPhysicSwarmCircleMotion(){
	return m_useChaoticSwarmMotion;
}
float Emitter::getPhysicAttGravityImpact(){
	return m_gravityImpact;
}
bool Emitter::getPhysicAttUseLocalCoordinates(){
	return m_useLocalCoordinates;
}
float Emitter::getPhysicAttGravityRange(){
	return m_gravityRange;
}
int Emitter::getPhysicAttGravityFunction(){
	return m_gravityFunction;
}
float Emitter::getPhysicAttSpeed(){
	return m_speed;
}
bool Emitter::getPhysicAttMovementVertical(){
	return m_movementVertical;
}
bool Emitter::getPhysicAttMovementHorizontalX(){
	return m_movementHorizontalX;
}
bool Emitter::getPhysicAttMovementHorizontalZ(){
	return m_movementHorizontalZ;
}

float Emitter::getTexBirthTime(){
	return m_birthTime;
}
float Emitter::getTexDeathTime(){
	return m_deathTime;
}
int Emitter::getTexScalingCount(){
	return m_scalingCount;
}
bool Emitter::getTexUseScaling(){
	return m_useScaling;
}
float Emitter::getTexParticleDefaultSize(){
	return m_particleDefaultSize;
}
bool Emitter::getTexRotateLeft(){
	return m_rotateLeft;
}
int Emitter::getTexTextureCount(){
	return m_textureCount;
}
float Emitter::getTexBlendingTime(){
	return m_blendingTime;
}

//set constructor attributes
void Emitter::setAttributes(){
	updateTime = 0.0;
	deltaTime = 0.0;
	generateTime = 0.0;

	//Var how the Output should flow
	m_output = static_cast<FLOW> (-1);
	m_outputType = -1;

	//Var for the buffer iteration
	indexBuffer = 0;

	//property of the emitter
	m_emitterPosition = glm::vec3(0.0, 0.0, 0.0);
	m_localPosition = glm::vec3(0.0, 0.0, 0.0);
	m_emitterMortal = false;
	m_emitLifetime = 0.0;
	emitterLifetime = 0.0;
	m_emitFrequency = 0.0;
	m_startTime = 0.0;
	m_isStarted = false;

	//Number of particles
	numMaxParticle = 0;
	particlesPerEmit = 0;
	computeGroupCount = 0;

	//property of the particle
	m_particleLifetime = 0.0;
	m_particleMortal = true;
	m_gravity = glm::vec4(0.0, -1.0, 0.0, 1.0);

	//Var for time measure
	generateTime = 0.0;
	updateTime = 0.0;
	deltaTime = 0.0;

	//Var for emitting in an Area
	m_areaEmittingXY = false;
	m_areaEmittingXZ = false;
	m_areaSize = 3.0;
	m_areaAccuracy = 1000;

	//Property of the rendering
	m_useGeometryShader = false;
	m_usePointSprites = true;
	m_useTexture = false;

	//defines if the emitter is movable
	m_movable = false;

	//Property of the Geometry Shader
	m_rotationSpeed = 0.0;
	
	//velocity type
	m_velocityType = 0;

	//physic type
	m_useTrajectory = false;
	m_usePointGravity = false;
	m_useDirectionGravity = true;
	m_useChaoticSwarmMotion = false;

	//physic attributes
	float m_gravityImpact = 0.0;
	glm::vec3 m_point = glm::vec3(0, 0, 0);
	bool m_backtoSource = false;

	m_gravityRange = 0.0f;
	m_gravityFunction = 0;
	m_speed = 0.0f;
	m_movementVertical = false;
	m_movementHorizontalX = false;
	m_movementHorizontalZ = false;

	//rotating, scaling and blending of the particle
	m_birthTime = 0.0;
	m_deathTime = 0.0;
	m_scalingCount = 0;
	m_useScaling = false;
	m_particleDefaultSize = 1.0;
	m_rotateLeft = false;
	m_textureCount = 0;
	m_blendingTime = 0.0f;
}