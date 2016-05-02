#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "Ray.h"
#include "hit.h"
#include "light.h"
#include "camera.h"
#include "group.h"
#include "scene_parser.h"
#include "image.h"
#include "raytracer.h"
#include "material.h"
#include "raytracing_status.h"
#include "glcanvas.h"
#include <process.h>

#include <GL/freeglut.h>

using namespace std;

const char * scene_file_name = "scene/scene5_rasterize_sphere_20x20x20.txt";
const char * output_file_name = "output.tga";
const char * depth_file_name = "depth.tga";
const char * normal_file_name = "normal.tga";
int x_res = 800;
int y_res = 800;
float min_depth = 0.0f;
float max_depth = 1.0f;

int bounce_limit = 0;
float cut_off_ray_weight = 0.0f;

bool cast_shadow = true;
bool grid_acceleration = false;
int grid_x = 5;
int grid_y = 5;
int grid_z = 5;
bool visualize_grid = false;

void dispSceneSummary(char *scene_file_name, char *output_file_name, int x_res,
		int y_res, char *depth_file_name, float min_depth, float max_depth,
		char *normal_file_name, int bounce_limit, float cut_off_ray_weight) {
	cout << "==============================" << endl;
	cout << "Scene Summary		  " << endl;
	cout << "---------------" << endl;
	cout << " Scene File: " << scene_file_name << endl;
	cout << " Output File: " << output_file_name << endl;
	cout << " X Res: " << x_res << endl;
	cout << " Y Res: " << y_res << endl;
	cout << " Depth File: " << depth_file_name << endl;
	cout << " min. output depth: " << min_depth << endl;
	cout << " max. output depth: " << max_depth << endl;
	cout << " Normal Visual File: " << normal_file_name << endl;
	cout << " Bounce upper-limit: " << bounce_limit << endl;
	cout << " Cut-off ray weight threshhold: " << cut_off_ray_weight << endl;
	cout << "==============================" << endl;
}

void RenderScene(SceneParser * sp) {
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("raycast");

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	RayTracingStats::Initialize(x_res, y_res,
		0, 0, 0,
		Vec3f(0.0f, 0.0f, 0.0f), Vec3f(0.0f, 0.0f, 0.0f));

	// ========================================================
	// ========================================================

	Camera *cam = sp->getCamera();
	Vec3f bg_color = sp->getBackgroundColor();
	Vec3f ambient = sp->getAmbientLight();
	Group *group = sp->getGroup();
	if (grid_acceleration) {
		Material *m = sp->getMaterial(0);
		group->EnableGridAccelerate(grid_x, grid_y, grid_z, m, visualize_grid);
	}
	RayTracer rt(sp, bounce_limit, cut_off_ray_weight, cast_shadow);

	// init output image files
	//Image img(x_res, y_res);
	//Image depth_img(x_res, y_res);
	//Image normal_img(x_res, y_res);

	GLint      ImageWidth;
	GLint      ImageHeight;
	GLint      PixelLength;
	GLfloat*   PixelData = NULL;

	ImageWidth = x_res;
	ImageHeight = y_res;
	PixelLength = ImageWidth * ImageHeight * 3;
	PixelData = new GLfloat[PixelLength];

	int i, j;
	float precalc = max_depth - min_depth;
	if (precalc == 0)
		precalc = 1;

	// Rendering Start
	int _index = 0;
	for (j =0; j < y_res; j++) {
		for (i = 0; i < x_res; i++) {
			float x = (float)i / x_res;
			float y = (float)j / y_res;
			Ray ray = cam->generateRay(Vec2f(x, y));
			Hit h(INFINITY, NULL, Vec3f(0.0f, 0.0f, 1.0f));

			float tmin = 0.0f;
			Vec3f pixel = rt.traceRay(ray, tmin, 0, 1.0, 1.0, h);
			//img.SetPixel(i, j, pixel);

			//float t = h.getT();
			//Vec3f n = h.getNormal();

			//if (t >= min_depth && t <= max_depth) {
			//	float volumn = (max_depth - t) / precalc;
			//	depth_img.SetPixel(i, j, Vec3f(volumn, volumn, volumn));
			//}

			//normal_img.SetPixel(i, j,
			//	Vec3f(fabs(n.x()), fabs(n.y()), fabs(n.z())));

			PixelData[_index++] = static_cast<GLfloat>(pixel.b());
			PixelData[_index++] = static_cast<GLfloat>(pixel.g());
			PixelData[_index++] = static_cast<GLfloat>(pixel.r());
		}
	}
	// Rendering End

	glClear(GL_COLOR_BUFFER_BIT);
	// »æÖÆÏñËØ
	if (PixelData)
		glDrawPixels(ImageWidth, ImageHeight, GL_BGR_EXT, GL_FLOAT, PixelData);

	glutSwapBuffers();

	// Output
	//if (output_file_name)
	//	img.SaveTGA(output_file_name);
	//if (depth_file_name)
	//	depth_img.SaveTGA(depth_file_name);
	//if (normal_file_name)
	//	normal_img.SaveTGA(normal_file_name);

	RayTracingStats::PrintStatistics();
}

int main(int argc, char** argv) {
	int err = 0;

	// sample command lines:
	// raycast -input input.txt -size 100 100 -output output.tga
	// raycast -input input.txt -size 100 100 -depth 5.5 8.8 output.tga

	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-input")) {
			i++;
			assert(i < argc);
			scene_file_name = argv[i];
		} else if (!strcmp(argv[i], "-size")) {
			i++;
			assert(i < argc);
			x_res = atoi(argv[i]);
			i++;
			assert(i < argc);
			y_res = atoi(argv[i]);
		} else if (!strcmp(argv[i], "-output")) {
			i++;
			assert(i < argc);
			output_file_name = argv[i];
		} else if (!strcmp(argv[i], "-depth")) {
			i++;
			assert(i < argc);
			min_depth = static_cast<float>(atof(argv[i]));
			i++;
			assert(i < argc);
			max_depth = static_cast<float>(atof(argv[i]));
			i++;
			assert(i < argc);
			depth_file_name = argv[i];

			if (min_depth > max_depth) {
				float tmp;
				tmp = max_depth;
				max_depth = min_depth;
				min_depth = tmp;
			}
		} else if (!strcmp(argv[i], "-normal")) {
			i++;
			assert(i < argc);
			normal_file_name = argv[i];
		} else if (!strcmp(argv[i], "-bounces")) {
			i++;
			assert(i < argc);
			bounce_limit = atoi(argv[i]);
		} else if (!strcmp(argv[i], "-weight")) {
			i++;
			assert(i < argc);
			cut_off_ray_weight = static_cast<float>(atof(argv[i]));
		} else if (!strcmp(argv[i], "-no_shadows")) {
			cast_shadow = false;
		} else if (!strcmp(argv[i], "-grid")) {
			i++;
			assert(i < argc);
			grid_x = atoi(argv[i]);
			i++;
			assert(i < argc);
			grid_y = atoi(argv[i]);
			i++;
			assert(i < argc);
			grid_z = atoi(argv[i]);

			grid_acceleration = (grid_x * grid_y * grid_z != 0);
		} else if (!strcmp(argv[i], "-visualize_grid")) {
			visualize_grid = true;
		} else {
			cerr << "whoops error with command line argument " << i <<	argv[i] << endl;
			assert(0);
		}
	}

	SceneParser sp(scene_file_name);
	GLCanvas canvas;

	// ========================================================
	// ========================================================
	glutInit(&argc, argv);

	canvas.initialize(&sp, RenderScene);

	return err;
}
