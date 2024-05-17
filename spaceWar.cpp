#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Glut.h"
#include <string.h>
#include <iostream>
#include <cstdio>
using namespace std;
const double Xmin = 0.0, Xmax = 10.0;
const double Ymin = 0.0, Ymax = 10.0;
double move_left_right = 0.0;
double move_up_down = 0.0;
int level = 1;

double circle_speed = 0.05;
const int num_circles = 50;
char buffer[50]; 
std::string str(buffer);
struct Circle {
    double x, y;
};


Circle circles[num_circles];

void myKeyboardFunc(unsigned char key, int x, int y) {}

double randDouble(double min, double max) {
    return min + (rand() / (RAND_MAX / (max - min)));
}

void mySpecialKeyFunc(int key, int x, int y) {
    switch (key) {
    case(GLUT_KEY_LEFT):
        if (move_left_right > -9.5) {
            move_left_right -= 0.25;
        }
        break;

    case(GLUT_KEY_RIGHT):
        if (move_left_right < 9.5) {
            move_left_right += 0.25;
        }
        break;
    case(GLUT_KEY_UP):
        if (move_up_down < 8.5) {
            move_up_down += 0.25;
        }
        else {
            level += 1;
            move_left_right = 0.0;
            move_up_down = 0.0;
        }
        break;
    case(GLUT_KEY_DOWN):
        if (move_up_down > 0) {
            move_up_down -= 0.25;
        }
    }
    glutPostRedisplay();
}

void drawCircle(Circle& circle) {
    glBegin(GL_POLYGON);
    glColor3f(0.0, 1.0, 0.0);
    for (int i = 0; i < num_circles; i++) {
        double teta = 2 * 3.14 * i / 100;
        double x = circle.x + 0.1 * cos(teta);
        double y = circle.y + 0.1 * sin(teta);
        glVertex3f(x, y, 0);
    }
    glEnd();
}

void updateCirclePositions() {
    for (int i = 0; i < num_circles; i++) {
        circles[i].x -= circle_speed*(level/2.5);
        if (circles[i].x < Xmin) {
            circles[i].x = Xmax;  
            circles[i].y = randDouble(1.1, 9); 
        }
    }
}
void renderBitmapString(float x, float y, void* font, const char* string) {
    glRasterPos2f(x, y);
    for (const char* c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

void drawScene(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBegin(GL_TRIANGLES);
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(4.75 + move_left_right, 0.2 + move_up_down, 0.0);
    glVertex3f(5.25 + move_left_right, 0.2 + move_up_down, 0.0);
    glVertex3f(5.0 + move_left_right, 0.5 + move_up_down, 0.0);
    glEnd();

    for (int i = 0; i < num_circles; i++) {
        drawCircle(circles[i]);
    }
    char str[50];
    sprintf_s(str, sizeof(str), "level %d", level);
    glColor3f(0.0, 1.0, 1.0);
    void* font = GLUT_BITMAP_TIMES_ROMAN_24;
    renderBitmapString(0, 0.3, font, str);

    glutSwapBuffers();
}

void timerFunc(int value) {
    updateCirclePositions();
    glutPostRedisplay();
    glutTimerFunc(16, timerFunc, 0);
}

void initRendering() {}

void resizeWindow(int w, int h) {
    double scale, center;
    double windowXmin, windowXmax, windowYmin, windowYmax;

    glViewport(0, 0, w, h);

    w = (w == 0) ? 1 : w;
    h = (h == 0) ? 1 : h;
    if ((Xmax - Xmin) / w < (Ymax - Ymin) / h) {
        scale = ((Ymax - Ymin) / h) / ((Xmax - Xmin) / w);
        center = (Xmax + Xmin) / 2;
        windowXmin = center - (center - Xmin) * scale;
        windowXmax = center + (Xmax - center) * scale;
        windowYmin = Ymin;
        windowYmax = Ymax;
    }
    else {
        scale = ((Xmax - Xmin) / w) / ((Ymax - Ymin) / h);
        center = (Ymax + Ymin) / 2;
        windowYmin = center - (center - Ymin) * scale;
        windowYmax = center + (Ymax - center) * scale;
        windowXmin = Xmin;
        windowXmax = Xmax;
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(windowXmin, windowXmax, windowYmin, windowYmax, -1, 1);
}


int main(int argc, char** argv) {
    srand(time(NULL));  

    for (int i = 0; i < num_circles; i++) {
        circles[i].x = randDouble(Xmin, Xmax);
        circles[i].y = randDouble(1.1, 9);
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(10, 60);
    glutInitWindowSize(360, 360);
    glutCreateWindow("Space War");

    initRendering();

    glutKeyboardFunc(myKeyboardFunc);
    glutSpecialFunc(mySpecialKeyFunc);
    glutReshapeFunc(resizeWindow);
    glutDisplayFunc(drawScene);
    timerFunc(16);
    glutMainLoop();

    return 0;
}
