#include "Model.h"
#include "func.h"


#define PI 3.14159265358979323846f  

glm::vec3 verticesFloor[] =
{
    glm::vec3(-2, 2, 0),
    glm::vec3(2, 2, 0),
    glm::vec3(-2, -2, 0),
    glm::vec3(2, -2, 0)
};
GLuint id[] = { 0,1,3, 0,2,3 };

glm::vec2 textures[] =
{
    glm::vec2(0.0f, 0.0f),
    glm::vec2(1.0f, 0.0f),
    glm::vec2(0.0f, 1.0f),
    glm::vec2(1.0f, 1.0f)
};

const glm::mat4 WorldMatrix = glm::mat4(1.0f);
int WinWidth;
int WinHeight;
bool frotate = true;

glm::vec3 CamPosition;
glm::vec3 CamTarget = glm::vec3(0);
bool CamMoved = false;
GLfloat CamSpeed = 0.1;

int main() {
    srand(time(nullptr));

    GLFWwindow* window = InitAll(1200, 900, false);
    if (window == nullptr)
    {
        EndAll();
        return -1;
    }

    //Время запуска программы, нужно для анимации.
    double start_time = glfwGetTime();

    //Массивы для хранения атрибутов вершин
    std::vector< glm::vec3 > temp_vertices;
    std::vector< glm::vec2 > temp_textures;
    std::vector< glm::vec3 > temp_normals;
    std::vector< glm::vec3 > temp_colors;
    std::vector< GLuint > temp_indices;
    std::vector<std::string> outMaterials;
    size_t size1;
    float r, g, b;
 
    Model floor(window, 2); //снежный пол
    floor.load_coords(verticesFloor, size(verticesFloor));
    floor.load_indices(id, size(id));
    floor.load_texcoord(textures, size(textures));
    floor.load_shaders("texvs.glsl", "texfs.glsl");
    floor.load_texture("textures//desert.jpg", "");
    floor.setMaterial(
        glm::vec3(0.5, 0.5, 0.5),
        glm::vec3(0.5, 0.5, 0.5),
        glm::vec3(0.5, 0.5, 0.5),
        25.6);
  
    loadOBJ("models//cactus1.obj", temp_vertices, temp_textures, temp_normals);
    size1 = temp_vertices.size();
    Model cactus1(window, 2);
    //Загрузка данных модели
    cactus1.load_coords(temp_vertices.data(), size1);
    cactus1.load_normals(temp_normals.data(), temp_normals.size());
    cactus1.load_texcoord(temp_textures.data(), temp_textures.size());
    cactus1.load_shaders("texvs.glsl", "texfs.glsl");
    cactus1.setMaterial(glm::vec3(1.0f, 0.8f, 0.8f), glm::vec3(1.0f, 0.8f, 0.8f), glm::vec3(1.0f, 0.8f, 0.8f), 28.7);
    cactus1.load_texture("textures//1.jpg", "");

    loadOBJ("models//cactus2.obj", temp_vertices, temp_textures, temp_normals);
    size1 = temp_vertices.size();
    Model cactus2(window, 2);
    //Загрузка данных модели
    cactus2.load_coords(temp_vertices.data(), size1);
    cactus2.load_normals(temp_normals.data(), temp_normals.size());
    cactus2.load_texcoord(temp_textures.data(), temp_textures.size());
    cactus2.load_shaders("texvs.glsl", "texfs.glsl");
    cactus2.setMaterial(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), 10);
    cactus2.load_texture("textures//cactus.jpg", "");

    loadOBJ("models//bird.obj", temp_vertices, temp_textures, temp_normals);
    size1 = temp_vertices.size();
    Model bird(window, 2);
    //Загрузка данных модели
    bird.load_coords(temp_vertices.data(), size1);
    bird.load_normals(temp_normals.data(), temp_normals.size());
    bird.load_texcoord(temp_textures.data(), temp_textures.size());
    bird.load_shaders("texvs.glsl", "texfs.glsl");
    bird.setMaterial(glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), 32);
    bird.load_texture("textures//bird1.jpg", "");

    loadOBJ("models//camel.obj", temp_vertices, temp_textures, temp_normals);
    size1 = temp_vertices.size();
    Model camel(window, 2);
    camel.load_coords(temp_vertices.data(), size1);
    camel.load_normals(temp_normals.data(), temp_normals.size());
    camel.load_texcoord(temp_textures.data(), temp_textures.size());
    camel.load_shaders("texvs.glsl", "texfs.glsl");
    camel.setMaterial(glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), 32);
    camel.load_texture("textures//skin2.jpg", "");
    loadOBJ("models//eyes.obj", temp_vertices, temp_textures, temp_normals);
    size1 = temp_vertices.size();
    Model eyes(window, 2);
    eyes.load_coords(temp_vertices.data(), size1);
    eyes.load_normals(temp_normals.data(), temp_normals.size());
    eyes.load_texcoord(temp_textures.data(), temp_textures.size());
    eyes.load_shaders("texvs.glsl", "texfs.glsl");
    eyes.setMaterial(glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), 32);
    eyes.load_texture("textures//eyes.jpg", "");

    // Параметры освещения сцены
    LightsInfo Lights;

    Lights.numPLights = 2;  // 2 точечных источника
    Lights.numSLights = 0;  // Нет прожекторов

    glm::vec3 la = { 0.4f, 0.4f, 0.45f }; 
    glm::vec3 ld = { 0.6f, 0.55f, 0.5f }; 
    glm::vec3 ls = { 0.3f, 0.3f, 0.35f }; 

    // Направленный свет 
    DirLight lSource;
    lSource.ambient = la;
    lSource.diffuse = ld;
    lSource.specular = ls;
    lSource.direction = glm::vec3(0, -1, -0.2f); // Свет сверху с легким наклоном вперед

    Lights.dirLight = lSource;

    // Первый точечный источник света 
    PointLight P;
    P.ambient = glm::vec3(0.35f, 0.35f, 0.4f); 
    P.diffuse = glm::vec3(0.4f, 0.4f, 0.45f); 
    P.specular = glm::vec3(0.25f, 0.25f, 0.3f); 
    P.position = glm::vec3(0, 8, 0); 
    P.constant = 1.0f;
    P.linear = 0.01f; 
    P.quadratic = 0.02f; 
    Lights.pointLights.push_back(P);

    // Второй точечный источник света 
    P.ambient = glm::vec3(0.3f, 0.3f, 0.35f); 
    P.diffuse = glm::vec3(0.35f, 0.35f, 0.4f);
    P.specular = glm::vec3(0.2f, 0.2f, 0.25f); 
    P.position = glm::vec3(3, 2, -3); 
    P.constant = 1.0f;
    P.linear = 0.02f; 
    P.quadratic = 0.05f;
    Lights.pointLights.push_back(P);

    //Теперь создание матриц
    glm::mat4 MMatr = WorldMatrix;
    glm::mat4 VMatr = WorldMatrix;
    glm::mat4 PMatr = WorldMatrix;

    //Матрицу проецирования можно задать сразу же, менять её не надо будет
    PMatr = glm::perspective(
        glm::radians(67.0f), // Угол обзора
        (float)WinWidth / WinHeight,       // Соотношение сторон
        0.01f,              // Ближняя плоскость отсечения
        10000.0f             // Дальняя плоскость отсечения
    );

    //Теперь начальные значения матрицы вида
    CamPosition = { 0, 0, -2 };
    VMatr = glm::lookAt(CamPosition, CamTarget, glm::vec3(0, 1, 0));

  
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); 

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    float RotationMode = 0.0f;
    float rotation = 0;
    float RotationAngle = 0.0f;
    float cact_rotation = 0.0f;
    float X = 103.8;
    float Y = -60.2;
    float Z = 0.15;
    float rotationSpeed = 0.35; // Скорость вращения (радианы в секунду)
  
    //Теперь рисование
    while (!glfwWindowShouldClose(window)) {

        double elapsed = glfwGetTime() - start_time;
        glViewport(0, 0, WinWidth, WinHeight);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Если камеру двигали - надо пересчитать матрицу вида
        if (CamMoved)
        {
            VMatr = WorldMatrix;
            VMatr = glm::lookAt(CamPosition, CamTarget, glm::vec3(0, 1, 0));
            CamMoved = false;
        }

        if (RotationMode)
            rotation = (float)elapsed * rotationSpeed;
        else
            rotation = -(float)elapsed *rotationSpeed;

        MMatr = WorldMatrix;
        floor.render(MMatr, VMatr, PMatr, Lights, GL_TRIANGLES); // отрисовка окр среды
        
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            Y -= 0.05f;
        }
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            Y += 0.05f;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            X -= 0.05f;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            X += 0.05f;
        }

        
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {//для вращения 
            cact_rotation += rotationSpeed;
            if (cact_rotation >= 360.0f) {
                cact_rotation -= 360.0f;
            }

        }
        MMatr = WorldMatrix;
        MMatr = glm::scale(MMatr, glm::vec3(0.05, 0.05, 0.05));
        MMatr = glm::translate(MMatr, glm::vec3(-15.3, -11.78, -5.0)); // Сдвигаем вверх
        MMatr = glm::rotate(MMatr, 20.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        MMatr = glm::rotate(MMatr, glm::radians(cact_rotation), glm::vec3(0.0f, 1.0f, 0.0f));
        cactus1.render(MMatr, VMatr, PMatr, Lights, GL_TRIANGLES);

       MMatr = WorldMatrix;
       MMatr = glm::scale(MMatr, glm::vec3(0.01, 0.01, 0.01));
       MMatr = glm::translate(MMatr, glm::vec3(X, Y, Z)); // Сдвигаем вверх
       cactus2.render(MMatr, VMatr, PMatr, Lights, GL_TRIANGLES);

       MMatr = WorldMatrix;
       if (frotate) {
           MMatr = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -6.0f)); // Начинаем с единичной матрицы
           MMatr = glm::rotate(MMatr, rotation, glm::vec3(0.0f, 1.0f, 0.0f));
           MMatr = glm::translate(MMatr, glm::vec3(5.0f, 0.4f, 0.0f));

       }
        MMatr = glm::scale(MMatr, glm::vec3(0.013, 0.013, 0.013));
        bird.render(MMatr, VMatr, PMatr, Lights, GL_TRIANGLES);

        MMatr = WorldMatrix;
        MMatr = glm::scale(MMatr, glm::vec3(0.15, 0.15, 0.15));
        MMatr = glm::translate(MMatr, glm::vec3(1.75, -9.65, -10)); // Сдвигаем вверх
        MMatr = glm::rotate(MMatr, glm::radians(-128.0f), glm::vec3(0, 1, 0));
        camel.render(MMatr, VMatr, PMatr, Lights, GL_TRIANGLES);
        MMatr = glm::scale(MMatr, glm::vec3(0.8, 0.8, 0.8));
        MMatr = glm::translate(MMatr, glm::vec3(5.78, 7.82, -0.62)); // Сдвигаем вверх
        eyes.render(MMatr, VMatr, PMatr, Lights, GL_TRIANGLES);

        glfwPollEvents();
        glfwSwapBuffers(window);
        if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE))
        {
            glfwSetWindowShouldClose(window, 1);
        }
    }

    return 0;
}