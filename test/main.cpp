#include "../include/Object/Circle.hpp"
#include "../include/Object/Line.hpp"
#include "../include/World/World.hpp"
#include "../include/Collision/Collision.hpp"
#include "../include/Object/Box.hpp"
#include <algorithm>
#include <iostream>
#include <chrono>
#include <thread>
#include <opencv2/opencv.hpp>

#define PI 3.14159265358979323846

using namespace phy2d;
using namespace std;

vec2 shoot_amount(0,0);
vec2 mother_ball_pos(0,0);
bool shooting=false;
bool aiming=false;
Circle* mother_ball;
cv::Mat window = cv::Mat::zeros(640, 1060, CV_8UC3);

void onMouse(int event,int x,int y,int flags,void* userdata) {
    switch (event) {
        case cv::EVENT_MOUSEMOVE:
            if (flags==cv::EVENT_FLAG_LBUTTON) {
                aiming=true;
                shoot_amount=vec2(x,y)-mother_ball_pos;
            }
            break;
        case cv::EVENT_LBUTTONUP:
            if (shooting)break;
            shooting=true;
            mother_ball->velocity=-shoot_amount*0.9f;
            aiming=false;
            break;
    }
}

int main() {
    cv::imshow("Physics2D", window);
    srand((unsigned)time(NULL));
    World world;
    Line *l1 = new Line(50, 50, 50, 590, 1);
    Line *l2 = new Line(50, 590, 1010, 590, 1);
    Line *l3 = new Line(1010, 590, 1010, 50, 1);
    Line *l4 = new Line(1010,50,50,50,1);
    world.add_object(l1);
    world.add_object(l2);
    world.add_object(l3);
    world.add_object(l4);

    std::vector<vec2> baskets={
        {50,50},{530,50},{1010,50},{50,590},{530,590},{1010,590}
    };

    vec2 ball_start_pos=vec2(690.0f,320.0f);
    vec2 ball_pos=ball_start_pos;
    float ball_radius=9;
    std::vector<Circle*> score_ball_list;

    for (int i =0;i<6;i++) {
        ball_pos.y=ball_start_pos.y-i*(ball_radius+3);
        for (int j=0;j<i+1;j++) {
            Circle* c=new Circle(ball_pos.x,ball_pos.y+j*2*(ball_radius+3),5,ball_radius);
            score_ball_list.push_back(c);
            world.add_object(c);
        }
        ball_pos.x+=sqrt(3)*ball_radius;
    }
    mother_ball=new Circle(100,320,5,9);
    mother_ball_pos=mother_ball->position;
    world.add_object(mother_ball);

    for (auto ball:score_ball_list) {
        ball->velocity=vec2(0,0);
    }
    std::vector<Circle*> to_remove;
    cv::setMouseCallback("Physics2D",onMouse);
    while (true)
    {
        window = cv::Mat::zeros(640, 1060, CV_8UC3);
        world.update(0.005f);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        mother_ball_pos=mother_ball->position;

        if (aiming) {
            vec2 target=mother_ball_pos-shoot_amount;
            cv::line(window,cv::Point(mother_ball_pos.x,mother_ball_pos.y),cv::Point(target.x,target.y),cv::Scalar(0,0,255));
        }

        if (length(mother_ball->velocity)<=1.0f) {
            mother_ball->velocity=vec2(0.0,0.0);
            shooting=false;
        }
        else {
            mother_ball->applyForce(-1.0f*normalize(mother_ball->velocity));
        }

        for (auto circ:score_ball_list) {
            cv::circle(window,cv::Point(circ->position.x,circ->position.y),ball_radius,cv::Scalar(0,255,0));
            for (auto &basket:baskets) {
                if(length(basket-circ->position)<40.0f) {
                    world.remove_object(circ);
                    to_remove.push_back(circ);
                }
            }
            if (length(circ->velocity)>=1.0f) {
                circ->applyForce(-1.0f*normalize(circ->velocity));
            }
            else {
                circ->velocity=vec2(0.0,0.0);
            }
        }
        for (auto a:to_remove) {
            score_ball_list.erase(std::remove(score_ball_list.begin(),score_ball_list.end(),a));
            delete a;
        }
        to_remove.clear();
        cv::circle(window,cv::Point(mother_ball->position.x,mother_ball->position.y),ball_radius,cv::Scalar(255,0,0));

        for (auto basket:baskets) {
            cv::circle(window,cv::Point(basket.x,basket.y),30,cv::Scalar(0,255,0));
        }

        for(auto obj: world.get_objects()){
            if(obj->getType() == BodyType::LINE){
                Line* l= dynamic_cast<Line*>(obj);
                cv::line(window, cv::Point(l->getPoint1().x, l->getPoint1().y), cv::Point(l->getPoint2().x, l->getPoint2().y), cv::Scalar(0, 255, 0));
            }
        }
        cv::imshow("Physics2D", window);
        if (cv::waitKey(5)==27) break;
    }
    for (auto o: world.get_objects()){
        delete o;
    }
    return 0;
}
