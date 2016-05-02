#include <stdio.h>
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

void dispSceneSummary(char *scene_file_name, char *output_file_name, int x_res,
		int y_res, char *depth_file_name, int min_depth, int max_depth,
		char *normal_file_name, int bounce_limit, float cut_off_ray_weight) {
	printf("\n==============================");
	printf("\n Scene Summary		  ");
	printf("\n---------------");
	printf("\n Scene File: %s", scene_file_name);
	printf("\n Output File: %s", output_file_name);
	printf("\n X Res: %d", x_res);
	printf("\n Y Res: %d", y_res);
	printf("\n Depth File: %s", depth_file_name);
	printf("\n min. output depth: %d", min_depth);
	printf("\n max. output depth: %d", max_depth);
	printf("\n Normal Visual File: %s", normal_file_name);
	printf("\n Bounce upper-limit: %d", bounce_limit);
	printf("\n Cut-off ray weight threshhold: %f", cut_off_ray_weight);
	printf("\n==============================");
	printf("\n");
}

int main(int argc, char** argv) {
	int err = 0;

	char * scene_file_name = NULL;
	char * output_file_name = NULL;
	char * depth_file_name = NULL;
	char * normal_file_name = NULL;
	int x_res = 0;
	int y_res = 0;
	float min_depth = 0.0f;
	float max_depth = 0.0f;

	int bounce_limit = 0;
	float cut_off_ray_weight = 0;

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
			min_depth = atof(argv[i]);
			i++;
			assert(i < argc);
			max_depth = atof(argv[i]);
			i++;
			assert(i < argc);
			depth_file_name = argv[i];

			if (min_depth > max_depth) {
				int tmp;
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
			bounce_limit = atof(argv[i]);
		} else if (!strcmp(argv[i], "-weight")) {
			i++;
			assert(i < argc);
			cut_off_ray_weight = atof(argv[i]);
		} else {
			printf("whoops error with command line argument %d: '%s'\n", i,
					argv[i]);
			assert(0);
		}
	}

	// ========================================================
	// ========================================================

	SceneParser sp(scene_file_name);
	Camera *cam = sp.getCamera();
	Vec3f bg_color = sp.getBackgroundColor();
	Vec3f ambient = sp.getAmbientLight();
	RayTracer rt(&sp, bounce_limit, cut_off_ray_weight);

	// init output image files
	Image img(x_res, y_res);
	Image depth_img(x_res, y_res);
	Image normal_img(x_res, y_res);

	int i, j;
	float precalc = max_depth - min_depth;
	if (precalc == 0)
		precalc = 1;
	dispSceneSummary(scene_file_name, output_file_name, x_res, y_res,
			depth_file_name, min_depth, max_depth, normal_file_name,
			bounce_limit, cut_off_ray_weight);

	// Rendering Start
	for (j = 0; j < y_res; j++) {
		for (i = 0; i < x_res; i++) {
			float x = (float) i / x_res;
			float y = (float) j / y_res;
			Ray ray = cam->generateRay(Vec2f(x, y));
			Hit h(INFINITY, NULL, Vec3f(0.0f, 0.0f, 1.0f));

			float tmin = 0.0f;
			Vec3f pixel = rt.traceRay(ray, tmin, 0, 1.0, 1.0, h);
			img.SetPixel(i, j, pixel);

			float t = h.getT();
			Vec3f n = h.getNormal();

			if (t >= min_depth && t <= max_depth) {
				float volumn = (max_depth - t) / precalc;
				depth_img.SetPixel(i, j, Vec3f(volumn, volumn, volumn));
			}

			normal_img.SetPixel(i, j,
					Vec3f(fabs(n.x()), fabs(n.y()), fabs(n.z())));
		}
	}
	// Rendering End

	// Output
	if (output_file_name)
		img.SaveTGA(output_file_name);
	if (depth_file_name)
		depth_img.SaveTGA(depth_file_name);
	if (normal_file_name)
		normal_img.SaveTGA(normal_file_name);

	return err;
}
