#pragma once

#include <glm/glm.hpp>

//����� ���������� ���������� � �������� ��� ���������� ������

#define PI 3.14159265358979323846f  

/// <summary>
/// ��������� �������. ��� �������� ������ ��������.
/// </summary>
extern const glm::mat4 WorldMatrix;

/// <summary>
/// ������ ����.
/// </summary>
extern int WinWidth;
/// <summary>
/// ������ ����.
/// </summary>
extern int WinHeight;

/// <summary>
/// ����������� �������� ������.
/// </summary>
extern bool RotationMode;

/// <summary>
/// ������� ������
/// </summary>
extern glm::vec3 CamPosition;
/// <summary>
/// ������� ���� ������
/// </summary>
extern glm::vec3 CamTarget;
/// <summary>
/// ��������� �� ������ ����� ����������� �����
/// </summary>
extern bool CamMoved;
/// <summary>
/// �������� ������
/// </summary>
extern GLfloat CamSpeed;

