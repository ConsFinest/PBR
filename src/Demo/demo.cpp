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
	int choice;
	bool choosing = false;
	bool textured;

	std::cout << "Press 1 = textured, Press 2 = untextured :";
	std::cin >> choice;

	while (!choosing)
	{
		switch (choice)
		{
		case 1:
			textured = true;
			choosing = true;
			break;
		case 2:
			textured = false;
			choosing = true;
			break;
		default:
			break;
		}
	}



	std::shared_ptr<Core> core = Core::init();
	Camera camera;

	Shape sphere(true, false);
	Shape sphereNT(true, false);
	Shape sphereSF(true, false);
	Shape cube(false, false);

	Shape quad(false, true);

	RenderTexture cap;

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	
	Shader shader("../shaders/simpleShader.vs", "../shaders/simpleShader.fs");

	Shader PBR("../shaders/PBRshader.vs", "../shaders/PBRshader.fs");
	Shader PBRNT("../shaders/PBRNTshader.vs", "../shaders/PBRNTshader.fs"); //PBR shader with no textures
	Shader convert("../shaders/convertShader.vs", "../shaders/convertShader.fs");
	Shader sky("../shaders/skyShader.vs", "../shaders/skyShader.fs");
	Shader ir("../shaders/irShader.vs", "../shaders/irShader.fs");
	Shader preFilter("../shaders/prefilterShader.vs", "../shaders/prefilterShader.fs");
	Shader brdf("../shaders/brdf.vs", "../shaders/brdf.fs");

	sky.use();
	sky.setInt("environmentMap", 0);

	Texture skyBox("../samples/textures/Ref.hdr", true);

	//metalBall
	Texture albedo("../samples/textures/basecolor.png", false);
	Texture normal("../samples/textures/normal.png", false);
	Texture metallic("../samples/textures/metallic.png", false);
	Texture roughness("../samples/textures/roughness.png", false);
	Texture ao("../samples/textures/grey.png", false);

	//sci-fiBall
	Texture Salbedo("../samples/textures/scialbedo.png", false);
	Texture Snormal("../samples/textures/scinormal.png", false);
	Texture Smetallic("../samples/textures/scimetallic.png", false);
	Texture Sroughness("../samples/textures/sciroughness.png", false);
	Texture Sao("../samples/textures/sciao.png", false);

	

	Texture envCubeMap({1024,1024}, 0);
	Texture irCubeMap({ 32,32 }, 0);
	Texture preFilterCubeMap({ 128,128 }, 1);
	Texture brdfLUTTexture({512,512});

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

	glBindFramebuffer(GL_FRAMEBUFFER, cap.GetFbo({ 1024,1024 }));
	cap.setViewPort({ 1024,1024 });

	cap.setTexture(envCubeMap.getID());

	for (unsigned int i = 0; i < 6; ++i)
	{
		convert.setMat4("view", captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cap.getTexture(), 0); 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cube.render();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	ir.use();
	ir.setInt("environmentMap", 0);
	ir.setMat4("projection", captureProjection);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubeMap.getID());

	glBindFramebuffer(GL_FRAMEBUFFER, cap.GetFbo({ 32,32 }));
	cap.setTexture(irCubeMap.getID());
	cap.setViewPort({ 32,32 });
	
	for (unsigned int i = 0; i < 6; ++i)
	{
		ir.setMat4("view", captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cap.getTexture(), 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cube.render();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	preFilter.use();
	preFilter.setInt("environmentMap", 0);
	preFilter.setMat4("projection", captureProjection);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubeMap.getID());
	cap.setTexture(preFilterCubeMap.getID());

	unsigned int maxMipLevels = 5;
	for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
	{
		unsigned int mipWidth = 128 * std::pow(0.5, mip);
		unsigned int mipHeight = 128 * std::pow(0.5, mip);
		glBindFramebuffer(GL_TEXTURE_CUBE_MAP, cap.GetFbo({ mipWidth,mipHeight }));
	
		cap.setViewPort({ mipWidth,mipHeight });
	
		float roughness = (float)mip / (float)(maxMipLevels - 1);
		preFilter.setFloat("roughness", roughness);
		for (unsigned int i = 0; i < 6; ++i)
		{
			preFilter.setMat4("view", captureViews[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cap.getTexture(), mip);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			cube.render();
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	glBindFramebuffer(GL_FRAMEBUFFER, cap.GetFbo({ 512,512 }));
	cap.setTexture(brdfLUTTexture.getID());
	cap.setViewPort({ 512,512 });
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, cap.getTexture(), 0);
	brdf.use();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	quad.render();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	cap.resetViewPort();

	PBR.use();

	PBR.setInt("albedoMap", 0);
	PBR.setInt("normalMap", 1);
	PBR.setInt("metallicMap", 2);
	PBR.setInt("roughnessMap", 3);
	PBR.setInt("aoMap", 4);
	PBR.setInt("irradianceMap", 5);
	PBR.setInt("prefilterMap", 6);
	PBR.setInt("brdfMap", 7);

	PBRNT.use();

	PBRNT.setInt("irradianceMap", 0);
	PBRNT.setInt("prefilterMap", 1);
	PBRNT.setInt("brdfMap", 2);
	PBRNT.setVec3("albedo", { 0.5f, 0.0f,0.0f });
	PBRNT.setFloat("ao", 1.0f);
	
	sphere.addTexture(albedo);
	sphere.addTexture(normal);
	if (textured)
	{
		sphere.addTexture(metallic);
		sphere.addTexture(roughness);
	}
	sphere.addTexture(ao);
	sphere.addTexture(irCubeMap);
	sphere.addTexture(preFilterCubeMap);
	sphere.addTexture(brdfLUTTexture);

	sphereSF.addTexture(Salbedo);
	sphereSF.addTexture(Snormal);
	if (textured)
	{
		sphereSF.addTexture(Smetallic);
		sphereSF.addTexture(Sroughness);
	}
	sphereSF.addTexture(Sao);
	sphereSF.addTexture(irCubeMap);
	sphereSF.addTexture(preFilterCubeMap);
	sphereSF.addTexture(brdfLUTTexture);

	sphereNT.addTexture(irCubeMap);
	sphereNT.addTexture(preFilterCubeMap);
	sphereNT.addTexture(brdfLUTTexture);
	
	cube.addTexture(envCubeMap);

	glm::vec3 lightPositions[] = {
	glm::vec3(-10.0f, 10.0f, 10.0f),
	glm::vec3(10.0f,  10.0f, 10.0f),
	glm::vec3(-10.0f, -10.0f, 10.0f),
	glm::vec3(10.0f, -10.0f, 10.0f),
	};
	glm::vec3 lightColors[] = {
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f)
	};
	int nrRows = 7;
	int nrColumns = 7;
	float spacing = 2.5;

	bool playing = true;
	bool cursorLock = true;
	Uint64 NOW = SDL_GetPerformanceCounter();
	Uint64 LAST = 0;
	double deltaTime = 0;
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	while (playing)
	{
		glm::mat4 proj = camera.GetProjMat();
		glm::mat4 view = camera.GetViewMat();
		glm::vec3 pos = camera.GetPos();
		LAST = NOW;
		NOW = SDL_GetPerformanceCounter();
		deltaTime = (double)((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency());
		
		//std::cout << deltaTime.delta << std::endl;
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//-----------------------TEXTURED--------------------------//
		if (textured)
		{
			

			glm::mat4 model = glm::mat4(1.0f);
			PBR.use();
			PBR.setVec3("camPos", pos);
			PBR.setMat4("view", glm::inverse(view));
			for (int row = 0; row < nrRows; ++row)
			{
				for (int col = 0; col < nrColumns; ++col)
				{
					model = glm::mat4(1.0f);
					model = glm::translate(model, glm::vec3(
						(float)(col - (nrColumns / 2)) * spacing,
						(float)(row - (nrRows / 2)) * spacing,
						-2.0f
					));

					PBR.setMat4("projection", proj);
					PBR.setMat4("model", model);

					sphere.bindTexture();
					sphere.render();
				}
			}
			for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
			{
				
				PBR.setVec3("lightPositions[" + std::to_string(i) + "]", lightPositions[i]);
				PBR.setVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);

				model = glm::mat4(1.0f);
				model = glm::translate(model, lightPositions[i]);
				model = glm::scale(model, glm::vec3(0.5f));
				PBR.setMat4("model", model);
				
				
				sphere.render(); //SHOWS LIGHT POSITIONS 
			}
		}
		if (!textured)
		{
			

			PBRNT.use();
			PBRNT.setVec3("camPos", pos);
			PBRNT.setMat4("view", glm::inverse(view));
			

			glm::mat4 model = glm::mat4(1.0f);
			for (int row = 0; row < nrRows; ++row)
			{
				PBRNT.setFloat("metallic", (float)row / (float)nrRows);
				for (int col = 0; col < nrColumns; ++col)
				{
					PBRNT.setFloat("roughness", glm::clamp((float)col / (float)nrColumns, 0.05f, 1.0f));

					model = glm::mat4(1.0f);
					model = glm::translate(model, glm::vec3(
						(float)(col - (nrColumns / 2)) * spacing,
						(float)(row - (nrRows / 2)) * spacing,
						-2.0f
					));
					PBRNT.setMat4("projection", proj);
					PBRNT.setMat4("model", model);

					sphereNT.bindTexture();
					sphereNT.render();
				}
			}


			for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
			{
				PBRNT.setVec3("lightPositions[" + std::to_string(i) + "]", lightPositions[i]);
				PBRNT.setVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);

				model = glm::mat4(1.0f);
				model = glm::translate(model, lightPositions[i]);
				model = glm::scale(model, glm::vec3(0.5f));
				PBRNT.setMat4("model", model);
				sphereNT.render(); //SHOWS LIGHT POSITIONS 
			}
		}

		glDepthFunc(GL_LEQUAL);
		sky.use();
		sky.setMat4("view", (view));
		sky.setMat4("projection", proj);
		cube.bindTexture();
		cube.render();
		
		camera.movement(deltaTime, core->getWindow());
		SDL_GL_SwapWindow(core->getWindow());
		
	}
	
	
	//glDeleteBuffers(1, &EBO);
	

}


//TO DO - no texture pbr
