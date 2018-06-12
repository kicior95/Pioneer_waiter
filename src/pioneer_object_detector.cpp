#include <ros/ros.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Bool.h>

using namespace std;

ros::Subscriber sub;
ros::Publisher pub;


void lineCallback(const std_msgs::Float32::ConstPtr& msg){
    std_msgs::Bool stop;

    if(msg->data>0.80) {
         stop.data = true;
    } else {
        stop.data = false;
    }
    pub.publish(stop);
}


int main(int argc, char** argv) {

    ros::init(argc, argv, "pioneer_object_detector");
    ros::NodeHandle n;
    ros::Rate rate(50);


    sub = n.subscribe("/ultrasonic", 10, &lineCallback);
    pub = n.advertise<std_msgs::Bool>("/stop", 10);



    while(n.ok()) {
        ros::spinOnce();
        rate.sleep();
    }

    return 0;
}
