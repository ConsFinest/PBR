//F12 peeks definition

#define STB_IMAGE_IMPLEMENTATION
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 760


#include <PBR/core.h>
#include <PBR/resources.h>
#include <PBR/shader.h>
#include <PBR/texture.h>
#include <PBR/camera.h>
#include <PBR/3Dshapes.h>
#include <PBR/renderTexture.h>

int main()
{


	std::shared_ptr<Core> core = Core::init();
	Camera camera;

	Shape sphere(true);
	Shape cube(false);
	RenderTexture cap;
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	Shader shader("../shaders/simpleShader.vs", "../shaders/simpleShader.fs");
	Shader PBR("../shaders/PBRshader.vs", "../shaders/PBRshader.fs");
	Shader convert("../shaders/convertShader.vs", "../shaders/convertShader.fs");
	Shader sky("../shaders/skyShader.vs", "../shaders/skyShader.fs");
	Shader ir("../shaders/irShader.vs", "../shaders/irShader.fs");
	sky.use();
	sky.setInt("environmentMap", 0);

	Texture skyBox("../samples/textures/skybox1.hdr", true);


	unsigned int captureFBO;
	unsigned int captureRBO;
	glGenFramebuffers(1, &captureFBO);
	glGenRenderbuffers(1, &captureRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

	
	Texture envCubemap({512,512});
	Texture irCubeMap({ 32,32 });

	glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 captureViews[] =
	{
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	convert.use();
	convert.setInt("equirectangularMap", 0);
	convert.setMat4("projection", captureProjection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, skyBox.getID());

	glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	for (unsigned int i = 0; i < 6; ++i)
	{
		convert.setMat4("view", captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap.getID() , 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cube.render();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

	ir.use();
	ir.setInt("environmentMap", 0);
	ir.setMat4("projection", captureProjection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap.getID());
	glViewport(0, 0, 32, 32);
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	
	for (unsigned int i = 0; i < 6; ++i)
	{
		ir.setMat4("view", captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irCubeMap.getID(), 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cube.render();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	

	cap.resetViewPort();

	PBR.use();

	PBR.setInt("albedoMap", 0);
	PBR.setInt("normalMap", 1);
	PBR.setInt("metallicMap", 2);
	PBR.setInt("roughnessMap", 3);
	PBR.setInt("aoMap", 4);
	PBR.setInt("irradianceMap", 5);
	
	Texture albedo("../samples/textures/basecolor.png", false);
	Texture normal("../samples/textures/normal.png", false);
	Texture metallic("../samples/textures/metallic.png", false);
	Texture ao("../samples/textures/grey.png", false);
	
	
	sphere.addTexture(albedo);
	sphere.addTexture(normal);
	sphere.addTexture(metallic);
	sphere.addTexture(ao);
	sphere.addTexture(irCubeMap);
	

	glm::vec3 lightPositions[] = {
	    glm::vec3(0.0f, 0.0f, 10.0f),
	};
	glm::vec3 lightColors[] = {
		glm::vec3(150.0f, 150.0f, 150.0f),
	};
	int nrRows = 7;
	int nrColumns = 7;
	float spacing = 2.5;

	//sphere.bindTexture();

	bool playing = true;

	Uint64 NOW = SDL_GetPerformanceCounter();
	Uint64 LAST = 0;
	double deltaTime = 0;
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	while (playing)
	{
		

		PBR.use();
		sphere.bindTexture();
		LAST = NOW;
		NOW = SDL_GetPerformanceCounter();
		deltaTime = (double)((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency());
		
		//std::cout << deltaTime.delta << std::endl;
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.movement(deltaTime);
		glm::mat4 model = glm::mat4(1.0f);
		PBR.setVec3("camPos", camera.GetPos());
		PBR.setMat4("view", glm::inverse(camera.GetViewMat()));

		std::cout << camera.GetPos().x << " " << camera.GetPos().y << " " << camera.GetPos().x << std::endl;

		for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
		{
			glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(deltaTime * 5.0) * 5.0, 10.0, 0.0);
			newPos = lightPositions[i];
			PBR.setVec3("lightPositions[" + std::to_string(i) + "]", newPos);
			PBR.setVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);

			model = glm::mat4(1.0f);
			model = glm::translate(model, newPos);
			model = glm::scale(model, glm::vec3(0.5f));
			PBR.setMat4("model", model);
			//sphere.render(); makes BLACK sphere appear 
		}

		for (int row = 0; row < nrRows; ++row)
		{
			for (int col = 0; col < nrColumns; ++col)
			{
				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(
					(float)(col - (nrColumns / 2)) * spacing,
					(float)(row - (nrRows / 2)) * spacing,
					0.0f
				));
				
				PBR.setMat4("projection", camera.GetProjMat());
				PBR.setMat4("model", model);
				sphere.render();
			}
		}
		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	
		sky.use();
		sky.setMat4("view", camera.GetViewMat());
		sky.setMat4("projection", camera.GetProjMat());
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyBox.getID());
		cube.render();
		

		SDL_GL_SwapWindow(core->getWindow());
	}
	
	
	//glDeleteBuffers(1, &EBO);
	

}


//TO DO : FrameBuffers
//TO DO : CubeMaps
