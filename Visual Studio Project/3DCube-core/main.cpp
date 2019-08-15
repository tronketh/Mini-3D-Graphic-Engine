#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <strstream>
#include <algorithm>

#include "vec3.h"
#include "mat4x4.h"

GLFWwindow* window;
bool isRunning = false;

float fNear = 0.1f;
float fFar = 1000.0f;
float fFov = 90.0f;
float fAspectRatio = 600.0f / 800.0f;
float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);
vec3 vCamera = { 0.0f, 0.0f, 0.0f }, lightSource = { 0.0f, 0.0f, -1.0f };

float deltaTime = 0, elapsedTime = 0;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

struct triangle {
	vec3 p[3];
	float intensity;
	vec3 normal() {
		vec3 temp1,temp2;
		temp2 = p[2] - p[0];
		temp1 = p[1] - p[0];
		vec3 n = temp1.crossProduct(temp2);
		return n/n.length();
	}

	vec3 mid() {
		vec3 temp;
		temp.x = (p[0].x + p[1].x + p[2].x) / 3.0f;
		temp.y = (p[0].y + p[1].y + p[2].y) / 3.0f;
		temp.z = (p[0].z + p[1].z + p[2].z) / 3.0f;
		return temp;
	}
};

bool compareTriangle(triangle &i1, triangle &i2)
{
	return (i1.mid().z > i2.mid().z);
}

struct mesh {
	std::vector<triangle> triangles;

	bool LoadFromObjectFile(std::string fileName) {
		std::fstream input;
		input.open(fileName, std::ios::in);
		if (!input.is_open()) {
			return false;
		}
		std::vector<vec3> verts;
		std::cout << "Start" << std::endl;;
		while (!input.eof()) {
			char line[128];
			input.getline(line,128);
			char junk;
			std::strstream s;
			s << line;

			if (line[0] == 'v') {
				vec3 v;
				s >> junk >> v.x >> v.y >> v.z;
				verts.push_back(v);
			}
			if (line[0] == 'f') {
				int f[3];
				s >> junk >> f[0] >> f[1] >> f[2];
				triangle t;
				t.p[0] = verts[f[0] - 1];
				t.p[1] = verts[f[1] - 1];
				t.p[2] = verts[f[2] - 1];
				triangles.push_back(t);
			}
		}
		return true;
	}
};

mesh teapot;

mat4x4 MakeProjectionMatrix() {
	mat4x4 matProj;
	matProj.m[0][0] = fAspectRatio * fFovRad;
	matProj.m[1][1] = fFovRad;
	matProj.m[2][2] = fFar / (fFar - fNear);
	matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
	matProj.m[2][3] = 1.0f;
	matProj.m[3][3] = 0.0f;
	return matProj;
}

mat4x4 MakeZRotationMatrix(float fTheta) {
	mat4x4 matRotZ;
	matRotZ.m[0][0] = cosf(fTheta);
	matRotZ.m[0][1] = sinf(fTheta);
	matRotZ.m[1][0] = -sinf(fTheta);
	matRotZ.m[1][1] = cosf(fTheta);
	matRotZ.m[2][2] = 1;
	matRotZ.m[3][3] = 1;
	return matRotZ;
}

mat4x4 MakeYRotationMatrix(float fTheta) {
	mat4x4 matRotY;
	matRotY.m[0][0] = cosf(fTheta);
	matRotY.m[0][2] = sinf(fTheta);
	matRotY.m[1][1] = 1;
	matRotY.m[2][0] = -sinf(fTheta);
	matRotY.m[2][2] = cosf(fTheta);
	matRotY.m[3][3] = 1;
	return matRotY;
}

mat4x4 MakeXRotationMatrix(float fTheta) {
	mat4x4 matRotX;
	matRotX.m[0][0] = 1;
	matRotX.m[1][1] = cosf(fTheta);
	matRotX.m[1][2] = sinf(fTheta);
	matRotX.m[2][1] = -sinf(fTheta);
	matRotX.m[2][2] = cosf(fTheta);
	matRotX.m[3][3] = 1;
	return matRotX;
}

mat4x4 MakeIdentityMatrix() {
	mat4x4 i;
	i.m[0][0] = 1.0f;
	i.m[1][1] = 1.0f;
	i.m[2][2] = 1.0f;
	i.m[3][3] = 1.0f;
	return i;
}

mat4x4 MakeTranslationMatrix(float x, float y, float z) {
	mat4x4 t = MakeIdentityMatrix();
	t.m[3][0] = x;
	t.m[3][1] = y;
	t.m[3][2] = z;
	return t;
}


void init(const char* title, int width, int height) {
	if (!glfwInit()) {
		std::cerr << "Failed to Initialize" << std::endl;
		return;
	}
	std::cout << "GLFW Initialized" << std::endl;

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(width, height, title, NULL, NULL);
	//Window Center
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	if (!monitor)
		return;
	std::cout << "Window Created" << std::endl;
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	if (!mode)
		return;

	int monitorX, monitorY;
	glfwGetMonitorPos(monitor, &monitorX, &monitorY);
	glfwSetWindowPos(window,
		monitorX + (mode->width - width) / 2,
		monitorY + (mode->height - height) / 2);
	//Window Center End

	glfwMakeContextCurrent(window);
	glViewport(0, 0, width, height);
	glfwSetKeyCallback(window, key_callback);

	if (glewInit() != GLEW_OK) {
		std::cerr << "GLEW not Initialized" << std::endl;
		return;
	}
	isRunning = true;
}

void Update() {
	glfwPollEvents();
	glfwSwapBuffers(window);
	if (glfwWindowShouldClose(window) == 1) {
		isRunning = false;
	}
}

void DrawFilledTriangle(triangle& tri, float intensity,bool wireframe = false) {
	//glLineWidth(2.0);
	glColor3f(intensity, 0, 0);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < 3; i++)
		glVertex2f(tri.p[i].x, tri.p[i].y);
	glEnd();
}

void DrawWireFrame(triangle& tri) {
	//glLineWidth(2.0);
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 3; i++)
		glVertex2f(tri.p[i].x, tri.p[i].y);
	glEnd();
}

void Draw() {
	float fTheta = 1.0f * elapsedTime;
	mat4x4 matRotX = MakeXRotationMatrix(fTheta * 0.5f),
		matRotZ = MakeZRotationMatrix(fTheta), 
		matRotY = MakeYRotationMatrix(fTheta);
	mat4x4 translationBeforeRotation = MakeTranslationMatrix(0.0f, 0.0f, 0.0f);
	mat4x4 translationAfterRotation = MakeTranslationMatrix(0.0f, 0.0f, 50.0f);

	mat4x4 worldMatrix = MakeIdentityMatrix();
	worldMatrix *= translationBeforeRotation;
	worldMatrix *= matRotZ*matRotX;
	worldMatrix *= translationAfterRotation;
	mat4x4 matProj = MakeProjectionMatrix();

	std::vector<triangle> triangleToDraw;

	for (auto &tri : teapot.triangles) {
		triangle triProjected, triTransformed;
		for(int i = 0; i < 3; i++)
			triTransformed.p[i] = tri.p[i] * worldMatrix;

		vec3 normal=triTransformed.normal();

		lightSource = lightSource.normal();

		float intensity = normal.dotProduct(lightSource);

		float Dot = normal.dotProduct(triTransformed.mid()-vCamera);

		if (Dot < 0.5f) {
			for (int i = 0; i < 3; i++) {
				triProjected.p[i] = triTransformed.p[i] * matProj;
				triProjected.p[i] /= triProjected.p[i].w;
			}

			triProjected.intensity = intensity;

			triangleToDraw.push_back(triProjected);
		}
	}

	sort(triangleToDraw.begin(),triangleToDraw.end(),compareTriangle);

	for (auto &triProjected : triangleToDraw) {
		DrawFilledTriangle(triProjected, 0.45f + (triProjected.intensity * 0.5f));
		//DrawWireFrame(triProjected);
	}
}

void Quit() {
	glfwDestroyWindow(window);
	glfwTerminate();
	std::cout << "Game Quit" << std::endl;
}

int main()
{
	//LoadMesh
	bool over=teapot.LoadFromObjectFile("utah-teapot.obj");

	if (!over) {
		std::cout << "Object Not loaded" << std::endl;
	}
	
	//Main Game
	init("3D Cube", 800, 600);
	glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
	while (isRunning) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Draw();
		Update();
		deltaTime = glfwGetTime();
		elapsedTime += deltaTime;
		glfwSetTime(0);

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			vCamera.z -= 50.0f * deltaTime;
		}
		else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			vCamera.z += 50.0f * deltaTime;
		}
		else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			vCamera.x += 50.0f * deltaTime;
		}
		else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			vCamera.x -= 50.0f * deltaTime;
		}
		//std::cout << deltaTime << std::endl;
	}
	Quit();
	return 0;
}

void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	
}