#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <conio.h>
#include <windows.h>

#define WIDTH 80
#define HEIGHT 25
#define MAX_OBJECTS 50

typedef enum {
    OBJ_LINE,
    OBJ_RECTANGLE,
    OBJ_CIRCLE,
    OBJ_TRIANGLE,
    OBJ_NONE
} ObjectType;

typedef struct {
    ObjectType type;
    int x1, y1;
    int x2, y2;
    int x3, y3; // Used for triangle
    int radius; // Used for circle
    int active;
} GraphicObject;

char picture[HEIGHT][WIDTH];
GraphicObject objects[MAX_OBJECTS];
int objectCount = 0;

void clearPicture(void) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            picture[y][x] = '_';
        }
    }
}

void setPixel(int x, int y) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        picture[y][x] = '*';
    }
}

void drawLine(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int sx = x1 < x2 ? 1 : -1;
    int dy = -abs(y2 - y1);
    int sy = y1 < y2 ? 1 : -1;
    int err = dx + dy;

    while (1) {
        setPixel(x1, y1);
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            x1 += sx;
        }
        if (e2 <= dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void drawRectangle(int x1, int y1, int x2, int y2) {
    int left = x1 < x2 ? x1 : x2;
    int right = x1 < x2 ? x2 : x1;
    int top = y1 < y2 ? y1 : y2;
    int bottom = y1 < y2 ? y2 : y1;

    for (int x = left; x <= right; x++) {
        setPixel(x, top);
        setPixel(x, bottom);
    }
    for (int y = top; y <= bottom; y++) {
        setPixel(left, y);
        setPixel(right, y);
    }
}

void drawCircle(int cx, int cy, int radius) {
    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;

    while (y >= x) {
        setPixel(cx + x, cy + y);
        setPixel(cx - x, cy + y);
        setPixel(cx + x, cy - y);
        setPixel(cx - x, cy - y);
        setPixel(cx + y, cy + x);
        setPixel(cx - y, cy + x);
        setPixel(cx + y, cy - x);
        setPixel(cx - y, cy - x);

        x++;
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }
    }
}

void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3) {
    drawLine(x1, y1, x2, y2);
    drawLine(x2, y2, x3, y3);
    drawLine(x3, y3, x1, y1);
}

void clearScreen(void) {
    system("cls");
}

void setConsoleCursor(SHORT x, SHORT y) {
    COORD coord = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void writeAt(SHORT x, SHORT y, const char *str) {
    setConsoleCursor(x, y);
    printf("%s", str);
    fflush(stdout);
}

void waitForKey(void) {
    setConsoleCursor(0, HEIGHT + 2);
    printf("Press any key to continue...");
    fflush(stdout);
    _getch();
}

int runMenu(void) {
    const char *options[] = {
        "Add object",
        "Delete object",
        "Display picture",
        "List objects",
        "Exit"
    };
    int choice = 0;
    int count = sizeof(options) / sizeof(options[0]);

    while (1) {
        clearScreen();
        writeAt(0, 0, "2D Graphics Editor (Windows Console)");
        writeAt(0, 1, "Use number keys (1-5) or arrow keys to select, then press Enter.");

        for (int i = 0; i < count; i++) {
            setConsoleCursor(2, 3 + i);
            if (i == choice) {
                printf(" >> %d. %s", i + 1, options[i]);
            } else {
                printf("    %d. %s", i + 1, options[i]);
            }
            fflush(stdout);
        }

        int ch = _getch();
        if (ch == 224) {
            int arrow = _getch();
            if (arrow == 72) {
                choice = (choice + count - 1) % count;
            } else if (arrow == 80) {
                choice = (choice + 1) % count;
            }
        } else if (ch == '\r') {
            return choice + 1;
        } else if (ch >= '1' && ch <= '0' + count) {
            return ch - '0';
        }
    }
}

void displayPicture(void) {
    clearPicture();

    for (int i = 0; i < objectCount; i++) {
        GraphicObject *obj = &objects[i];
        if (!obj->active) continue;

        switch (obj->type) {
            case OBJ_LINE:
                drawLine(obj->x1, obj->y1, obj->x2, obj->y2);
                break;
            case OBJ_RECTANGLE:
                drawRectangle(obj->x1, obj->y1, obj->x2, obj->y2);
                break;
            case OBJ_CIRCLE:
                drawCircle(obj->x1, obj->y1, obj->radius);
                break;
            case OBJ_TRIANGLE:
                drawTriangle(obj->x1, obj->y1, obj->x2, obj->y2, obj->x3, obj->y3);
                break;
            default:
                break;
        }
    }

    clearScreen();
    for (int y = 0; y < HEIGHT; y++) {
        setConsoleCursor(0, y);
        for (int x = 0; x < WIDTH; x++) {
            putchar(picture[y][x]);
        }
        putchar('\n');
    }
    waitForKey();
}

void printObjectPrompt(const char *prompt) {
    clearScreen();
    writeAt(0, 0, prompt);
}

void addObject(void) {
    if (objectCount >= MAX_OBJECTS) {
        clearScreen();
        writeAt(0, 0, "Cannot add more objects. Max objects reached.");
        waitForKey();
        return;
    }

    clearScreen();
    writeAt(0, 0, "Choose object type:");
    writeAt(2, 2, "1. Line");
    writeAt(2, 3, "2. Rectangle");
    writeAt(2, 4, "3. Circle");
    writeAt(2, 5, "4. Triangle");
    setConsoleCursor(0, 7);
    printf("Enter choice: ");
    fflush(stdout);
    int choice;
    scanf("%d", &choice);

    GraphicObject obj = {0};
    obj.active = 1;

    switch (choice) {
        case 1:
            obj.type = OBJ_LINE;
            printObjectPrompt("Enter x1 y1 x2 y2:");
            setConsoleCursor(0, 2);
            scanf("%d %d %d %d", &obj.x1, &obj.y1, &obj.x2, &obj.y2);
            break;
        case 2:
            obj.type = OBJ_RECTANGLE;
            printObjectPrompt("Enter top-left x y and bottom-right x y:");
            setConsoleCursor(0, 2);
            scanf("%d %d %d %d", &obj.x1, &obj.y1, &obj.x2, &obj.y2);
            break;
        case 3:
            obj.type = OBJ_CIRCLE;
            printObjectPrompt("Enter center x y and radius:");
            setConsoleCursor(0, 2);
            scanf("%d %d %d", &obj.x1, &obj.y1, &obj.radius);
            break;
        case 4:
            obj.type = OBJ_TRIANGLE;
            printObjectPrompt("Enter x1 y1 x2 y2 x3 y3:");
            setConsoleCursor(0, 2);
            scanf("%d %d %d %d %d %d", &obj.x1, &obj.y1, &obj.x2, &obj.y2, &obj.x3, &obj.y3);
            break;
        default:
            clearScreen();
            writeAt(0, 0, "Invalid choice.");
            waitForKey();
            return;
    }

    objects[objectCount++] = obj;
    clearScreen();
    setConsoleCursor(0, 0);
    printf("Object added as ID %d.", objectCount);
    fflush(stdout);
    waitForKey();
}

void deleteObject(void) {
    if (objectCount == 0) {
        clearScreen();
        writeAt(0, 0, "No objects to delete.");
        waitForKey();
        return;
    }

    clearScreen();
    setConsoleCursor(0, 0);
    printf("Enter object ID to delete (1..%d): ", objectCount);
    fflush(stdout);
    int id;
    scanf("%d", &id);

    if (id < 1 || id > objectCount) {
        clearScreen();
        writeAt(0, 0, "Invalid object ID.");
        waitForKey();
        return;
    }

    for (int i = id - 1; i < objectCount - 1; i++) {
        objects[i] = objects[i + 1];
    }
    objectCount--;
    clearScreen();
    setConsoleCursor(0, 0);
    printf("Object %d deleted.", id);
    fflush(stdout);
    waitForKey();
}

void listObjects(void) {
    clearScreen();
    if (objectCount == 0) {
        writeAt(0, 0, "No objects in the picture.");
        waitForKey();
        return;
    }

    writeAt(0, 0, "Object List:");
    int line = 2;
    for (int i = 0; i < objectCount && line < 24; i++) {
        GraphicObject *obj = &objects[i];
        setConsoleCursor(0, line);
        printf("%2d: ", i + 1);
        switch (obj->type) {
            case OBJ_LINE:
                printf("Line (%d,%d)-(%d,%d)", obj->x1, obj->y1, obj->x2, obj->y2);
                break;
            case OBJ_RECTANGLE:
                printf("Rectangle (%d,%d)-(%d,%d)", obj->x1, obj->y1, obj->x2, obj->y2);
                break;
            case OBJ_CIRCLE:
                printf("Circle center=(%d,%d) radius=%d", obj->x1, obj->y1, obj->radius);
                break;
            case OBJ_TRIANGLE:
                printf("Triangle (%d,%d)-(%d,%d)-(%d,%d)", obj->x1, obj->y1, obj->x2, obj->y2, obj->x3, obj->y3);
                break;
            default:
                printf("Unknown object");
                break;
        }
        fflush(stdout);
        line++;
    }
    waitForKey();
}

int main(void) {
    memset(objects, 0, sizeof(objects));
    clearPicture();

    while (1) {
        int choice = runMenu();

        switch (choice) {
            case 1:
                addObject();
                break;
            case 2:
                deleteObject();
                break;
            case 3:
                displayPicture();
                break;
            case 4:
                listObjects();
                break;
            case 5:
                clearScreen();
                writeAt(0, 0, "Exiting graphics editor. Goodbye!");
                return 0;
            default:
                break;
        }
    }

    return 0;
}
