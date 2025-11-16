#include <GL/glut.h>
#include <GL/glu.h>
#include <cmath>
#include <vector>
#include <cstdlib>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

const int WINDOW_WIDTH = 900;
const int WINDOW_HEIGHT = 600;

// Seasons: 0=spring,1=summer,2=autumn,3=winter
int currentSeason = 3;

// Interior mode toggle
bool interiorMode = false;

// Sun and cloud positions
float sunX = -50.0f;
float sunSpeed = 1.0f;
float cloudX[3];

// Fire flicker offsets
float fireOffset[5] = {0};

// Snowflake structure
struct Snowflake { float x, y, size; };
std::vector<Snowflake> snowflakes;

// Draw rectangle utility
void drawRectangle(float x, float y, float w, float h, float r, float g, float b){
    glColor3f(r,g,b);
    glBegin(GL_QUADS);
        glVertex2f(x,y);
        glVertex2f(x+w,y);
        glVertex2f(x+w,y+h);
        glVertex2f(x,y+h);
    glEnd();
}

// Draw triangle utility
void drawTriangle(float x1,float y1,float x2,float y2,float x3,float y3,float r,float g,float b){
    glColor3f(r,g,b);
    glBegin(GL_TRIANGLES);
        glVertex2f(x1,y1);
        glVertex2f(x2,y2);
        glVertex2f(x3,y3);
    glEnd();
}

// Draw filled circle
void drawCircle(float cx,float cy,float r,int segments,float cr,float cg,float cb){
    glColor3f(cr,cg,cb);
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx,cy);
        for(int i=0;i<=segments;i++){
            float ang=i*2*M_PI/segments;
            glVertex2f(cx+cos(ang)*r, cy+sin(ang)*r);
        }
    glEnd();
}

// Draw sun with parabolic movement
void drawSun(float x) {
    float h = WINDOW_WIDTH/2.0f;
    float k = 520;
    float a = -0.0015f;
    float y = a*(x - h)*(x - h) + k;

    // Sun circle
    drawCircle(x, y, 50, 60, 1.0f, 0.9f, 0.0f);

    // Sun rays
    for (int i = 0; i < 12; i++) {
        float ang = i * (2 * M_PI / 12);
        float x1 = x + cos(ang) * 60;
        float y1 = y + sin(ang) * 60;
        float x2 = x + cos(ang + 0.2f) * 75;
        float y2 = y + sin(ang + 0.2f) * 75;
        float x3 = x + cos(ang - 0.2f) * 75;
        float y3 = y + sin(ang - 0.2f) * 75;
        drawTriangle(x1, y1, x2, y2, x3, y3, 1.0f, 0.8f, 0.0f);
    }
}

// Draw a cloud
void drawCloud(float x, float y) {
    drawCircle(x, y, 30, 20, 1,1,1);
    drawCircle(x+25, y+10, 25, 20, 1,1,1);
    drawCircle(x-25, y+10, 25, 20, 1,1,1);
    drawCircle(x, y+15, 28, 20, 1,1,1);
}

// Draw flower
void drawFlower(float x,float y){
    float petalR=10;
    drawCircle(x, y+petalR, petalR, 20, 1,0.6f,0.8f);
    drawCircle(x, y-petalR, petalR, 20, 1,0.6f,0.8f);
    drawCircle(x+petalR, y, petalR, 20, 1,0.6f,0.8f);
    drawCircle(x-petalR, y, petalR, 20, 1,0.6f,0.8f);
    drawCircle(x, y, petalR*0.7f, 20, 1,1,0.2f);
}

// Draw a tree
void drawTree(float x,float y,bool flowers,bool autumn,bool winter){
    drawRectangle(x, y, 35, 120, 0.55f,0.27f,0.07f);
    if(winter){
        drawCircle(x+18, y+150, 60, 30, 0.9f,0.9f,0.9f);
        return;
    }
    if(autumn){
        drawCircle(x+18, y+150, 60, 30, 0.95f,0.65f,0.2f);
        return;
    }
    drawCircle(x+18, y+150, 60, 30, 0.0f,0.7f,0.1f);
    if(flowers){
        drawFlower(x+5, y+170);
        drawFlower(x+40, y+140);
        drawFlower(x+10, y+120);
    }
}

// Draw fire with flicker
void drawFire(float x, float y, float offset){
    drawTriangle(x-20, y, x+20, y, x, y+40 + offset, 1.0f,0.3f,0.0f);
    drawTriangle(x-15, y+20, x+15, y+20, x, y+55 + offset, 1.0f,0.6f,0.0f);
    drawTriangle(x-10, y+35, x+10, y+35, x, y+65 + offset, 1.0f,0.9f,0.0f);
}

// Draw a Bowl
void drawBowl(float cx, float cy, float radius, float r, float g, float b) {
    // Bowl base (upward half-circle)
    glColor3f(r, g, b);
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx, cy); // center top
        for(int i=0; i<=20; i++){
            float ang = M_PI * i / 20.0f;
            glVertex2f(cx + cos(ang)*radius, cy - sin(ang)*radius);
        }
    glEnd();
}

// Draw the interior house scene
void drawInteriorScene() {
    // Walls and floor
    drawRectangle(0, 80, WINDOW_WIDTH, WINDOW_HEIGHT-80, 0.98f, 0.76f, 0.29f);
    drawRectangle(0, 0, WINDOW_WIDTH, 80, 0.55f, 0.27f, 0.07f);

    // Table
    drawRectangle(260, 140, 400, 20, 0.55f,0.27f,0.07f); // top
    drawRectangle(280, 80, 20, 60, 0.55f,0.27f,0.07f);   // left leg
    drawRectangle(620, 80, 20, 60, 0.55f,0.27f,0.07f);   // right leg

    // Bowls on table
    drawBowl(320, 180, 20, 1.0f, 0.0f, 0.0f); // red
    drawBowl(450, 180, 20, 0.0f, 0.0f, 1.0f); // blue
    drawBowl(580, 180, 20, 0.0f, 1.0f, 0.0f); // green
}

// Display callback
void display(){
    glClear(GL_COLOR_BUFFER_BIT);

    if(interiorMode){
        drawInteriorScene();
    } else {
        // Draw outdoor scene depending on season
        if(currentSeason == 0){ drawRectangle(0,120,WINDOW_WIDTH,480,0.46f,0.92f,0.96f); drawSun(sunX); drawRectangle(0,0,WINDOW_WIDTH,120,0.0f,0.75f,0.29f); }
        else if(currentSeason == 1){ drawRectangle(0,120,WINDOW_WIDTH,480,0.46f,0.92f,0.96f); drawSun(sunX); drawRectangle(0,0,WINDOW_WIDTH,120,0.0f,0.75f,0.29f); }
        else if(currentSeason == 2){ drawRectangle(0,120,WINDOW_WIDTH,480,0.45f,0.65f,1.0f); drawSun(sunX); drawRectangle(0,0,WINDOW_WIDTH,120,0.98f,0.75f,0.35f); }
        else if(currentSeason == 3){ drawRectangle(0,120,WINDOW_WIDTH,480,0.8f,0.9f,1.0f); drawSun(sunX); drawRectangle(0,0,WINDOW_WIDTH,120,1.0f,1.0f,1.0f); }

        // Clouds
        drawCloud(cloudX[0], 500);
        drawCloud(cloudX[1], 550);
        drawCloud(cloudX[2], 480);

        // House and fire
        drawRectangle(120,120,150,120,0.98f,0.76f,0.29f);
        drawTriangle(120,240,270,240,195,300,0.45f,0.17f,0.02f);
        drawRectangle(180,120,40,70,0.05f,0.05f,0.05f);

        if(currentSeason == 1){
            drawFire(150,250, fireOffset[0]);
            drawFire(230,252, fireOffset[1]);
        }

        // Trees
        drawTree(600,120, currentSeason==0, currentSeason==2, currentSeason==3);
        drawTree(700,120, currentSeason==0, currentSeason==2, currentSeason==3);
        drawTree(800,120, currentSeason==0, currentSeason==2, currentSeason==3);

        if(currentSeason == 1){
            drawFire(613,250, fireOffset[2]);
            drawFire(710,245, fireOffset[3]);
            drawFire(817,253, fireOffset[4]);
        }

        // Snow
        if(currentSeason == 3){
            for(auto &s : snowflakes){
                drawCircle(s.x, s.y, s.size, 10, 1,1,1);
            }
        }

        // Season label
        glColor3f(0,0,0);
        glRasterPos2f(420,30);
        const char *labels[] = {"Spring Time","Summer Time","Autumn","Winter"};
        const char *text = labels[currentSeason];
        for(int i=0;text[i];i++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,text[i]);
    }

    glutSwapBuffers();
}

// Keyboard input
void keyboard(unsigned char key,int x,int y){
    if(key=='n'){
        currentSeason = (currentSeason + 1) % 4;
        glutPostRedisplay();
    }
    if(key=='i'){ // toggle interior mode
        interiorMode = !interiorMode;
        glutPostRedisplay();
    }
}

// Update loop for animation
void update(int value){
    if(!interiorMode){
        sunX += sunSpeed;
        if(sunX > WINDOW_WIDTH + 50) sunX = -50.0f;

        for(int i=0;i<3;i++){
            cloudX[i] += 0.5f + i*0.2f;
            if(cloudX[i] > WINDOW_WIDTH + 50) cloudX[i] = -50.0f;
        }

        for(int i=0;i<5;i++){
            fireOffset[i] = rand()%10;
        }

        if(currentSeason == 3){
            for(auto &s : snowflakes){
                s.y -= 2;
                if(s.y < 0) s.y = WINDOW_HEIGHT;
            }
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

// OpenGL initialization
void initGL(){
    glClearColor(0.46f,0.92f,1.0f,1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,WINDOW_WIDTH,0,WINDOW_HEIGHT);
    glMatrixMode(GL_MODELVIEW);

    for(int i=0;i<100;i++){
        snowflakes.push_back({(float)(rand()%WINDOW_WIDTH), (float)(rand()%WINDOW_HEIGHT), 2 + rand()%3});
    }

    for(int i=0;i<3;i++){
        cloudX[i] = rand()%WINDOW_WIDTH;
    }
}

// Main function
int main(int argc,char**argv){
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH,WINDOW_HEIGHT);
    glutCreateWindow("Four Seasons Scene with Interior and Bowls");
    initGL();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, update, 0);
    glutMainLoop();
    return 0;
}
