#include <ros/ros.h>
#include <sensor_msgs/PointCloud.h>
#include <opencv2/opencv.hpp>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/image_encodings.h>
#include <zbar.h>
#include <std_msgs/String.h>
#include <iostream>

using namespace std;
using namespace cv;
using namespace zbar;

ros::Publisher pub_cloud;
ros::Publisher pub_qr;
std_msgs::String msg;

typedef struct {
    string type;
    std::string data;
    vector <Point> location;
} decodedObject;



void decode(Mat &im, vector<decodedObject>&decodedObjects) {
    ImageScanner scanner;

    Mat bin;
    Mat imGray;

    scanner.set_config(ZBAR_QRCODE, ZBAR_CFG_ENABLE, 1);
    cvtColor(im, imGray,CV_BGR2GRAY);
    Image image(imGray.cols, imGray.rows, "Y800", (uchar *)imGray.data, imGray.cols * imGray.rows);
    int n = scanner.scan(image);

    for(Image::SymbolIterator symbol = image.symbol_begin(); symbol != image.symbol_end(); ++symbol) {
        decodedObject obj;

        obj.type = symbol->get_type_name();
        obj.data = symbol->get_data();

        msg.data = obj.data;
        pub_qr.publish(msg);

        for(int i = 0; i< symbol->get_location_size(); i++) {
            obj.location.push_back(Point(symbol->get_location_x(i), symbol->get_location_y(i)));
        }
        decodedObjects.push_back(obj);
    }
}


void display(Mat &im, vector<decodedObject>&decodedObjects) {
    for(int i = 0; i < decodedObjects.size(); i++) {
        vector<Point> points = decodedObjects[i].location;
        vector<Point> hull;

        string text = decodedObjects[i].data;
        int fontFace = FONT_HERSHEY_SCRIPT_SIMPLEX;
        double fontScale = 2;
        int thickness = 3;
        int baseline=0;
        Size textSize = getTextSize(text, fontFace, fontScale, thickness, &baseline);
        baseline += thickness;
        Point textOrg((im.cols - textSize.width)/2, (im.rows + textSize.height)/2);


        if(points.size() > 4)
            convexHull(points, hull);
        else
            hull = points;

        int n = hull.size();

        for(int j = 0; j < n; j++) {
            line(im, hull[j], hull[ (j+1) % n], Scalar(0, 128, 0), 2);
        }

        putText(im, text, textOrg, fontFace, fontScale, Scalar::all(255), thickness, 8);
    }


    imshow("Results", im);
    waitKey(30);
}

void imageCallback(const sensor_msgs::ImageConstPtr& msg) {
    Mat input;
    cv_bridge::CvImagePtr cv_ptr;

    try {
        cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
        input = cv_ptr->image;

        Mat im = input;

        if(! im.data ) {
            cout <<  "Could not open or find the image" << std::endl ;
        }
        else {
            flip(im,im,0);

            vector<decodedObject> decodedObjects;
            decode(im, decodedObjects);
            display(im, decodedObjects);
        }

    } catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
    }
    cv::waitKey(30);
}




int main(int argc, char** argv) {
    ros::init(argc, argv, "pioneer_qrcode");
    ros::NodeHandle n;
    ros::Rate rate(50);

    pub_qr = n.advertise<std_msgs::String>("/qr", 10);
    image_transport::ImageTransport it(n);
    image_transport::Subscriber sub = it.subscribe("/image", 1, &imageCallback);



    while(n.ok()) {
        ros::spinOnce();
        rate.sleep();
    }
    return 0;
}

