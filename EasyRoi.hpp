#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

class EasyROI 
{
  private:
      bool verbose;
      bool drawing;
      Mat img;
      int quantity;
      Scalar brush_color_ongoing;
      Scalar brush_color_finished;
      int cursor_x, cursor_y;
      vector<Point> polygon_vertices;
      bool polygon_dblclk;
      Mat orig_frame;
      Mat last_orig_frame;
      vector<bool> line_drawn;
      vector<bool> circle_drawn;
      vector<bool> polygon_drawn;
      map<int, map<string, int>> roi_dict;

  public:
      /*********************************************************************/
      EasyROI(bool verbose=false) 
      {
          this->verbose = verbose;
          init_variables();
      }
      /*********************************************************************/
      void init_variables() 
      {
          destroyAllWindows();
          roi_dict.clear();
          drawing = false;
          img.release();
          quantity = 0;
          brush_color_ongoing = Scalar(255, 0, 0);
          brush_color_finished = Scalar(0, 255, 0);
          cursor_x = -1;
          cursor_y = -1;
          polygon_vertices.clear();
          polygon_dblclk = false;
          orig_frame.release();
          last_orig_frame.release();
          line_drawn.clear();
          circle_drawn.clear();
          polygon_drawn.clear();
      }
      /*********************************************************************/
      map<int, map<string, int>> draw_line(Mat frame, int quantity=1) 
      {
          if (verbose) 
          {
              cout << "[DEBUG] Entered draw_line" << endl;
              cout << "[DEBUG] Draw " << quantity << " line(s)" << endl;
              cout << "[DEBUG] Drag the cursor to draw the line" << endl;
              cout << "[DEBUG] Press Esc to leave the process" << endl;
          }
  
          img = frame.clone();
          this->quantity = quantity;
  
          string window_name = "Draw " + to_string(this->quantity) + " Line(s)";
          namedWindow(window_name);
          setMouseCallback(window_name, draw_line_callback);
  
          last_orig_frame = img.clone();
          orig_frame = img.clone();
  
          line_drawn = vector<bool>(this->quantity, false);
  
          roi_dict["type"] = "line";
          roi_dict["roi"] = map<string, int>();

          while (true) 
          {
              imshow(window_name, img);
  
              int key = waitKey(1) & 0xFF;
              if (key == 27 || (line_drawn.size() > 0 && line_drawn.back())) {
                  destroyWindow(window_name);
                  line_drawn.clear();
                  break;
              }
          }

          if (verbose && roi_dict["roi"].size() != this->quantity) 
          {
              cout << "[DEBUG] Not all ROI's drawn" << endl;
              roi_dict.clear();
          }
  
          map<int, map<string, int>> roi_dict_temp = roi_dict;
  
          init_variables();
  
          return roi_dict_temp;
      }
      /*********************************************************************/
      map<int, map<string, int>> draw_rectangle(Mat frame, int quantity=1) 
      {
          if (verbose) 
          {
              cout << "[DEBUG] Entered draw_rectangle" << endl;
          }
  
          img = frame.clone();
          this->quantity = quantity;
  
          roi_dict["type"] = "rectangle";
          roi_dict["roi"] = map<string, int>();
  
          for (int i = 0; i < this->quantity; i++) 
          {
              Rect roi_ = selectROI("Draw " + to_string(this->quantity) + " Rectangle(s)", img, false, false);
              int tl_x = roi_.x;
              int tl_y = roi_.y;
              int w = roi_.width;
              int h = roi_.height;
              int br_x = tl_x + w;
              int br_y = tl_y + h;
  
              if (verbose && (w == 0 || h == 0)) {
                  cout << "[DEBUG] Not all ROI's drawn" << endl;
                  return map<int, map<string, int>>();
              }
  
              rectangle(img, Point(tl_x, tl_y), Point(br_x, br_y), brush_color_finished, 2);
  
              roi_dict["roi"][i] = 
              {
                  {"tl_x", tl_x},
                  {"tl_y", tl_y},
                  {"br_x", br_x},
                  {"br_y", br_y},
                  {"w", w},
                  {"h", h}
              };
          }
  
          map<int, map<string, int>> roi_dict_temp = roi_dict;  
          init_variables();  
          return roi_dict_temp;
      }
      /*********************************************************************/
      map<int, map<string, int>> draw_polygon(Mat frame, int quantity=1) 
      {
          if (verbose) 
          {
              cout << "[DEBUG] Entered draw_polygon" << endl;
              cout << "[DEBUG] Draw " << quantity << " polygon(s)" << endl;
              cout << "[DEBUG] Draw multiple lines by dragging the cursor" << endl;
              cout << "[DEBUG] Double Click to complete the polygon" << endl;
              cout << "[DEBUG] Press Esc to leave the process" << endl;
          }
  
          img = frame.clone();
          this->quantity = quantity;
  
          string window_name = "Draw " + to_string(this->quantity) + " Polygon(s)";
          namedWindow(window_name);
          setMouseCallback(window_name, draw_polygon_callback);
  
          last_orig_frame = img.clone();
          orig_frame = img.clone();
  
          polygon_drawn = vector<bool>(this->quantity, false);
  
          roi_dict["type"] = "polygon";
          roi_dict["roi"] = map<string, int>();
  
          while (true) 
          {
              imshow(window_name, img);
  
              int key = waitKey(1) & 0xFF;
              if (key == 27 || (polygon_drawn.size() > 0 && polygon_drawn.back())) {
                  destroyWindow(window_name);
                  polygon_drawn.clear();
                  break;
              }
          }
  
          if (verbose && roi_dict["roi"].size() != this->quantity) 
          {
              cout << "[DEBUG] Not all ROI's drawn" << endl;
              roi_dict.clear();
          }
  
          map<int, map<string, int>> roi_dict_temp = roi_dict;
  
          init_variables();
  
          return roi_dict_temp;
      }
      /*********************************************************************/
      map<int, map<string, int>> draw_cuboid(Mat frame, int quantity=1) 
      {
          // TODO: Implement draw_cuboid
      }
      /*********************************************************************/
      map<int, map<string, int>> draw_circle(Mat frame, int quantity=1) 
      {
          if (verbose) 
          {
              cout << "[DEBUG] Entered draw_circle" << endl;
              cout << "[DEBUG] Draw " << quantity << " circle(s)" << endl;
              cout << "[DEBUG] Select center and drag for radius" << endl;
              cout << "[DEBUG] Press Esc to leave the process" << endl;
          }
  
          img = frame.clone();
          this->quantity = quantity;
  
          string window_name = "Draw " + to_string(this->quantity) + " Circle(s)";
          namedWindow(window_name);
          setMouseCallback(window_name, draw_circle_callback);
  
          last_orig_frame = img.clone();
          orig_frame = img.clone();
  
          circle_drawn = vector<bool>(this->quantity, false);
  
          roi_dict["type"] = "circle";
          roi_dict["roi"] = map<string, int>();
  
          while (true) 
          {
              imshow(window_name, img);
  
              int key = waitKey(1) & 0xFF;
              if (key == 27 || (circle_drawn.size() > 0 && circle_drawn.back())) 
              {
                  destroyWindow(window_name);
                  circle_drawn.clear();
                  break;
              }
          }
  
          if (verbose && roi_dict["roi"].size() != this->quantity) 
          {
              cout << "[DEBUG] Not all ROI's drawn" << endl;
              roi_dict.clear();
          }
  
          map<int, map<string, int>> roi_dict_temp = roi_dict;
  
          init_variables();
  
          return roi_dict_temp;
      }
      /********************************************************************************/
      static void draw_line_callback(int event, int x, int y, int flags, void* param) 
      {
          EasyROI* self = static_cast<EasyROI*>(param);
  
          if (event == EVENT_LBUTTONDOWN) 
          {
              self->drawing = true;
              self->cursor_x = x;
              self->cursor_y = y;
          }
          else if (event == EVENT_MOUSEMOVE && self->drawing) 
          {
              self->img = self->orig_frame.clone();
              line(self->img, Point(self->cursor_x, self->cursor_y), Point(x, y), self->brush_color_ongoing, 2);
          }
          else if (event == EVENT_LBUTTONUP) 
          {
              self->drawing = false;
  
              int line_index = -1;
              for (int i = 0; i < self->line_drawn.size(); i++) 
              {
                  if (!self->line_drawn[i]) 
                  {
                      line_index = i;
                      break;
                  }
              }
  
              line(self->img, Point(self->cursor_x, self->cursor_y), Point(x, y), self->brush_color_finished, 2);  
              self->roi_dict["roi"][line_index] =
              {
                  {"point1", Point(self->cursor_x, self->cursor_y)},
                  {"point2", Point(x, y)}
              };
  
              self->orig_frame = self->img.clone();  
              self->line_drawn[line_index] = true;
          }
      }
      /********************************************************************************/
      static void draw_circle_callback(int event, int x, int y, int flags, void* param) 
      {
          EasyROI* self = static_cast<EasyROI*>(param);
  
          if (event == EVENT_LBUTTONDOWN) 
          {
              self->drawing = true;
              self->cursor_x = x;
              self->cursor_y = y;
          }
          else if (event == EVENT_MOUSEMOVE && self->drawing) 
          {
              self->img = self->orig_frame.clone();
  
              Point center(self->cursor_x, self->cursor_y);
              Point pt_on_circle(x, y);
              int radius = norm(pt_on_circle - center);
  
              line(self->img, Point(self->cursor_x, self->cursor_y), Point(x, y), self->brush_color_ongoing, 2);
              circle(self->img, center, radius, self->brush_color_ongoing, 2);
          }
          else if (event == EVENT_LBUTTONUP) 
          {
              self->drawing = false;
  
              int circle_index = -1;
              for (int i = 0; i < self->circle_drawn.size(); i++) {
                  if (!self->circle_drawn[i]) 
                  {
                      circle_index = i;
                      break;
                  }
              }
  
              Point center(self->cursor_x, self->cursor_y);
              Point pt_on_circle(x, y);
              int radius = norm(pt_on_circle - center);
  
              line(self->img, Point(self->cursor_x, self->cursor_y), Point(x, y), self->brush_color_finished, 2);
              circle(self->img, center, radius, self->brush_color_finished, 2);
  
              self->roi_dict["roi"][circle_index] = 
              {
                  {"center", Point(self->cursor_x, self->cursor_y)},
                  {"point2", Point(x, y)},
                  {"radius", radius}
              };
  
              self->orig_frame = self->img.clone();
  
              self->circle_drawn[circle_index] = true;
          }
      }
      /*********************************************************************************/
      static void draw_polygon_callback(int event, int x, int y, int flags, void* param) 
      {
          EasyROI* self = static_cast<EasyROI*>(param);
  
          if (event == EVENT_LBUTTONDOWN) 
          {
              if (self->polygon_dblclk) 
              {
                  self->polygon_dblclk = false;
              }
              else 
              {
                  self->drawing = true;
  
                  self->polygon_vertices.push_back(Point(x, y));
  
                  if (self->polygon_vertices.size() > 1) 
                  {
                      Point prev_vertex = self->polygon_vertices[self->polygon_vertices.size() - 2];
                      Point current_vertex = self->polygon_vertices[self->polygon_vertices.size() - 1];
  
                      line(self->img, prev_vertex, current_vertex, self->brush_color_finished, 2);
                      self->orig_frame = self->img.clone();
                  }
              }
          }
          else 
          if (event == EVENT_MOUSEMOVE && self->drawing) 
          {
              self->img = self->orig_frame.clone();
  
              Point last_vertex = self->polygon_vertices.back();
              line(self->img, last_vertex, Point(x, y), self->brush_color_ongoing, 2);
          }
          else 
          if (event == EVENT_LBUTTONDBLCLK) 
          {
              self->drawing = false;
  
              vector<Point> hull_output;
              convexHull(self->polygon_vertices, hull_output, false);
  
              self->polygon_vertices.clear();
              for (Point pt : hull_output) 
              {
                  self->polygon_vertices.push_back(pt);
              }
  
              self->img = self->last_orig_frame.clone();
              for (int v = 1; v < self->polygon_vertices.size(); v++) 
              {
                  line(self->img, self->polygon_vertices[v], self->polygon_vertices[v - 1], self->brush_color_finished, 2);
              }
  
              line(self->img, self->polygon_vertices[0], self->polygon_vertices[self->polygon_vertices.size() - 1], self->brush_color_finished, 2);
  
              int polygon_index = -1;
              for (int i = 0; i < self->polygon_drawn.size(); i++) 
              {
                  if (!self->polygon_drawn[i]) 
                  {
                      polygon_index = i;
                      break;
                  }
              }
  
              self->roi_dict["roi"][polygon_index] = 
              {
                  {"vertices", self->polygon_vertices}
              };
  
              self->orig_frame = self->img.clone();
              self->last_orig_frame = self->orig_frame.clone();
  
              self->polygon_drawn[polygon_index] = true;
              self->polygon_dblclk = true;
              self->polygon_vertices.clear();
          }
      }
      /*********************************************************************/
      ~EasyROI() 
      {
          destroyAllWindows();
  
          if (verbose) 
          {
              cout << "Thank You!!" << endl;
          }
      }
      /*********************************************************************/
      Mat visualize_roi(Mat frame, map<int, map<string, int>> roi_dict) 
      {
          if (roi_dict.find("roi") == roi_dict.end()) 
          {
              return frame;
          }
  
          Mat img = frame.clone();
  
          if (roi_dict["type"] == "rectangle") 
          {
              visualize_rect(img, roi_dict);
          }
          else 
          if (roi_dict["type"] == "line") 
          {
              visualize_line(img, roi_dict);
          }
          else 
          if (roi_dict["type"] == "circle") 
          {
              visualize_circle(img, roi_dict);
          }
          else 
          if (roi_dict["type"] == "polygon") 
          {
              visualize_polygon(img, roi_dict);
          }
          else 
          if (roi_dict["type"] == "cuboid") 
          {
              visualize_cuboid(img, roi_dict);
          }
  
          return img;
      }
      /*********************************************************************/
      vector<Mat> crop_roi(Mat frame, map<int, map<string, int>> roi_dict) 
      {
          if (roi_dict.find("roi") == roi_dict.end()) 
          {
              return vector<Mat>();
          }
  
          Mat img = frame.clone();
          vector<Mat> cropped_images;
  
          if (roi_dict["type"] == "rectangle") 
          {
            cropped_images = crop_rect(img, roi_dict);
          }
          else 
          if (roi_dict["type"] == "line") 
          {
              cout << "[ERROR] What to crop in line roi:)" << endl;
          }
          else 
          if (roi_dict["type"] == "circle") 
          {
              cropped_images = crop_circle(img, roi_dict);
          }
          else 
          if (roi_dict["type"] == "polygon") 
          {
              cropped_images = crop_polygon(img, roi_dict);
          }
          else 
          if (roi_dict["type"] == "cuboid")
          {
              cropped_images = crop_cuboid(img, roi_dict);
          }
  
          return cropped_images;
      }
};


