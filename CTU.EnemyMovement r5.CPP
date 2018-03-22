#include<SFML/Graphics.hpp>
#include<iostream>
#include<math.h>
#include<algorithm>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include <windows.h>
#include <conio.h>
#include <fstream> //file I/O

#include <SFML/Audio.hpp>

using namespace std; using namespace sf;

int main(){
    int x,y,x1,y1;
    int distance;
    int fc=0;
    Mouse mouse;

    bool mousereleased=true;
    float shotslope;
    int j (0); int k (0);
    int numenemy (12);
    float angle;

// SOUNDS START SECTION

SoundBuffer coward; SoundBuffer shot1; SoundBuffer explosion1; SoundBuffer start1;
coward.loadFromFile("F:/sounds/runcowar.wav");
shot1.loadFromFile("F:/sounds/shot1.wav");
explosion1.loadFromFile("F:/sounds/explosion1.wav");
start1.loadFromFile("F:/sounds/defstart.wav");
Sound sound;
sound.setBuffer(start1);
sound.play();


vector <Sprite> enemy1; vector <Sprite> backgrounds;

Texture spritesheet; spritesheet.loadFromFile("F:/animation.png");

// set up main character
    CircleShape circle(30);  circle.setFillColor(Color::Blue);
    circle.setOrigin(30,30); circle.setPosition(Vector2f(750,400));


// enemy1 set-up
for (int i=0;i<=numenemy-1; i++){
    Sprite sprite;
    sprite.setTexture(spritesheet);
    sprite.setTextureRect(IntRect(0, 600, 100, 100));
    sprite.setScale(Vector2f(.5,.5));
    sprite.setPosition(rand()%1500, rand()%800);
    sprite.setOrigin(50,50);
    enemy1.push_back(sprite);    }

// backgrounds set-up
    Sprite sprite;
    sprite.setTexture(spritesheet);
    sprite.setTextureRect(IntRect(100, 500, 100, 100));
    sprite.setScale(Vector2f(3,3));
    sprite.setPosition(300, 400);
    sprite.setOrigin(50,50);
    backgrounds.push_back(sprite);

//shots set-up
    vector <Vector2f> sdirection;   //shot direction vector - keeps track of the direction of each shot
    Vector2f sxydir;                //x and y speed for each shot
    vector <CircleShape> shots;     //shots vector
    CircleShape myshots;            //shape and color of shots
    myshots.setRadius(5);
    myshots.setFillColor(Color(Color::Red));
    myshots.setOrigin(5,5);

// dots set-up
        Vertex dots[3] =
    { (Vector2f(100,100)),
      (Vector2f(1400,700)),
      (Vector2f(1400,100))};

    RenderWindow window(sf::VideoMode(1500,800), "SFML window");
    window.setFramerateLimit(60);

    while (window.isOpen())
    {  sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed) {window.close();}
            if (event.type==Event::MouseButtonReleased and (event.mouseButton.button==Mouse::Left or event.mouseButton.button==Mouse::Right)) mousereleased=true;
            if (Keyboard::isKeyPressed(Keyboard::Escape)){window.close();}
        }
        window.clear();

        if(Keyboard::isKeyPressed(Keyboard::W) && circle.getPosition().y > 30){
            circle.setPosition(circle.getPosition().x,circle.getPosition().y-3);}
        if(Keyboard::isKeyPressed(Keyboard::A) && circle.getPosition().x > 30){
            circle.setPosition(circle.getPosition().x-3,circle.getPosition().y);}
        if(Keyboard::isKeyPressed(Keyboard::S) && circle.getPosition().y < 770){
            circle.setPosition(circle.getPosition().x,circle.getPosition().y+3);}
        if(Keyboard::isKeyPressed(Keyboard::D) && circle.getPosition().x < 1470){
            circle.setPosition(circle.getPosition().x+3,circle.getPosition().y);}

// mouse click reaction
        if (Mouse::isButtonPressed(Mouse::Left) and mousereleased==true) {
    //calculate position
            mousereleased=false;  // only one shot per mouse click
    //calculate slope of shot between mouse click and main character
            shotslope=((circle.getPosition().y-mouse.getPosition(window).y)/(circle.getPosition().x-mouse.getPosition(window).x));
    //determine if you are shooting left (negative) or right (positive) relative to main character.  5 is increment of shot movement - bigger number is faster

            if (((circle.getPosition().x-mouse.getPosition(window).x)<60) and ((circle.getPosition().x-mouse.getPosition(window).x)>-60)) sxydir.x=(mouse.getPosition(window).x-circle.getPosition().x)/20;
            else if (circle.getPosition().x<mouse.getPosition(window).x) sxydir.x=5; else sxydir.x=-5;
            sxydir.y=shotslope*sxydir.x;  //calculate y based on delta x
            if (circle.getPosition().x==mouse.getPosition(window).x) sxydir.y=5;  //fixes if slope is undefined because shot and hero or on same x
            sdirection.push_back(sxydir);  // save direction in vector
            myshots.setPosition(circle.getPosition().x, circle.getPosition().y);
    //save shot shape in vector.  Note vectors sdirection (shot direction) and shots are parallel so shots[1] is the position of shot two (0 is shot 1) and sdirection[1] is the direction
            shots.push_back(myshots);
    //shot sound effect
    sound.setBuffer(shot1);
    sound.play();
            }

//move shots
      if (shots.size()>0) for (int i=0; i<=shots.size()-1; i++) {shots[i].setPosition((shots[i].getPosition().x+sdirection[i].x),(shots[i].getPosition().y+sdirection[i].y));}

//remove shots once they travel off the edge of the screen
        j=0;
        while (shots.size()>j and j>=0){
        if  (shots[j].getPosition().x>=1500 or shots[j].getPosition().x<=0 or shots[j].getPosition().y>=800 or shots[j].getPosition().y<=0 )
        {shots.erase(shots.begin()+j); sdirection.erase(sdirection.begin()+j); j--;}
        j++;}

// henry's dot flocking routine

        for(int c=0;c<3;c++){
            x = dots[c].position.x;
            y = dots[c].position.y;
            x1 = circle.getPosition().x;
            y1 = circle.getPosition().y;

            //distance = sqrt(pow(x-x1,2)+pow(y1-y,2));

            if(x-x1>0){dots[c].position = Vector2f(x-1,y);}
            else if(x-x1<0){ dots[c].position = Vector2f(x+1,y);}
            x = dots[c].position.x;
            y = dots[c].position.y;
            x1 = circle.getPosition().x;
            y1 = circle.getPosition().y;
            if(y-y1>0){dots[c].position = Vector2f(x,y-1);}
            else if(y-y1<0){dots[c].position = Vector2f(x,y+1);}}

 // move enemy based on henry's flocking algorithm

        for(int c=0;c<1;c++){

            angle = fabs(atan((enemy1[c].getPosition().y-circle.getPosition().y)/(enemy1[c].getPosition().x-circle.getPosition().x)));

            if(enemy1[c].getPosition().x-circle.getPosition().x>0){
                enemy1[c].setPosition (Vector2f(enemy1[c].getPosition().x-2*cos(angle),enemy1[c].getPosition().y));
            }
            else if(enemy1[c].getPosition().x-circle.getPosition().x<0){
                enemy1[c].setPosition (Vector2f(enemy1[c].getPosition().x+2*cos(angle),enemy1[c].getPosition().y));
            }
            if(enemy1[c].getPosition().y-circle.getPosition().y>0){
                enemy1[c].setPosition  (Vector2f(enemy1[c].getPosition().x,enemy1[c].getPosition().y-2*sin(angle)));
            }
            else if(enemy1[c].getPosition().y-circle.getPosition().y<0){
                enemy1[c].setPosition  (Vector2f(enemy1[c].getPosition().x,enemy1[c].getPosition().y+2*sin(angle)));
            }

        }




// detect shot hit on enemy 1

        k=0;
        while (enemy1.size()>0 and k<=enemy1.size()-1){ // check each enemy
            j=0;
            while (shots.size()>0 and j<=shots.size()-1) {  // against each shot
            if (enemy1[k].getPosition().x>=shots[j].getPosition().x-25 and enemy1[k].getPosition().x<=shots[j].getPosition().x+25
                and enemy1[k].getPosition().y>=shots[j].getPosition().y-25 and enemy1[k].getPosition().y<=shots[j].getPosition().y+25)
                {   //if a shot has hit an enemy
                    cout<<"enemy1 shot\n";
                //remove enemy
                    enemy1.erase(enemy1.begin()+k);
                    k--; //check same position again next loop since vector size decreased, next element is now same # ie 4th is now 3rd when 3rd is deleted
                // play sound
                sound.setBuffer(explosion1); sound.play();

                //remove shot enemy is hit/killed
                    shots.erase(shots.begin()+j);
                    sdirection.erase(sdirection.begin()+j);
                    j--; //when vector size decreased, next element is now same # ie 4th is now 3rd when 3rd is deleted
                }
                j++;}
            k++;}

// detect shot hit on enemy dot
      //  for(int c=0;c<3;c++){
      //  j=0;
      //  while (shots.size()>0 and j<=shots.size()-1) {
      //  if (dots[c].position.x>=shots[j].getPosition().x-5 and dots[c].position.x<=shots[j].getPosition().x+5
      //      and dots[c].position.y>=shots[j].getPosition().y-5 and dots[c].position.y<=shots[j].getPosition().y+5)
       //     {   cout<<"enemy dot shot\n";
            //move enemy to a new position
        //        dots[c].position = Vector2f(rand()%1500, rand()%800); //alternative delete enemy from vertex like shots below
            //remove shot and shot direction after enemy is hit/killed
        //        shots.erase(shots.begin()+j);
        //        sdirection.erase(sdirection.begin()+j);
        //        j--; //when vector size decreased, next element is now same # ie 4th is now 3rd when 3rd is deleted
     //       }
      //      j++;}}

// draw screen objects
for (int i=0; i<backgrounds.size(); i++) {window.draw(backgrounds[i]);}  //draw backgrounds
//window.draw(dots,3,Points); // draw dots
window.draw(circle);  // draw main character
for (int i=0; i<shots.size(); i++) {window.draw(shots[i]); } //draw shots
for (int i=0; i<enemy1.size(); i++) {window.draw(enemy1[i]);}  //draw enemies

window.display();
    }
}


