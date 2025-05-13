#include "AppController.h"

int main() {
    int mapWidth = 1000;
    int mapHeight = 1000;
    float tubeLength = 2.0f;


    AppController app(mapWidth, mapHeight, tubeLength);
    app.run();
    return 0;
}



