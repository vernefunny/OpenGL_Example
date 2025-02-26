#include "func.h"
#include "globals.h"


string LoadShader(const char* filename)
{
    string res;
    ifstream file(filename, ios::in);
    if (file.is_open()) {
        std::stringstream sstr; // Буфер для чтения
        sstr << file.rdbuf(); // Считываем файл
        res = sstr.str();   //Получаем строку из буфера
        file.close();       //Закрываем файл
    }
    return res;
}

GLFWwindow* InitAll(int w, int h, bool Fullscreen)
{
    //В глобальные переменные сохраняем размеры окна
    WinWidth = w;
    WinHeight = h;

    //Создаём переменную для хранения ID окна
    GLFWwindow* window = nullptr;

    //Подключение GLFW
    if (!glfwInit()) {
        cerr << "ERROR: could not start GLFW3\n";
        exit(-1);
    }

    // Это для MacOS
    /*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/


    //В зависимости от режима экрана создаётся окно с заголовком Capybara OpenGL
    if (Fullscreen)
    {

        GLFWmonitor* mon = glfwGetPrimaryMonitor();
        const GLFWvidmode* vmode = glfwGetVideoMode(mon);
        WinWidth = vmode->width;
        WinHeight = vmode->height;
        window = glfwCreateWindow(WinWidth, WinHeight, "Desert OpenGL", mon, NULL);

    }
    else
        window = glfwCreateWindow(WinWidth, WinHeight, "Desert OpenGL", NULL, NULL);

    //Привязываются функции для обработки событий
    glfwSetWindowSizeCallback(window, window_size_callback);
    glfwSetKeyCallback(window, key_callback);

    //Активируется контекст окна
    glfwMakeContextCurrent(window);

    //Подключение новейшей из доступных версий OpenGL
    glewExperimental = GL_TRUE;

    //Подключение функций OpenGL
    if (glewInit() != GLEW_OK) {
        cerr << "ERROR: could not start GLEW\n";
        return nullptr;
    }

    //Если дошли до сюда - возвращаем окно
    return window;
}

void EndAll()
{
    //Просто выключение GLFW
    //Сюда же можно добавить очистку памяти если будет нужно
    glfwTerminate();
}

void window_size_callback(GLFWwindow* window, int width, int height)
{
    //Просто меняем глобальные переменные
    WinWidth = width;
    WinHeight = height;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
 
   

}

bool loadOBJ(const char* path, std::vector<glm::vec3>& outVertices,
    std::vector<glm::vec2>& outTextures,
    std::vector<glm::vec3>& outNormals)
{
    outNormals.clear();
    outTextures.clear();
    outVertices.clear();
    // Временные массивы для хранения данных из файла
    std::vector<unsigned int> vertexIndices, vtIndices, normalIndices;
    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec2> temp_textures;
    std::vector<glm::vec3> temp_normals;

    // Открытие файла
    std::ifstream file(path, std::ios::in);
    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << path << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream lineStream(line);
        std::string prefix;
        lineStream >> prefix;

        // Обработка координат вершин
        if (prefix == "v") {
            glm::vec3 vertex;
            lineStream >> vertex.x >> vertex.y >> vertex.z;
            temp_vertices.push_back(vertex);
        }
        // Обработка текстурных координат
        else if (prefix == "vt") {
            glm::vec2 texture;
            lineStream >> texture.x >> texture.y;
            temp_textures.push_back(texture);
        }
        // Обработка нормалей
        else if (prefix == "vn") {
            glm::vec3 normal;
            lineStream >> normal.x >> normal.y >> normal.z;
            temp_normals.push_back(normal);
        }
        // Обработка граней
        else if (prefix == "f") {
            std::string vertexData;
            std::vector<std::string> faceData;
            while (lineStream >> vertexData) {
                faceData.push_back(vertexData);
            }

            // Преобразование многоугольника в треугольники
            for (size_t i = 1; i + 1 < faceData.size(); ++i) {
                std::vector<std::string> triangle = {
                    faceData[0], faceData[i], faceData[i + 1]
                };
                for ( auto& vertexInfo : triangle) {
                    std::replace(vertexInfo.begin(), vertexInfo.end(), '/', ' ');
                    std::istringstream vertexStream(vertexInfo);

                    int vIndex = 0, vtIndex = 0, vnIndex = 0;
                    vertexStream >> vIndex;

                    // Индексы OBJ начинаются с 1, относительные индексы — отрицательные
                    if (vIndex < 0) vIndex = temp_vertices.size() + vIndex + 1;
                    vertexIndices.push_back(vIndex);

                    if (vertexStream.peek() == ' ') {
                        vertexStream.ignore();
                        if (vertexStream >> vtIndex) {
                            if (vtIndex < 0) vtIndex = temp_textures.size() + vtIndex + 1;
                            vtIndices.push_back(vtIndex);
                        }
                    }

                    if (vertexStream.peek() == ' ') {
                        vertexStream.ignore();
                        if (vertexStream >> vnIndex) {
                            if (vnIndex < 0) vnIndex = temp_normals.size() + vnIndex + 1;
                            normalIndices.push_back(vnIndex);
                        }
                    }
                }
            }
        }
    }

    file.close();

    // Проверка наличия данных
    if (temp_vertices.empty()) {
        std::cerr << "No vertex data found in file: " << path << std::endl;
        return false;
    }

    // Формирование выходных массивов
    for (size_t i = 0; i < vertexIndices.size(); ++i) {
        if (vertexIndices[i] > 0 && vertexIndices[i] <= temp_vertices.size()) {
            outVertices.push_back(temp_vertices[vertexIndices[i] - 1]);
        }
        if (!temp_textures.empty() && vtIndices[i] > 0 && vtIndices[i] <= temp_textures.size()) {
            outTextures.push_back(temp_textures[vtIndices[i] - 1]);
        }
        else {
            outTextures.emplace_back(0.0f, 0.0f); // Если текстур нет, добавляем заглушку
        }
        if (!temp_normals.empty() && normalIndices[i] > 0 && normalIndices[i] <= temp_normals.size()) {
            outNormals.push_back(temp_normals[normalIndices[i] - 1]);
        }
        else {
            outNormals.emplace_back(0.0f, 0.0f, 0.0f); // Если нормалей нет, добавляем заглушку
        }
    }

    return true;
}

void genSphere(std::vector<GLuint>& outIndexes, std::vector<glm::vec3>& outVertices, std::vector<glm::vec2>& outTextures, std::vector<glm::vec3>& outNormals, float radius, int sectorCount, int stackCount)
{
    //Очистка выходных массивов
    outIndexes.clear();
    outNormals.clear();
    outVertices.clear();
    outTextures.clear();

    //Вспомогательные переменные для хранения промежуточных данных
    float x, y, z, xy;
    float nx, ny, nz, lengthInv = 1.0f / radius;
    float s, t;
    float sectorStep = 2.0f * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stackAngle;

    //Цикл по каждой параллели
    for (int i = 0; i <= stackCount; ++i)
    {
        stackAngle = PI / 2 - i * stackStep;        // начиная от PI/2 и до -PI/2
        xy = radius * cosf(stackAngle);             // r * cos(u)
        z = radius * sinf(stackAngle);              // r * sin(u)

        // На каждую параллель добавляется (sectorCount+1) вершин
        // для первой и последней совпадают позиция и нормаль, но отличаются текстурные координаты
        for (int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;           // от 0 до 2PI

            // высчитываются координаты (x, y, z)
            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
            glm::vec3 vert;
            vert.x = x;
            vert.y = y;
            vert.z = z;
            outVertices.push_back(vert);

            // высчитывается вектор нормали (nx, ny, nz)
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            glm::vec3 norm;
            norm.x = nx;
            norm.y = ny;
            norm.z = nz;
            outNormals.push_back(norm);

            // высчитываются текстурные координаты (s, t) в диапазоне [0, 1]
            s = (float)j / sectorCount;
            t = (float)i / stackCount;
            glm::vec2 vt;
            vt.s = s;
            vt.t = t;
            outTextures.push_back(vt);
        }

        //Но координат мало - нужен порядок обхода, т.е. индексы
        int k1, k2;
        for (int i = 0; i < stackCount; ++i)
        {
            k1 = i * (sectorCount + 1);     // начало текущего меридиана
            k2 = k1 + sectorCount + 1;      // начало следующего меридиана

            for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
            {
                // Для первой и последней параллели по 1 треугольнику, для остальных - по два
                // k1 => k2 => k1+1
                if (i != 0)
                {
                    outIndexes.push_back(k1);
                    outIndexes.push_back(k2);
                    outIndexes.push_back(k1 + 1);
                }

                // k1+1 => k2 => k2+1
                if (i != (stackCount - 1))
                {
                    outIndexes.push_back(k1 + 1);
                    outIndexes.push_back(k2);
                    outIndexes.push_back(k2 + 1);
                }
            }
        }
    }
}

void genCylinder(std::vector<GLuint>& outIndexes, std::vector<glm::vec3>& outVertices, std::vector<glm::vec2>& outTextures, std::vector<glm::vec3>& outNormals, float baseRadius, float topRadius, float height, int sectorCount, int stackCount)
{
    // Очистка выходных массивов
    outIndexes.clear();
    outNormals.clear();
    outVertices.clear();
    outTextures.clear();

    float radius;

    // Вызов вспомогательной функции
    std::vector<glm::vec3> unitVertices = getUnitCircleVertices(sectorCount);

    // Генерация координат вершин боковых граней
    // Выполняется для каждой секции
    for (int i = 0; i < stackCount + 1; ++i)
    {
        //Высота и радиус текущей секции
        float h = 0 + i * (height / stackCount);
        float t = 1.0f - i / stackCount;
        radius = baseRadius + (float)i / stackCount * (topRadius - baseRadius);

        // Для каждого сектора в секции высчитываются вершины
        for (int j = 0, k = 0; j <= sectorCount; ++j, k++)
        {
            float ux = unitVertices[k].x;
            float uy = unitVertices[k].y;
            float uz = unitVertices[k].z;

            // Координаты
            glm::vec3 v;
            v.x = ux * radius;
            v.y = uy * radius;
            v.z = h;
            outVertices.push_back(v);

            // Вектор нормали
            glm::vec3 n;
            n.x = ux;
            n.y = uy;
            n.z = uz;
            outNormals.push_back(n);

            //Текстурные координаты
            glm::vec2 vt;
            vt.s = (float)j / sectorCount;
            vt.t = t;
            outTextures.push_back(vt);

        }
    }

    // Теперь надо сгенерировать вершины, описывающие основание и верхушку
    // На будущее сохраняются индексы начала и конца массива этих вершин
    int baseCenterIndex = (int)outVertices.size();
    int topCenterIndex = baseCenterIndex + sectorCount + 1;

    for (int i = 0; i < 2; ++i)
    {
        float h = 0 + i * height;
        float nz = (float)i;

        // Центральная точка
        outVertices.push_back(glm::vec3(0, 0, h));
        outNormals.push_back(glm::vec3(0, 0, nz));
        outTextures.push_back(glm::vec2(0.5, 0.5));

        radius = i ? topRadius : baseRadius;

        //Для каждой вершины в секторе высчитываются параметры
        for (int j = 0, k = 0; j < sectorCount; ++j, k++)
        {
            float ux = unitVertices[k].x;
            float uy = unitVertices[k].y;
            // координаты
            outVertices.push_back(glm::vec3(ux * radius, uy * radius, h));

            // вектор нормали
            outNormals.push_back(glm::vec3(0, 0, nz));

            // текстурные координаты
            outTextures.push_back(glm::vec2(-ux * 0.5f + 0.5f, -uy * 0.5f + 0.5f));

        }
    }

    //Теперь массив индексов

    int k1 = 0;
    int k2 = sectorCount + 1;

    // Сначала грани
    for (int i = 0; i < sectorCount * (stackCount + 2); ++i, ++k1, ++k2)
    {
        // На каждый сектор по две грани
        // k1 => k1+1 => k2
        outIndexes.push_back(k1);
        outIndexes.push_back(k1 + 1);
        outIndexes.push_back(k2);

        // k2 => k1+1 => k2+1
        outIndexes.push_back(k2);
        outIndexes.push_back(k1 + 1);
        outIndexes.push_back(k2 + 1);
    }

    // Теперь индексы основания и верхушки
    for (int i = 0, k = baseCenterIndex + 1; i < sectorCount; ++i, ++k)
    {
        if (i < sectorCount - 1)
        {
            outIndexes.push_back(baseCenterIndex);
            outIndexes.push_back(k + 1);
            outIndexes.push_back(k);
        }
        else
        {
            outIndexes.push_back(baseCenterIndex);
            outIndexes.push_back(baseCenterIndex + 1);
            outIndexes.push_back(k);
        }
    }

    for (int i = 0, k = topCenterIndex + 1; i < sectorCount; ++i, ++k)
    {
        if (i < sectorCount - 1)
        {
            outIndexes.push_back(topCenterIndex);
            outIndexes.push_back(k);
            outIndexes.push_back(k + 1);
        }
        else // Отдельно последний треугольник
        {
            outIndexes.push_back(topCenterIndex);
            outIndexes.push_back(k);
            outIndexes.push_back(topCenterIndex + 1);
        }
    }
}

std::vector<glm::vec3> getUnitCircleVertices(int sectorCount)
{
    float sectorStep = 2 * PI / sectorCount;
    float sectorAngle;

    std::vector<glm::vec3> unitCircleVertices;
    for (int i = 0; i <= sectorCount; ++i)
    {
        sectorAngle = i * sectorStep;
        glm::vec3 pos;
        pos.x = cos(sectorAngle);
        pos.y = sin(sectorAngle);
        pos.z = 0;
        unitCircleVertices.push_back(pos);
    }
    return unitCircleVertices;
}

void genTorus(std::vector<GLuint>& outIndexes, std::vector<glm::vec3>& outVertices, std::vector<glm::vec2>& outTextures, std::vector<glm::vec3>& outNormals, float outerRadius, float tubeRadius, int sectorCount, int stackCount)
{
    //Очистка выходных массивов
    outIndexes.clear();
    outNormals.clear();
    outVertices.clear();
    outTextures.clear();


    float du = 2 * PI / stackCount;
    float dv = 2 * PI / sectorCount;

    //Генерация вершин для каждой секции
    for (size_t i = 0; i < stackCount; i++) {

        float u = i * du;

        //Генерация для каждого сектора в секции
        for (size_t j = 0; j <= sectorCount; j++) {

            float v = (j % sectorCount) * dv;

            for (size_t k = 0; k < 2; k++)
            {
                float uu = u + k * du;

                // Координаты
                float x = (outerRadius + tubeRadius * cos(v)) * cos(uu);
                float y = (outerRadius + tubeRadius * cos(v)) * sin(uu);
                float z = tubeRadius * sin(v);
                outVertices.push_back(glm::vec3(x, y, z));

                // Вектор нормали
                float nx = cos(v) * cos(uu);
                float ny = cos(v) * sin(uu);
                float nz = sin(v);
                outNormals.push_back(glm::vec3(nx, ny, nz));

                // Текстурные координаты
                float tx = uu / (2 * PI);
                float ty = v / (2 * PI);
                outTextures.push_back(glm::vec2(tx, ty));

            }
            v += dv;
        }
    }

    // Массив индексов - тут есть нюанс. 
    // Координаты ранее были сформированы для ленты треугольников
    // Теперь из ленты их надо преобразовать в набор треугольников
    size_t size = outVertices.size();
    outIndexes.push_back(0);
    outIndexes.push_back(1);
    outIndexes.push_back(2);
    for (int k = 3; k < size; k++)
    {
        outIndexes.push_back(k - 2);
        outIndexes.push_back(k - 1);
        outIndexes.push_back(k);
    }
}
