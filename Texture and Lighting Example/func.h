#pragma once

#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>


using namespace std;

/// <summary>
/// �������� ������� �� ������
/// </summary>
/// <param name="filename">���� � �����.</param>
/// <returns>������ � ����� �������.</returns>
string LoadShader(const char* filename);

/// <summary>
/// ������� ������������� ����� � �������� ����.
/// </summary>
/// <param name="w">������ ����.</param>
/// <param name="h">������ ����.</param>
/// <param name="Fullscreen">����� ������� ������. </param>
/// <returns>��������� �� ��������� ����.</returns>
GLFWwindow* InitAll(int w = 640, int h = 480, bool Fullscreen = false);
/// <summary>
/// ������� ��� ����������� ���������� ���������.
/// </summary>
void EndAll();

//����� ������� ��� ��������� �������

/// <summary>
/// ���������� ��������� ������� ����.
/// </summary>
/// <param name="window">��������� �� ����.</param>
/// <param name="width">����� ������ ����.</param>
/// <param name="height">����� ������ ����.</param>
void window_size_callback(GLFWwindow* window, int width, int height);
/// <summary>
/// ���������� ������� ������.
/// </summary>
/// <param name="window">��������� �� ����.</param>
/// <param name="key">��� �������.</param>
/// <param name="scancode">��� �������.</param>
/// <param name="action">��������.</param>
/// <param name="mods">��������� ������������� �������.</param>
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
/// <summary>
/// �������� OBJ �����
/// </summary>
/// <param name="path">���� � �����.</param>
/// <param name="outVertices">�������� ������ ��������� ������.</param>
/// <param name="outTextures">�������� ������ ���������� ���������.</param>
/// <param name="outNormals">�������� ������ ��������.</param>
/// <returns>������� �� �������� (true/false)</returns>
bool loadOBJ(
    const char* path,
    std::vector < glm::vec3 >& outVertices,
    std::vector < glm::vec2 >& outTextures,
    std::vector < glm::vec3 >& outNormals
);
/// <summary>
/// ��������� ������ �����
/// </summary>
/// <param name="outIndexes">�������� ������ ��������.</param>
/// <param name="outVertices">�������� ������ ��������� ������.</param>
/// <param name="outTextures">�������� ������ ���������� ���������.</param>
/// <param name="outNormals">�������� ������ ��������.</param>
/// <param name="radius">������ �����.</param>
/// <param name="sectorCount">����� ����������.</param>
/// <param name="stackCount">����� ����������.</param>
void genSphere(
    std::vector < GLuint >& outIndexes,
    std::vector < glm::vec3 >& outVertices,
    std::vector < glm::vec2 >& outTextures,
    std::vector < glm::vec3 >& outNormals,
    float radius,
    int sectorCount,
    int stackCount
);
/// <summary>
/// ��������� ��������.
/// </summary>
/// <param name="outIndexes">�������� ������ ��������.</param>
/// <param name="outVertices">�������� ������ ��������� ������.</param>
/// <param name="outTextures">�������� ������ ���������� ���������.</param>
/// <param name="outNormals">�������� ������ ��������.</param>
/// <param name="baseRadius">������ ���������.</param>
/// <param name="topRadius">������ �������.</param>
/// <param name="height">������ ��������.</param>
/// <param name="sectorCount">����� ������.</param>
/// <param name="stackCount">����� ������.</param>
void genCylinder(
    std::vector < GLuint >& outIndexes,
    std::vector < glm::vec3 >& outVertices,
    std::vector < glm::vec2 >& outTextures,
    std::vector < glm::vec3 >& outNormals,
    float baseRadius,
    float topRadius,
    float height,
    int sectorCount,
    int stackCount
);
/// <summary>
/// ��������������� ������� ��� ���������� ������������� ��������� ������ ��������.
/// </summary>
/// <param name="sectorCount">����� ������.</param>
/// <returns></returns>
std::vector < glm::vec3 > getUnitCircleVertices(
    int sectorCount
);
/// <summary>
/// ��������� ����.
/// </summary>
/// <param name="outIndexes">�������� ������ ��������.</param>
/// <param name="outVertices">�������� ������ ��������� ������.</param>
/// <param name="outTextures">�������� ������ ���������� ���������.</param>
/// <param name="outNormals">�������� ������ ��������.</param>
/// <param name="outerRadius">������� ������.</param>
/// <param name="tubeRadius">������ ������ ����.</param>
/// <param name="sectorCount">����� ������.</param>
/// <param name="stackCount">����� ������.</param>
void genTorus(
    std::vector < GLuint >& outIndexes,
    std::vector < glm::vec3 >& outVertices,
    std::vector < glm::vec2 >& outTextures,
    std::vector < glm::vec3 >& outNormals,
    float outerRadius,
    float tubeRadius,
    int sectorCount,
    int stackCount
);
