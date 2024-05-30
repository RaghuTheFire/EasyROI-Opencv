
#include <opencv2/opencv.hpp>
#include <vector>
#include <unordered_map>

using namespace cv;
using namespace std;

Mat visualize_rect(Mat img, const unordered_map<int, vector<int>>& roi_dict, const Scalar& color = Scalar(0, 255, 0)) 
{
    for (const auto& roi : roi_dict) 
    {
        const int tl_x = roi.second[0];
        const int tl_y = roi.second[1];
        const int br_x = roi.second[2];
        const int br_y = roi.second[3];

        rectangle(img, Point(tl_x, tl_y), Point(br_x, br_y), color, 2);
    }

    return img;
}

Mat visualize_line(Mat img, const unordered_map<int, vector<Point>>& roi_dict, const Scalar& color = Scalar(0, 255, 0)) 
{
    for (const auto& roi : roi_dict) 
    {
        const Point pt1 = roi.second[0];
        const Point pt2 = roi.second[1];

        line(img, pt1, pt2, color, 2);
    }

    return img;
}

Mat visualize_circle(Mat img, const unordered_map<int, vector<int>>& roi_dict, const Scalar& color = Scalar(0, 255, 0)) 
{
    for (const auto& roi : roi_dict) 
    {
        const Point center(roi.second[0], roi.second[1]);
        const int radius = roi.second[2];

        circle(img, center, radius, color, 2);
    }

    return img;
}

Mat visualize_polygon(Mat img, const unordered_map<int, vector<Point>>& roi_dict, const Scalar& color = Scalar(0, 255, 0)) 
{
    for (const auto& roi : roi_dict) 
    {
        const vector<Point>& poly_vertices = roi.second;

        for (size_t v = 1; v < poly_vertices.size(); ++v) 
        {
            line(img, poly_vertices[v], poly_vertices[v - 1], color, 2);
        }

        line(img, poly_vertices[0], poly_vertices.back(), color, 2);
    }

    return img;
}

unordered_map<int, Mat> crop_rect(const Mat& img, const unordered_map<int, vector<int>>& roi_dict) 
{
    unordered_map<int, Mat> cropped_images;

    for (const auto& roi : roi_dict) 
    {
        const int tl_x = roi.second[0];
        const int tl_y = roi.second[1];
        const int br_x = roi.second[2];
        const int br_y = roi.second[3];

        cropped_images[roi.first] = img(Rect(tl_x, tl_y, br_x - tl_x, br_y - tl_y));
    }

    return cropped_images;
}

unordered_map<int, Mat> crop_circle(const Mat& img, const unordered_map<int, vector<int>>& roi_dict) 
{
    unordered_map<int, Mat> cropped_images;

    for (const auto& roi : roi_dict) 
    {
        const Point center(roi.second[0], roi.second[1]);
        const int radius = roi.second[2];

        Mat mask(img.size(), CV_8UC1, Scalar(0));
        circle(mask, center, radius, Scalar(255), -1);

        Mat masked_image;
        bitwise_and(img, img, masked_image, mask);

        const Rect roi_rect(center.x - radius, center.y - radius, 2 * radius, 2 * radius);
        cropped_images[roi.first] = masked_image(roi_rect);
    }

    return cropped_images;
}

unordered_map<int, Mat> crop_polygon(const Mat& img, const unordered_map<int, vector<Point>>& roi_dict) 
{
    unordered_map<int, Mat> cropped_images;

    const int num_channel = img.channels();
    const Scalar mask_color = Scalar(255, 255, 255);

    for (const auto& roi : roi_dict) 
    {
        const vector<Point>& poly_vertices = roi.second;

        Rect roi_rect = boundingRect(poly_vertices);

        Mat mask(img.size(), CV_8UC3, Scalar(0, 0, 0));
        fillConvexPoly(mask, poly_vertices, mask_color);

        Mat masked_image;
        bitwise_and(img, mask, masked_image);

        cropped_images[roi.first] = masked_image(roi_rect);
    }

    return cropped_images;
}

