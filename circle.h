#ifndef CIRCLE_H #define CIRCLE_H

struct Box_Coordinate { int TopLeft_x; int TopLeft_y; int box_width; int box_height; };

class __declspec(dllexport) circle 
 { private: float distance,r,area,c,d; char choice[2]; float pi=3.14; public: void GetInput(void); void Display(void); vector<Box_Coordinate> Detecting(cv::Mat frame); cv::Mat GetFrame(); };