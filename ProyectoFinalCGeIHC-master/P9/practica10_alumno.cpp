
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
Texture mantel;
Texture piso_madera;
Texture calavera_chocolate;
Texture pan;

//materiales
Material Material_brillante;
Material Material_opaco;
//luz direccional
DirectionalLight mainLight;


/************************************Declaracion de las luces de ambos tipos*********************************/
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];
Model stone;
Model cuarto;
Model mesa_ofrenda;
Model calavera;
Model calaveras_azucar;
Model casa;
Model pared1;
Model paredDOS;
Model CAJA1;
Model CAJA2;
Model CAJA3;
Model picado_fi;
Model picado_rojo;
Model picado_verde;
Model picado_cielo_fi;
Model picado_morado_fi;
Model picado_morado;
Model picado_rosa;
Model picado_amarillo;
Model picado_verdeazulado;
Model catrina;
Model candlestick;
Model base_pan;
Model pan_muerto;
Model pan_muerto2;
Model pan_muerto3;
Model apple;
Model plato1;
Model amaranto;
Model tumba;
Model retrato;
Model pecebre;
Model flor_tallo;
Model florero;
Model pollo;
Model botella;
Model candelabro;
Model base_vela;
Model canasta;
Model ofrenda;


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

	ISound* ambiental = engine->play2D("Audio/music-muertos.mp3", true);
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

	plainTexture = Texture("Textures/piso_madera.jpg");
	plainTexture.LoadTextureA();
	Amarillo = Texture("Textures/yellow.tga");
	Amarillo.LoadTexture();
	mantel = Texture("Textures/mantel.png");
	mantel.LoadTexture();
	piso_madera = Texture("Textures/piso_madera.jpg");
	piso_madera.LoadTexture();
	calavera_chocolate = Texture("Textures/calavera_chocolate.jpg");
	calavera_chocolate.LoadTexture();

	pan = Texture("Textures/pan.png");
	pan.LoadTexture();

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	//LUMINARIA

	stone = Model();
	stone.LoadModel("Models/CobbleStones2.obj");
	// casa
	casa = Model();
	casa.LoadModel("Models/casa.obj");

	// pared1
	pared1 = Model();
	pared1.LoadModel("Models/pared1.obj");


	// pared2
	paredDOS = Model();
	paredDOS.LoadModel("Models/paredDOS.obj");

	// caja1
	CAJA1 = Model();
	CAJA1.LoadModel("Models/caja1.obj");

	// caja3
	CAJA2 = Model();
	CAJA2.LoadModel("Models/caja2.obj");

	// caja3
	CAJA3 = Model();
	CAJA3.LoadModel("Models/caja3.obj");
	// picado_fi
	picado_fi = Model();
	picado_fi.LoadModel("Models/picado_fi.obj");

	// picado_rojo
	picado_rojo = Model();
	picado_rojo.LoadModel("Models/picado_rojo.obj");

	// picado_verde
	picado_verde = Model();
	picado_verde.LoadModel("Models/picado_verde.obj");


	// picado_cielo_fi
	picado_cielo_fi = Model();
	picado_cielo_fi.LoadModel("Models/picado_cielo_fi.obj");

	// picado_morado_fi
	picado_morado_fi = Model();
	picado_morado_fi.LoadModel("Models/picado_morado_fi.obj");

	// picado_morado
	picado_morado = Model();
	picado_morado.LoadModel("Models/picado_morado.obj");

	// picado_rosa
	picado_rosa = Model();
	picado_rosa.LoadModel("Models/picado_rosa.obj");

	// picado_amarillo
	picado_amarillo = Model();
	picado_amarillo.LoadModel("Models/picado_amarillo.obj");


	// picado_verdezulado
	picado_verdeazulado = Model();
	picado_verdeazulado.LoadModel("Models/picado_verdeazulado.obj");
	// picado_amarillo
	catrina = Model();
	catrina.LoadModel("Models/catrina.obj");

	//mesa_ofrenda

	mesa_ofrenda = Model();
	mesa_ofrenda.LoadModel("Models/mesa_ofrenda.obj");
	//calavera

	calavera = Model();
	calavera.LoadModel("Models/calavera.obj");

	//calavera
	calaveras_azucar = Model();
	calaveras_azucar.LoadModel("Models/calaveras_azucar.obj");

	//base_pan
	base_pan = Model();
	base_pan.LoadModel("Models/base_pan.obj");
	//pan muerto
	pan_muerto = Model();
	pan_muerto.LoadModel("Models/pan_muerto.obj");

	//pan muerto 2
	pan_muerto2 = Model();
	pan_muerto2.LoadModel("Models/pan_muerto2.obj");

	//pan muerto 3
	pan_muerto3 = Model();
	pan_muerto3.LoadModel("Models/pan_muerto3.obj");

	//apple
	apple = Model();
	apple.LoadModel("Models/apple.obj");

	//amaranto
	amaranto = Model();
	amaranto.LoadModel("Models/amaranto.obj");

	//plato1
	plato1 = Model();
	plato1.LoadModel("Models/plato1.obj");

	//vela
	candlestick = Model();
	candlestick.LoadModel("Models/candlestick.obj");

	//tumba
	tumba = Model();
	tumba.LoadModel("Models/tumba.obj");

	//retrato

	retrato = Model();
	retrato.LoadModel("Models/retrato.obj");

	//pecebre

	pecebre = Model();
	pecebre.LoadModel("Models/pecebre.obj");

	flor_tallo = Model();
	flor_tallo.LoadModel("Models/flor_tallo.obj");

	//florero

	florero = Model();
	florero.LoadModel("Models/florero.obj");

	//pollo

	pollo = Model();
	pollo.LoadModel("Models/pollo.obj");

	//botella
	botella = Model();
	botella.LoadModel("Models/botella.obj");
	//botella

	candelabro = Model();
	candelabro.LoadModel("Models/candelabro.obj");

	//base_vela

	base_vela= Model();
	base_vela.LoadModel("Models/base_vela.obj");


	//canasta
	canasta = Model();
	canasta.LoadModel("Models/canasta.obj");

	//canasta
	ofrenda = Model();
	ofrenda.LoadModel("Models/ofrenda.obj");
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

		//   casa implementación

		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(1.2f, -2.f, -17.20));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));   /// cambiar la escala 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		casa.RenderModel();

		//piso casa
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, -25.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		piso_madera.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();
		//pared1
		
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(7.999995f, 13.200014f, -2.500000f));
		model = glm::scale(model, glm::vec3(-0.8f, -1.1f, 2.1f));   /// cambiar la escala 
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		pared1.RenderModel();
		
		
		//pared2
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(posx, posy,posz));
		model = glm::scale(model, glm::vec3(escx, escy, escz));   /// cambiar la escala
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		paredDOS.RenderModel();
		
		
		//cubos para ofrenda 
		//caja1

		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(-10.600004f, 0.40f, -30.600080));
		model = glm::scale(model, glm::vec3(-0.7f, 0.6f, -0.60f));   /// cambiar la escala 
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		CAJA1.RenderModel();
		
		//caja2
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(-6.699996f, -0.3f, -26.100063f));
		model = glm::scale(model, glm::vec3(0.5f, 0.4f, 0.4f));   /// cambiar la escala 
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		CAJA2.RenderModel();

		//caja3
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(-7.099996f, -1.2f, -23.300053f));
		model = glm::scale(model, glm::vec3(0.4f, 0.2f, 0.5f));   /// cambiar la escala 
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		CAJA3.RenderModel();

		
		//picado_fi
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(-14.200018f, 1.3f, -27.000067f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));   /// cambiar la escala 
		model = glm::rotate(model, -180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		picado_fi.RenderModel();


		//picado_rojo
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(-12.600012f, 1.3f, -25.400061));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.0f));   /// cambiar la escala 
		model = glm::rotate(model, -180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		picado_rojo.RenderModel();

		//picado_verde
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(-11.300007f, 1.3000f, -27.200068f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));   /// cambiar la escala 
		model = glm::rotate(model, -180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		picado_verde.RenderModel();

		//picado_cielo_fi
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(-9.5000f, 1.300f, -27.200068f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));   /// cambiar la escala 
		model = glm::rotate(model, -180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		picado_cielo_fi.RenderModel();
        //picado_morado_fi
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(-7.899995f, 1.3000f,-27.100067f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));   /// cambiar la escala 
		model = glm::rotate(model, -180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		picado_morado_fi.RenderModel();
		
		//picado_morado
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(-13.600016f, -0.30f, -24.200056f));
		model = glm::scale(model, glm::vec3(0.1f,0.1f,0.1f));   /// cambiar la escala 
		model = glm::rotate(model, -180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		picado_morado.RenderModel();
		
		//picado_rosa
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(-11.800009f, -0.3f,-22.400048f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.0f));   /// cambiar la escala 
		model = glm::rotate(model, -180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		picado_rosa.RenderModel();

		//picado_amarillo
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(-10.100002f,-0.300f,-24.200056f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));   /// cambiar la escala 
		model = glm::rotate(model, -180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		picado_amarillo.RenderModel();

		//picado_verdeazulado
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(-8.399996f, -0.3000f, -24.200056f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));   /// cambiar la escala 
		model = glm::rotate(model, -180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		picado_verdeazulado.RenderModel();
		
		/*
		//catrina
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(posx, posy, posz));
		model = glm::scale(model, glm::vec3(escx, escy, escz));   /// cambiar la escala 
		model = glm::rotate(model, -180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		catrina.RenderModel();
		*/
		
		//mesa de ofrenda
		/*
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(1.2f, -2.0f, -17.20));
		model = glm::scale(model, glm::vec3(0.10f,0.10f,0.10f));   /// cambiar la escala
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mantel.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		mesa_ofrenda.RenderModel();
		*/
		
		////calavera
		//model = glm::mat4(1.0);              //  x       y      z   
		//model = glm::translate(model, glm::vec3(-6.899996f,-0.2f,-22.700050f));
		//model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));   /// cambiar la escala
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, -90* toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//calavera_chocolate.UseTexture();
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//calavera.RenderModel();
	
		////calaveras_azucar
		//model = glm::mat4(1.0);              //  x       y      z   
		//model = glm::translate(model, glm::vec3(-10.400003f, -0.2000f, -23.300053f));
		//model = glm::scale(model, glm::vec3(0.5f,0.5f, 0.5f));   /// cambiar la escala
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//calaveras_azucar.RenderModel();
	
		////
		////base_pan
		//model = glm::mat4(1.0);              //  x       y      z   
		//model = glm::translate(model, glm::vec3(posx, posy, posz));
		//model = glm::scale(model, glm::vec3(0.001f,0.001f, 0.001f));   /// cambiar la escala
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//base_pan.RenderModel();
	
	
		//pan de muerto
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(-9.800001f, -0.4000f, -24.500057f));
		model = glm::scale(model, glm::vec3(0.1f,0.1f, 0.1f));   /// cambiar la escala
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pan.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		pan_muerto.RenderModel();
		/*
		//pan_muerto2
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(posx, posy,posz));
		model = glm::scale(model, glm::vec3(escx, escy, escz));   /// cambiar la escala
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		pan_muerto2.RenderModel();
	  
		//pan_muerto3
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(posx, posy, posz));
		model = glm::scale(model, glm::vec3(escx, escy, escz));   /// cambiar la escala
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		pan_muerto3.RenderModel();
		*/
		//apple
	
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(-6.199996f, -0.300f,-23.700054f));
		model = glm::scale(model, glm::vec3(0.015f, 0.015f,0.015f));   /// cambiar la escala
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		apple.RenderModel();

		//plato1
		//model = glm::mat4(1.0);              //  x       y      z   
		//model = glm::translate(model, glm::vec3(-6.999996f,-0.400f, -23.200052f));
		//model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));   /// cambiar la escala
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//plato1.RenderModel();

		////amaranto
		//model = glm::mat4(1.0);              //  x       y      z   
		//model = glm::translate(model, glm::vec3(-6.899996f, -0.2f,-23.200052f));
		//model = glm::scale(model, glm::vec3(0.1f,0.2f, 0.3f));   /// cambiar la escala
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//amaranto.RenderModel();
		//
		//vela
		/*
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(posx, posy, posz));
		model = glm::scale(model, glm::vec3(escx, escy, escz));   /// cambiar la escala
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		candlestick.RenderModel();
		*/
		
		////tumba
		//model = glm::mat4(1.0);              //  x       y      z   
		//model = glm::translate(model, glm::vec3(-9.800001f, -1.3f, -7.299995f));
		//model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.2f));   /// cambiar la escala
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//tumba.RenderModel();
		/*
		//retrato
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(posx, posy, posz));
		model = glm::scale(model, glm::vec3(escx, escy, escz));   /// cambiar la escala
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		retrato.RenderModel();
		*/
		/*
		//pesebre
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(posx, posy, posz));
		model = glm::scale(model, glm::vec3(escx, escy, escz));   /// cambiar la escala
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		pecebre.RenderModel();
		*/
		////pesebre
		//model = glm::mat4(1.0);              //  x       y      z   
		//model = glm::translate(model, glm::vec3(posx, posy, posz));
		//model = glm::scale(model, glm::vec3(escx, escy, escz));   /// cambiar la escala
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//caballo.RenderModel();
		//flor_tallo
		//model = glm::mat4(1.0);              //  x       y      z   
		//model = glm::translate(model, glm::vec3(-6.899996f, -0.2f, -23.200052f));
		//model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));   /// cambiar la escala
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//flor_tallo.RenderModel();
		//Florero
		//model = glm::mat4(1.0);              //  x       y      z   
		//model = glm::translate(model, glm::vec3(-6.899996f, -0.2f, -23.200052f));
		//model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));   /// cambiar la escala
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//florero.RenderModel();

		//model = glm::mat4(1.0);              //  x       y      z   
		//model = glm::translate(model, glm::vec3(-6.899996f, -0.2f, -23.200052f));
		//model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));   /// cambiar la escala
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//pollo.RenderModel();
		//
		//model = glm::mat4(1.0);              //  x       y      z   
		//model = glm::translate(model, glm::vec3(-6.899996f, -0.2f, -23.200052f));
		//model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));   /// cambiar la escala
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//botella.RenderModel();
		//candelabro
		//model = glm::mat4(1.0);              //  x       y      z   
		//model = glm::translate(model, glm::vec3(-6.899996f, -0.2f, -23.200052f));
		//model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));   /// cambiar la escala
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//candelabro.RenderModel();

		////base_vela
		//model = glm::mat4(1.0);              //  x       y      z   
		//model = glm::translate(model, glm::vec3(posx, posy, posz));
		//model = glm::scale(model, glm::vec3(escx, escy, escz));   /// cambiar la escala
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//base_vela.RenderModel();

		//canasta
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(-6.899996f, -0.2f, -23.200052f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));   /// cambiar la escala
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		canasta.RenderModel();

		//ofrenda
		model = glm::mat4(1.0);              //  x       y      z   
		model = glm::translate(model, glm::vec3(posx, posy, posz));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));   /// cambiar la escala
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		ofrenda.RenderModel();
		

		mainWindow.swapBuffers();
	}
//
	engine->drop();
	return 0;
}
