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
	int choice = true;
	bool choosing1 = false;
	bool choosing2 = false;
	bool textured;

	int nrRows = 1;
	int nrColumns = 1;
	float spacing = 2.5;
	int textureChoice = 1;
	int backGroundChoice = 1;

	while (!choosing1)
	{
		
		std::cout << "Press 1 = textured, Press 2 = untextured :";
		std::cin >> choice;
		switch (choice)
		{
		case 1:
			textured = true;
			choosing1 = true;
			break;
		case 2:
			textured = false;
			choosing1 = true;
			break;
		default:
			system("CLS");
			break;
		}
	}
	choice = false;
	if (!textured)
	{
		system("CLS");
		std::cout << "Choose Dimensions of grid (x/y) between 1 - 10" << std::endl;
		int x = 0;
		int y = 0;
		while (!choosing2)
		{
			std::cout << "X :" << std::endl;
			std::cin >> x;
		    nrColumns = x;
			
			std::cout << "Y :" << std::endl;
			std::cin >> y;
			nrRows = y;
			break;
		}
	}
	if (textured)
	{
		system("CLS");
		std::cout << "Choose Texture For Ball: " << std::endl;
		std::cout << "1. Rusty Iron" << std::endl;
		std::cout << "2. SciFi" << std::endl;
		std::cout << "3. Gold" << std::endl;
		std::cout << "4. Braided Carpet" << std::endl;
		std::cout << "5. Plastic" << std::endl;
		std::cout << "6. Painted Cement" << std::endl;
		std::cout << "7. Limestone" << std::endl;
		std::cout << "8. Bark" << std::endl;
		std::cin >> textureChoice;
	}

	system("CLS");
	std::cout << "Choose Skybox: " << std::endl;
	std::cout << "1. Field" << std::endl;
	std::cout << "2. RoofTop" << std::endl;
	std::cout << "3. Containers" << std::endl;
	std::cout << "4. Studio" << std::endl;
	std::cout << "5. Seaside" << std::endl;
	std::cin >> backGroundChoice;

	std::shared_ptr<Core> core = Core::init();
	Camera camera;

	Shape sphereR(true, false);
	Shape sphereNT(true, false);
	
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

	std::string skybox;
	switch (backGroundChoice)
	{
	case 1:
		skybox = "../samples/textures/Ref.hdr";
		break;
	case 2:
		skybox = "../samples/textures/skybox1.hdr";
		break;
	case 3:
		skybox = "../samples/textures/containerRef.hdr";
		break;
	case 4:
		skybox = "../samples/textures/StudioRef.hdr";
		break;
	case 5:
		skybox = "../samples/textures/SeasideRef.hdr";
		break;
	}

	sky.use();
	sky.setInt("environmentMap", 0);

	Texture skyBox(skybox, true);

	std::string albedo;
	std::string normal;
	std::string metallic;
	std::string roughness;
	std::string ao;

	switch (textureChoice)
	{
	case 1: //rustyiron
		albedo = "../samples/textures/basecolor.png";
		normal = "../samples/textures/normal.png";
		metallic = "../samples/textures/metallic.png";
		roughness = "../samples/textures/roughness.png";
		ao = "../samples/textures/grey.png";

		break;
	case 2: //scifi
		albedo = "../samples/textures/scialbedo.png";
		normal = "../samples/textures/scinormal.png";
		metallic = "../samples/textures/scimetallic.png";
		roughness = "../samples/textures/sciroughness.png";
		ao = "../samples/textures/sciao.png";
		break;
	case 3: //gold
		albedo = "../samples/textures/Gbasecolor.png";
		normal = "../samples/textures/Gnormal.png";
		metallic = "../samples/textures/Gmetallic.png";
		roughness = "../samples/textures/Groughness.png";
		ao = "../samples/textures/Gao.png";
		break;
	case 4: //braided carpet
		albedo = "../samples/textures/BCalbedo.png";
		normal = "../samples/textures/BCnormal.png";
		metallic = "../samples/textures/BCmetallic.png";
		roughness = "../samples/textures/BCroughness.png";
		ao = "../samples/textures/BCao.png";
		break;
	case 5: //plastic
		albedo = "../samples/textures/Palbedo.png";
		normal = "../samples/textures/Pnormal.png";
		metallic = "../samples/textures/Pmetallic.png";
		roughness = "../samples/textures/Proughness.png";
		ao = "../samples/textures/Pao.png";
		break;
	case 6: //painted cement
		albedo = "../samples/textures/PCalbedo.png";
		normal = "../samples/textures/PCnormal.png";
		metallic = "../samples/textures/PCmetallic.png";
		roughness = "../samples/textures/PCroughness.png";
		ao = "../samples/textures/PCao.png";
		break;
	case 7: //limestone
		albedo = "../samples/textures/Lalbedo.png";
		normal = "../samples/textures/Lnormal.png";
		metallic = "../samples/textures/Lmetallic.png";
		roughness = "../samples/textures/Lroughness.png";
		ao = "../samples/textures/Lao.png";
		break;
	case 8: //bark
		albedo = "../samples/textures/Balbedo.png";
		normal = "../samples/textures/Bnormal.png";
		metallic = "../samples/textures/Bmetallic.png";
		roughness = "../samples/textures/Broughness.png";
		ao = "../samples/textures/Bao.png";
		break;
	}


    //texture creation
	Texture Talbedo(albedo, false);
	Texture Tnormal(normal, false);
	Texture Tmetallic(metallic, false);
	Texture Troughness(roughness, false);
	Texture Tao(ao, false);
	

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
	

	//Textured
	sphereR.addTexture(Talbedo);
	sphereR.addTexture(Tnormal);
	if (textured)
	{
		sphereR.addTexture(Tmetallic);
		sphereR.addTexture(Troughness);
	}
	sphereR.addTexture(Tao);
	sphereR.addTexture(irCubeMap);
	sphereR.addTexture(preFilterCubeMap);
	sphereR.addTexture(brdfLUTTexture);

	//Untextured
	sphereNT.addTexture(irCubeMap);
	sphereNT.addTexture(preFilterCubeMap);
	sphereNT.addTexture(brdfLUTTexture);
	
	//skybox
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
	bool playing = true;
	
	float timeT;
	float lastTime = SDL_GetTicks();
	
	SDL_SetRelativeMouseMode(SDL_TRUE);

	while (playing)
	{
	
		glm::mat4 proj = camera.GetProjMat();
		glm::mat4 view = camera.GetViewMat();
		glm::vec3 pos = camera.GetPos();
		//DELTATIME

		timeT = SDL_GetTicks();
		float diff = timeT - lastTime;
		float deltaTime = diff / 1000.0f;
		lastTime = timeT;
		float idealTime = 1.0f / 60.0f;
		if (idealTime > deltaTime)
		{
			SDL_Delay((idealTime - deltaTime)*1000.0f);
		}
		
		//std::cout << deltaTime << std::endl;
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
					
					sphereR.bindTexture();
					sphereR.render();
					

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
				
				
				sphereR.render(); //SHOWS LIGHT POSITIONS 
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
		sky.setMat4("view", glm::inverse(view));
		sky.setMat4("projection", proj);
		cube.bindTexture();
		cube.render();

		if (camera.getMouseLock())
		{
			SDL_ShowCursor(0);
			SDL_SetWindowGrab(core->getWindow(), SDL_TRUE);
			SDL_WarpMouseInWindow(core->getWindow(), (WINDOW_WIDTH / 2), (WINDOW_HEIGHT / 2));
		}
		if (!camera.getMouseLock())
		{
			SDL_ShowCursor(1);
			SDL_SetWindowGrab(core->getWindow(), SDL_FALSE);
		}
		if (camera.getQuit())
		{
			playing = false;
		}
		
		camera.movement(deltaTime, core->getWindow());
		SDL_GL_SwapWindow(core->getWindow());
	}
	
	
	

}


//TO DO - no texture pbr
