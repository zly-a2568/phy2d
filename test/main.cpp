#include "../include/Object/Circle.hpp"
#include "../include/Object/Line.hpp"
#include "../include/World/World.hpp"
#include "../include/Collision/Collision.hpp"
#include "../include/Object/Box.hpp"

#include <iostream>
#include <chrono>
#include <thread>
#include <opencv2/opencv.hpp>

#define PI 3.14159265358979323846

using namespace phy2d;
using namespace std;

int main() {

    cv::Mat window = cv::Mat::zeros(1000, 1000, CV_8UC3);
    cv::imshow("Physics2D", window);
    srand((unsigned)time(NULL));

    Circle* c1=new Circle(410,100,1,30);
    Circle* c2=new Circle(400,400,1,30);
    c1->setVelocity(0.0f, 200.0f);

    World world;
    world.add_object(c1);
    world.add_object(c2);
    //Box* box = new Box(500, 700,100,50,1);
    Line* line1 = new Line(100,100, 900, 100,1);
    Line* line2 = new Line(900, 100, 900, 900, 1);
    Line* line3 = new Line(900, 900, 100, 900, 1);
    Line* line4 = new Line(100, 900, 100, 100, 1);
    //box->setAngleVelocity(PI/4);
    //world.add_object(box);
    world.add_object(line1);
    world.add_object(line2);
    world.add_object(line3);
    world.add_object(line4);
    
    /*for(int i = 0; i < 60; i++){
        Circle* c = new Circle(100+rand()%800, 100+rand()%800, 1, 10);
        c->setVelocity(1.0f, 200.0f);
        world.add_object(c);
    }*/
    

    while (true)
    {
        window = cv::Mat::zeros(1000, 1000, CV_8UC3);
        world.update(0.01f);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        for(auto obj: world.get_objects()){
            if(obj->getType() == BodyType::CIRCLE){
                Circle* c= dynamic_cast<Circle*>(obj);
                cv::circle(window, cv::Point(c->getPosition().x, c->getPosition().y), c->getRadius(), cv::Scalar(255, 255, 0));
                cv::line(window, cv::Point(c->getPosition().x, c->getPosition().y), cv::Point(c->getPosition().x + cos(c->getAngle())*c->getRadius(), c->getPosition().y + sin(c->getAngle())*c->getRadius()), cv::Scalar(255, 255, 0));        
            }
            else if(obj->getType() == BodyType::LINE){
                Line* l= dynamic_cast<Line*>(obj);
                cv::line(window, cv::Point(l->getPoint1().x, l->getPoint1().y), cv::Point(l->getPoint2().x, l->getPoint2().y), cv::Scalar(0, 255, 0));
            }
            else if(obj->getType() == BodyType::BOX){
                Box* b= dynamic_cast<Box*>(obj);
                auto points = b->getVertices();
                cv::line(window, cv::Point(points[0].x, points[0].y), cv::Point(points[1].x, points[1].y), cv::Scalar(0, 0, 255));
                cv::line(window, cv::Point(points[1].x, points[1].y), cv::Point(points[2].x, points[2].y), cv::Scalar(0, 0, 255));
                cv::line(window, cv::Point(points[2].x, points[2].y), cv::Point(points[3].x, points[3].y), cv::Scalar(0, 0, 255));
                cv::line(window, cv::Point(points[3].x, points[3].y), cv::Point(points[0].x, points[0].y), cv::Scalar(0, 0, 255));
                if(b->getPosition().x<0||b->getPosition().y<0||b->getPosition().x>1000||b->getPosition().y>1000){
                    b->setVelocity(-b->getVelocity().x, -b->getVelocity().y);
                }
            }
        }
        cv::imshow("Physics2D", window);
        if (cv::waitKey(10)==27) break;
    }
    for (auto o: world.get_objects()){
        delete o;
    }
    return 0;
}