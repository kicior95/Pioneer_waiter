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

void imageCallback(const sensor_msgs::ImageConstPtr& msg) {
    Mat binary;
    Mat input;
    vector<Mat>split_img;
    cv_bridge::CvImagePtr cv_ptr;

    try {
        sensor_msgs::PointCloud point_cloud;
        cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
        input=cv_ptr->image;

        flip(input, input, 0);
        cvtColor(input, input,cv:: COLOR_BGR2HSV);
        split(input, split_img);


        inRange(split_img[1], 200, 255, binary);
        imshow("okienko", binary);

        for (int x = 0; x < binary.cols; x++) {
            for (int y = 0; y < binary.rows; y++) {
                if(binary.at<uchar>(y, x) == 255) {
                    point_cloud.points.push_back(fromXYWH(x, y, 1, 1));
                }
            }
        }
        pub_cloud.publish(point_cloud);
    } catch (cv_bridge::Exception& e) {
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
    }
    cv::waitKey(30);
}




int main(int argc, char** argv) {
    ros::init(argc, argv, "pioneer_line_tracing");
    ros::NodeHandle n;
    ros::Rate rate(30);

    pub_cloud = n.advertise<sensor_msgs::PointCloud>("/line", 10);
    image_transport::ImageTransport it(n);
    image_transport::Subscriber sub = it.subscribe("/image", 1, &imageCallback);



    while(n.ok()) {
        ros::spinOnce();
        rate.sleep();
    }
    return 0;
}
