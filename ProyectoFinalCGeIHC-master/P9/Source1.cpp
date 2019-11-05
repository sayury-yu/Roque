
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
bool avanza, sube = 1, kilauea = 0, noria = 0;
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
Model casa;
Model Calceta1;
Model Calceta2;
Model Calceta3;
Model caballito;
Model pino;
Model pinata;
Model Regalo;
Model Regalo1;
Model Regalo2;
Model Regalo3;
Model Regalo4;
Model Regalo5;
Model Regalo6;
Model Regalo7;
Model Rieles;
Model estrella;
Model locomotora;
Model baston;
Model chimenea;
Model Corona;
Model Esfera;
Model Esfera1;
Model Pelota;
Model Dino;
Model Spiderman;
Model Trineo;
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
float distancia_luz1 = -8.0f, distancia_luz2 = -1.5f, distanca_Luz1P = 9.5f, distancia_Luz2P = -1.5f, aumenta_globo = 0, mover_lata_x = 0, posx = 0, posy = 0, posz = 0, escx, escy, escz;

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

	ISound* ambiental = engine->play2D("Audio/navidad2.mp3", true);
	if (ambiental)
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
	Amarillo = Texture("Textures/yellow.tga");
	Amarillo.LoadTexture();
	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	//LUMINARIA

	stone = Model();
	stone.LoadModel("Models/CobbleStones2.obj");


	// cuarto
	casa = Model();
	casa.LoadModel("Models/casa.obj");


	// pino
	pino = Model();
	pino.LoadModel("Models/pino/pino1.obj");
	// pinata
	pinata = Model();
	pinata.LoadModel("Models/pinata/pinata.obj");
	// Calceta
	Calceta1 = Model();
	Calceta1.LoadModel("Models/calceta/calceta1.obj");
	Calceta2 = Model();
	Calceta2.LoadModel("Models/calceta/calceta2.obj");
	Calceta3 = Model();
	Calceta3.LoadModel("Models/calceta/calceta_roja.obj");
	// Chimenea
	chimenea = Model();
	chimenea.LoadModel("Models/chimenea/chimenea1.obj");
	// Regalo2
	Regalo = Model();
	Regalo.LoadModel("Models/regalo2/regalo1.obj");
	Regalo2 = Model();
	Regalo2.LoadModel("Models/regalo2/regalo1.obj");
	Regalo3 = Model();
	Regalo3.LoadModel("Models/regalo2/regalo3.obj");
	Regalo4 = Model();
	Regalo4.LoadModel("Models/regalo2/regalo4.obj");
	Regalo5 = Model();
	Regalo5.LoadModel("Models/regalo2/regalo5.obj");
	Regalo6 = Model();
	Regalo6.LoadModel("Models/regalo2/regalo6.obj");

	//Regalo
	Regalo1 = Model();
	Regalo1.LoadModel("Models/regalo/REGALO_rosa.obj");
	// Corona
	Corona = Model();
	Corona.LoadModel("Models/pino/corona/12170_Wreath_l1.obj");
	// Dino
	Dino = Model();
	Dino.LoadModel("Models/Dinosaur/Dinosaur1.obj");
	// Trineo
	Trineo = Model();
	Trineo.LoadModel("Models/trineo/trineo.obj");
	// Baston
	baston = Model();
	baston.LoadModel("Models/baston/baston1.obj");
	//Caballo
	caballito = Model();
	caballito.LoadModel("Models/caballito/caballito.obj");
	// Pelota
	Pelota = Model();
	Pelota.LoadModel("Models/pelota/pelota.obj");
	// Rieles
	Rieles = Model();
	Rieles.LoadModel("Models/rieles/Rails_OBJ.obj");
	////Locomotora
	locomotora = Model();
	locomotora.LoadModel("Models/tren/loco.obj");
	//// Esfera    /***si carga color**/
	Esfera = Model();
	Esfera.LoadModel("Models/esfera/esfera5.obj"); /*****probar     espe.obj, sayury.obj*/
	Esfera1 = Model(); /****prueba***/
	Esfera1.LoadModel("Models/esfera/sayury.obj");
	//Estrella
	estrella = Model();
	estrella.LoadModel("Models/estrella/estrella.obj");
	// Spiderman
	Spiderman = Model();
	Spiderman.LoadModel("Models/spiderman/M-CoC_iOS_HERO_Peter_Parker_Spider-Man_Stark_Enhanced.obj");

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
		model = glm::translate(model, glm::vec3(0.1f, -2.05f, 1.3f));
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
		casa.RenderModel();

		// trineo
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(7.799995f, -1.60000f, -30.200079f));
		model = glm::scale(model, glm::vec3(0.020000f, 0.021000f, 0.017000));   /// cambiar la escala 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Trineo.RenderModel();
		// regalo_2 1  vertical
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(6.699996f, 1.3000f, -29.900078f));
		model = glm::scale(model, glm::vec3(0.011000f, 0.008000f, 0.007000f));  /// cambiar la escala 
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Regalo.RenderModel();

		// regalo_2 2 caja horizontal
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(5.099998f, 2.70000f, -30.300079f));
		model = glm::scale(model, glm::vec3(0.007000f, 0.005000f, 0.005000f));  /// cambiar la escala 
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Regalo2.RenderModel();

		// regalo_3 2 caja horizontal
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(5.099998f, 2.70000f, -30.300079f));
		model = glm::scale(model, glm::vec3(0.007000f, 0.005000f, 0.005000f));  /// cambiar la escala 
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Regalo3.RenderModel();


		// regalo_4 2 caja horizontal
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(6.699996f, 0.80000f, -30.6000f));
		model = glm::scale(model, glm::vec3(0.0060000f, 0.008000f, 0.006000f)); /// cambiar la escala 
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Regalo4.RenderModel();

		//// regalo_5 2 caja horizontal
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(6.199996f, 1.30000f, -29.500076f));
		model = glm::scale(model, glm::vec3(0.005000f, 0.0050000f, 0.003000));  /// cambiar la escala 
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Regalo5.RenderModel();
		//// regalo_6 2 caja horizontal
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(6.799996f, 0.30000f, -29.400076f));
		model = glm::scale(model, glm::vec3(0.005000, 0.005000f, 0.005000f));  /// cambiar la escala 
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Regalo6.RenderModel();
		/// regalo_2 2 caja horizontal
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(3.899999f, 1.7000f, -29.300076f));
		model = glm::scale(model, glm::vec3(0.006000f, 0.00400f, 0.005000f));  /// cambiar la escala 
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Regalo.RenderModel();


		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(7.799995f, -0.30000f, -28.100071f));
		model = glm::scale(model, glm::vec3(0.009000f, 0.013000f, 0.008000f));  /// cambiar la escala 
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Regalo6.RenderModel();

		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(10.400003f, -0.2000f, -26.300064));
		model = glm::scale(model, glm::vec3(0.013000f, 0.014000f, 0.009000f));  /// cambiar la escala 
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Regalo6.RenderModel();

		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(6.599996f, 1.200000f, -31.400084f));
		model = glm::scale(model, glm::vec3(0.00400000f, 0.005000f, 0.005000f));  /// cambiar la escala 
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Regalo6.RenderModel();


		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(5.499997f, 2.30000f, -29.500076f));
		model = glm::scale(model, glm::vec3(0.006000f, 0.0050000, 0.0060000f));  /// cambiar la escala 
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Regalo6.RenderModel();

		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(8.799997f, -1.30000f, -25.000059f));
		model = glm::scale(model, glm::vec3(0.010000f, 0.0050000, 0.0090000f));  /// cambiar la escala 
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Regalo6.RenderModel();


		/********************VERETICAL*****************************/
		//// regalo_7 2 VERTICAL
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(8.499996f, -0.30000f, -31.600084f));
		model = glm::scale(model, glm::vec3(0.0100000f, 0.0070000f, 0.004000f));  /// cambiar la escala 
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Regalo6.RenderModel();

		//regalo_8 2 VERTICAL
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(6.699996f, 0.80000f, -30.600080f));
		model = glm::scale(model, glm::vec3(0.006000, 0.0080000f, 0.0060000f));  /// cambiar la escala 
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Regalo6.RenderModel();

		//regalo_9 2 VERTICAL
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(6.599996f, 1.10000f, -31.400084f));
		model = glm::scale(model, glm::vec3(0.008000f, 0.006000, 0.006000f));  /// cambiar la escala 
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Regalo6.RenderModel();


		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(8.099995f, -0.4000f, -29.500076f));
		model = glm::scale(model, glm::vec3(0.0060000f, 0.004000f, 0.004000));  /// cambiar la escala 
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Regalo6.RenderModel();

		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(7.399995f, 0.10000f, -31.500082f));
		model = glm::scale(model, glm::vec3(0.007000f, 0.008000f, 0.005000f));  /// cambiar la escala 
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Regalo6.RenderModel();

		/***************************REGALOS TIPO 1*******************************************/

		// regalo 2
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(4.899998f, 0.400000f, -30.000078f));
		model = glm::scale(model, glm::vec3(0.400000f, 0.50000f, 0.400000f));   /// cambiar la escala 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Regalo1.RenderModel();
		// regalo 3
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(5.499997f, 0.300000f, -30.700081f));
		model = glm::scale(model, glm::vec3(0.70000000f, 0.700000f, 0.300000f));  /// cambiar la escala 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Regalo1.RenderModel();
		/*************************** Corona de Navidad***********************************************/

		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(7.299995f, 6.699996f, -31.600082f));
		model = glm::scale(model, glm::vec3(0.0930000f, 0.069000f, 0.072000f));   /// cambiar la escala 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Corona.RenderModel();
		/*****************************JUGUETES*********************************************/
		//Pelota
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(14.100018f, -1.20000f, -21.200045f));
		model = glm::scale(model, glm::vec3(0.0630000f, 0.052000f, 0.055000f));   /// cambiar la escala 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pelota.RenderModel();

		// Spiderman
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(11.700008f, -1.80000f, -20.900043f));
		model = glm::scale(model, glm::vec3(1.7000000f, 1.1000f, 1.00000f));   /// cambiar la escala 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Spiderman.RenderModel();
		/// Dinosaurio
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(12.200010f, -1.200000f, -22.200048f));
		model = glm::scale(model, glm::vec3(0.300000f, 0.300000f, 0.300000f));   /// cambiar la escala 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Dino.RenderModel();

		// Baston
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(13.700016f, 1.6000f, -20.600042f));
		model = glm::scale(model, glm::vec3(0.50000f, 0.40000f, 0.30000f));   /// cambiar la escala
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		baston.RenderModel();

		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(6.699996f, -0.4000f, -28.800074f));
		model = glm::scale(model, glm::vec3(0.50000f, 0.40000f, 0.30000f));   /// cambiar la escala
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		baston.RenderModel();

		// chimenea

		model = glm::translate(model, glm::vec3(14.100018f, 42.299881f, 2.300000f));
		model = glm::scale(model, glm::vec3(4.899998f, 3.599999f, 3.999998f));   /// cambiar la escala
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		chimenea.RenderModel();
		/*****************************ACCESORIOS DE CHIMENEA**********************************************/
		// Calceta1
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(12.900013f, 1.8000f, -16.500027));
		model = glm::scale(model, glm::vec3(0.0790000f, 0.04300f, 0.04000f));   /// cambiar la escala
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Calceta1.RenderModel();
		// Calceta2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(12.900013f, 1.8000f, -15.000021f));
		model = glm::scale(model, glm::vec3(0.0790000f, 0.04300f, 0.04000f));   /// cambiar la escala
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Calceta2.RenderModel();
		//Calceta3
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(12.900013f, 1.80000f, -17.700031f));
		model = glm::scale(model, glm::vec3(0.0790000f, 0.04300f, 0.04000f));   /// cambiar la escala
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Calceta3.RenderModel();
		/***TREN**/
		//model = glm::mat4(1.0);              //  x       y      z   
		//model = glm::translate(model, glm::vec3(posx,posy,posz));
		//model = glm::scale(model, glm::vec3(escx, escy,escz));   /// cambiar la escala 
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//locomotora.RenderModel();
		//

		/***TREN**/
		//model = glm::mat4(1.0);              //  x       y      z   
		//model = glm::translate(model, glm::vec3(posx,posy,posz));
		//model = glm::scale(model, glm::vec3(escx, escy,escz));   /// cambiar la escala 
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//caballito.RenderModel();
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(3.8999f, -2.0000f, -23.900055f));
		model = glm::scale(model, glm::vec3(0.002000f, 0.004000f, 0.005000));   /// cambiar la escala 
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		caballito.RenderModel();

		// Rieles de tren
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(14.800020f, -1.90000f, -19.500038f));
		model = glm::scale(model, glm::vec3(0.000100f, 0.000110f, 0.000210f));   /// cambiar la escala 
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Rieles.RenderModel();


		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(10.700005f, -1.9000f, -19.500038f));
		model = glm::scale(model, glm::vec3(0.000242f, 0.000081f, 0.000213f));   /// cambiar la escala 
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Rieles.RenderModel();

		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(7.799995f, -1.9000f, -22.200048f));
		model = glm::scale(model, glm::vec3(0.000242f, 0.000081f, 0.000213f));   /// cambiar la escala 
		model = glm::rotate(model, 90 * toRadians, glm::vec3(.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Rieles.RenderModel();






		/***************************DECORACIÓN DE ARBOL*****************************/
		/// pino de navidad
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(11.90009f, -3.299999f, -25.200060f));
		model = glm::scale(model, glm::vec3(0.60000f, 0.60000f, 0.60000f));   /// cambiar la escala 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		pino.RenderModel();
		/// pino de navidad
		//model = glm::mat4(1.0);              //  x       y      z   
		//model = glm::translate(model, glm::vec3(posx,posy,posz));
		//model = glm::scale(model, glm::vec3(escx, escy,escz));   /// cambiar la escala 
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//pinata.RenderModel();
		// Estrella de arbol
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(11.900009f, 9.700001f, -25.300060f));
		model = glm::scale(model, glm::vec3(0.022000f, 0.012000f, 0.01000f));   /// cambiar la escala 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		estrella.RenderModel();

		// Espera1

		//model = glm::mat4(1.0);              //  x       y      z   
		//model = glm::translate(model, glm::vec3(posx, posy, posz));
		//model = glm::scale(model, glm::vec3(escx, escy, escz));   /// cambiar la escala 
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//Esfera.RenderModel();

		// Esfera 2
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(14.600019f, 5.799997, -24.700056f));
		model = glm::scale(model, glm::vec3(0.0270000f, 0.01700f, 0.017000f));   /// cambiar la escala 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Esfera.RenderModel();
		// Esfera 2
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(12.500011f, 3.99999, -26.100061f));
		model = glm::scale(model, glm::vec3(0.027000f, 0.017000f, 0.017000f));   /// cambiar la escala 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Esfera.RenderModel();
		//// Esfera 2
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(12.500011f, 5.599997f, -26.100061f));
		model = glm::scale(model, glm::vec3(0.027000f, 0.017000f, 0.017000f));   /// cambiar la escala 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Esfera.RenderModel();


		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(12.700012f, 7.199996f, -25.100058f));
		model = glm::scale(model, glm::vec3(0.027000f, 0.017000f, 0.01700f));   /// cambiar la escala 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Esfera.RenderModel();
		// Esfera 2
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(12.200010f, 5.499997f, -23.600052f));
		model = glm::scale(model, glm::vec3(0.027000f, 0.017000f, 0.01700f));   /// cambiar la escala 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Esfera.RenderModel();
		//// Esfera 2
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(13.800016f, 6.199996f, -23.600052f));
		model = glm::scale(model, glm::vec3(0.027000f, 0.017000f, 0.01700f));   /// cambiar la escala 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Esfera.RenderModel();

		//// Esfera 2
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(14.700020f, 5.199997f, -23.600052f));
		model = glm::scale(model, glm::vec3(0.027000f, 0.017000f, 0.01700f));   /// cambiar la escala 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Esfera.RenderModel();
		//// Esfera 2
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(15.300022f, 4.299998f, -23.600052f));
		model = glm::scale(model, glm::vec3(0.027000f, 0.017000f, 0.01700f));   /// cambiar la escala 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Esfera.RenderModel();
		//// Esfera 2
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(12.700012f, 4.499998f, -23.400051f));
		model = glm::scale(model, glm::vec3(0.027000f, 0.017000f, 0.01700f));   /// cambiar la escala 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Esfera.RenderModel();
		//// Esfera 2
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(12.400011f, 1.00000f, -21.800045f));
		model = glm::scale(model, glm::vec3(0.027000f, 0.017000f, 0.01700f));   /// cambiar la escala 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Esfera.RenderModel();

		//// Esfera 2
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(13.100014f, 2.1000f, -21.800045f));
		model = glm::scale(model, glm::vec3(0.027000f, 0.017000f, 0.01700f));   /// cambiar la escala 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Esfera.RenderModel();
		//// Esfera 2
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(14.500019f, 2.0000f, -21.800045f));
		model = glm::scale(model, glm::vec3(0.027000f, 0.017000f, 0.01700f));   /// cambiar la escala 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Esfera.RenderModel();
		//// Esfera 2
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(10.400003, 2.2000f, -25.000057f));
		model = glm::scale(model, glm::vec3(0.027000f, 0.017000f, 0.01700f));   /// cambiar la escala 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Esfera.RenderModel();
		//// Esfera 2
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(10.400003, 1.50000f, -24.000054f));
		model = glm::scale(model, glm::vec3(0.027000f, 0.017000f, 0.01700f));   /// cambiar la escala 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);

		Esfera.RenderModel();
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(11.700008, 2.00000f, -25.100058f));
		model = glm::scale(model, glm::vec3(0.026000f, 0.012000f, 0.01200f));   /// cambiar la escala 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Esfera.RenderModel();

		Esfera.RenderModel();
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(11.700008, 2.00000f, -25.100058f));
		model = glm::scale(model, glm::vec3(0.026000f, 0.012000f, 0.01200f));   /// cambiar la escala 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Esfera.RenderModel();

		/////
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(11.00006f, 3.099999f, -22.500050f));
		model = glm::scale(model, glm::vec3(0.019000f, 0.011000f, 0.011000f));   /// cambiar la escala 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Esfera.RenderModel();

		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(14.600019f, 2.799999f, -23.200052f));
		model = glm::scale(model, glm::vec3(0.019000f, 0.011000f, 0.011000f));   /// cambiar la escala 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Esfera.RenderModel();

		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(12.800013f, 3.399999f, -22.800021f));
		model = glm::scale(model, glm::vec3(0.019000f, 0.011000f, 0.011000f));   /// cambiar la escala 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Esfera.RenderModel();

		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(13.000013f, 7.599995f, -24.100056f));
		model = glm::scale(model, glm::vec3(0.019000f, 0.011000f, 0.011000f));   /// cambiar la escala 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Esfera.RenderModel();

		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(13.400015f, 8.399996f, -24.500057f));
		model = glm::scale(model, glm::vec3(0.019000f, 0.012000f, 0.012000f));   /// cambiar la escala 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Esfera.RenderModel();

		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(13.200014f, 9.099998f, -24.500057f));
		model = glm::scale(model, glm::vec3(0.019000f, 0.012000f, 0.012000f));   /// cambiar la escala 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Esfera.RenderModel();

		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(12.400011f, 8.399996f, -24.500057f));
		model = glm::scale(model, glm::vec3(0.019000f, 0.012000f, 0.012000f));   /// cambiar la escala 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Esfera.RenderModel();

		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(11.500008f, 4.699998f, -24.500057f));
		model = glm::scale(model, glm::vec3(0.019000f, 0.012000f, 0.012000f));   /// cambiar la escala 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Esfera.RenderModel();

		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(15.300022f, 2.8000f, -22.800051f));
		model = glm::scale(model, glm::vec3(0.020000f, 0.012000f, 0.012000f));   /// cambiar la escala 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Esfera.RenderModel();

		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(15.400023f, -1.30000f, -21.400045f));
		model = glm::scale(model, glm::vec3(0.019000f, 0.012000f, 0.012000f));   /// cambiar la escala 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Esfera.RenderModel();


		// Esfera 
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(10.200003f, 3.299999f, -26.100063f));
		model = glm::scale(model, glm::vec3(0.018000f, 0.012000f, 0.011000f));   /// cambiar la escala 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Esfera.RenderModel();
		// Esfera 
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(9.299999f, -1.20000f, -22.200048f));
		model = glm::scale(model, glm::vec3(0.018000f, 0.012000f, 0.011000f));   /// cambiar la escala 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Esfera.RenderModel();
		// Esfera 
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(10.500004f, 0.300000f, -22.600050f));
		model = glm::scale(model, glm::vec3(0.01800f, 0.012000f, 0.0110000f));   /// cambiar la escala 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Esfera.RenderModel();
		// Esfera 
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(12.500011f, 0.50000f, -22.600050f));
		model = glm::scale(model, glm::vec3(0.018000f, 0.012000f, 0.0110000f));   /// cambiar la escala 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		// Esfera 
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(15.400023f, -0.30000f, -24.500057f));
		model = glm::scale(model, glm::vec3(0.018000f, 0.012000f, 0.011000f));   /// cambiar la escala 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Esfera.RenderModel();
		// Esfera 
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(14.500019f, 0.1000f, -22.300049f));
		model = glm::scale(model, glm::vec3(0.018000f, 0.012000f, 0.0110000f));   /// cambiar la escala 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Esfera.RenderModel();
		// Esfera 
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(14.500019f, 1.90000f, -23.300053f));
		model = glm::scale(model, glm::vec3(0.0180000, 0.012000f, 0.011000f));   /// cambiar la escala 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Esfera.RenderModel();


		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(posx, posy, posz));
		model = glm::scale(model, glm::vec3(escx, escy, escz));   /// cambiar la escala 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Esfera1.RenderModel();


		//Platilla
		//model = glm::mat4(1.0);              //  x       y      z   
		//model = glm::translate(model, glm::vec3(posx, posy, posz));
		//model = glm::scale(model, glm::vec3(escx, escy, escz));   /// cambiar la escala 
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//Regalo.RenderModel();


		mainWindow.swapBuffers();
	}
	//
	engine->drop();
	return 0;
}