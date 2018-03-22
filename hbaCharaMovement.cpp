#include<SFML/Graphics.hpp>
#include<iostream>
#include<math.h>
#include<algorithm>
#include<time.h>
#include<stdlib.h>

using namespace std;
using namespace sf;

int main(){

    CircleShape circle(30);
    circle.setFillColor(Color::Blue);
    circle.setOrigin(30,30);
    circle.setPosition(Vector2f(750,400));

    float angle;

    Mouse mouse;

    vector<Vertex>dots;

    srand(time(NULL));
    for(int i=0; i < 10; i++){
        dots.push_back(Vector2f(rand()%1500,rand()%800));

    }

    RenderWindow window(sf::VideoMode(1500,800), "SFML window");
    window.setFramerateLimit(60);

    while (window.isOpen()){

        sf::Event event;
        while (window.pollEvent(event))
        {

            if (event.type == Event::Closed) {
                window.close();
            }
            if (Keyboard::isKeyPressed(Keyboard::Escape)){
                window.close();
            }
        }

        window.clear();

        if(Keyboard::isKeyPressed(Keyboard::W) && circle.getPosition().y > 30){
            circle.setPosition(circle.getPosition().x,circle.getPosition().y-3);
        }
        if(Keyboard::isKeyPressed(Keyboard::A) && circle.getPosition().x > 30){
            circle.setPosition(circle.getPosition().x-3,circle.getPosition().y);
        }
        if(Keyboard::isKeyPressed(Keyboard::S) && circle.getPosition().y < 770){
            circle.setPosition(circle.getPosition().x,circle.getPosition().y+3);
        }
        if(Keyboard::isKeyPressed(Keyboard::D) && circle.getPosition().x < 1470){
            circle.setPosition(circle.getPosition().x+3,circle.getPosition().y);
        }

        for(int c=0;c<10;c++){

            angle = fabs(atan((dots[c].position.y-circle.getPosition().y)/(dots[c].position.x-circle.getPosition().x)));

            if(dots[c].position.x-circle.getPosition().x>0){
                dots[c].position = Vector2f(dots[c].position.x-2*cos(angle),dots[c].position.y);
            }
            else if(dots[c].position.x-circle.getPosition().x<0){
                dots[c].position = Vector2f(dots[c].position.x+2*cos(angle),dots[c].position.y);
            }
            if(dots[c].position.y-circle.getPosition().y>0){
                dots[c].position = Vector2f(dots[c].position.x,dots[c].position.y-2*sin(angle));
            }
            else if(dots[c].position.y-circle.getPosition().y<0){
                dots[c].position = Vector2f(dots[c].position.x,dots[c].position.y+2*sin(angle));
            }

        }

        window.draw(&dots[0],dots.size(),Points);
        window.draw(circle);

        window.display();
    }
}
