#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <cmath>
#include <iostream>

// --- STRUCTURES ET VARIABLES GLOBALES ---
struct Point { double x, y; };

std::vector<Point> P;      // Liste des points de contrôle
int k = 50;                // Résolution
double t_visuel = 0.5;     // Le 't' pour l'animation
int draggedPoint = -1;     // Index du point déplacé

int width = 800, height = 600;

double getDistance(Point a, Point b) {
    return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

Point deCasteljau(std::vector<Point> points, double t) {
    if (points.empty()) return { 0, 0 };
    while (points.size() > 1) {
        std::vector<Point> nouveaux_points;
        for (size_t i = 0; i < points.size() - 1; ++i) {
            nouveaux_points.push_back({
                (1 - t) * points[i].x + t * points[i + 1].x,
                (1 - t) * points[i].y + t * points[i + 1].y
            });
        }
        points = nouveaux_points;
    }
    return points[0];
}

void dessinerSqueletteCasteljau(std::vector<Point> points, double t) {
    if (points.size() < 2) return;
    while (points.size() > 1) {
        std::vector<Point> nouveaux_points;
        for (size_t i = 0; i < points.size() - 1; ++i) {
            nouveaux_points.push_back({
                (1 - t) * points[i].x + t * points[i + 1].x,
                (1 - t) * points[i].y + t * points[i + 1].y
            });
        }
        glColor3f(0.2f, 0.8f, 0.2f);
        glBegin(GL_LINE_STRIP);
        for (const auto& p : nouveaux_points) glVertex2d(p.x, p.y);
        glEnd();
        
        glColor3f(1.0f, 1.0f, 0.0f);
        glPointSize(6.0f);
        glBegin(GL_POINTS);
        for (const auto& p : nouveaux_points) glVertex2d(p.x, p.y);
        glEnd();
        points = nouveaux_points;
    }
    glColor3f(1.0f, 0.0f, 0.0f);
    glPointSize(10.0f);
    glBegin(GL_POINTS);
    glVertex2d(points[0].x, points[0].y);
    glEnd();
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT);
    if (P.empty()) return;

    glColor3f(0.4f, 0.4f, 0.4f);
    glBegin(GL_LINE_STRIP);
    for (const auto& p : P) glVertex2d(p.x, p.y);
    glEnd();

    if (P.size() >= 2) {
        glColor3f(0.2f, 0.5f, 1.0f);
        glLineWidth(2.0f);
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i <= k; ++i) {
            double t = (double)i / k;
            Point q = deCasteljau(P, t);
            glVertex2d(q.x, q.y);
        }
        glEnd();
        glLineWidth(1.0f);
    }

    dessinerSqueletteCasteljau(P, t_visuel);

    glColor3f(1.0f, 1.0f, 1.0f);
    glPointSize(8.0f);
    glBegin(GL_POINTS);
    for (const auto& p : P) glVertex2d(p.x, p.y);
    glEnd();
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        if (action == GLFW_PRESS) {
            for (int i = 0; i < P.size(); ++i) {
                if (getDistance(P[i], { xpos, ypos }) < 10.0) {
                    draggedPoint = i;
                    return; 
                }
            }
            P.push_back({ xpos, ypos });
        }
        else if (action == GLFW_RELEASE) draggedPoint = -1; 
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    if (draggedPoint != -1) P[draggedPoint] = { xpos, ypos }; 
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        if (key == GLFW_KEY_RIGHT && t_visuel < 1.0) t_visuel += 0.01; 
        if (key == GLFW_KEY_LEFT && t_visuel > 0.0)  t_visuel -= 0.01; 
        if (key == GLFW_KEY_UP) k += 5;   
        if (key == GLFW_KEY_DOWN && k > 5) k -= 5; 
        if (key == GLFW_KEY_C) P.clear(); 
    }
}

int main() {
    if (!glfwInit()) return -1;
    GLFWwindow* window = glfwCreateWindow(width, height, "Bezier - Rider", NULL, NULL);
    if (!window) { glfwTerminate(); return -1; }

    glfwMakeContextCurrent(window);
    glewInit();

    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetKeyCallback(window, key_callback);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    std::cout << "--- COMMANDES ---" << std::endl;
    std::cout << "Clic Gauche : Ajouter/Deplacer un point" << std::endl;
    std::cout << "Fleches G/D : Modifier t" << std::endl;
    std::cout << "Fleches H/B : Modifier resolution (k)" << std::endl;
    std::cout << "Touche C : Effacer" << std::endl;

    while (!glfwWindowShouldClose(window)) {
        render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}