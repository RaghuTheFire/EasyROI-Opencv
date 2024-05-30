#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>
#include <vector>
#include <map>

using namespace cv;
using namespace std;

int main() 
{
    string video_path = "overpass.mp4";

    // Initialize cam
    VideoCapture cap(video_path);
    if (!cap.isOpened()) 
    {
        cerr << "Cannot capture source" << endl;
        return -1;
    }
    Mat frame;
    cap >> frame;

    EasyROI roi_helper(true);

    // DRAW RECTANGULAR ROI
    vector<vector<Point>> rect_roi = roi_helper.draw_rectangle(frame, 3);
    cout << "Rectangle Example:" << endl;
    for (const auto& rect : rect_roi) 
    {
        cout << rect << endl;
    }

    Mat frame_temp = roi_helper.visualize_roi(frame, rect_roi);

    // crop drawn rectangles
    map<int, Mat> cropped_rects = roi_helper.crop_roi(frame, rect_roi);
    for (const auto& crop : cropped_rects) 
    {
        imshow(to_string(crop.first), crop.second);
    }

    imshow("frame", frame_temp);
    waitKey(0);
    destroyAllWindows();

    // DRAW LINE ROI
    vector<vector<Point>> line_roi = roi_helper.draw_line(frame, 3);
    cout << "Line Example:" << endl;
    for (const auto& line : line_roi) 
    {
        cout << line << endl;
    }

    frame_temp = roi_helper.visualize_roi(frame, line_roi);
    imshow("frame", frame_temp);
    waitKey(0);
    destroyAllWindows();

    // DRAW CIRCLE ROI
    vector<vector<Point>> circle_roi = roi_helper.draw_circle(frame, 3);
    cout << "Circle Example:" << endl;
    for (const auto& circle : circle_roi) 
    {
        cout << circle << endl;
    }

    frame_temp = roi_helper.visualize_roi(frame, circle_roi);

    // crop drawn circles
    map<int, Mat> cropped_circles = roi_helper.crop_roi(frame, circle_roi);
    for (const auto& crop : cropped_circles) 
    {
        imshow(to_string(crop.first), crop.second);
    }

    imshow("frame", frame_temp);
    waitKey(0);
    destroyAllWindows();

    // DRAW POLYGON ROI
    vector<vector<Point>> polygon_roi = roi_helper.draw_polygon(frame, 3);
    cout << "Polygon Example:" << endl;
    for (const auto& poly : polygon_roi) 
    {
        cout << poly << endl;
    }

    frame_temp = roi_helper.visualize_roi(frame, polygon_roi);

    // crop drawn polygons
    map<int, Mat> cropped_polys = roi_helper.crop_roi(frame, polygon_roi);
    for (const auto& crop : cropped_polys) 
    {
        imshow(to_string(crop.first), crop.second);
    }

    imshow("frame", frame_temp);
    waitKey(0);
    destroyAllWindows();

    return 0;
}
