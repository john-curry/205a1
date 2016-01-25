/* draw_bvg.cpp
   B. Bird - 01/02/2016
*/

#include <string>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include "vector2d.h"
#include "colourRGB.h"
#include "bvg.h"
#include "png_canvas.h"
#include "debug.h"

using namespace std;

class BVGRenderer: public BVGRendererBase{
public:
	virtual void create_canvas(Vector2d dimensions, ColourRGB background_colour, int scale_factor){
		cout << "Canvas " << dimensions << background_colour << scale_factor << endl;
		width = dimensions[0];
		height = dimensions[1];
		canvas.initialize_canvas(dimensions[0],dimensions[1]);
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				canvas[x][y] = background_colour;
        if (x % 100 == 0 || y % 100 == 0) {
          canvas[x][y] = ColourRGB(255, 255, 255);
        }
      }
    }
	}
  vector<Vector2d> render_steep_line(Vector2d endpoint1, Vector2d endpoint2, ColourRGB colour, int thickness){
    vector<Vector2d> ret;
    cout << "START RENDERING STEEPLINE" << endl;
    int F = 0;
    int x = endpoint1.x;
    int y = endpoint1.y;
    int end = endpoint2.y;
    auto L = Vector2d(endpoint2.x - endpoint1.x, endpoint2.y - endpoint1.y);

    cout << "Starting x: " << x << " and starting y: "<< y << endl;
    cout << "Ending x: " << endpoint2.x  << " and ending y: "<< endpoint2.y << endl;

    while (y < end) {
      canvas[x][y] = colour;

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
    cout << "Ending up at x: " << x << " and y: " << y << endl;
    cout << "DONE RENDERING STEEP LINE"<< endl;
    return ret;
  }

 vector<Vector2d> render_positive_line(Vector2d endpoint1, Vector2d endpoint2, ColourRGB colour, int thickness){
    vector<Vector2d> ret;
    int F = 0;
    int x = endpoint1.x;
    int y = endpoint1.y;
    int end = endpoint2.x;
    auto L = Vector2d(endpoint2.x - endpoint1.x, endpoint2.y - endpoint1.y);

    cout << "Starting x: " << x << " and starting y: "<< y << endl;
    cout << "Ending x: " << endpoint2.x  << " and ending y: "<< endpoint2.y << endl;

    while (x < end) {
      ret.push_back(Vector2d(x, y));
      canvas[x][y] = colour;
      
      if (abs(F + L.y) < abs(F + (L.y - L.x))) {
        x++;
        F += L.y;
      } else {
        x++;
        y++;
        F += L.y - L.x;
      }
    }
    cout << "Ending up at x: " << x << " and y: " << y << endl;
    return ret;
  }

  
	vector<Vector2d> render_negative_line(Vector2d endpoint1, Vector2d endpoint2, ColourRGB colour, int thickness){
    vector<Vector2d> ret;
    cout << "START RENDERING NEGATIVE LINE" << endl;
    int F = 0;
    int x = endpoint1.x;
    int y = endpoint1.y;
    int end = endpoint2.x;
    auto L = Vector2d(endpoint2.x - endpoint1.x, endpoint2.y - endpoint1.y);

    cout << "Starting x: " << x << " and starting y: "<< y << endl;
    cout << "Ending x: " << endpoint2.x  << " and ending y: "<< endpoint2.y << endl;

    while (x < end && y >= 0) {
      canvas[x][y] = colour;
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
    cout << "Ending up at x: " << x << " and y: " << y << endl;
    cout << "DONE RENDERING NEGATIVE LINE" << endl;
    return ret;
  } 
	vector<Vector2d> render_negative_steep_line(Vector2d endpoint1, Vector2d endpoint2, ColourRGB colour, int thickness){
  	vector<Vector2d> ret;
    cout << "START RENDERING NEGATIVE STEEP LINE " << endl;
    int F = 0;
    int x = endpoint1.x;
    int y = endpoint1.y;
    int end = endpoint2.y;
    auto L = Vector2d(endpoint2.x - endpoint1.x, endpoint2.y - endpoint1.y);

    cout << "Starting x: " << x << " and starting y: "<< y << endl;
    cout << "Ending x: " << endpoint2.x  << " and ending y: "<< endpoint2.y << endl;

    while (y >= end) {
      canvas[x][y] = colour;

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
    cout << "Ending up at x: " << x << " and y: " << y << endl;
    cout << "DONE RENDERING NEGATIVE STEEP LINE" << endl;
    return ret;
  } 
 vector<Vector2d> render_vertical_line(int x, int start_y, int end_y, ColourRGB colour, int thickness) {
   cout << "RENDERING VERTICAL LINE " << endl;
  	vector<Vector2d> ret;
    if (start_y < end_y) {
      int i = 0;
      cout << "Going from start y: " << start_y << " to end y: " << end_y <<  " on x: " <<  x << endl; 
      for (i = start_y; i < end_y; ++i) {
        canvas[x][i] = colour;
        ret.push_back(Vector2d(x, i));
      }
      cout << "Ended up at y: " << i << endl;
    } else {
      cout << "Going from start y: " << end_y << " to end y: " << start_y <<  " on x: " <<  x << endl; 
      int i = 0;
      for (i = end_y; i < start_y; ++i) {
        canvas[x][i] = colour;
        ret.push_back(Vector2d(x, i));
      }
      cout << "Ended up at y: " << i << endl;
    }
    cout << "DONE RENDERING VERTICAL LINE " << endl;
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
      canvas[endpoint1.x][endpoint1.y] = colour;
      ret = { endpoint1 };
      return ret;
    }

    if (delta_x == 0) {
      ret = render_vertical_line(endpoint1.x, endpoint1.y, endpoint2.y, colour, thickness);   
      return ret;
    }

    if ((delta_x < 0) ^ (delta_y < 0)) { 
      if (abs(delta_x) < abs(delta_y)) {
        if (delta_x < 0 && delta_y < 0) {
          ret = render_negative_steep_line(endpoint2, endpoint1, colour, thickness);
        } else {
          ret = render_negative_steep_line(endpoint1, endpoint2, colour, thickness);
        }
      } else {
        if (delta_x < 0 && delta_y < 0) {
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
    cout << "RENDERING CIRCLE" << endl;
    int F = 0;
    int x = 0;
    int y = radius;
    cout << "start x: " << x << " start y: " << y << endl;
    while (x <= y) {
      canvas[ x + center.x][ y + center.y] = line_colour;
      canvas[-x + center.x][ y + center.y] = line_colour;
      canvas[ x + center.x][-y + center.y] = line_colour;
      canvas[-x + center.x][-y + center.y] = line_colour;
      canvas[ y + center.y][ x + center.x] = line_colour;
      canvas[-y + center.y][ x + center.x] = line_colour;
      canvas[ y + center.y][-x + center.x] = line_colour;
      canvas[-y + center.y][-x + center.x] = line_colour;
      if (abs(F + (2 * x) + 1) < abs(F + (2 * x) - (2 * y) + 2)) {
        x++;
        F = F + 2 * x;
      } else {
        x++;
        y--;
        F = F + 2 * x - 2 * y + 2;
      }
    }
    cout << "end x: " << x << " end y: " << y << endl;
    cout << "DONE RENDERING CIRCLE" << endl;
	}
	virtual void render_filledcircle(Vector2d center, int radius, ColourRGB line_colour, int line_thickness, ColourRGB fill_colour){
    cout << "RENDERING FILLED CIRCLE" << endl;
    int F = 0;
    int x = 0;
    int y = radius;
    cout << "start x: " << x << " start y: " << y << endl;
    while (x <= y) {
      canvas[ x + center.x][ y + center.y] = fill_colour;
      canvas[-x + center.x][ y + center.y] = fill_colour;
      canvas[ x + center.x][-y + center.y] = fill_colour;
      canvas[-x + center.x][-y + center.y] = fill_colour;
      canvas[ y + center.y][ x + center.x] = fill_colour;
      canvas[-y + center.y][ x + center.x] = fill_colour;
      canvas[ y + center.y][-x + center.x] = fill_colour;
      canvas[-y + center.y][-x + center.x] = fill_colour;

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
    cout << "end x: " << x << " end y: " << y << endl;
    render_circle(center, radius, line_colour, line_thickness);
    cout << "DONE RENDERING FILLED CIRCLE" << endl;

	}
	virtual void render_triangle(Vector2d point1, Vector2d point2, Vector2d point3, ColourRGB line_colour, int line_thickness, ColourRGB fill_colour){
    cout << "RENDERING TRIANGLE" << endl;
    cout << "Points are as follows " << point1 << point2 << point3 << endl;
    vector<Vector2d> vertices = { point1, point2, point3 }; 
    
    sort(vertices.begin(), vertices.end(), [] (auto a, auto b) { return a.x < b.x; });

    cout << "Points in order are as follows " << vertices[0] << vertices[1] << vertices[2] << endl;
    // line from start to middle 
	  auto line0 = render_and_return_line(vertices[0], vertices[1], line_colour, line_thickness);
    
    // line from start to end
	  auto line1 = render_and_return_line(vertices[0], vertices[2], line_colour, line_thickness);
    
    // line from middle to end
	  auto line2 = render_and_return_line(vertices[1], vertices[2], line_colour, line_thickness);
    
    sort(line0.begin(), line0.end(), [] (auto a, auto b) { return a.x < b.x; });
    sort(line1.begin(), line1.end(), [] (auto a, auto b) { return a.x < b.x; });
    sort(line2.begin(), line2.end(), [] (auto a, auto b) { return a.x < b.x; });

    // fill in the triangle
    int j = 0;
    int i = 0;
    bool midpoint = false;
    while (i < line1.size()) {
      if (line1[i].x < vertices[1].x) {
        render_vertical_line(line1[i].x, line1[i].y, line0[i].y, fill_colour, line_thickness);
      } else {
        if (!midpoint) {
          cout << "MIDPOINT" << endl;
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
    
    cout << "DONE RENDERING TRIANGLE" << endl;
	}

	virtual void render_gradient_triangle(Vector2d point1, Vector2d point2, Vector2d point3, ColourRGB line_colour, int line_thickness, ColourRGB colour1, ColourRGB colour2, ColourRGB colour3){
		cout << "Triangle " << point1 << point2 << point3 << line_colour << line_thickness << colour1 << colour2 << colour3 << endl;
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
