#include <string>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <cassert>
#include "vector2d.h"
#include "colourRGB.h"
#include "bvg.h"
#include "png_canvas.h"
#include "debug.h"

using namespace std;

class BVGRenderer: public BVGRendererBase{
public:
  void render_rectangle(Vector2d center, int width, int height, ColourRGB colour) {
    Vector2d origin(center.x - (width / 2), center.y - (height / 2));
    for (int i = origin.x; i < origin.x + width; ++i) {
      for (int j = origin.y; j < origin.y + height; ++j) {
        if (i >= 0 && j  >= 0) {
          canvas[i][j] = colour;
        }
      }
    }
  }

	virtual void create_canvas(Vector2d dimensions, ColourRGB background_colour, int scale_factor){
		if(d::debug) cout << "Canvas " << dimensions << background_colour << scale_factor << endl;
		width = dimensions[0];
		height = dimensions[1];
		canvas.initialize_canvas(dimensions[0],dimensions[1]);
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				canvas[x][y] = background_colour;
        //if (x % 100 == 0 || y % 100 == 0) {
        //  canvas[x][y] = ColourRGB(255, 255, 255);
        //}
      }
    }
	}
  vector<Vector2d> render_steep_line(Vector2d endpoint1, Vector2d endpoint2, ColourRGB colour, int thickness){
    vector<Vector2d> ret;
    if(d::debug) cout << "START RENDERING STEEPLINE" << endl;
    int F = 0;
    int x = endpoint1.x;
    int y = endpoint1.y;
    int end = endpoint2.y;
    auto L = Vector2d(endpoint2.x - endpoint1.x, endpoint2.y - endpoint1.y);

    if(d::debug) cout << "Starting x: " << x << " and starting y: "<< y << endl;
    if(d::debug) cout << "Ending x: " << endpoint2.x  << " and ending y: "<< endpoint2.y << endl;

    while (y < end) {
      render_rectangle(Vector2d(x, y), thickness, thickness, colour);

      if (abs(F - L.x) < abs(F + (L.y - L.x))) {
        y++;
        F -= L.x;
      } else {
        x++;
        y++;
      ret.push_back(Vector2d(x, y));
        F += L.y - L.x;
      }
    }
    if(d::debug) cout << "Ending up at x: " << x << " and y: " << y << endl;
    if(d::debug) cout << "DONE RENDERING STEEP LINE"<< endl;
    return ret;
  }

 vector<Vector2d> render_positive_line(Vector2d endpoint1, Vector2d endpoint2, ColourRGB colour, int thickness){
    vector<Vector2d> ret;
    int F = 0;
    int x = endpoint1.x;
    int y = endpoint1.y;
    int end = endpoint2.x;
    auto L = Vector2d(endpoint2.x - endpoint1.x, endpoint2.y - endpoint1.y);

    if(d::debug) cout << "Starting x: " << x << " and starting y: "<< y << endl;
    if(d::debug) cout << "Ending x: " << endpoint2.x  << " and ending y: "<< endpoint2.y << endl;

    while (x < end) {
      ret.push_back(Vector2d(x, y));
      //canvas[x][y] = colour;
      render_rectangle(Vector2d(x, y), thickness, thickness, colour); 
      if (abs(F + L.y) < abs(F + (L.y - L.x))) {
        x++;
        F += L.y;
      } else {
        x++;
        y++;
        F += L.y - L.x;
      }
    }
    if(d::debug) cout << "Ending up at x: " << x << " and y: " << y << endl;
    return ret;
  }

  
	vector<Vector2d> render_negative_line(Vector2d endpoint1, Vector2d endpoint2, ColourRGB colour, int thickness){
    vector<Vector2d> ret;
    if(d::debug) cout << "START RENDERING NEGATIVE LINE" << endl;
    int F = 0;
    int x = endpoint1.x;
    int y = endpoint1.y;
    int end = endpoint2.x;
    auto L = Vector2d(endpoint2.x - endpoint1.x, endpoint2.y - endpoint1.y);

    if(d::debug) cout << "Starting x: " << x << " and starting y: "<< y << endl;
    if(d::debug) cout << "Ending x: " << endpoint2.x  << " and ending y: "<< endpoint2.y << endl;

    while (x < end && y >= 0) {
      render_rectangle(Vector2d(x, y), thickness, thickness, colour);
      ret.push_back(Vector2d(x, y));
      // TODO: figure out why switching the signs fixed y not being decremented 
      // TODO: why does y get negative
      if (abs(F + L.y) < abs(F + L.y + L.x)) {
        x++;
        F += L.y;
      } else {
        x++;
        y--;
        F += L.y + L.x;
      }
    }
    if(d::debug) cout << "Ending up at x: " << x << " and y: " << y << endl;
    if(d::debug) cout << "DONE RENDERING NEGATIVE LINE" << endl;
    return ret;
  } 
	vector<Vector2d> render_negative_steep_line(Vector2d endpoint1, Vector2d endpoint2, ColourRGB colour, int thickness){
  	vector<Vector2d> ret;

    if(d::debug) cout << "START RENDERING NEGATIVE STEEP LINE " << endl;
    assert(endpoint1.x <= endpoint2.x);
    int F = 0;
    int x = endpoint1.x;
    int y = endpoint1.y;
    int end = endpoint2.y;
    auto L = Vector2d(endpoint2.x - endpoint1.x, endpoint2.y - endpoint1.y);

    if(d::debug) cout << "Starting x: " << x << " and starting y: "<< y << endl;
    if(d::debug) cout << "Ending x: " << endpoint2.x  << " and ending y: "<< endpoint2.y << endl;

    while (y >= end) {
      render_rectangle(Vector2d(x, y), thickness, thickness, colour);

      if (abs(F - L.x) < abs(F - L.y - L.x)) {
        y--;
        F -= L.x;
      } else {
        y--;
        x++;
      ret.push_back(Vector2d(x, y));
        F += -L.y - L.x;
      }
    }
    if(d::debug) cout << "Ending up at x: " << x << " and y: " << y << endl;
    if(d::debug) cout << "DONE RENDERING NEGATIVE STEEP LINE" << endl;
    return ret;
  } 
 vector<Vector2d> render_vertical_line(int x, int start_y, int end_y, ColourRGB colour, int thickness) {
   if(d::debug) cout << "RENDERING VERTICAL LINE " << endl;
  	vector<Vector2d> ret;
    if (start_y < end_y) {
      int i = 0;
      if(d::debug) cout << "Going from start y: " << start_y << " to end y: " << end_y <<  " on x: " <<  x << endl; 
      for (i = start_y; i < end_y; ++i) {
        render_rectangle(Vector2d(x, i), thickness, thickness, colour);
        ret.push_back(Vector2d(x, i));
      }
      if(d::debug) cout << "Ended up at y: " << i << endl;
    } else {
      if(d::debug) cout << "Going from start y: " << end_y << " to end y: " << start_y <<  " on x: " <<  x << endl; 
      int i = 0;
      for (i = end_y; i < start_y; ++i) {
        render_rectangle(Vector2d(x, i), thickness, thickness, colour);
        ret.push_back(Vector2d(x, i));
      }
      if(d::debug) cout << "Ended up at y: " << i << endl;
    }
    if(d::debug) cout << "DONE RENDERING VERTICAL LINE " << endl;
    return ret;
  }
  vector<Vector2d> render_and_return_line(Vector2d endpoint1, Vector2d endpoint2, ColourRGB colour, int thickness){
    vector<Vector2d> ret;
    // check which quadrent we are drawing in and what the slope is
    // if slope is too high flip x and y
    // reconfigure endpoints to be in top right quad
    // do algorithm
    // convert all points back to their respective quad

    // if the slope is to large switch x and y
    int delta_x = endpoint2.x - endpoint1.x;
    int delta_y = endpoint2.y - endpoint1.y;

    if (delta_y == 0 && delta_x == 0) {
      render_rectangle(Vector2d(endpoint1.x, endpoint1.y), thickness, thickness, colour);
      ret = { endpoint1 };
      return ret;
    }

    if (delta_x == 0) {
      ret = render_vertical_line(endpoint1.x, endpoint1.y, endpoint2.y, colour, thickness);   
      return ret;
    }

    if ((delta_x < 0) ^ (delta_y < 0)) { // if the slope is negative
      if (abs(delta_x) < abs(delta_y)) { // if the slope is steep
        if (endpoint1.x > endpoint2.x) { // if the endpoints are switched
          ret = render_negative_steep_line(endpoint2, endpoint1, colour, thickness);
        } else {
          ret = render_negative_steep_line(endpoint1, endpoint2, colour, thickness);
        }
      } else {
        if (endpoint1.x > endpoint2.x) {
          ret = render_negative_line(endpoint2, endpoint1, colour, thickness);
        } else {
          ret = render_negative_line(endpoint1, endpoint2, colour, thickness);
        }
      }
      return ret;
    }
    if (abs(delta_x) < abs(delta_y)) {
      if (delta_x < 0 && delta_y < 0) {
        ret = render_steep_line(endpoint2, endpoint1, colour, thickness);
      } else {
        ret = render_steep_line(endpoint1, endpoint2, colour, thickness);
      }
    } else {
      if (delta_x < 0 && delta_y < 0) {
        ret = render_positive_line(endpoint2, endpoint1, colour, thickness);
      } else {
        ret = render_positive_line(endpoint1, endpoint2, colour, thickness);
      }
    }
    return ret;
  }

	virtual void render_line(Vector2d endpoint1, Vector2d endpoint2, ColourRGB colour, int thickness){
    render_and_return_line(endpoint1, endpoint2, colour, thickness);
	}

	virtual void render_circle(Vector2d center, int radius, ColourRGB line_colour, int line_thickness){
    if(d::debug) cout << "RENDERING CIRCLE" << endl;
    int F = 0;
    int x = 0;
    int y = radius;
    if(d::debug) cout << "start x: " << x << " start y: " << y << endl;
    while (x <= y) {
      render_rectangle(Vector2d( x + center.x,  y + center.y), line_thickness, line_thickness, line_colour);
      render_rectangle(Vector2d(-x + center.x,  y + center.y), line_thickness, line_thickness, line_colour);
      render_rectangle(Vector2d( x + center.x, -y + center.y), line_thickness, line_thickness, line_colour);
      render_rectangle(Vector2d(-x + center.x, -y + center.y), line_thickness, line_thickness, line_colour);
      render_rectangle(Vector2d( y + center.y,  x + center.x), line_thickness, line_thickness, line_colour);
      render_rectangle(Vector2d(-y + center.y,  x + center.x), line_thickness, line_thickness, line_colour);
      render_rectangle(Vector2d( y + center.y, -x + center.x), line_thickness, line_thickness, line_colour);
      render_rectangle(Vector2d(-y + center.y, -x + center.x), line_thickness, line_thickness, line_colour);
      if (abs(F + (2 * x) + 1) < abs(F + (2 * x) - (2 * y) + 2)) {
        x++;
        F = F + 2 * x;
      } else {
        x++;
        y--;
        F = F + 2 * x - 2 * y + 2;
      }
    }
    if(d::debug) cout << "end x: " << x << " end y: " << y << endl;
    if(d::debug) cout << "DONE RENDERING CIRCLE" << endl;
	}
	virtual void render_filledcircle(Vector2d center, int radius, ColourRGB line_colour, int line_thickness, ColourRGB fill_colour){
    if(d::debug) cout << "RENDERING FILLED CIRCLE" << endl;
    int F = 0;
    int x = 0;
    int y = radius;
    if(d::debug) cout << "start x: " << x << " start y: " << y << endl;
    while (x <= y) {
      render_rectangle(Vector2d( x + center.x,  y + center.y), line_thickness, line_thickness, line_colour);
      render_rectangle(Vector2d(-x + center.x,  y + center.y), line_thickness, line_thickness, line_colour);
      render_rectangle(Vector2d( x + center.x, -y + center.y), line_thickness, line_thickness, line_colour);
      render_rectangle(Vector2d(-x + center.x, -y + center.y), line_thickness, line_thickness, line_colour);
      render_rectangle(Vector2d( y + center.y,  x + center.x), line_thickness, line_thickness, line_colour);
      render_rectangle(Vector2d(-y + center.y,  x + center.x), line_thickness, line_thickness, line_colour);
      render_rectangle(Vector2d( y + center.y, -x + center.x), line_thickness, line_thickness, line_colour);
      render_rectangle(Vector2d(-y + center.y, -x + center.x), line_thickness, line_thickness, line_colour);

      render_line(Vector2d(-x + center.x,  y + center.y), Vector2d( x + center.x,  y + center.y), fill_colour, 1);
      render_line(Vector2d(-y + center.y,  x + center.x), Vector2d( y + center.y,  x + center.x), fill_colour, 1);
      render_line(Vector2d(-x + center.x, -y + center.y), Vector2d( x + center.x, - y + center.y), fill_colour, 1);
      render_line(Vector2d(-y + center.y, -x + center.x), Vector2d( y + center.y, - x + center.x), fill_colour, 1);

      if (abs(F + (2 * x) + 1) < abs(F + (2 * x) - (2 * y) + 2)) {
        x++;
        F = F + 2 * x;
      } else {
        x++;
        y--;
        F = F + 2 * x - 2 * y + 2;
      }
    }
    if(d::debug) cout << "end x: " << x << " end y: " << y << endl;
    render_circle(center, radius, line_colour, line_thickness);
    if(d::debug) cout << "DONE RENDERING FILLED CIRCLE" << endl;

	}
	virtual void render_triangle(Vector2d point1, Vector2d point2, Vector2d point3, ColourRGB line_colour, int line_thickness, ColourRGB fill_colour){
    if(d::debug) cout << "RENDERING TRIANGLE" << endl;
    if(d::debug) cout << "Points are as follows " << point1 << point2 << point3 << endl;
    vector<Vector2d> vertices = { point1, point2, point3 }; 
    auto cmp_x = [] (Vector2d a, Vector2d b) { return a.x < b.x; };

    sort(vertices.begin(), vertices.end(), cmp_x);

    if(d::debug) cout << "Points in order are as follows " << vertices[0] << vertices[1] << vertices[2] << endl;
    // line from start to middle 
	  auto line0 = render_and_return_line(vertices[0], vertices[1], line_colour, line_thickness);
    
    // line from start to end
	  auto line1 = render_and_return_line(vertices[0], vertices[2], line_colour, line_thickness);
    
    // line from middle to end
	  auto line2 = render_and_return_line(vertices[1], vertices[2], line_colour, line_thickness);
    
    sort(line0.begin(), line0.end(), cmp_x);
    sort(line1.begin(), line1.end(), cmp_x);
    sort(line2.begin(), line2.end(), cmp_x);
    // fill in the triangle
    int j = 0;
    int i = 0;
    bool midpoint = false;
    while (i < line1.size()) {
      if (line1[i].x < vertices[1].x) {
        render_vertical_line(line1[i].x, line1[i].y, line0[i].y, fill_colour, line_thickness);
      } else {
        if (!midpoint) {
          if(d::debug) cout << "MIDPOINT" << endl;
          midpoint = true;
        }
        render_vertical_line(line1[i].x, line1[i].y, line2[j].y, fill_colour, line_thickness);
        j++;
      }
      i++;
    }

    // redraw the edges to make it look all pretty
	  render_and_return_line(vertices[0], vertices[1], line_colour, line_thickness);
    
	  render_and_return_line(vertices[0], vertices[2], line_colour, line_thickness);
    
	  render_and_return_line(vertices[1], vertices[2], line_colour, line_thickness);
    
    if(d::debug) cout << "DONE RENDERING TRIANGLE" << endl;
	}

	virtual void render_gradient_triangle(Vector2d point1, Vector2d point2, Vector2d point3, ColourRGB line_colour, int line_thickness, ColourRGB colour1, ColourRGB colour2, ColourRGB colour3){
		// find the bounding box of the triangle // interate over all points 
		// if the point is inside the triangle
		// fill it with the color associated with its barycentric coordinates
    if(d::debug) cout << "RENDERING GRADIENT TRIANGLE" << endl;		
    if(d::debug) cout << "Rendering points " << point1 << point2 << point3 << endl;		
		vector<Vector2d> vertices = { point1, point2, point3 };
		
		auto min_x = accumulate(vertices.begin(), vertices.end(), vertices[0], 
      [] (Vector2d v1, Vector2d v2) { 
        return v1.x < v2.x ? v1 : v2; 
      }
    );
		auto min_y = accumulate(vertices.begin(), vertices.end(), vertices[0],
      [] (Vector2d v1, Vector2d v2) { 
        return v1.y < v2.y ? v1 : v2; 
      }
    );
		auto max_x = accumulate(vertices.begin(), vertices.end(), vertices[0],
      [] (Vector2d v1, Vector2d v2) { 
        return v1.x > v2.x ? v1 : v2; 
      }
    );
		auto max_y = accumulate(vertices.begin(), vertices.end(), vertices[0],
      [] (Vector2d v1, Vector2d v2) { 
        return v1.y > v2.y ? v1 : v2; 
      }
    );
		
		auto origin = Vector2d(min_x.x, min_y.y);
		
		auto width = max_x.x - min_x.x;
		auto height = max_y.y - min_y.y;

	  if(d::debug) cout << "Bounding rectangle is at " << origin << " with a width of " << width << " and a height of " << height << endl;	
		for (int i = origin.x; i < width + origin.x; ++i) {
			for (int j = origin.y; j < height + origin.y; ++j) {
				auto num1 = (point2.y - point3.y)*(i - point3.x) + (point3.x - point2.x)*(j - point3.y);
				auto den1 = (point2.y - point3.y)*(point1.x - point3.x) + (point3.x - point2.x)*(point1.y - point3.y);
				auto num2 = (point3.y - point1.y)*(i - point3.x) + (point1.x - point3.x)*(j - point3.y);
				auto den2 = (point2.y - point3.y)*(point1.x - point3.x) + (point3.x - point2.x)*(point1.y - point3.y);
				
				auto lambda_1 = (float)num1 / (float)den1;
				auto lambda_2 = (float)num2 / (float)den2;
				auto lambda_3 = 1 - lambda_1 - lambda_2;
			  if(d::debug) cout << "Converted coordinates " << lambda_1 << " " << lambda_2 << " " << lambda_3 << endl;	
				if (((lambda_1 + lambda_2 + lambda_3) <= 1) && (lambda_1 >= 0 && lambda_2 >= 0 && lambda_3 >= 0)) { // the point is in the triangle
          if(d::debug) cout << "Found point inside the triangle" << endl;

          int red = lambda_1*colour1.r + lambda_2*colour2.r + lambda_3*colour3.r;
          int gre = lambda_1*colour1.g + lambda_2*colour2.g + lambda_3*colour3.g;
          int blu = lambda_1*colour1.b + lambda_2*colour2.b + lambda_3*colour3.b;

					canvas[i][j] = ColourRGB(red, gre, blu);
				}
			}
		}

    // redraw the edges to make it look all pretty
	  render_and_return_line(vertices[0], vertices[1], line_colour, line_thickness);
    
	  render_and_return_line(vertices[0], vertices[2], line_colour, line_thickness);
    
	  render_and_return_line(vertices[1], vertices[2], line_colour, line_thickness);
    if(d::debug) cout << "DONE RENDERING GRADIENT TRIANGLE" << endl;		
	}
	
	void save_image(string filename){
		canvas.save_image(filename);
	}
private:
	PNG_Canvas canvas;
	int width,height;
};

int main(int argc, char** argv){
	if (argc < 3){
		cerr << "Usage: " << argv[0] << " <input file> <output file>" << endl;
		return 0;
	}
	string input_filename = argv[1];
	string output_filename = argv[2];
	
	BVGRenderer r;
	BVGReader reader(r);
	if (!reader.parse_file(input_filename)){
		cerr << "Unable to parse file" << endl;
		return 0;
	}
	r.save_image(output_filename);
	
	
}
