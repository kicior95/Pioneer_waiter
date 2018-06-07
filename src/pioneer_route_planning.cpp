#include <ros/ros.h>
#include <std_msgs/String.h>


using namespace std;

ros::Subscriber sub;
ros::Publisher pub;

std_msgs::String pub_msg;

string lewo="lewo";
string prawo="prawo";
string prosto="prosto";
string A="3_23";


void lineCallback(const std_msgs::String::ConstPtr& sub_msg){

    if(sub_msg->data==A)
    {
        pub_msg.data=lewo;
          pub.publish(pub_msg);
    }
    if(sub_msg->data=="1_23")
    {
        pub_msg.data=prawo;
        pub.publish(pub_msg);
    }
    else
    {
        //pub_msg.data=prosto;
    }

}


int main(int argc, char** argv)
{

    ros::init(argc, argv, "pioneer_route_planning");
    ros::NodeHandle n;
    ros::Rate rate(30);


    sub = n.subscribe("/qr", 10, &lineCallback);
    pub = n.advertise<std_msgs::String>("/direction", 10);



    while(n.ok())
    {
        //Spin
        ros::spinOnce();
        //Sleep
        rate.sleep();
    }

    return 0;
}
