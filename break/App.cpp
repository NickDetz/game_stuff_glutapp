#include "App.h"


static App* singleton;

void app_timer(int value){
    if (singleton->game_over){
        singleton->gameOver->advance();
    }
    
    if (singleton->moving){
        singleton->ball->jump();
        float bx = singleton->ball->x + singleton->ball->w/2;
        float by = singleton->ball->y - singleton->ball->h + 0.1;
        if (singleton->platform->contains(bx, by)){
            singleton->ball->rising = true;
            singleton->ball->yinc +=0.001;
            singleton->ball->xinc = singleton->ball->yinc;
            if (singleton->ball->yinc > 0.10){
                singleton->ball->yinc = 0.10;
            }
        }
        
        
            if(singleton->layout.contains(bx,by)) {
                singleton->ball->rising = !singleton->ball->rising;
                singleton->ball->increaseScore(singleton->layout.getfile());
                if(singleton->layout.getfile() == "/Users/nicholasdeters/Documents/CSE165Labs/break/images/silver.png") {
                    
                    int random = rand() % 10;
                    if(random == 1 || random == 2 || random == 3)
                        singleton->platform->w += 0.2;
                    else if(random == 4 || random == 5 || random == 6)
                        singleton->platform->w -= 0.2;
                    else if(random == 7) {
                        if(singleton->ball->w < 0.25) {
                            singleton->ball->w += 0.025;
                            singleton->ball->h += 0.025;
                        }
                    } else if(random == 8) {
                        if(singleton->ball->w > 0.06) {
                            singleton->ball->w -= 0.1;
                            singleton->ball->h -= 0.1;
                        }
                    }
                }
                
            
        }
        
        if (singleton->ball->y - singleton->ball->h < -0.99){
            singleton->moving = false;
            singleton->game_over = true;
            singleton->ball->setHighScore(singleton->ball->getScore());
            singleton->gameOver->setHighScore(singleton->ball->getHighScore());
            singleton->gameOver->animate();
        }
    }
    if (singleton->up){
        singleton->platform->moveUp(0.05);
    }
    if (singleton->down){
        singleton->platform->moveDown(0.05);
    }
    if (singleton->left){
        singleton->platform->moveLeft(0.05);
    }
    if (singleton->right){
        singleton->platform->moveRight(0.05);
    }
    
    if (singleton->game_over){
        singleton->redraw();
        glutTimerFunc(100, app_timer, value);
    }
    else{
        if (singleton->up || singleton->down || singleton->left || singleton->right || singleton->moving && !singleton->game_over){
            singleton->redraw();
            glutTimerFunc(16, app_timer, value);
        }
    }
    
    
}

App::App(const char* label, int x, int y, int w, int h): GlutApp(label, x, y, w, h){
    // Initialize state variables
    
    singleton = this;
    mx = 0.0;
    my = 0.0;
    
    ball = new TexRect("/Users/nicholasdeters/Documents/CSE165Labs/break/images/spiked_ball.png", 0, 0, 0.09, 0.09);

    platform = new TexRect("/Users/nicholasdeters/Documents/CSE165Labs/break/images/board.png", 0, -0.7, 0.45, 0.15);
    
    gameOver = new AnimatedRect("/Users/nicholasdeters/Documents/CSE165Labs/break/images/game_over.png", 7, 1, -1.0, 0.8, 2, 1.2);
    
    layout.init();
    
    
    
    
    up = down = left = right = false;
    
    moving = true;
    game_over = false;
    
    app_timer(1);

}

void App::specialKeyPress(int key){
    if (!game_over){
        if (key == 100){
            left = true;
        }
        if (key == 101){
            //up = true;
        }
        if (key == 102){
            right = true;
        }
        if (key == 103){
            //down = true;
        }
    }
}

void App::specialKeyUp(int key){
    if (key == 100) {
        left = false;
    }
    if (key == 101) {
        up = false;
    }
    if (key == 102) {
        right = false;
    }
    if (key == 103) {
        down = false;
    }
}

void App::draw() {

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Set background color to black
    glClearColor(0.0, 0.0, 0.0, 1.0);
    
    // Set up the transformations stack
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    layout.draw();
    platform->draw();
    ball->draw();
    gameOver->draw();
    
    
    string message = "Score: " + std::to_string(ball->getScore());
    glColor3f(1.0, 1.0, 1.0);
    glRasterPos2f(0.6, -0.95);
    int len = (int)message.size();
    
    for (int i = 0; i < len; i++)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, message[i]);
    
    
    string message2 = "High Score: " + std::to_string(ball->getHighScore());
    glColor3f(1.0, 1.0, 1.0);
    glRasterPos2f(-0.9, -0.95);
    int len2 = (int)message2.size();
    
    for (int i = 0; i < len2; i++)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, message2[i]);
    
    
    // We have been drawing everything to the back buffer
    // Swap the buffers to see the result of what we drew
    glFlush();
    glutSwapBuffers();
}

void App::mouseDown(float x, float y){
    // Update app state
    mx = x;
    my = y;

}

void App::mouseDrag(float x, float y){
    // Update app state
    mx = x;
    my = y;

}

void App::idle(){

}

void App::keyPress(unsigned char key) {
    if (key == 27){
        // Exit the app when Esc key is pressed
        
        delete ball;
        delete platform;
        delete gameOver;
//        if (key == 'p'){
//            stop();
//        }
//        else if (key == 'r'){
//            start();
//        }
        
        delete this;
        
        exit(0);
    }
    
    if (key == ' '){
        ball->x = 0;
        ball->y = 0;
        ball->w = 0.09;
        ball->h = 0.09;
        ball->yinc = 0.01;
        ball->xinc = 0.01;
        platform->w = 0.45;
        ball->rising = false;
        ball->score = 0;
        game_over = false;
        gameOver->stop();
        moving = true;
        layout.init();
        layout.draw();
    }
}
