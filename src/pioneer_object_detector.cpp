#include <ros/ros.h>
#include <sensor_msgs/PointCloud.h>
#include <geometry_msgs/Twist.h>

using namespace std;

ros::Subscriber sub;
ros::Publisher pub;


void lineCallback(const sensor_msgs::PointCloudPtr& msg){

}


int main(int argc, char** argv)
{

    ros::init(argc, argv, "pioneer_route_planning");
    ros::NodeHandle n;
    ros::Rate rate(30);


    //sub_cloud = n.subscribe("/qr", 10, &lineCallback);
    //pub_vel = n.advertise<geometry_msgs::Twist>("/cvel", 10);



    while(n.ok())
    {
        //Spin
        ros::spinOnce();
        //Sleep
        rate.sleep();
    }

    return 0;
}
