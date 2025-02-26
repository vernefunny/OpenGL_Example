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

using namespace std;

/// <summary>
/// ��������� � ������� ��� �������� �������. �������� ����������� �� ��������� � ����������� "�������"
/// </summary>
struct Material
{
    glm::vec3 ka; // ����. �������� ��������� (���� �������� ���������)
    glm::vec3 kd; // ����. ���������� ��������� (���� �������)
    glm::vec3 ks; // ����. ����������� �����
    GLfloat shininess; // ���������� �����������
    // �������� �� ���������
    Material()
    {
        ka = glm::vec3(0.0215, 0.1745, 0.0215);
        kd = glm::vec3(0.07568, 0.61424, 0.07568);
        ks = glm::vec3(0.633, 0.727811, 0.633);
        shininess = 128 * 0.6;
    };
};

/// <summary>
/// ��������� � ��������� ���������� �������� ���������
/// </summary>
struct DirLight
{
    /// <summary>
    /// �����������
    /// </summary>
    glm::vec3 direction;
    //��������� �����
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};
/// <summary>
/// ��������� � ��������� ��������� ��������� �����
/// </summary>
struct PointLight
{
    /// <summary>
    /// ����������
    /// </summary>
    glm::vec3 position;
    //���������
    float constant;
    float linear;
    float quadratic;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};
/// <summary>
/// ��������� � ��������� ������������� ��������� �����
/// </summary>
struct SpotLight
{
    //�������������� � �����������
    glm::vec3 position;
    glm::vec3 direction;
    //������ ����
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

/// <summary>
/// ��������� � ��������� ���������� ����� �� �����
/// </summary>
struct LightsInfo {
    /// <summary>
    /// ��������� �������� ���������
    /// </summary>
    DirLight dirLight;
    /// <summary>
    /// ������ � ��������� ����������� �����
    /// </summary>
    std::vector<PointLight> pointLights;
    /// <summary>
    /// ������ � ������������� ����������� �����
    /// </summary>
    std::vector<SpotLight> spotLights;

    //���������� �������� � ������������ ���������� �����
    int numPLights;
    int numSLights;

};



/// <summary>
/// �������� ����� ������ �������. �������� ������ ��� �������� ���������� � ����������.
/// </summary>
class Model
{
public:
    /// <summary>
    /// ������� ����������� - ������ ������ ������, ���� ������. � ���������� ����.
    /// </summary>
    /// <param name="w">��������� �� ����.</param>
    /// <param name="mode">��� ������. 0 - �������, 1 - � ����������, 2 - � ���������.</param>
    Model(GLFWwindow* w, GLuint mode = 0) {
        glGenVertexArrays(1, &vao);
        window = w;
        modelMode = mode;
    };

    //����������. ��� ��� ������������ ������ ���, �� �� ������ �� ������.
    ~Model() {};


    //������ ��� ����������� ������. ������� ��� ���������� � ����� � ����������� � ����������� �� ������.
    //� ������ ������� ����� ����������� ������ ������ ���

    /// <summary>
    /// ������ ����� ��� ���������� - ������ ���������� ������ ������.
    /// </summary>
    /// <param name="mode">������������ �������� - ����� ���������.</param>
    void render(GLuint mode = GL_TRIANGLES);
    /// <summary>
    /// ������ � ������ ��������������� ������� ��������������.
    /// </summary>
    /// <param name="MVPMatrix">������� ��������������.</param>
    /// <param name="mode">����� ���������.</param>
    void render(glm::mat4 MVPMatrix, GLuint mode);
    /// <summary>
    /// ������ � ������������ ��������� ������, ���� � ��������.
    /// </summary>
    /// <param name="modelMatrix">������� ������.</param>
    /// <param name="viewMatrix">������� ����.</param>
    /// <param name="projMatrix">������� ��������.</param>
    /// <param name="mode">����� ���������.</param>
    void render(glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 projMatrix, GLuint mode);

    /// <summary>
    /// ������ � ���������� ���������
    /// </summary>
    /// <param name="modelMatrix">������� ������.</param>
    /// <param name="viewMatrix">������� ����.</param>
    /// <param name="projMatrix">������� ��������.</param>
    /// <param name="lights">��������� ��������� �����.</param>
    /// <param name="mode">����� ���������.</param>
    void render(glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 projMatrix, LightsInfo lights ,GLuint mode);

    //����� ������� ��� �������� ������������ ������� ������
    //� ���������� ���������� ����� ��������� ����� �������

    /// <summary>
    /// ����� ��� �������� ��������� ������.
    /// </summary>
    /// <param name="verteces">������ � ������������.</param>
    /// <param name="count">������ �������.</param>
    void load_coords(glm::vec3* verteces, size_t count);
    /// <summary>
    /// ����� ��� �������� ������ ������.
    /// </summary>
    /// <param name="colors">������ ������.</param>
    /// <param name="count">������ �������.</param>
    void load_colors(glm::vec3* colors, size_t count);

    /// <summary>
    /// ����� ��� �������� ������� ��������.
    /// </summary>
    /// <param name="indices">������ ��������.</param>
    /// <param name="count">������ �������.</param>
    void load_indices(GLuint* indices, size_t count);

    /// <summary>
    /// ����� ��� �������� ��������
    /// </summary>
    /// <param name="normals">������ � ���������.</param>
    /// <param name="count">������ �������.</param>
    void load_normals(glm::vec3* normals, size_t count); 

    /// <summary>
    /// ����� ��� �������� ���������� ���������.
    /// </summary>
    /// <param name="tex">������ � ����������� ������������.</param>
    /// <param name="count">������ �������.</param>
    void load_texcoord(glm::vec2* tex, size_t count); 
    
    /// <summary>
    /// ����� ��� �������� �������.
    /// </summary>
    /// <param name="path">���� � �������� ��������.</param>
    /// <param name="pathtomap">���� � ����� ���������. (����� ���� ������)</param>
    void load_texture(string path, string pathtomap);

    /// <summary>
    /// ����� ��� �������� ��������. � ����� ������� ��������� ������ ��������� � ����������� �������
    /// � ���������� ���������� ����� ������������ ��������� ���������.
    /// </summary>
    /// <param name="vect">���� � ���������� �������</param>
    /// <param name="frag">���� � ������������ �������</param>
    void load_shaders(const char* vect, const char* frag);

    /// <summary>
    /// ����� ��� ������� ���������� ���������
    /// </summary>
    /// <param name="a">������� ����.</param>
    /// <param name="d">��������� ����.</param>
    /// <param name="s">��������� ����.</param>
    /// <param name="shine">�����������.</param>
    void setMaterial(glm::vec3 a, glm::vec3 d, glm::vec3 s, GLfloat shine);

private:

    /// <summary>
    /// ID ������� ������
    /// </summary>
    GLuint vao = -1;    //��������� �������� - ������������ �����, �������� �����������
    /// <summary>
    /// ���������� ������
    /// </summary>
    size_t verteces_count = 0;
    /// <summary>
    /// ���������� ��������
    /// </summary>
    size_t indices_count = 0;

    /// <summary>
    /// ID ��������� ���������
    /// </summary>
    GLuint shader_programme = -1;//��������� �������� - ������������ �����, �������� �����������

    /// <summary>
    /// ��������� �� ����
    /// </summary>
    GLFWwindow* window;

    /// <summary>
    /// �������� �� �������� ������� ������
    /// </summary>
    Material material;

    /// <summary>
    /// ��������������� ����������, ��������������� ��� ������ - � ������� ��� ����������/���������
    /// </summary>
    GLuint modelMode = 0;

    /// <summary>
    /// ��������
    /// </summary>
    GLuint texture = -1;
    /// <summary>
    /// ����� ���������
    /// </summary>
    GLuint texturemap = -1;
};

    