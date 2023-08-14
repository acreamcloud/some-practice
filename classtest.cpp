#include <iostream>
using namespace std;
class animal
{
    public:
    int feeling,feed,health;
    void printhello()
    {
        printf("welcome!");
    }
    virtual void getdata()
    {
        cout<<height<<endl;
    }
    void running()
    {
        feed-=20;
        feeling+=20;
    }
    void eat()
    {
        feed+=30;
        feeling+=10;
    }

    protected:
    float height;
};
class sheep:public animal
{
    public:
    int health=100;
    int feed=0;
    int feeling=0;
    int woolsonsheep=0;
    float height=10;
    void GetWool()
    {
        if(woolsonsheep>0)
            printf("You Got some wools!");
            woolsonsheep-=1;
    }
    void getdata()
    {
        cout<<"this is your sheep's data:"<<endl;
        cout<<"height:"<<height<<endl;
        cout<<"feed:"<<feed<<endl;
        cout<<"feeling:"<<feeling<<endl;
        cout<<"the wools on sheep:"<<woolsonsheep<<endl;
    }
    void GrowUp()
    {
        if(feed>=90)
        {
            if(feeling>=80)
            {
                height+=1;
                feed-=70;
                cout<<"sucessful grew your sheep up!"<<endl;
            }
            else
                cout<<"Your sheep is too sad!"<<endl;
            
        }
        else
            cout<<"Your sheep is too hungry!"<<endl;
    }

};