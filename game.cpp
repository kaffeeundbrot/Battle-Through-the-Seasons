#include <GL/glut.h>
#include <GL/glu.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <cmath>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float BASKET_WIDTH = 80.0f;
const float BASKET_HEIGHT = 20.0f;
const float ITEM_SIZE = 30.0f;
const float BASKET_SPEED = 15.0f;
const float FALL_SPEED = 3.0f;

enum Season {
    SPRING = 0,
    SUMMER = 1,
    AUTUMN = 2,
    WINTER = 3
};

const std::vector<std::string> springGoodItems = {"Rice/Oats", "Milk Bottle", "Honey", "Apple"};
const std::vector<std::string> springBadItems = {"Crumpled Paper", "Spoiled Fruit", "Broken Furniture"};

const std::vector<std::string> summerGoodItems = {"Plastic Bottles", "Drink Cans", "Candy Wrappers", "Flip Flops"};
const std::vector<std::string> summerBadItems = {"Teddy Bear", "Flowers", "Kitchen Utensil", "Jams"};

const std::vector<std::string> autumnGoodItems = {"Pumpkins", "Jam Jars", "Corn Cobs", "Bread Loaf"};
const std::vector<std::string> autumnBadItems = {"Crumpled Paper", "Spoiled Fruit", "Broken Furniture"};

const std::vector<std::string> winterGoodItems = {"Hot Cocoa", "Cookies", "Gift Box", "Scarf"};
const std::vector<std::string> winterBadItems = {"Crumpled Paper", "Spoiled Fruit", "Broken Furniture"};

struct Item {
    float x, y;
    bool isGood;
    int seasonType;
    std::string name;
};

class Game {
private:
    float basketX;
    int score;
    int lives;
    Season currentSeason;
    bool isPaused;
    bool gameOver;
    std::vector<Item> items;
    int frameCount;
    bool leftKey, rightKey;

    std::string getRandomItemName(Season season, bool isGood) {
        const std::vector<std::string>* itemList;
        
        if (isGood) {
            switch (season) {
                case SPRING: itemList = &springGoodItems; break;
                case SUMMER: itemList = &summerGoodItems; break;
                case AUTUMN: itemList = &autumnGoodItems; break;
                case WINTER: itemList = &winterGoodItems; break;
            }
        } else {
            switch (season) {
                case SPRING: itemList = &springBadItems; break;
                case SUMMER: itemList = &summerBadItems; break;
                case AUTUMN: itemList = &autumnBadItems; break;
                case WINTER: itemList = &winterBadItems; break;
            }
        }
        
        return (*itemList)[rand() % itemList->size()];
    }

    void spawnItem() {
        Item item;
        item.x = rand() % (WINDOW_WIDTH - 40) + 20;
        item.y = WINDOW_HEIGHT + ITEM_SIZE;
        item.isGood = (rand() % 100) < 75;
        item.seasonType = currentSeason;
        item.name = getRandomItemName(currentSeason, item.isGood);
        items.push_back(item);
    }

    void updateSeason() {
        if (score >= 150) {
            currentSeason = WINTER;
        } else if (score >= 100) {
            currentSeason = AUTUMN;
        } else if (score >= 50) {
            currentSeason = SUMMER;
        } else {
            currentSeason = SPRING;
        }
    }

    bool checkCollision(const Item& item) {
        float basketLeft = basketX - BASKET_WIDTH / 2;
        float basketRight = basketX + BASKET_WIDTH / 2;
        float basketTop = 50;
        float basketBottom = 50 + BASKET_HEIGHT;

        float itemLeft = item.x - ITEM_SIZE / 2;
        float itemRight = item.x + ITEM_SIZE / 2;
        float itemBottom = item.y;
        float itemTop = item.y - ITEM_SIZE;

        return (itemLeft < basketRight && itemRight > basketLeft &&
                itemTop < basketBottom && itemBottom > basketTop);
    }

    void drawText(float x, float y, const std::string& text) {
        glRasterPos2f(x, y);
        for (char c : text) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }
    }

    void drawBasket() {
        glColor3f(0.6f, 0.4f, 0.2f);
        glBegin(GL_QUADS);
        glVertex2f(basketX - BASKET_WIDTH/2, 50);
        glVertex2f(basketX + BASKET_WIDTH/2, 50);
        glVertex2f(basketX + BASKET_WIDTH/2 - 5, 50 + BASKET_HEIGHT);
        glVertex2f(basketX - BASKET_WIDTH/2 + 5, 50 + BASKET_HEIGHT);
        glEnd();

        glColor3f(0.5f, 0.3f, 0.1f);
        glBegin(GL_LINE_LOOP);
        glVertex2f(basketX - BASKET_WIDTH/2, 50);
        glVertex2f(basketX + BASKET_WIDTH/2, 50);
        glVertex2f(basketX + BASKET_WIDTH/2 - 5, 50 + BASKET_HEIGHT);
        glVertex2f(basketX - BASKET_WIDTH/2 + 5, 50 + BASKET_HEIGHT);
        glEnd();
    }

    void drawSmallText(float x, float y, const std::string& text) {
        glRasterPos2f(x, y);
        for (char c : text) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, c);
        }
    }

    void drawItem(const Item& item) {
        if (item.isGood) {
            switch (item.seasonType) {
                case SPRING:
                    glColor3f(1.0f, 0.7f, 0.8f);
                    break;
                case SUMMER:
                    glColor3f(1.0f, 0.9f, 0.0f);
                    break;
                case AUTUMN:
                    glColor3f(1.0f, 0.5f, 0.0f);
                    break;
                case WINTER:
                    glColor3f(0.9f, 0.9f, 1.0f);
                    break;
            }
        } else {
            glColor3f(0.3f, 0.3f, 0.3f);
        }

        if (item.isGood) {
            glBegin(GL_POLYGON);
            for (int i = 0; i < 8; i++) {
                float angle = i * 3.14159f * 2.0f / 8.0f;
                float radius = (i % 2 == 0) ? ITEM_SIZE/2 : ITEM_SIZE/3;
                glVertex2f(item.x + cos(angle) * radius, 
                          item.y + sin(angle) * radius);
            }
            glEnd();
        } else {
            glBegin(GL_QUADS);
            glVertex2f(item.x - ITEM_SIZE/3, item.y - ITEM_SIZE/3);
            glVertex2f(item.x + ITEM_SIZE/3, item.y - ITEM_SIZE/3);
            glVertex2f(item.x + ITEM_SIZE/3, item.y + ITEM_SIZE/3);
            glVertex2f(item.x - ITEM_SIZE/3, item.y + ITEM_SIZE/3);
            glEnd();
        }

        glColor3f(0.0f, 0.0f, 0.0f);
        float textWidth = item.name.length() * 5.0f;
        drawSmallText(item.x - textWidth/2, item.y + ITEM_SIZE/2 + 5, item.name);
    }

    void drawBear(float x, float y, float size, const std::string& name) {
        glColor3f(0.6f, 0.4f, 0.2f);
        
        glBegin(GL_POLYGON);
        for (int i = 0; i < 20; i++) {
            float angle = i * 3.14159f * 2.0f / 20.0f;
            glVertex2f(x + cos(angle) * size, y + sin(angle) * size);
        }
        glEnd();
        
        glColor3f(0.7f, 0.5f, 0.3f);
        glBegin(GL_POLYGON);
        for (int i = 0; i < 15; i++) {
            float angle = i * 3.14159f * 2.0f / 15.0f;
            glVertex2f(x - size*0.5 + cos(angle) * size*0.3, 
                      y + size*0.6 + sin(angle) * size*0.3);
        }
        glEnd();
        
        glBegin(GL_POLYGON);
        for (int i = 0; i < 15; i++) {
            float angle = i * 3.14159f * 2.0f / 15.0f;
            glVertex2f(x + size*0.5 + cos(angle) * size*0.3, 
                      y + size*0.6 + sin(angle) * size*0.3);
        }
        glEnd();
        
        glColor3f(0.0f, 0.0f, 0.0f);
        float textWidth = name.length() * 4.0f;
        drawSmallText(x - textWidth/2, y - size - 10, name);
    }

    void drawBackground() {
        switch (currentSeason) {
            case SPRING:
                glColor3f(0.7f, 0.9f, 0.7f);
                break;
            case SUMMER:
                glColor3f(0.5f, 0.8f, 1.0f);
                break;
            case AUTUMN:
                glColor3f(0.9f, 0.7f, 0.5f);
                break;
            case WINTER:
                glColor3f(0.8f, 0.9f, 1.0f);
                break;
        }
        glBegin(GL_QUADS);
        glVertex2f(0, 0);
        glVertex2f(WINDOW_WIDTH, 0);
        glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT);
        glVertex2f(0, WINDOW_HEIGHT);
        glEnd();
        
        drawBear(100, 150, 25, "Baby Bear");
        drawBear(WINDOW_WIDTH/2, 120, 35, "Mama Bear");
        drawBear(WINDOW_WIDTH - 100, 130, 40, "Papa Bear");
    }

    std::string getSeasonName() {
        switch (currentSeason) {
            case SPRING: return "SPRING";
            case SUMMER: return "SUMMER";
            case AUTUMN: return "AUTUMN";
            case WINTER: return "WINTER";
            default: return "UNKNOWN";
        }
    }

public:
    Game() : basketX(WINDOW_WIDTH / 2.0f), score(0), lives(3), 
             currentSeason(SPRING), isPaused(false), gameOver(false),
             frameCount(0), leftKey(false), rightKey(false) {
        srand(time(nullptr));
    }

    void update() {
        if (isPaused || gameOver) return;

        if (leftKey && basketX > BASKET_WIDTH/2) {
            basketX -= BASKET_SPEED;
        }
        if (rightKey && basketX < WINDOW_WIDTH - BASKET_WIDTH/2) {
            basketX += BASKET_SPEED;
        }

        frameCount++;
        if (frameCount % 60 == 0) {
            spawnItem();
        }

        for (size_t i = 0; i < items.size(); i++) {
            items[i].y -= FALL_SPEED;

            if (checkCollision(items[i])) {
                if (items[i].isGood) {
                    score += 10;
                    updateSeason();
                } else {
                    lives--;
                    if (lives <= 0) {
                        gameOver = true;
                    }
                }
                items.erase(items.begin() + i);
                i--;
            } else if (items[i].y < -ITEM_SIZE) {
                items.erase(items.begin() + i);
                i--;
            }
        }
    }

    void render() {
        glClear(GL_COLOR_BUFFER_BIT);

        drawBackground();

        drawBasket();

        for (const auto& item : items) {
            drawItem(item);
        }

        glColor3f(0.0f, 0.0f, 0.0f);
        drawText(10, WINDOW_HEIGHT - 20, "Score: " + std::to_string(score));
        drawText(10, WINDOW_HEIGHT - 45, "Lives: " + std::to_string(lives));
        drawText(10, WINDOW_HEIGHT - 70, "Season: " + getSeasonName());

        if (isPaused && !gameOver) {
            glColor3f(0.0f, 0.0f, 0.0f);
            drawText(WINDOW_WIDTH/2 - 50, WINDOW_HEIGHT/2, "PAUSED");
            drawText(WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2 - 30, "Press SPACE to resume");
        }

        if (gameOver) {
            glColor3f(1.0f, 0.0f, 0.0f);
            drawText(WINDOW_WIDTH/2 - 70, WINDOW_HEIGHT/2, "GAME OVER!");
            drawText(WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT/2 - 30, "Final Score: " + std::to_string(score));
            drawText(WINDOW_WIDTH/2 - 120, WINDOW_HEIGHT/2 - 60, "Press SPACE to restart");
        }

        glutSwapBuffers();
    }

    void handleKeyPress(unsigned char key) {
        if (key == 27) {
            exit(0);
        } else if (key == ' ') {
            if (gameOver) {
                restart();
            } else {
                isPaused = !isPaused;
            }
        }
    }

    void handleSpecialKeyPress(int key, bool pressed) {
        if (key == GLUT_KEY_LEFT) {
            leftKey = pressed;
        } else if (key == GLUT_KEY_RIGHT) {
            rightKey = pressed;
        }
    }

    void restart() {
        basketX = WINDOW_WIDTH / 2.0f;
        score = 0;
        lives = 3;
        currentSeason = SPRING;
        isPaused = false;
        gameOver = false;
        items.clear();
        frameCount = 0;
        leftKey = false;
        rightKey = false;
    }
};

Game* game = nullptr;

void display() {
    if (game) {
        game->render();
    }
}

void update(int value) {
    if (game) {
        game->update();
    }
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void keyboard(unsigned char key, int x, int y) {
    if (game) {
        game->handleKeyPress(key);
    }
}

void specialKeyDown(int key, int x, int y) {
    if (game) {
        game->handleSpecialKeyPress(key, true);
    }
}

void specialKeyUp(int key, int x, int y) {
    if (game) {
        game->handleSpecialKeyPress(key, false);
    }
}

void initGL() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Season Catcher");

    initGL();

    game = new Game();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeyDown);
    glutSpecialUpFunc(specialKeyUp);
    glutTimerFunc(16, update, 0);

    glutMainLoop();

    delete game;
    return 0;
}
