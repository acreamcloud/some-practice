#include <iostream>
#include <cstring>
#include <sstream>
#include <cmath>
using namespace std;
class circle
{
    private:
        float distance,r,area,c,d;
        char choice[3];
        float pi=3.14;
    public:
        void GetInput(void)
        {
            printf("请输入你的数据是半径还是直径，d为直径，r为半径：");
            cin.getline(choice,3);
            if (0==strcmp("d",choice))
            {
                printf("请输入直径：");
                cin>>d;
                cin.ignore();
                r=d/2;
                area=pow(r,2)*pi;
                c=d*pi;

            }
            else if (0==strcmp("r",choice))
            {
                printf("请输入半径：");
                cin>>r;
                cin.ignore();
                area=pow(r,2)*pi;
                c=r*2*pi;
            }

        }
        void Display(void)
        {
            cout<<"该圆直径为："<<d<<endl;
            cout<<"该圆半径为："<<r<<endl;
            cout<<"该圆面积为："<<area<<endl;
            cout<<"该圆周长为："<<c<<endl;
        }

};


 
int main()
{
    circle c;
    c.GetInput();
    c.Display();
    return 0;
}
int main()
{
    
}