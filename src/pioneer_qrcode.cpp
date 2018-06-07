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


ros::Publisher pub_qr;

typedef struct
{
  string type;
  std::string data;
  vector <Point> location;
} decodedObject;

std_msgs::String msg;

void decode(Mat &im, vector<decodedObject>&decodedObjects)
{

  // Create zbar scanner
  ImageScanner scanner;

  Mat bin;
  // Configure scanner
  scanner.set_config(ZBAR_QRCODE, ZBAR_CFG_ENABLE, 1);

  // Convert image to grayscale
  Mat imGray;


  cvtColor(im, imGray,CV_BGR2GRAY);

//threshold( imGray, bin, threshold_value, max_BINARY_value,threshold_type );

//imshow("Results", bin);
  // Wrap image data in a zbar image
  Image image(imGray.cols, imGray.rows, "Y800", (uchar *)imGray.data, imGray.cols * imGray.rows);

  // Scan the image for barcodes and QRCodes
  int n = scanner.scan(image);

  // Print results
  for(Image::SymbolIterator symbol = image.symbol_begin(); symbol != image.symbol_end(); ++symbol)
  {
    decodedObject obj;

    obj.type = symbol->get_type_name();
    obj.data = symbol->get_data();

    // Print type and data
    cout << "Type : " << obj.type << endl;
    cout << "Data : " << obj.data << endl << endl;

    msg.data=obj.data;
    pub_qr.publish(msg);


    // Obtain location
    for(int i = 0; i< symbol->get_location_size(); i++)
    {
      obj.location.push_back(Point(symbol->get_location_x(i),symbol->get_location_y(i)));
    }

    decodedObjects.push_back(obj);
  }
}

// Display barcode and QR code location
void display(Mat &im, vector<decodedObject>&decodedObjects)
{
  // Loop over all decoded objects
  for(int i = 0; i < decodedObjects.size(); i++)
  {
    vector<Point> points = decodedObjects[i].location;
    vector<Point> hull;

    // If the points do not form a quad, find convex hull
    if(points.size() > 4)
      convexHull(points, hull);
    else
      hull = points;

    // Number of points in the convex hull
    int n = hull.size();

    for(int j = 0; j < n; j++)
    {
      line(im, hull[j], hull[ (j+1) % n], Scalar(255,0,0), 3);
    }

  }

  // Display results
  imshow("Results", im);

  waitKey(30);

}

ros::Publisher pub_cloud;


void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
   Mat input;

    cv_bridge::CvImagePtr cv_ptr;
    try
    {

        cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
        input=cv_ptr->image;

        Mat im = input;


        //Mat im=imread("/home/kicior/catkin_ws/src/TEST/barcode.jpg");
        if(! im.data )                              // Check for invalid input
            {
                cout <<  "Could not open or find the image" << std::endl ;

            }
        else
        {
        flip(im,im,-1);

            // Variable for decoded objects
            vector<decodedObject> decodedObjects;

            // Find and decode barcodes and QR codes
            decode(im, decodedObjects);

            // Display location
            display(im, decodedObjects);
        }




    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
    }
    cv::waitKey(30);
}




int main(int argc, char** argv)
{
    //ROS node initialization
    ros::init(argc, argv, "pioneer_qrcode");
    ros::NodeHandle n;
    ros::Rate rate(30);


    //Subscriber
    pub_qr = n.advertise<std_msgs::String>("/qr", 10);
    image_transport::ImageTransport it(n);
    image_transport::Subscriber sub = it.subscribe("/image", 1, &imageCallback);



    while(n.ok())
    {

        //Spin
        ros::spinOnce();

        //Sleep
        rate.sleep();

    }
    return 0;
}

