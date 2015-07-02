#include "Renderer.h"
#include "GeKo_Graphics/GUI/GUI.h"

void OpenGL3Context::bindContext() const{
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glewExperimental = GL_TRUE;
}

Renderer::Renderer(const RenderContext &context) :

m_currentFBOIndex(false),
m_gBuffer(NULL), m_shaderGBuffer(NULL),
m_ping(NULL), m_pong(NULL),
m_firstRender(true),
m_shaderSFQ(NULL),

m_useDeferredShading(false),  m_shaderDSLighting(NULL), m_shaderDSCompositing(NULL), m_dsLightRootNode(NULL), m_dsLightColor(NULL),
m_useReflections(false),      m_shaderRLR(NULL),
m_useAntiAliasing(false),     m_shaderFXAA(NULL),
m_useBloom(false),            m_shaderBloom(NULL),
m_useBlur(false),			  m_shaderBlur(NULL),
m_useRadialBlur(false),		  m_shaderRadialBlur(NULL),
m_useDoF(false),			  m_shaderDoF(NULL), m_shaderDepth(NULL),
m_useSSAO(false),             m_shaderSSAOcalc(NULL), m_shaderSSAOblur(NULL), m_shaderSSAOfinal(NULL),
m_useShadowMapping(false),    m_shaderShadowMapping(NULL), m_smCam(NULL),

m_currentViewMatrix(glm::mat4()), m_currentProjectionMatrix(glm::mat4()),
m_windowWidth(0), m_windowHeight(0)

{
  m_sfq.loadBufferData();
  context.bindContext();
  printInfo();
}

Renderer::~Renderer()
{
  delete m_ping;
  delete m_pong;
  delete m_gBuffer;
  delete m_shaderGBuffer;
  delete m_shaderSFQ;

  if(m_useReflections)       delete m_shaderRLR;
  if(m_useAntiAliasing)      delete m_shaderFXAA;
  if(m_useDeferredShading)   delete m_shaderDSLighting;
  if(m_useDeferredShading)   delete m_shaderDSCompositing;
  if(m_useBloom)             delete m_shaderBloom;
  if(m_useBlur)				 delete m_shaderBlur;
  if(m_useRadialBlur)		 delete m_shaderRadialBlur;
  if(m_useDoF)				 delete m_shaderDoF;
  if(m_useDoF)				 delete m_shaderDepth;
  if(m_useSSAO)              delete m_shaderSSAOcalc;
  if(m_useSSAO)              delete m_shaderSSAOblur;
  if(m_useSSAO)              delete m_shaderSSAOfinal;
  if(m_useShadowMapping)     delete m_shaderShadowMapping;

}

void Renderer::printInfo(){

  std::cout << "=============================================" << std::endl;
  // print out some info about the graphics drivers
  std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
  std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
  std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
  std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
  std::cout << "=============================================" << std::endl;
}

void Renderer::init(int windowWidth, int windowHeight)
{
  m_windowWidth  = windowWidth;
  m_windowHeight = windowHeight;

  //create ping pong fbos
  m_ping    = new FBO(windowWidth, windowHeight, 3, true, false);
  m_pong    = new FBO(windowWidth, windowHeight, 3, true, false);
  m_gBuffer = new FBO(windowWidth, windowHeight, 3, true, false);
  m_smFBO   = new FBO(windowWidth, windowHeight, 1, true, false);

  //load Shader

  if (!m_shaderGBuffer)
  {
    VertexShader vsGBuffer(loadShaderSource(SHADERS_PATH + std::string("/GBuffer/GBuffer.vert")));
    FragmentShader fsGBuffer(loadShaderSource(SHADERS_PATH + std::string("/GBuffer/GBuffer.frag")));
    m_shaderGBuffer = new ShaderProgram(vsGBuffer, fsGBuffer);
  }

  if (m_useDeferredShading && !m_shaderDSLighting && !m_shaderDSCompositing)
  {
    VertexShader vsDsLighting(loadShaderSource(SHADERS_PATH + std::string("/DeferredShading/dsLighting.vert")));
    FragmentShader fsDsLighting(loadShaderSource(SHADERS_PATH + std::string("/DeferredShading/dsLighting.frag")));
    m_shaderDSLighting = new ShaderProgram(vsDsLighting, fsDsLighting);

    VertexShader vsDsCompositing(loadShaderSource(SHADERS_PATH + std::string("/DeferredShading/dsFinalCompositing.vert")));
    FragmentShader fsDsCompositing(loadShaderSource(SHADERS_PATH + std::string("/DeferredShading/dsFinalCompositing.frag")));
    m_shaderDSCompositing = new ShaderProgram(vsDsCompositing, fsDsCompositing);
  }

  if (m_useReflections && !m_shaderRLR)
  {
    VertexShader vsRLR(loadShaderSource(SHADERS_PATH + std::string("/RealtimeLocalReflections/RealtimeLocalReflections.vert")));
    FragmentShader fsRLR(loadShaderSource(SHADERS_PATH + std::string("/RealtimeLocalReflections/RealtimeLocalReflections.frag")));
    m_shaderRLR = new ShaderProgram(vsRLR, fsRLR);
  }

  if (!m_shaderSFQ)
  {
    VertexShader vsSfq(loadShaderSource(SHADERS_PATH + std::string("/ScreenFillingQuad/ScreenFillingQuad.vert")));
    FragmentShader fsSfq(loadShaderSource(SHADERS_PATH + std::string("/ScreenFillingQuad/ScreenFillingQuad.frag")));
    m_shaderSFQ = new ShaderProgram(vsSfq, fsSfq);
  }

  if (m_useBloom && !m_shaderBloom)
  {
    VertexShader vsBloom(loadShaderSource(SHADERS_PATH + std::string("/ScreenFillingQuad/ScreenFillingQuad.vert")));
    FragmentShader fsBloom(loadShaderSource(SHADERS_PATH + std::string("/Bloom/Bloom.frag")));
    m_shaderBloom = new ShaderProgram(vsBloom, fsBloom);
  }

  if (m_useBlur && !m_shaderBlur)
  {
	  VertexShader vsBlur(loadShaderSource(SHADERS_PATH + std::string("/ScreenFillingQuad/ScreenFillingQuad.vert")));
	  FragmentShader fsBlur(loadShaderSource(SHADERS_PATH + std::string("/Blur/blur1D.frag")));
	  m_shaderBlur = new ShaderProgram(vsBlur, fsBlur);
  }

  if (m_useRadialBlur && !m_shaderRadialBlur)
  {
	  VertexShader vsBlur(loadShaderSource(SHADERS_PATH + std::string("/ScreenFillingQuad/ScreenFillingQuad.vert")));
	  FragmentShader fsBlur(loadShaderSource(SHADERS_PATH + std::string("/RadialBlur/RadialBlur.frag")));
	  m_shaderRadialBlur = new ShaderProgram(vsBlur, fsBlur);
  }

  if (m_useDoF && !m_shaderDoF)
  {
	  VertexShader vsDoF(loadShaderSource(SHADERS_PATH + std::string("/ScreenFillingQuad/ScreenFillingQuad.vert")));
	  FragmentShader fsDoF(loadShaderSource(SHADERS_PATH + std::string("/DoF/DoF.frag")));
	  m_shaderDoF = new ShaderProgram(vsDoF, fsDoF);

	  VertexShader vsDepth(loadShaderSource(SHADERS_PATH + std::string("/ShadowMapping/ShadowMap.vert")));
	  FragmentShader fsDepth(loadShaderSource(SHADERS_PATH + std::string("/ShadowMapping/ShadowMap.frag")));
	  m_shaderDepth = new ShaderProgram(vsDepth, fsDepth);
  }

  if (m_useAntiAliasing && !m_shaderFXAA)
  {
    VertexShader vsFXAA(loadShaderSource(SHADERS_PATH + std::string("/FXAA/FXAA.vert")));
    FragmentShader fsFXAA(loadShaderSource(SHADERS_PATH + std::string("/FXAA/FXAA.frag")));
    m_shaderFXAA = new ShaderProgram(vsFXAA, fsFXAA);
  }

  if (m_useSSAO && !m_shaderSSAOcalc && !m_shaderSSAOblur && !m_shaderSSAOfinal)
  {
    VertexShader vsSSAO(loadShaderSource(SHADERS_PATH + std::string("/SSAO/ssao.vert")));
    FragmentShader fsSSAO(loadShaderSource(SHADERS_PATH + std::string("/SSAO/ssao.frag")));
    m_shaderSSAOcalc = new ShaderProgram(vsSSAO, fsSSAO);

    VertexShader vsSfq(loadShaderSource(SHADERS_PATH + std::string("/ScreenFillingQuad/ScreenFillingQuad.vert")));
    FragmentShader fsBLUR(loadShaderSource(SHADERS_PATH + std::string("/SSAO/ssaoBlur.frag")));
    m_shaderSSAOblur = new ShaderProgram(vsSfq, fsBLUR);

    FragmentShader fsFinal(loadShaderSource(SHADERS_PATH + std::string("/SSAO/ssaoFinal.frag")));
    m_shaderSSAOfinal = new ShaderProgram(vsSfq, fsFinal);
  }

  if (m_useShadowMapping && !m_shaderShadowMapping)
  {
    VertexShader vsSM(loadShaderSource(SHADERS_PATH + std::string("/ShadowMapping/ShadowMap.vert")));
    FragmentShader fsSM(loadShaderSource(SHADERS_PATH + std::string("/ShadowMapping/ShadowMap.frag")));
    m_shaderShadowMapping = new ShaderProgram(vsSM, fsSM);

    if(!m_smCam)
      m_smCam = new Pilotview("smCam");

    m_smCam->setNearFar(1.0f,30.0f);
  }
 }

void Renderer::renderScene(Scene& scene, Window& window)
{
  if (!m_ping || !m_pong)
    init(window.getWidth(), window.getHeight());
  
  glClearColor(0.0, 0.0, 0.0, 1);
  
  if (m_useShadowMapping)
    renderShadowMapping(scene);

  m_firstRender = true;
  
  m_currentFBOIndex = 0;
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  m_gBuffer->bind();

  m_shaderGBuffer->bind();

  m_currentViewMatrix = scene.getScenegraph()->getActiveCamera()->getViewMatrix();
  m_currentProjectionMatrix = scene.getScenegraph()->getActiveCamera()->getProjectionMatrix();

  m_shaderGBuffer->sendMat4("viewMatrix", m_currentViewMatrix);
  m_shaderGBuffer->sendMat4("projectionMatrix", m_currentProjectionMatrix);


  //==================


  m_shaderGBuffer->sendInt("useShadowMap", m_useShadowMapping);

  if (m_useShadowMapping)
  {

	m_shaderGBuffer->sendInt("shadowMode", *m_pcf);

    m_shaderGBuffer->sendVec4("light.pos", m_smConeLight->m_position);
    m_shaderGBuffer->sendVec3("light.col", glm::vec3(m_smConeLight->m_color));

    m_shaderGBuffer->sendVec3("light.spot_direction", m_smConeLight->m_direction);
    m_shaderGBuffer->sendFloat("light.spot_exponent", m_smConeLight->m_exponent);
    m_shaderGBuffer->sendFloat("light.spot_cutoff", m_smConeLight->m_radius);

    m_shaderGBuffer->sendVec3("lightAmbient", glm::fvec3(0.3f,0.3f,0.3f));

    //Shadow mapping
    glm::mat4 lightPerspective, lightView, lightMVPBias;
    lightPerspective = m_smCam->getProjectionMatrix();
    lightView = m_smCam->getViewMatrix();

    glm::mat4 sm_lightViewport(
      0.5, 0.0, 0.0, 0.0,
      0.0, 0.5, 0.0, 0.0,
      0.0, 0.0, 0.5, 0.0,
      0.5, 0.5, 0.5, 1.0
      );

    //Build "shadow matrix"
    lightMVPBias = sm_lightViewport * lightPerspective * lightView;
    m_shaderGBuffer->sendMat4("lightVPBias", lightMVPBias);

    m_shaderGBuffer->sendVec3("mat.diffuse", darkgrey);
    m_shaderGBuffer->sendVec3("mat.specular", grey);
    m_shaderGBuffer->sendFloat("mat.shininess", 100.0f);
    m_shaderGBuffer->sendFloat("mat.alpha", 1.0f);

    //Bind and Pass shadow map. Only use SHADOW_TEXTURE_UNIT when Normal Mapping is applied.
    m_shaderGBuffer->sendSampler2D("depthTexture", m_smFBO->getDepthTexture(), 3);
  }
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  if (scene.hasSkybox())
  {
	  glDisable(GL_DEPTH_TEST);
	  m_shaderGBuffer->sendSkyboxTexture("cubeMap", reinterpret_cast<Skybox*>(scene.getSkyboxNode()->getGeometry())->getSkyboxTexture(),4);
	  m_shaderGBuffer->sendInt("renderSkybox", 1);
	  scene.getSkyboxNode()->render(*m_shaderGBuffer);
	  glEnable(GL_DEPTH_TEST);
  }

  m_shaderGBuffer->sendInt("renderSkybox", 0);
  
  scene.render(*m_shaderGBuffer);
  m_shaderGBuffer->unbind();
  
  //renderParticleSystems
  scene.renderParticleSystems();
  m_gBuffer->unbind();


  if (m_useSSAO)
    renderSSAO();

  if (m_useDeferredShading)
    renderDeferredShading();

  if (m_useReflections)
    renderReflections(scene.getScenegraph()->getActiveCamera()->getNear(), scene.getScenegraph()->getActiveCamera()->getFar());

  if (m_useBloom)
    renderBloom();

  if (m_useBlur)
	  renderBlur();

  if (m_useRadialBlur)
	  renderRadialBlur();

  if (m_useDoF)
	  renderDoF(scene);

  if (m_useAntiAliasing)
    renderAntiAliasing();


  //Render SFQ
  m_shaderSFQ->bind();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  m_shaderSFQ->sendSampler2D("texture", getLastFBO()->getColorTexture(2));
  m_sfq.renderGeometry();
  m_shaderSFQ->unbind();

  if (m_guis.size() > 0)
  {
    for (std::vector<GUI*>::iterator it = m_guis.begin(); it != m_guis.end(); ++it)
    {
      (*it)->render(window);
    }
  }
}

void Renderer::renderGUI(GUI &guiToRender, Window &window)
{
  guiToRender.render(window);
}

void Renderer::addGui(GUI *guiToAdd)
{
  m_guis.push_back(guiToAdd);
}

void Renderer::useReflections(bool useReflections, float *reflectionStrength)
{
  m_useReflections = useReflections;

  if (m_screenSpaceReflectionsStrength != reflectionStrength)
    m_screenSpaceReflectionsStrength = reflectionStrength;
}

void Renderer::useAntiAliasing(bool useAntiAliasing)
{
  m_useAntiAliasing = useAntiAliasing;
}

void Renderer::useDeferredShading(bool useDeferredShading, Node *lightRootNode, glm::fvec3 *lightColor)
{
  m_useDeferredShading = useDeferredShading;
  
  if (!lightRootNode)
    return;

  if (lightRootNode != m_dsLightRootNode)
    m_dsLightRootNode = lightRootNode;
  
  if (lightColor != m_dsLightColor)
    m_dsLightColor = lightColor;
}

void Renderer::useBloom(bool useBloom, float *bloomStrength)
{
  m_useBloom = useBloom;

  if (bloomStrength != m_bloomStrength)
	  m_bloomStrength = bloomStrength;
}

void Renderer::useBlur(bool useBlur, float *blurStrength)
{
	m_useBlur = useBlur;

	if (m_blurStrength != blurStrength)
		m_blurStrength = blurStrength;
}

void Renderer::useRadialBlur(bool useRadialBlur, float *radialBlurStrength)
{
	m_useRadialBlur = useRadialBlur;

	if (m_radialBlurStrength != radialBlurStrength)
		m_radialBlurStrength = radialBlurStrength;
}

void Renderer::useDoF(bool useDoF, float *focusDepth)
{
	m_useDoF = useDoF;

	if (m_focusDepth != focusDepth)
		m_focusDepth = focusDepth;
}

void Renderer::useSSAO(bool useSSAO, float *quality, float *radius)
{
  m_useSSAO = useSSAO;

  if (m_ssaoQuality != quality)
    m_ssaoQuality = quality;

  if (m_ssaoRadius != radius)
    m_ssaoRadius = radius;
}

void Renderer::useShadowMapping(bool useShadowMapping, int *pcf, ConeLight *coneLight)
{
  m_useShadowMapping = useShadowMapping;

  if (!coneLight)
    return;

  if (!m_smCam)
    m_smCam = new Pilotview("smCam");
  m_smCam->setNearFar(m_smCam->getNear(), 1000.0f);

  if (m_pcf != pcf)
	  m_pcf = pcf;

  m_smConeLight = coneLight;
  m_smCam->setFOV(m_smConeLight->m_angle);

  //Set the position of the camera inside the light source
  glm::vec4 lightPosition = m_smConeLight->m_position;
  m_smCam->setPosition(lightPosition);

  //Set the lookAt point of the camera along the direction of the spot light
  glm::vec3 lookAt = glm::vec3(lightPosition) + (m_smConeLight->m_direction);
  m_smCam->setLookAt(lookAt);
}


void Renderer::bindFBO()
{
  if (!m_currentFBOIndex)
  {
    m_ping->bind();
  }

  else if (m_currentFBOIndex)
  {
    m_pong->bind();
  }
}

void Renderer::unbindFBO()
{
  if (!m_currentFBOIndex)
  {
    m_ping->unbind();
    m_currentFBOIndex = 1;
  }

  else if (m_currentFBOIndex)
  {
    m_pong->unbind();
    m_currentFBOIndex = 0;
  }
}

FBO* Renderer::getLastFBO()
{
  if (m_firstRender)
  {
    m_firstRender = false;
    return m_gBuffer;
  }
  if (!m_currentFBOIndex)
  {
    return m_pong;
  }

  else
  {
    return m_ping;
  }
}

void Renderer::renderReflections(float camNear, float camFar)
{
  if (!m_shaderRLR)
    init(m_windowWidth, m_windowHeight);

  bindFBO();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  m_shaderRLR->bind();


  m_shaderRLR->sendSampler2D("positionTexture", m_gBuffer->getColorTexture(0), 0);
  m_shaderRLR->sendSampler2D("normalTexture", m_gBuffer->getColorTexture(1), 1);
  m_shaderRLR->sendSampler2D("colorTexture", getLastFBO()->getColorTexture(2), 2);
  m_shaderRLR->sendSampler2D("depthBuffer", m_gBuffer->getDepthTexture(), 3);

  m_shaderRLR->sendMat4("projectionMatrix", m_currentProjectionMatrix);

  m_shaderRLR->sendInt("screenWidth", m_windowWidth);
  m_shaderRLR->sendInt("screenHeight", m_windowHeight);

  m_shaderRLR->sendFloat("zNear", camNear);
  m_shaderRLR->sendFloat("zFar", camFar);

  m_shaderRLR->sendFloat("reflectivity", *m_screenSpaceReflectionsStrength);

  m_sfq.renderGeometry();

  m_shaderRLR->unbind();
  unbindFBO();
}

void Renderer::renderAntiAliasing()
{
  if (!m_shaderFXAA)
    init(m_windowWidth, m_windowHeight);

  bindFBO();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  m_shaderFXAA->bind();

  m_shaderFXAA->sendSampler2D("colorTexture", getLastFBO()->getColorTexture(2), 2);
  m_shaderFXAA->sendInt("fboWidth", m_windowWidth);
  m_shaderFXAA->sendInt("fboHeight", m_windowHeight);

  m_sfq.renderGeometry();

  m_shaderFXAA->unbind();
  unbindFBO();
}

void Renderer::renderDeferredShading()
{
  if (!m_shaderDSLighting || !m_shaderDSCompositing)
    init(m_windowWidth, m_windowHeight);

  if (!m_dsLightColor || !m_dsLightRootNode)
    throw std::string("Error in renderDeferredShading - Light Color or Light Root Node was not set");

  FBO *a = new FBO(m_windowWidth,m_windowHeight,3,false,false);

  a->bind();

  glCullFace(GL_FRONT);
  glEnable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ONE);
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  m_shaderDSLighting->bind();

  m_shaderDSLighting->sendMat4("viewMatrix", m_currentViewMatrix);
  m_shaderDSLighting->sendMat4("projectionMatrix", m_currentProjectionMatrix);

  m_shaderDSLighting->sendSampler2D("positionMap", m_gBuffer->getColorTexture(0), 0);
  m_shaderDSLighting->sendSampler2D("normalMap", m_gBuffer->getColorTexture(1), 1);

  m_shaderDSLighting->sendInt("windowWidth", m_windowWidth);
  m_shaderDSLighting->sendInt("windowHeight", m_windowHeight);

  m_shaderDSLighting->sendVec3("lightColor", *m_dsLightColor);

  m_dsLightRootNode->render(*m_shaderDSLighting);

  glDisable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);
  glClearColor(1.0, 1.0, 1.0, 0.0);
  m_shaderDSLighting->unbind();
  a->unbind();

  //COMPOSITING TEIL ===============================
  bindFBO();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  m_shaderDSCompositing->bind();

  m_shaderDSCompositing->sendSampler2D("colorMap", getLastFBO()->getColorTexture(2), 0);
  m_shaderDSCompositing->sendSampler2D("lightMap", a->getColorTexture(2), 1);

  m_sfq.renderGeometry();

  m_shaderDSCompositing->unbind();
  unbindFBO();

  delete a;
}

void Renderer::renderBloom()
{
  if (!m_shaderBloom)
    init(m_windowWidth, m_windowHeight);

  bindFBO();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  m_shaderBloom->bind();

  m_shaderBloom->sendSampler2D("bgl_RenderedTexture", getLastFBO()->getColorTexture(2),2);
  m_shaderBloom->sendFloat("bloomStrength", *m_bloomStrength);
  m_sfq.renderGeometry();

  m_shaderBloom->unbind();

 unbindFBO();
}

void Renderer::renderBlur()
{
	if (!m_shaderBlur)
		init(m_windowWidth, m_windowHeight);

	bindFBO();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_shaderBlur->bind();
	
	m_shaderBlur->sendSampler2D("image", getLastFBO()->getColorTexture(2), 2);
	m_shaderBlur->sendFloat("blurstrength", *m_blurStrength);
	m_sfq.renderGeometry();

	m_shaderBlur->unbind();
	unbindFBO();
}

void Renderer::renderRadialBlur()
{
	if (!m_shaderRadialBlur)
		init(m_windowWidth, m_windowHeight);

	bindFBO();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_shaderRadialBlur->bind();

	m_shaderRadialBlur->sendSampler2D("tex", getLastFBO()->getColorTexture(2), 2);
	m_shaderRadialBlur->sendFloat("blurstrength", *m_radialBlurStrength);
	m_sfq.renderGeometry();

	m_shaderRadialBlur->unbind();

	unbindFBO();
}

void Renderer::renderDoF(Scene& scene)
{
	if (!m_shaderDepth || !m_shaderDoF)
		init(m_windowWidth, m_windowHeight);

	m_smFBO->bind();

	//Clear Shadow Map
	glClear(GL_DEPTH_BUFFER_BIT);

	//Set the shader for the light pass. This shader is highly optimized because the scene depth is the only thing that matters here!
	m_shaderDepth->bind();
	m_shaderDepth->sendMat4("viewMatrix", m_currentViewMatrix);
	m_shaderDepth->sendMat4("projectionMatrix", m_currentProjectionMatrix);

	//Render the scene
	scene.render(*m_shaderDepth);

	//Restore the default framebuffer
	m_shaderDepth->unbind();
	m_smFBO->unbind();

	bindFBO();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_shaderDoF->bind();

	m_shaderDoF->sendSampler2D("tex", getLastFBO()->getColorTexture(2), 2);
	m_shaderDoF->sendSampler2D("depth", m_smFBO->getDepthTexture(), 1);

	m_shaderDoF->sendFloat("focus_depth", *m_focusDepth);

	m_sfq.renderGeometry();

	m_shaderDoF->unbind();

	unbindFBO();
}

void Renderer::renderSSAO()
{
  if (!m_shaderSSAOcalc || !m_shaderSSAOblur || !m_shaderSSAOfinal)
    init(m_windowWidth, m_windowHeight);

  FBO *a = new FBO(m_windowWidth, m_windowHeight, 3, false, false);
  FBO *b = new FBO(m_windowWidth, m_windowHeight, 3, false, false);

  a->bind();
  glClearColor(1, 1, 1, 1);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  m_shaderSSAOcalc->bind();

  m_shaderSSAOcalc->sendSampler2D("positionMap", m_gBuffer->getColorTexture(0), 0);
  m_shaderSSAOcalc->sendSampler2D("normalMap", m_gBuffer->getColorTexture(1), 1);
  m_shaderSSAOcalc->sendMat4("sceneProjectionMatrix", m_currentProjectionMatrix);
  m_shaderSSAOcalc->sendFloat("radius", *m_ssaoRadius);
  m_shaderSSAOcalc->sendFloat("quality", *m_ssaoQuality);

  m_sfq.renderGeometry();

  m_shaderSSAOcalc->unbind();
  a->unbind();



  //Blur #1 SSAO calculation
  b->bind();

  //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  m_shaderSSAOblur->bind();
  m_shaderSSAOblur->sendSampler2D("colortexture", a->getColorTexture(2), 0);
  m_shaderSSAOblur->sendInt("secondPass", 0);
  m_sfq.renderGeometry();

  m_shaderSSAOblur->unbind();
  b->unbind();



  //Blur #2 SSAO calculation
  a->bind();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  m_shaderSSAOblur->bind();
  m_shaderSSAOblur->sendSampler2D("colortexture", b->getColorTexture(2), 0);
  m_shaderSSAOblur->sendFloat("secondPass", 1);
  m_sfq.renderGeometry();

  m_shaderSSAOblur->unbind();
  a->unbind();

  glClearColor(0, 0, 0, 0);


  //COLOR COMPOSITING
  bindFBO();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  m_shaderSSAOfinal->bind();
  m_shaderSSAOfinal->sendSampler2D("colorMap", getLastFBO()->getColorTexture(2), 0);
  m_shaderSSAOfinal->sendSampler2D("ssaoMap", a->getColorTexture(2), 1);
  m_sfq.renderGeometry();
  m_shaderSSAOfinal->unbind();
  unbindFBO();

  delete a;
  delete b;
}

void Renderer::renderShadowMapping(Scene& scene)
{
  if (!m_shaderShadowMapping || !m_smCam)
    init(m_windowWidth, m_windowHeight);

  if (!m_smConeLight)
    throw std::string("Error in renderShadowMapping - Cone Light was not set");

  if (!scene.getScenegraph()->containsCamera("smCam"))
  {
    scene.getScenegraph()->addCamera(m_smCam);
  }

  std::string mainCamName = scene.getScenegraph()->getActiveCamera()->getName();

  scene.getScenegraph()->setActiveCamera("smCam");

  //Bind Shadow Map FBO
  m_smFBO->bind();

  //Clear Shadow Map
  glClear(GL_DEPTH_BUFFER_BIT);

  //Set the shader for the light pass. This shader is highly optimized because the scene depth is the only thing that matters here!
  m_shaderShadowMapping->bind();
  m_shaderShadowMapping->sendMat4("viewMatrix", m_smCam->getViewMatrix());
  m_shaderShadowMapping->sendMat4("projectionMatrix", m_smCam->getProjectionMatrix());

  //Render the scene
  scene.render(*m_shaderShadowMapping);

  //Restore the default framebuffer
  m_shaderShadowMapping->unbind();
  m_smFBO->unbind();

  scene.getScenegraph()->setActiveCamera(mainCamName);
}