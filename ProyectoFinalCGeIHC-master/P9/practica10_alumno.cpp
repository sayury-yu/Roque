
#define STB_IMAGE_IMPLEMENTATION
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include "Window.h"
#include "Mesh_texturizado.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Material.h"
#include"Model.h"
#include "Skybox.h"
#include"SpotLight.h"

//para audio
#include <iostream>
#include <irrKlang.h>



using namespace irrklang;
const float toRadians = 3.14159265f / 180.0f;
float movOffset;
bool avanza, sube = 1, kilauea = 0, noria=0;
float altitud = 0.0f, giro = 0.0f;
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

float reproduciranimacion, habilitaranimacion, guardoFrame, reinicioFrame, ciclo, ciclo2, contador = 0;

Texture plainTexture;
Texture pisoTexture;
Texture Amarillo;
//materiales
Material Material_brillante;
Material Material_opaco;
//luz direccional
DirectionalLight mainLight;


/************************************Declaracion de las luces de ambos tipos*********************************/
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];
Model cuarto;
Model street_Lamp;
Model Iron_door;
Model wall;
Model tree;
Model stone;
Model RoadsFair;
Model Bath;
Model Kart;
Model BaseTree;
Model Hojas;
Model Tree;
Model Globos;
Model Globo_A;
Model Globo_B;
Model HotDog;
Model Coke;
Model Can;
Model Cotton;
Model Corn;
Model Kilahuea;
Model base_kilahuea;
Model BaseNoria;
Model RuedaNoria;
Model CabinaNoria;
Model Pan;
Model Duck;
Model Mesa;
Model Park;
Model Ice_Cream;
Model PopCorn;
Model Stand;
Model Dardo;

Skybox skybox;
Skybox skybox2;
Skybox skybox3;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

int cont = 0, disparo = 0;
float hora = 0.0, mov_z_dardo = 0.0, aux_x = 0.0, aux_y = 0.0;
int camara1 = 1, camara2 = 0, camaraNoria = 0, juego_encendido = 0, colision = 0, bandera = 1, globo_arriba = 0;
float mov_x_globo = 0, mov_y_globo = -0.01;
int apagarS1 = 0, apagarS2 = 0, apagarP1 = 0, apagarP2 = 0, luces_entrada = 0, globo = 0, trigger_globo = 0, lata = 0, contador_lata = 0, trigger_coca = 0;
float distancia_luz1 = -8.0f, distancia_luz2 = -1.5f,  distanca_Luz1P = 9.5f, distancia_Luz2P = -1.5f,aumenta_globo = 0, mover_lata_x = 0, posx = 0, posy = 0, posz = 0, escx, escy, escz;

static double limitFPS = 1.0 / 60.0;

//void my_input(GLFWwindow *window);
//void inputKeyframes(bool* keys);

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";
//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int * indices, unsigned int indiceCount, GLfloat * vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-30.0f, 0.0f, -30.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		30.0f, 0.0f, -30.0f,	30.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-30.0f, 0.0f, 30.0f,	0.0f, 30.0f,	0.0f, -1.0f, 0.0f,
		30.0f, 0.0f, 30.0f,		30.0f, 30.0f,	0.0f, -1.0f, 0.0f
	};

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);


}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}



#define MAX_FRAMES 9

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 0.5f);

	ISoundEngine* engine = createIrrKlangDevice();

	if (!engine)
		return 0;

	/*************************************SONIDO MP3 *****************************************/

	ISound* ambiental=engine->play2D("Audio/navidad2.mp3", true);
	if(ambiental)
		ambiental->setVolume(0.5);
	/*ISound* track3D=engine->play3D("Audio/gritos.wav", vec3df(0,0,0),true,false,true);
	if (track3D) {
		track3D->setMinDistance(5.0f);
		track3D->setVolume(1000);
	}

	ISound* track3D2 = engine->play3D("Audio/cash.wav", vec3df(0, 0, 0), true, false, true);
	if (track3D2) {
		track3D2->setMinDistance(2.0f);
		track3D2->setVolume(1000);
	}

	ISound* track3D3 = engine->play3D("Audio/cash.wav", vec3df(0, 0, 0), true, false, true);
	if (track3D3) {
		track3D3->setMinDistance(2.0f);
		track3D3->setVolume(1000);
	}*/

	plainTexture = Texture("Textures/pasto.png");
	plainTexture.LoadTextureA();
	Amarillo=Texture("Textures/yellow.tga");
	Amarillo.LoadTexture();
	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	//LUMINARIA
	
	stone = Model();
	stone.LoadModel("Models/CobbleStones2.obj");

	
	// cuarto
	cuarto = Model();
	cuarto.LoadModel("Models/cuarto.obj");
	



	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);

	glm::vec3 posbase = glm::vec3(-7.5f, -2.0f, -29.0f);

	//skybox para dia
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/hills_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/hills_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/hills_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/hills_up.tga");
	skyboxFaces.push_back("Textures/Skybox/hills_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/hills_ft.tga");
	skybox = Skybox(skyboxFaces);
	//skybox para amanecer o atardecer
	std::vector<std::string> skyboxFaces2;
	skyboxFaces2.push_back("Textures/Skybox/hills_rt_night.tga");
	skyboxFaces2.push_back("Textures/Skybox/hills_lf_night.tga");
	skyboxFaces2.push_back("Textures/Skybox/hills_dn_night.tga");
	skyboxFaces2.push_back("Textures/Skybox/hills_up_night.tga");
	skyboxFaces2.push_back("Textures/Skybox/hills_bk_night.tga");
	skyboxFaces2.push_back("Textures/Skybox/hills_ft_night.tga");
	skybox2 = Skybox(skyboxFaces2);
	//skybox para noche
	std::vector<std::string> skyboxFaces3;
	skyboxFaces3.push_back("Textures/Skybox/hills_rt_noche1.tga");
	skyboxFaces3.push_back("Textures/Skybox/hills_lf_noche1.tga");
	skyboxFaces3.push_back("Textures/Skybox/hills_dn_noche1.tga");
	skyboxFaces3.push_back("Textures/Skybox/hills_up_noche1.tga");
	skyboxFaces3.push_back("Textures/Skybox/hills_bk_noche1.tga");
	skyboxFaces3.push_back("Textures/Skybox/hills_ft_noche1.tga");
	skybox3 = Skybox(skyboxFaces3);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 300.0f);

	movOffset = 1.0f;
	avanza = 1;



	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		engine->setListenerPosition(vec3df(camera.getCameraPosition()[0], camera.getCameraPosition()[1], camera.getCameraPosition()[2]), vec3df(0, 0, 1));
		/*track3D->setPosition(vec3df(-7, posYbase + movbase_y, -29));
		track3D2->setPosition(vec3df(-8, -2, -19));
		track3D3->setPosition(vec3df(16, -2, -12));*/

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;


		unsigned int spotLightCount = 0;
		unsigned int pointLightCount = 0;

		//Recibir eventos del usuario
		glfwPollEvents();


		//contador para cambiar de hora cada cierto tiempo
		//de esta forma cambiamos el skybox de acuerdo a la hora del dia.
		while (cont != 500000000) {
			cont += 1;
		}
		if (cont == 500000000) {
			cont = 0;
			hora += 0.008;//si se quiere aumentar o disminuir la velocidad del cambio de hora, se modifica esta variable
			if (hora >= 24.0) {//reinicia el contador al cumplir las 24 horas
				hora = 0.0;
			}
		}

		//Camaras
		if (camara1 == 1 && camara2 == 0)
		{
			camera.keyControl(mainWindow.getsKeys(), deltaTime);
			camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		}
		else if (camara2 == 1 && camara1 == 0)
		{


			camera.keyControlAerea(mainWindow.getsKeys(), deltaTime);
			camera.mouseControlAerea(0, 0);
		}
		else if (camaraNoria == 1)
		{

			camera.keyControlNoria(mainWindow.getsKeys(), deltaTime);
			camera.mouseControlNoria(0, 0);
		}


		
		
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		///////////////////////////////////////
		//Cambia skybox de acuerdo a la hora del dia
		//dia
		if (hora >= 0.0 && hora <= 8.0) {
			skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
			luces_entrada = 0;
		}
		//amanecer o atardecer
		else if ((hora > 8.0 && hora <= 10.0) || (hora > 21.0 && hora <= 24.0)) {
			skybox2.DrawSkybox(camera.calculateViewMatrix(), projection);
		}
		//noche
		else if (hora > 10.0 && hora <= 21.0) {
			skybox3.DrawSkybox(camera.calculateViewMatrix(), projection);
			luces_entrada = 1;
		}

	

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		shaderList[0].SetDirectionalLight(&mainLight);
		//Luces
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		glm::mat4 model(1.0);
		glm::mat4 modelBaseNoria(1.0);
		glm::mat4 modelBaseNoria2(1.0);

		//Piso Pasto
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, -25.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		plainTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		/***************************************************************** ELEMENTOS EXTRA **************************************************************************/

		//path
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.1f , -2.05f  , 1.3f));
		model = glm::scale(model, glm::vec3(0.3f + 0.3f, 0.3f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		stone.RenderModel();
	
	//   cuarto1 implementación 
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(1.2f, -2.f, -17.20));  
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));   /// cambiar la escala 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		cuarto.RenderModel();

		mainWindow.swapBuffers();
	}
//
	engine->drop();
	return 0;
}
