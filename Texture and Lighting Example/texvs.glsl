#version 400
in vec3 vertex_position;
in vec3 vertex_normal;
in vec2 vertex_texture;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;



out vec3 normal;
out vec3 f_pos;
out vec2 TexCoord;


void main() {
	//�������������� ��������� ��������� � ������ ������� ������
	f_pos = vec3(M * vec4(vertex_position, 1.0));
	//�������������� ��������� �������� � ������ ������� ������
	normal = mat3(transpose(inverse(M))) * vertex_normal; ;

	TexCoord = vertex_texture;


	//������� ���������� ��������� ���������
	gl_Position = P * V * vec4(f_pos, 1.0);
}
