#include "App.h"

int main(int argc, char** argv) {
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
    
   
    App* myApp = new App("MyApp", 50, 50, 800, 600);

    myApp->run();
}
