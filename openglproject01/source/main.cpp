#include "prefix.h"
#include "Window.h"
#include "rendering/Mesh.h"
#include "rendering/Texture.h"


#include "Camera.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"


windowProperties wp01 = { "openglproject01", 1280.0f, 720.0f, true };
Window w01(wp01);

float previousFPT, currentFPT; // FrameProcessTime
float deltaTime;
void processInput(GLFWwindow * window, float deltaTime);

light light01;
Camera cam01;



int main(void)
{
	

	// enabling OpenGL features
	glEnable(GL_CULL_FACE);
	enableBlend(); // has extra something in it
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST); // for outlining
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	
	

	// creating shaders

	ShaderProgram solidcolor("source/rendering/shaders/solidcolor.glsl");
	ShaderProgram shader01("source/rendering/shaders/shader01.glsl");
	ShaderProgram depthshader("source/rendering/shaders/depthshader.glsl");
	

	// gathering uniform locations
	///  for shader01

	int u_cameraPosition = shader01.getUniformLocation("u_cameraPosition");
	int u_cameraProjection = shader01.getUniformLocation("u_cameraProjection");
	int u_cameraView = shader01.getUniformLocation("u_cameraView");
	int u_modelToWorldTransformation = shader01.getUniformLocation("u_modelToWorldTransformation");
	std::vector<int> u_material;
	u_material.push_back(shader01.getUniformLocation("u_material.ambient"));
	u_material.push_back(shader01.getUniformLocation("u_material.diffuse"));
	u_material.push_back(shader01.getUniformLocation("u_material.specular"));
	u_material.push_back(shader01.getUniformLocation("u_material.shininess"));
	std::vector<int> u_light;
	u_light.push_back(shader01.getUniformLocation("u_light.position"));
	u_light.push_back(shader01.getUniformLocation("u_light.color"));
	u_light.push_back(shader01.getUniformLocation("u_light.linearAttenuation"));
	u_light.push_back(shader01.getUniformLocation("u_light.quadraticAttenuation"));
	int u_Texture = shader01.getUniformLocation("u_Texture");
	int u_depthMap = shader01.getUniformLocation("u_depthMap");

	/// for solidcolor

	int u_cameraProjection02 = solidcolor.getUniformLocation("u_cameraProjection");
	int u_cameraView02 = solidcolor.getUniformLocation("u_cameraView");
	int u_modelToWorldTransformation02 = solidcolor.getUniformLocation("u_modelToWorldTransformation");

	/// for depthshader

	int u_modelToWorldTransformation03 = depthshader.getUniformLocation("u_ModelToWorldTransformation");
	int u_lightProjection = depthshader.getUniformLocation("u_lightProjection");
	int u_lightView = depthshader.getUniformLocation("u_lightView");


	
	
	// loading meshes

	Mesh plane("source/rendering/objects/plane.ply");
	Mesh shape("source/rendering/objects/uvsphere.ply");
	Mesh lightIndicator("source/rendering/objects/uvsphere.ply");

	// loading textures
	uint nextTextureSlot = 0;
	Texture sun;
	sun.loadFromFile("source/rendering/textures/sunuv.jpg");
	sun.bind(nextTextureSlot++);
	sun.unBind();

	Texture venus;
	venus.loadFromFile("source/rendering/textures/venusuv.jpg");
	venus.bind(nextTextureSlot++);
	venus.unBind();

	Texture mars;
	mars.loadFromFile("source/rendering/textures/marsuv.jpg");
	mars.bind(nextTextureSlot++);
	mars.unBind();

	Texture earth;
	earth.loadFromFile("source/rendering/textures/earthuv.jpg");
	earth.bind(nextTextureSlot++);
	earth.unBind();

	Texture jupiter;
	jupiter.loadFromFile("source/rendering/textures/jupiteruv.jpg");
	jupiter.bind(nextTextureSlot++);
	jupiter.unBind();


	// setting light properties for materials

	material defaultMaterial;
	defaultMaterial.ambient = 0.1f;
	defaultMaterial.diffuse = 0.5f;
	defaultMaterial.specular = 0.1f;
	defaultMaterial.shininess = 1.0f;

	material defaultMaterialBright;
	defaultMaterialBright.ambient = 0.7f;
	defaultMaterialBright.diffuse = 0.5f;
	defaultMaterialBright.specular = 0.1f;
	defaultMaterialBright.shininess = 1.0f;

	material metal;
	metal.ambient = 0.2f;
	metal.diffuse = 0.7f;
	metal.specular = 0.9f;
	metal.shininess = 128.0f;

	material plastic;
	plastic.ambient = 0.2f;
	plastic.diffuse = 0.3f;
	plastic.specular = 0.9f;
	plastic.shininess = 1.0f;
	
	// setting light

	light01.color = glm::vec3(1.0f);
	light01.position = glm::vec3(0.0f, 10.0f, 0.0f);
	light01.linearAttenuation = 0.027f;
	light01.quadraticAttenuation = 0.0028f;

	// shadow things
	/*
	int shadowWidth = w01.getWidth(), shadowHeight = w01.getHeight();

	Texture depthMap;
	depthMap.bind(nextTextureSlot++);
	depthMap.setProperties(GL_DEPTH_COMPONENT, shadowWidth, shadowHeight);
	depthMap.setPixels(malloc(shadowWidth * shadowHeight));
	shader01.setUniform1i(u_depthMap, depthMap.getBindedSlot());

	uint depthFB;
	glGenFramebuffers(1, &depthFB);

	glBindFramebuffer(GL_FRAMEBUFFER, depthFB);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap.getID(), 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
	*/

	//imgui
	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(w01.window, true);
	ImGui::StyleColorsDark();
	bool show_demo_window = false;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


	float rtt = 0.0f; // for arbitrary rotation

	glm::vec3 clearColor = glm::vec3(0.002f, 0.009f, 0.08f);
	glm::vec3 earthsPosition = glm::vec3(0.0f, 2.0f, 0.0f);
	glm::vec3 venusPosition = glm::vec3(0.0f, 5.0f, -2.0f);
	glm::vec3 marsPosition = glm::vec3(0.0f, 3.0f, -5.0f);

	/* Loop until the user closes the window */

	while (!glfwWindowShouldClose(w01.window))
	{

		//TIME

		currentFPT = glfwGetTime();
		deltaTime = currentFPT - previousFPT;
		previousFPT = currentFPT;

		// INPUT

		processInput(w01.window, deltaTime);

		// RENDER

		glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		//imgui
		ImGui_ImplGlfwGL3_NewFrame();


		// setting uniforms
		shader01.setUniformMat4f(u_cameraProjection, cam01.getProjectionMatrix());
		shader01.setUniformMat4f(u_cameraView, cam01.getViewMatrix());
		shader01.setUniform3f(u_cameraPosition, cam01.getCameraPosition());

		solidcolor.setUniformMat4f(u_cameraProjection02, cam01.getProjectionMatrix());
		solidcolor.setUniformMat4f(u_cameraView02, cam01.getViewMatrix());

		// multipurpose matrices

		glm::mat4 scale = glm::mat4(1.0f), rotate = glm::mat4(1.0f), translate = glm::mat4(1.0f), transform = glm::mat4(1.0f);
		

		// rendering depth of the scene to a texture for shadow
		/*
		glm::mat4 lightView = glm::lookAt(light01.position, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 lightProjection = cam01.getProjectionMatrix();
		depthshader.setUniformMat4f(u_lightProjection, lightProjection);
		depthshader.setUniformMat4f(u_lightView, lightView);
		
		glBindFramebuffer(GL_FRAMEBUFFER, depthFB);
		glClear(GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, shadowWidth, shadowHeight);
		
			// plane
			translate = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));
			rotate = glm::rotate(glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			transform = translate * rotate;
			depthshader.setUniformMat4f(u_modelToWorldTransformation03, transform);
			plane.draw(depthshader);
			// primary shape
			translate = glm::translate(glm::vec3(0.0f, 2.0f, 0.0f));
			rotate = glm::rotate(glm::radians(15.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			rotate *= glm::rotate(glm::radians(rtt), glm::vec3(0.0f, 1.0f, 0.0f));
			transform = translate * rotate;
			depthshader.setUniformMat4f(u_modelToWorldTransformation03, transform);
			shape.draw(depthshader);

		glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
		

		// resetting viewport
		glViewport(0, 0, w01.getWidth(), w01.getHeight());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		*/

		/*
		glStencilFunc(GL_ALWAYS, 1, 0xFF); // all fragments should update the stencil buffer
		glStencilMask(0xFF); // enable writing to the stencil buffer
		*/
		

		// light
		sun.bind();
		shader01.setUniform1i(u_Texture, sun.getBindedSlot());
		shader01.setMaterial(u_material, defaultMaterialBright);
		shader01.setLight(u_light, light01);
		translate = glm::translate(light01.position);
		scale = glm::scale(glm::vec3(1.25f));
		transform = translate * scale;
		shader01.setUniformMat4f(u_modelToWorldTransformation, transform);
		lightIndicator.draw(shader01);
		sun.unBind();

		// primary shape(s)
		
		earth.bind();
		shader01.setUniform1i(u_Texture, earth.getBindedSlot());
		shader01.setMaterial(u_material, metal);
		translate = glm::translate(earthsPosition);
		rotate = glm::rotate(glm::radians(15.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		rotate *= glm::rotate(glm::radians(rtt), glm::vec3(0.0f, 1.0f, 0.0f));
		transform = translate * rotate;
		shader01.setUniformMat4f(u_modelToWorldTransformation, transform);
		shape.draw(shader01);
		earth.unBind();
		
		

		venus.bind();
		shader01.setUniform1i(u_Texture, venus.getBindedSlot());
		shader01.setMaterial(u_material, metal);
		translate = glm::translate(venusPosition);
		rotate = glm::rotate(glm::radians(15.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		rotate *= glm::rotate(glm::radians(-rtt), glm::vec3(0.0f, 1.0f, 0.0f));
		scale = glm::scale(glm::vec3(0.5f));
		transform = translate * rotate * scale;
		shader01.setUniformMat4f(u_modelToWorldTransformation, transform);
		shape.draw(shader01);
		venus.unBind();

		mars.bind();
		shader01.setUniform1i(u_Texture, mars.getBindedSlot());
		shader01.setMaterial(u_material, metal);
		translate = glm::translate(marsPosition);
		rotate = glm::rotate(glm::radians(15.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		rotate *= glm::rotate(glm::radians(-rtt), glm::vec3(0.0f, 1.0f, 0.0f));
		scale = glm::scale(glm::vec3(0.25f));
		transform = translate * rotate * scale;
		shader01.setUniformMat4f(u_modelToWorldTransformation, transform);
		shape.draw(shader01);
		mars.unBind();

		/*
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00); // disable writing to the stencil buffer

		translate = glm::translate(earthsPosition);
		rotate = glm::rotate(glm::radians(15.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		rotate *= glm::rotate(glm::radians(rtt), glm::vec3(0.0f, 1.0f, 0.0f));
		scale = glm::scale(glm::vec3(1.1f));
		transform = translate * rotate * scale;
		solidcolor.setUniformMat4f(u_modelToWorldTransformation02, transform);
		shape.draw(solidcolor);

		glStencilMask(0xFF);
		*/

		rtt += 0.5f;
		if (rtt > 360.0f) rtt = 0.0f;

		// plane
		jupiter.bind();
		shader01.setUniform1i(u_Texture, jupiter.getBindedSlot());
		shader01.setMaterial(u_material, plastic);
		translate = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));
		rotate = glm::rotate(glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		transform = translate * rotate;
		shader01.setUniformMat4f(u_modelToWorldTransformation, transform);
		plane.draw(shader01);
		jupiter.unBind();

		//imgui
		// Show a simple window.
		// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
		{
			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our windows open/close state
			ImGui::ColorEdit3("clear color", (float*)&clearColor); // Edit 3 floats representing a color
			ImGui::Text("");
			ImGui::SliderFloat3("Earth's Position", (float*)&earthsPosition, -10.0f, 10.0f);  
			ImGui::SliderFloat3("Venus' Position", (float*)&venusPosition,-10.0f, 10.0f);   
			ImGui::SliderFloat3("Mars' Position", (float*)&marsPosition, -10.0f, 10.0f);

			ImGui::Text("");
			ImGui::Text("Camera's Position (%f, %f, %f)", cam01.getCameraPosition().x, cam01.getCameraPosition().y, cam01.getCameraPosition().z);
			ImGui::Text("Light's Position (%f, %f, %f)", light01.position.x, light01.position.y, light01.position.z);

			ImGui::Text("");
			ImGui::Text("Camera's and light's position is");
			ImGui::Text("controlled by WASD and arrow keys.");
			ImGui::Text("");
			ImGui::Text("Application average is %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		}

		// Show the ImGui demo window. Most of the sample code is in ImGui::ShowDemoWindow(). Read its code to learn more about Dear ImGui!
		if (show_demo_window)
		{
			ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
			ImGui::ShowDemoWindow(&show_demo_window);
		}
		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

		

		w01.onUpdate();

	}

	ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow * window, float deltaTime)
{
	#define buttonPressed(x) glfwGetMouseButton(window, x) == GLFW_PRESS
	#define keyPressed(x) glfwGetKey(window, x) == GLFW_PRESS

	//window attributes

	/// window resize
	glm::ivec2 newWindowSize;
	glfwGetWindowSize(window, &newWindowSize.x, &newWindowSize.y);
	w01.setWidth(newWindowSize.x);
	w01.setHeight(newWindowSize.y);

	/// window focus
	int focused = glfwGetWindowAttrib(w01.window, GLFW_FOCUSED);
	//if (focused == GLFW_FALSE) printf("NOT focused\n");
	//else printf("focused\n");

	// keyboard
	cam01.keyboardUpdate(window, deltaTime);

	// mouse
	cam01.mouseUpdate(window);

	// light

	if (keyPressed(GLFW_KEY_ENTER)) light01.position.y += 0.1f;
	else if (keyPressed(GLFW_KEY_RIGHT_SHIFT)) light01.position.y -= 0.1f;
	if (keyPressed(GLFW_KEY_UP)) light01.position.z -= 0.1f;
	else if (keyPressed(GLFW_KEY_DOWN)) light01.position.z += 0.1f;
	if (keyPressed(GLFW_KEY_RIGHT)) light01.position.x += 0.1f;
	else if (keyPressed(GLFW_KEY_LEFT)) light01.position.x -= 0.1f;
}


