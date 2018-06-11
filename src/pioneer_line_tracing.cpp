#include <ros/ros.h>
#include <sensor_msgs/PointCloud.h>
#include <opencv2/opencv.hpp>

#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/image_encodings.h>


using namespace std;
using namespace cv;

ros::Publisher pub_cloud;

geometry_msgs::Point32 fromXYWH(int x, int y, int width, int height) {
    geometry_msgs::Point32 point;
    point.x = (512.0-y) / 1.0;
    point.y = (256.0-x) / 1.0;

    return point;
}

    Point  geom_center(sensor_msgs::PointCloud msg, int x2, int x1) {
    float sumx = 0;
    float sumy = 0;
    float sredniax = 0;
    float sredniay = 0;
    Point point;



    for (int i = 0; i < msg.points.size(); i++) {
        if(msg.points[i].x< x1 && msg.points[i].x > x2) {
            sumx += msg.points[i].x;
            sumy += msg.points[i].y;
        }
    }

    sredniax = sumx/msg.points.size();
    sredniay = sumy/msg.points.size();
    point.x = sredniax;
    point.y = sredniay;
    return point;
}


void imageCallback(const sensor_msgs::ImageConstPtr& msg) {
    Mat binary;
    Mat input;
    Mat draw;
    vector<Mat>split_img;
    cv_bridge::CvImagePtr cv_ptr;

    try {
        sensor_msgs::PointCloud point_cloud;
        sensor_msgs::PointCloud point_cloud_draw;
        
        cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
        input=cv_ptr->image;
        input.copyTo(draw);
        flip(input, input, 0);
        flip(draw, draw, 0);
        cvtColor(input, input,cv:: COLOR_BGR2HSV);
        split(input, split_img);
        inRange(split_img[1], 200, 255, binary);
       

        for (int x = 0; x < binary.cols; x++) {
            for (int y = 0; y < binary.rows; y++) {
                if(binary.at<uchar>(y, x) == 255) {
                    geometry_msgs::Point32 point;
                    point.x=x;
                    point.y=y;
                    point_cloud_draw.points.push_back(point);
                    point_cloud.points.push_back(fromXYWH(x, y, 1, 1));
                }
            }
        }
        pub_cloud.publish(point_cloud);
        
        Point srodek = geom_center(point_cloud_draw,-1000, 20000);
        Point start;
        start.x = 256;
        start.y = 512;
        Point end;
        end.x = 256;
        end.y = 0;


        circle(draw, srodek,5, Scalar(0, 128, 0), 3);
        line(draw, start, srodek, Scalar(0, 0, 0), 2);
        line(draw, start, end, Scalar(0, 0, 0), 1);
        imshow("Line", draw);
        
    } catch (cv_bridge::Exception& e) {
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
    }
    cv::waitKey(30);
}




int main(int argc, char** argv) {
    ros::init(argc, argv, "pioneer_line_tracing");
    ros::NodeHandle n;
    ros::Rate rate(50);

    pub_cloud = n.advertise<sensor_msgs::PointCloud>("/line", 10);
    image_transport::ImageTransport it(n);
    image_transport::Subscriber sub = it.subscribe("/image", 1, &imageCallback);



    while(n.ok()) {
        ros::spinOnce();
        rate.sleep();
    }
    return 0;
}
