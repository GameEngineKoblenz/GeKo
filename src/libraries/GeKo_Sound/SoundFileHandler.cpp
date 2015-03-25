#include "SoundFileHandler.h"
#include <cstdio>


/**
Creates a Soundfilehandler with the max number of Sources = maxSources
Feel free to set a really high number
Sets a default Listener at the position 0 0 0 with Velocity 0 0 0, an "at" vector 0 0 1 and an up vector 0 1 0
Be sure to update the Listener-Properties as the player moves
Most important method is generateSource : You can either assign a name to the Source or reference it by the ID it gets assigned automatically
The rest of the methods are pretty self-explanatory
*/
///Creates a Soundfilehandler with the max number of Sources = maxSources. Also sets default Listener-Properties.
SoundFileHandler::SoundFileHandler(int maxSources){
	maxNumberOfSources = maxSources;
	
	//initializes the ALuint-arrays sources and buffers
	sources = new ALuint[maxSources];
	buffers = new ALuint[maxSources];
	//initializes the counter for the number of Sources
	numberOfSources = 0;

	ALCdevice *device;
	ALCcontext *context;
	device = alcOpenDevice(NULL);
	if (!device){
		std::cout << "No sound device" << std::endl;
		system("PAUSE");
		return;
	}
	context = alcCreateContext(device, NULL);
	alcMakeContextCurrent(context);
	if (!context){
		std::cout << "No sound context" << std::endl;
		system("PAUSE");
		return;
	}

	glm::vec3 pos = { 0.0, 0.0, 0.0 };
	glm::vec3 at = { 0.0, 0.0, -1.0 };
	glm::vec3 up = { 0.0, 1.0, 0.0 };
	SoundFileHandler::updateListenerPosition(pos);
	SoundFileHandler::updateListenerVelocity(pos);
	SoundFileHandler::updateListenerOrientation(at, up);
}

/**Destroys the SoundFileHandler : 
Stops all Sources, then deletes all Sources and Buffers.
*/
/// Destroys the SoundFileHandler
SoundFileHandler::~SoundFileHandler(){
	std::cout << "Destroying Sound-Handler" << std::endl;

	for (int i = 0; i < numberOfSources; i++){
		alSourceStop(sources[i]);
		std::cout << "Stopped Source " << i << std::endl;
		alDeleteSources(1, &sources[i]);
		std::cout << "Deleted Source " << i << std::endl;
		alDeleteBuffers(1, &buffers[i]);
		std::cout << "Deleted Buffer " << i << std::endl;
	}

	//alcMakeContextCurrent(NULL);
	//alcDestroyContext(context);
	//alcCloseDevice(device);

}

int SoundFileHandler::endWithError(const char* msg){
	std::cout << msg << "\n";
	system("PAUSE");
	return 0;
}

/**Generates a Source at the position importing the Soundfile (.WAV) from the filepath 
The Source has an ID which starts at 0 and increases for every successfully generated Source
Be careful with the indices when a file cannot be read properly (path is wrong)
*/
void SoundFileHandler::generateSource(ALfloat position[3], const char *filepath){
	if (numberOfSources >= maxNumberOfSources){
		std::cout << "Maximale Anzahl Quellen erreicht" << std::endl;
		return;
	}

	fopen(filepath, "rb");

	if (NULL == fp)
	{
		std::cout << "Soundfile not Found, please check spelling and path" << std::endl;
		return;
	}

	alGenBuffers(1, &buffers[numberOfSources]);
	alGenSources(1, &sources[numberOfSources]);
	if (SoundFileHandler::generateBufferData(filepath, numberOfSources))
	{
		alSourcei(sources[numberOfSources], AL_BUFFER, buffers[numberOfSources]);
		ALfloat vel[] = { 0.0, 0.0, 0.0 };
		if (tempformat == AL_FORMAT_STEREO16 || tempformat == AL_FORMAT_STEREO8)
		{
			alSourcef(sources[numberOfSources], AL_PITCH, 2.0);
		}
		else
		{
			alSourcef(sources[numberOfSources], AL_PITCH, 1.0);
		}
		alSourcef(sources[numberOfSources], AL_GAIN, 1.0);
		alSourcefv(sources[numberOfSources], AL_POSITION, position);
		alSourcefv(sources[numberOfSources], AL_VELOCITY, vel);
		alSourcei(sources[numberOfSources], AL_LOOPING, AL_TRUE);
		numberOfSources++;
	}
}

/**Generates a Source at the position importing the Soundfile (.WAV) from the filepath
The Source has an ID which starts at 0 and increases for every successfully generated Source
Be careful with the indices when a file cannot be read properly (path is wrong)
*/
void SoundFileHandler::generateSource(glm::vec3 position, const char *filepath){
	ALfloat temp[3];
	temp[0] = position.x;
	temp[1] = position.y;
	temp[2] = position.z;
	SoundFileHandler::generateSource(temp, filepath);
}

/**
Generates a Source at the Position "position", and attaches a buffer to it, which reads the .wav file from "filepath"
Giving a wrong or corrupted file causes an output of the error and termination.
The velocity of the Source is by default 0 0 0.
The Source has an autogenerated ID (starting with 0), but it can also be referenced by its name, given to it in this method.
You should use this method primarily for creating Sources.
*/
///Generates a Source at Position position with the filepath and a name
void SoundFileHandler::generateSource(std::string name, ALfloat position[3], const char *filepath){
	try{
		std::cout<<sourceMap.at(name)<<std::endl;
		std::cout << "Name allready taken" << std::endl;
		return;
	}
	catch (const std::out_of_range& oor)
	{


		if (numberOfSources >= maxNumberOfSources){
			std::cout << "Maximale Anzahl Quellen erreicht" << std::endl;
			return;
		}

		fopen(filepath, "rb");

		if (NULL == fp)
		{
			std::cout << "Soundfile not Found, please check spelling and path" << std::endl;
			return;
		}

		alGenBuffers(1, &buffers[numberOfSources]);
		alGenSources(1, &sources[numberOfSources]);

		if (SoundFileHandler::generateBufferData(filepath, numberOfSources))
		{
			alSourcei(sources[numberOfSources], AL_BUFFER, buffers[numberOfSources]);
			ALfloat vel[] = { 0.0, 0.0, 0.0 };
			if (tempformat == AL_FORMAT_STEREO16 || tempformat == AL_FORMAT_STEREO8)
			{
				alSourcef(sources[numberOfSources], AL_PITCH, 2.0);
			}
			else
			{
				alSourcef(sources[numberOfSources], AL_PITCH, 1.0);
			}
			alSourcef(sources[numberOfSources], AL_GAIN, 1.0);
			alSourcefv(sources[numberOfSources], AL_POSITION, position);
			alSourcefv(sources[numberOfSources], AL_VELOCITY, vel);
			alSourcei(sources[numberOfSources], AL_LOOPING, AL_TRUE);
			sourceMap.emplace(name, numberOfSources);
			numberOfSources++;
		}
	}

}

/**
Generates a Source at the Position "position", and attaches a buffer to it, which reads the .wav file from "filepath"
Giving a wrong or corrupted file causes an output of the error and termination.
The velocity of the Source is by default 0 0 0.
The Source has an autogenerated ID (starting with 0), but it can also be referenced by its name, given to it in this method.
You should use this method primarily for creating Sources.
*/
///Generates a Source at Position position with the filepath and a name

void SoundFileHandler::generateSource(std::string name, glm::vec3 position, const char *filepath){
	ALfloat temp[3];
	temp[0] = position.x;
	temp[1] = position.y;
	temp[2] = position.z;
	SoundFileHandler::generateSource(name, temp, filepath);
}

void SoundFileHandler::setPitch(int i, float f){
	alSourcef(sources[i], AL_PITCH, f);
}

void SoundFileHandler::setPitch(std::string name, float f){
	try {
		int i = sourceMap.at(name);
		alSourcef(sources[i], AL_PITCH, f);

	}
	catch (const std::out_of_range& oor) {
		std::cerr << "Out of Range error: " << oor.what() << "  Check spelling of Source " << '\n';
		return;
	}
}

void SoundFileHandler::setGain(int i, float f){
	alSourcef(sources[i], AL_GAIN, f);
}

void SoundFileHandler::setGain(std::string name, float f){
	try {
		int i = sourceMap.at(name);
		alSourcef(sources[i], AL_GAIN, f);

	}
	catch (const std::out_of_range& oor) {
		std::cerr << "Out of Range error: " << oor.what() <<"  Check spelling of Source "<<  '\n';
		return;
	}
}

void SoundFileHandler::updateListenerPosition(ALfloat position[3]){
	alListenerfv(AL_POSITION, position);
}

void SoundFileHandler::updateListenerPosition(glm::vec3 position){
	ALfloat temp[3];
	temp[0] = position.x;
	temp[1] = position.y;
	temp[2] = position.z;
	alListenerfv(AL_POSITION, temp);
}

glm::vec3 SoundFileHandler::getListenerPosition(){ 
	ALfloat position[3];
	alGetListenerfv(AL_POSITION, position);
	glm::vec3 temp = { position[0], position[1], position[2]};
	return temp;
}

void SoundFileHandler::updateListenerVelocity(ALfloat velocity[3]){
	alListenerfv(AL_VELOCITY, velocity);
}

void SoundFileHandler::updateListenerVelocity(glm::vec3 velocity){
	ALfloat temp[3];
	temp[0] = velocity.x;
	temp[1] = velocity.y;
	temp[2] = velocity.z;
	alListenerfv(AL_VELOCITY, temp);
}

glm::vec3 SoundFileHandler::getListenerVelocity(){
	ALfloat velocity[3];
	alGetListenerfv(AL_POSITION, velocity);
	glm::vec3 temp = { velocity[0], velocity[1], velocity[2] };
	return temp;
}

void SoundFileHandler::updateListenerOrientation(ALfloat orientation[6]){
	alListenerfv(AL_ORIENTATION, orientation);
}

void SoundFileHandler::updateListenerOrientation(glm::vec3 at, glm::vec3 up){
	ALfloat temp[6];
	temp[0] = at.x;
	temp[1] = at.y;
	temp[2] = at.z;
	temp[3] = up.x;
	temp[4] = up.y;
	temp[5] = up.z;
	alListenerfv(AL_ORIENTATION, temp);
}

void SoundFileHandler::updateSourcePosition(int i, ALfloat position[3]){
	alSourcefv(sources[i], AL_POSITION, position);
}

void SoundFileHandler::updateSourcePosition(std::string name, ALfloat position[3]){
	try {
		int i = sourceMap.at(name);
		SoundFileHandler::updateSourcePosition(i, position);
	}
	catch (const std::out_of_range& oor) {
		std::cerr << "Out of Range error: " << oor.what() << "  Check spelling of Source." << '\n';
		return;
	}
}

void SoundFileHandler::updateSourcePosition(int i, glm::vec3 position){
	ALfloat temp[3];
	temp[0] = position.x;
	temp[1] = position.y;
	temp[2] = position.z;
	alSourcefv(sources[i], AL_POSITION, temp);
}

void SoundFileHandler::updateSourcePosition(std::string name, glm::vec3 position){
	try {
		int i = sourceMap.at(name);
		SoundFileHandler::updateSourcePosition(i, position);
	}
	catch (const std::out_of_range& oor) {
		std::cerr << "Out of Range error: " << oor.what() << "  Check spelling of Source." << '\n';
		return;
	}
}

glm::vec3 SoundFileHandler::getSourcePosition(int i){
	ALfloat position[3];
	alGetSourcefv(sources[i], AL_POSITION, position);
	glm::vec3 temp = { position[0], position[1], position[2] };
	return temp;
}

glm::vec3 SoundFileHandler::getSourcePosition(std::string name){
	try {
		int i = sourceMap.at(name);
		ALfloat position[3];
		alGetSourcefv(sources[i], AL_POSITION, position);
		glm::vec3 temp = { position[0], position[1], position[2] };
		return temp;
	}
	catch (const std::out_of_range& oor) {
		std::cerr << "Out of Range error: " << oor.what() << "  Check spelling of Source. WARNING : returning 0,0,0 as vector" << '\n';
		return { 0.0, 0.0, 0.0 };
	}
}

void SoundFileHandler::updateSourceVelocity(int i, ALfloat velocity[3]){
	alSourcefv(sources[i], AL_VELOCITY, velocity);
}

void SoundFileHandler::updateSourceVelocity(std::string name, ALfloat velocity[3]){
	try {
		int i = sourceMap.at(name);
		alSourcefv(sources[i], AL_VELOCITY, velocity);
	}
	catch (const std::out_of_range& oor) {
		std::cerr << "Out of Range error: " << oor.what() << "  Check spelling of Source." << '\n';
		return;
	}
}

void SoundFileHandler::updateSourceVelocity(std::string name, glm::vec3 velocity){
	try {
		int i = sourceMap.at(name);
		SoundFileHandler::updateSourceVelocity(i, velocity);
	}
	catch (const std::out_of_range& oor) {
		std::cerr << "Out of Range error: " << oor.what() << "  Check spelling of Source." << '\n';
		return;
	}
}

void SoundFileHandler::updateSourceVelocity(int i, glm::vec3 velocity){
	ALfloat temp[3];
	temp[0] = velocity.x;
	temp[1] = velocity.y;
	temp[2] = velocity.z;
	alSourcefv(sources[i], AL_VELOCITY, temp);
}

void SoundFileHandler::enableLooping(int i){
	alSourcei(sources[i], AL_LOOPING, AL_TRUE);
}

void SoundFileHandler::enableLooping(std::string name){
	try {
		int i = sourceMap.at(name);
		SoundFileHandler::enableLooping(i);
	}
	catch (const std::out_of_range& oor) {
		std::cerr << "Out of Range error: " << oor.what() << "  Check spelling of Source." << '\n';
		return;
	}
}

void SoundFileHandler::disableLooping(int i){
	alSourcei(sources[i], AL_LOOPING, AL_FALSE);
}

void SoundFileHandler::disableLooping(std::string name){
	try {
		int i = sourceMap.at(name);
		SoundFileHandler::disableLooping(i);
	}
	catch (const std::out_of_range& oor) {
		std::cerr << "Out of Range error: " << oor.what() << "  Check spelling of Source." << '\n';
		return;
	}
}

void SoundFileHandler::playSource(int i){
	alSourcePlay(sources[i]);
}

void SoundFileHandler::playSource(std::string name){
	try {
		int i = sourceMap.at(name);
		SoundFileHandler::playSource(i);
	}
	catch (const std::out_of_range& oor) {
		std::cerr << "Out of Range error: " << oor.what() << "  Check spelling of Source." << '\n';
		return;
	}
}

void SoundFileHandler::pauseSource(int i){
	alSourcePause(sources[i]);
}

void SoundFileHandler::pauseSource(std::string name){
	try {
		int i = sourceMap.at(name);
		SoundFileHandler::pauseSource(i);
	}
	catch (const std::out_of_range& oor) {
		std::cerr << "Out of Range error: " << oor.what() << "  Check spelling of Source." << '\n';
		return;
	}
}

void SoundFileHandler::stopSource(int i){
	alSourceStop(sources[i]);
}

void SoundFileHandler::stopSource(std::string name){
	try {
		int i = sourceMap.at(name);
		SoundFileHandler::stopSource(i);
	}
	catch (const std::out_of_range& oor) {
		std::cerr << "Out of Range error: " << oor.what() << "  Check spelling of Source." << '\n';
		return;
	}
}
/** plays all Sources*/
void SoundFileHandler::playAllSources(){
	for (int i = 0; i < maxNumberOfSources; i++){
		alSourcePlay(sources[i]);
		//std::cout << "Playing Source "<< i << std::endl;
	}
}

void SoundFileHandler::stopAllSources(){
	for (int i = 0; i < numberOfSources; i++){
		alSourceStop(sources[i]);
	}
}

void SoundFileHandler::pauseAllSources(){
	for (int i = 0; i < numberOfSources; i++){
		alSourcePause(sources[i]);
	}
}

int SoundFileHandler::generateBufferData(const char *filepath, int numberOfSources){
	
	/*char type[4];
	int size, chunkSize;
	short formatType, channels;
	int sampleRate, avgBytesPerSec;
	short bytesPerSample, bitsPerSample;
	int dataSize;
	ALuint frequency;
	ALenum format;
	FILE *fp = NULL;*/
	//unsigned char* buf;

	//fopen(&fp, filepath, "rb");

	

	fread(type, sizeof(char), 4, fp);
	if (type[0] != 'R' || type[1] != 'I' || type[2] != 'F' || type[3] != 'F')
	{
		std::cout << "No Riff" << std::endl;
		return endWithError("No Riff");

	}

	fread(&size, sizeof(int), 1, fp);
	fread(type, sizeof(char), 4, fp);
	if (type[0] != 'W' || type[1] != 'A' || type[2] != 'V' || type[3] != 'E')
		return endWithError("Not Wave");

	fread(type, sizeof(char), 4, fp);
	if (type[0] != 'f' || type[1] != 'm' || type[2] != 't' || type[3] != ' ')
		return endWithError("Not fmt ");

	fread(&chunkSize, sizeof(int), 1, fp);
	fread(&formatType, sizeof(short), 1, fp);
	fread(&channels, sizeof(short), 1, fp);
	fread(&sampleRate, sizeof(int), 1, fp);
	fread(&avgBytesPerSec, sizeof(int), 1, fp);
	fread(&bytesPerSample, sizeof(short), 1, fp);
	fread(&bitsPerSample, sizeof(short), 1, fp);

	fread(type, sizeof(char), 4, fp);
	if (type[0] != 'd' || type[1] != 'a' || type[2] != 't' || type[3] != 'a')
		return endWithError("Missing Data");

	fread(&dataSize, sizeof(int), 1, fp);

	buf = new unsigned char[dataSize];
	fread(buf, sizeof(unsigned char), dataSize, fp);



	frequency = sampleRate;
	format = 0;
	if (bitsPerSample == 8)
	{
		if (channels == 1)
		{
			format = AL_FORMAT_MONO8;
			//std::cout << "Format : " << format <<"(AL_FORMAT_MONO8)"<< std::endl;
		}
		else if (channels == 2)
		{
			format = AL_FORMAT_STEREO8;
			//std::cout << "Format : " << format << "(AL_FORMAT_STEREO8)" << std::endl;
		}
	}
	else if (bitsPerSample == 16)
	{
		if (channels == 1)
		{
			format = AL_FORMAT_MONO16;
			//std::cout << "Format : " << format << "(AL_FORMAT_MONO16)" << std::endl;
		}
		else if (channels == 2)
		{
			format = AL_FORMAT_STEREO16;
			//std::cout << "Format : " << format << "(AL_FORMAT_STEREO16)" << std::endl;
		}
	}

	tempformat = format;
	if (format == AL_FORMAT_STEREO16)
		format = AL_FORMAT_MONO16;
	if (format == AL_FORMAT_STEREO8)
		format = AL_FORMAT_MONO8;

	alBufferData(buffers[numberOfSources], format, buf, dataSize, frequency);
	
	fclose(fp);
	return 1;

}

bool SoundFileHandler::sourceIsPlaying(std::string name)
{
	try {
		int i = sourceMap.at(name);
		ALenum state;
		alGetSourcei(sources[i], AL_SOURCE_STATE, &state);
		return (state == AL_PLAYING);
	}
	catch (const std::out_of_range& oor) {
		std::cerr << "Out of Range error: " << oor.what() << "  Check spelling of Source." << '\n';
		return false;
	}
}
