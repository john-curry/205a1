/* draw_bvg.cpp
   B. Bird - 01/02/2016
*/


#include <string>
#include <iostream>
#include <cstdlib>
#include "vector2d.h"
#include "colourRGB.h"
#include "bvg.h"
#include "png_canvas.h"

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
      }
    }
	}
	virtual void render_line(Vector2d endpoint1, Vector2d endpoint2, ColourRGB colour, int thickness){
    // check which quadrent we are drawing in and what the slope is
    // if slope is too high flip x and y
    // reconfigure endpoints to be in top right quad
    // do algorithm
    // convert all points back to their respective quad

    // if the slope is to large switch x and y
    //  int delta_x = endpoint2.x - endpoint1.x;
    //  int delta_y = endpoint2.y - endpoint1.y;

    auto L = Vector2d(endpoint2.x - endpoint1.x, endpoint2.y - endpoint1.y);
    int x, y, end;
    x = endpoint1.y;
    y = endpoint1.x;
    end = endpoint2.y;

    int F = 0;
    cout << "Starting x: " << x << " and starting y: "<< y << endl;

    while (x <= end) {
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
	}

	virtual void render_circle(Vector2d center, int radius, ColourRGB line_colour, int line_thickness){
		cout << "Circle " << center << radius << line_colour << line_thickness << endl;
	}
	virtual void render_filledcircle(Vector2d center, int radius, ColourRGB line_colour, int line_thickness, ColourRGB fill_colour){
		cout << "Filled Circle " << center << radius << line_colour << line_thickness << fill_colour << endl;
	}
	virtual void render_triangle(Vector2d point1, Vector2d point2, Vector2d point3, ColourRGB line_colour, int line_thickness, ColourRGB fill_colour){
		cout << "Triangle " << point1 << point2 << point3 << line_colour << line_thickness << fill_colour << endl;
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
