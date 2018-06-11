#include <ros/ros.h>
#include <std_msgs/String.h>
#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include <time.h>

using namespace std;

ros::Subscriber sub;
ros::Publisher pub;

std_msgs::String pub_msg;

int ile_wierz=18;
int mapa [18][18];
int wartosc_sciezki;
int s;
int t;
string qrkod;
int ktory;
bool wracam=0;
//TESTY


bool decyzja=0;

void sciezki(int a,int b,int waga){
    mapa[a][b]=waga;
    mapa[b][a]=waga;
}

struct wierzcholek{
    int odleglosc;
    int poprzedni;
    int numer;
    int x;
    int y;
};

wierzcholek w_p, w_a, w_n;
wierzcholek wie[17];

struct porownaj
{
    bool operator ()( const wierzcholek & wierzcholek1, const wierzcholek & wierzcholek2 )
    {
        //kolejność - rosnąco
        if( wierzcholek1.odleglosc > wierzcholek2.odleglosc )
            return true;
        if( wierzcholek1.odleglosc < wierzcholek2.odleglosc )
            return false;
    return false;
    }
};

void graf(){
    for(int i = 0; i < ile_wierz; i++ )
    {
        for(int j = 0; j < ile_wierz; j++ )
        {
            mapa[ i ][ j ] = 999999;
        }
    }

    sciezki(0,1,2);
    sciezki(0,17,2);
    sciezki(1,2,1);
    sciezki(1,13,2);
    sciezki(1,16,2);
    sciezki(2,3,1);
    sciezki(3,4,1);
    sciezki(3,14,2);
    sciezki(4,5,1);
    sciezki(5,6,1);
    sciezki(5,15,2);
    sciezki(6,7,3);
    sciezki(7,8,3);
    sciezki(7,15,2);
    sciezki(8,9,1);
    sciezki(9,10,1);
    sciezki(9,15,2);
    sciezki(10,11,1);
    sciezki(11,12,1);
    sciezki(11,14,2);
    sciezki(12,13,3);
    sciezki(13,14,2);
    sciezki(14,15,2);

}
void wspolrzedne(wierzcholek tablica[17]){
    tablica[0].x=2;
    tablica[0].y=0;
    tablica[1].x=2;
    tablica[1].y=2;
    tablica[2].x=2;
    tablica[2].y=3;
    tablica[3].x=2;
    tablica[3].y=4;
    tablica[4].x=2;
    tablica[4].y=5;
    tablica[5].x=2;
    tablica[5].y=6;
    tablica[6].x=2;
    tablica[6].y=7;
    tablica[7].x=4;
    tablica[7].y=8;
    tablica[8].x=6;
    tablica[8].y=7;
    tablica[9].x=6;
    tablica[9].y=6;
    tablica[10].x=6;
    tablica[10].y=5;
    tablica[11].x=6;
    tablica[11].y=4;
    tablica[12].x=6;
    tablica[12].y=3;
    tablica[13].x=4;
    tablica[13].y=2;
    tablica[14].x=4;
    tablica[14].y=4;
    tablica[15].x=4;
    tablica[15].y=6;
    tablica[16].x=0;
    tablica[16].y=2;
    tablica[17].x=2;
    tablica[17].y=-2;
}

string kierunek(wierzcholek w0,wierzcholek w1,wierzcholek w2){
    string kierunki;
    int x0=w0.x;
    int y0=w0.y;
    int x1=w1.x;
    int y1=w1.y;
    int x2=w2.x;
    int y2=w2.y;

    if(x0-x1<0){ //x rosnie - wschod
        if(y1-y2<0){ //y rosnie - polnoc
            kierunki="lewo";
        }
        if(y1-y2>0){ //y maleje - polunie
            kierunki="prawo";
        }
        if(x1-x2<0){ //x rosnie - wschod
            kierunki="prosto";
        }
    }
    if(x0-x1>0){ //x maleje - zachod
        if(y1-y2<0){ //y rosnie - polnoc
            kierunki="prawo";
        }
        if(y1-y2>0){ //y maleje - polunie
            kierunki="lewo";
        }
        if(x1-x2>0){ //x maleje - zachod
            kierunki="prosto";
        }
    }

    if(y0-y1<0){ //y rosnie - polnoc
        if(x1-x2<0){ //x rosnie - wschod
            kierunki="prawo";
        }
        if(x1-x2>0){ //x maleje - zachod
            kierunki="lewo";
        }
        if(y1-y2<0){ //y rosnie - polnoc
            kierunki="prosto";
        }
    }
    if(y0-y1>0){ //y maleje - polunie
        if(x1-x2<0){ //x rosnie - wschod
            kierunki="lewo";
        }
        if(x1-x2>0){ //x maleje - zachod
            kierunki="prawo";
        }
        if(y1-y2>0){ //y maleje - polunie
            kierunki="prosto";
        }
    }
    if(w1.numer==13 && w2.numer==12 && w0.numer!=14){
        kierunki="prosto";
    }
    if(w1.numer==12 && w2.numer==13 && w0.numer!=14){
        kierunki="prosto";
    }
    if(w1.numer==6 && w2.numer==7 && w0.numer!=15){
        kierunki="prosto";
    }
    if(w1.numer==7 && w2.numer==6 && w0.numer!=15){
        kierunki="prosto";
    }
    if(w1.numer==7 && w2.numer==8 && w0.numer!=15){
        kierunki="prosto";
    }
    if(w1.numer==8 && w2.numer==7 && w0.numer!=15){
        kierunki="prosto";
    }
    if(w0.numer==17){
        kierunki="prosto";
    }
    return kierunki;

}

void droga_A(int s, int t){

    int u;

    std::priority_queue < wierzcholek, std::vector <wierzcholek>, porownaj > wierz;

    for(int i=0;i<ile_wierz;i++){
    wie[i].odleglosc=9999;
    wie[i].poprzedni=9999;
    wie[i].numer=i;
    }
    wie[s].odleglosc=0;
    for(int i=0;i<ile_wierz;i++){
        wierz.push(wie[i]);
    }

    while(wierz.empty()==false){
        u=wierz.top().numer;
        wierz.pop();
        for(int i=0;i<ile_wierz;i++){
            if(wie[i].odleglosc>wie[u].odleglosc+mapa[u][i]){
               wie[i].odleglosc=wie[u].odleglosc+mapa[u][i];
               wie[i].poprzedni=u;
               wierz.push(wie[i]);
            }
        }
    }

    std::stack < wierzcholek > droga;
    wierzcholek aktualny;
    aktualny=wie[t];
    droga.push(aktualny);
    while(aktualny.numer!=s){
        aktualny=wie[wie[aktualny.numer].poprzedni];
        droga.push(aktualny);
    }
    int ile=(int)droga.size();
    wierzcholek droga_tablica[ile];

    for(int i=0;i<ile;i++){
        droga_tablica[i]=droga.top();
        droga.pop();
    }

    w_n=droga_tablica[1];
}

void wait(int seconds)
{
    clock_t endwait;
    endwait = clock () + seconds * CLOCKS_PER_SEC ;
    while (clock() < endwait) {}
}

void lineCallback(const std_msgs::String::ConstPtr& sub_msg){
if(decyzja)
{
    if(w_a.numer!=t){

        qrkod=sub_msg->data;    //pobieram dane
        ktory = atoi(qrkod.c_str());
        if(w_a.numer!=ktory)
        w_p=w_a;
        w_a=wie[ktory];
        if(w_a.numer!=t){
            sciezki(w_p.numer,w_a.numer,99999);
            droga_A(w_a.numer,t);
            graf();

            pub_msg.data=kierunek(w_p,w_a,w_n);     //wysylam kierunek
            pub.publish(pub_msg);


        }

        if(w_a.numer==t){

            //wait(15);
            t=0;
            wracam=1;
        }
        if(w_a.numer==0 && wracam==1){

            pub_msg.data="stop";
            pub.publish(pub_msg);
            w_a=wie[17]; w_a.numer=17;
            cout<<"Gdzie jechac?"<<endl;
            cin>>t;           //wierzchołek docelowy t
            wracam=0;
            graf();

        }
    }
}
else
   {
    pub_msg.data="stop";
    pub.publish(pub_msg);
}
}


int main(int argc, char** argv)
{



    ros::init(argc, argv, "pioneer_route_planning");
    ros::NodeHandle n;
    ros::Rate rate(50);

    sub = n.subscribe("/qr", 10, &lineCallback);
    pub = n.advertise<std_msgs::String>("/direction", 10);

    pub_msg.data="stop";
    pub.publish(pub_msg);

    cout<<"Gdzie jechac?"<<endl;
    cin>>t;           //wierzchołek docelowy t

    decyzja = 1;
    pub_msg.data="prosto";
    pub.publish(pub_msg);

    wspolrzedne(wie);
    graf();
    w_a=wie[17]; w_a.numer=17;     //wierzchołek startowy s

    //to juz Ci zostawiam bo chyba w petli powinno byc ale nie wiem




    while(n.ok())
    {
        //Spin
        ros::spinOnce();


       // pub.publish(pub_msg);

        //Sleep
        rate.sleep();
    }

    return 0;
}
