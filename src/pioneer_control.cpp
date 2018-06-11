#include <ros/ros.h>
#include <message_filters/subscriber.h>
#include <sensor_msgs/PointCloud.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/String.h>

using namespace std;
using namespace message_filters;


ros::Publisher pub_vel;
ros::Publisher pub_point;


geometry_msgs::Point32 point;
geometry_msgs::Twist velocity;

static int line = 0;

float getPointL2Norm(geometry_msgs::Point32 p) {
    return sqrt(pow(p.x, 2) + pow(p.y,2) + pow(p.z, 2));
}


void stop() {
    velocity.linear.x = 0;
    velocity.angular.z = 0;
    pub_vel.publish(velocity);
    cout<<"Robot stop"<<endl;
}

float angle(geometry_msgs::Point32 p) {
    float x=p.x;
    float y=p.y;

    return atan2(y, x);
}



geometry_msgs::Point32 geom_center(const sensor_msgs::PointCloudPtr& msg, int x2, int x1) {
    float sumx = 0;
    float sumy = 0;
    float sredniax = 0;
    float sredniay = 0;



    for (int i = 0; i < msg->points.size(); i++) {
        if(msg->points[i].y < x1 && msg->points[i].y > x2) {
            sumx += msg->points[i].x;
            sumy += msg->points[i].y;
        }
    }

    sredniax = sumx/msg->points.size();
    sredniay = sumy/msg->points.size();
    point.x = sredniax;
    point.y = sredniay;
    return point;
}

void move(geometry_msgs::Twist velocity) {
    pub_vel.publish(velocity);
}

void lineCallback(const sensor_msgs::PointCloudPtr& msg) {
    const int min_num_points = 5;

    if(msg->points.size() < min_num_points) {
        stop();
        //velocity.linear.x = 1;
        //velocity.angular.z = 1;
        //pub_vel.publish(velocity);
    }
    else {


        switch (line) {
        case 0:
            velocity.linear.x = 0;
            velocity.angular.z = 0;
            break;
        case 1:
            point = geom_center(msg, 120, 10000);
            velocity.linear.x = 1;
            velocity.angular.z = 2*angle(point);
            line = 4;
            break;
        case 2:
            point = geom_center(msg, -1000,-120);
            velocity.linear.x = 1;
            velocity.angular.z = 2*angle(point);
            line = 4;
            break;
        case 3:
            point = geom_center(msg, -80, 80);
            velocity.linear.x = 1;
            velocity.angular.z = angle(point);
            line = 4;
            break;
        case 4:
            point = geom_center(msg, -1000, 1000);
            velocity.linear.x = 2;
            velocity.angular.z = angle(point);
            break;
        }

        pub_vel.publish(velocity);
    }
}


void line2Callback(const std_msgs::String::ConstPtr& msg_qr) {
    if(msg_qr->data == "lewo"){
        line = 1;
    } else if(msg_qr->data == "prawo") {
        line = 2;
    } else if(msg_qr->data == "prosto") {
        line=  3;
    } else if(msg_qr->data == "stop") {
        line = 0;
    } else {
        line = 4;
    }
}


int main(int argc, char** argv) {
    ros::init(argc, argv, "pioneer_control");
    ros::NodeHandle n;
    ros::Rate rate(50);

    ros::Subscriber sub_cloud = n.subscribe("/line", 10, &lineCallback);
    ros::Subscriber sub_String = n.subscribe("/direction", 10, &line2Callback);

    pub_vel = n.advertise<geometry_msgs::Twist>("/cvel", 10);

    while(n.ok()) {
        ros::spinOnce();
        rate.sleep();
    }

    return 0;
}
