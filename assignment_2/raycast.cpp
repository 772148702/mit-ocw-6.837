#include <stdio.h>
#include <stdlib.h>
#include "Ray.h"
#include "hit.h"
#include "light.h"
#include "camera.h"
#include "group.h"
#include "scene_parser.h"
#include "image.h"

void dispSceneSummary(char *scene_file_name, char *output_file_name, int x_res, int y_res, char *depth_file_name, int min_depth, int max_depth, char *normal_file_name)
{
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
  printf("\n==============================");
  printf("\n");
}

int main (int argc, char** argv)
{
  int err = 0;

  if (argc < 9)
    {
      printf("Usage: raycast scene_file outputfile x_res y_res depth_file min_depth max_depth normal_file_name\n");
    }
  else
    {
      char * scene_file_name = argv[1];
      char * output_file_name = argv[2];
      char * depth_file_name = argv[5];
      char * normal_file_name = argv[8];
      int x_res = atoi(argv[3]);
      int y_res = atoi(argv[4]);
      if (x_res > y_res)
	x_res = y_res;
      else if (y_res > x_res)
	y_res = x_res;

      int min_depth = atoi(argv[6]);
      int max_depth = atoi(argv[7]);
      if (min_depth > max_depth)
	{
	  int tmp;
	  tmp = max_depth;
	  max_depth = min_depth;
	  min_depth = tmp;
	}

      SceneParser sp(scene_file_name);
      Camera *cam = sp.getCamera();
      Group *group = sp.getGroup();
      int light_num = sp.getNumLights();
      Vec3f bg_color = sp.getBackgroundColor();
      Vec3f ambient = sp.getAmbientLight();
      Image img(x_res, y_res);
      img.SetAllPixels(bg_color);
      Image depth_img(x_res, y_res);
      depth_img.SetAllPixels(Vec3f(0.0f, 0.0f, 0.0f));
      Image normal_img(x_res, y_res);
      depth_img.SetAllPixels(Vec3f(0.0f, 0.0f, 0.0f));

      int i, j, k;
      int precalc = max_depth - min_depth;
      if (precalc == 0) precalc = 1;
      dispSceneSummary(scene_file_name, output_file_name, x_res, y_res, depth_file_name, min_depth, max_depth, normal_file_name);

      // Rendering Start
      for ( j = 0; j < y_res; j++)
	{
	  for ( i = 0; i < x_res; i++)
	    {
	      float x = (float) i / x_res;
	      float y = (float) j / y_res;
	      Ray ray = cam->generateRay(Vec2f(x, y));
	      Hit h(INFINITY, bg_color, Vec3f(0.0f, 0.0f, 1.0f));
	      float tmin = 0.0f;
	      if (group->Intersect(ray, h, tmin))
		{
		  Vec3f color = h.getColor();
		  Vec3f pixel (ambient.r() * color.r(), ambient.g() * color.g(), ambient.b() * color.b());

		  float t = h.getT();
		  Vec3f p = ray.getOrigin() + ray.getDirection() * t;
		  Vec3f n = h.getNormal();
		  for ( k = 0; k < light_num; k++)
		    {
		      Light *light = sp.getLight(k);
		      Vec3f dir;
		      Vec3f col;
		      light->getIllumination(p, dir, col);
		      float diffuse = dir.Dot3(n);
		      if (diffuse < 0) diffuse = 0;
		      pixel += Vec3f(color.r() * col.r(), color.g() * col.g(), color.b() * col.b()) * diffuse;
		    }

		  img.SetPixel(i, j, pixel);
		  
		  if (t >= min_depth && t <= max_depth)
		    { 
		      float volumn = (max_depth - t) / precalc;
		      depth_img.SetPixel(i, j, Vec3f(volumn, volumn, volumn));
		    }

		  normal_img.SetPixel(i, j, Vec3f(fabs(n.x()), fabs(n.y()), fabs(n.z())));
		}
	    }
	}
      // Rendering End

      // Output
      img.SaveTGA(output_file_name);
      depth_img.SaveTGA(depth_file_name);
      normal_img.SaveTGA(normal_file_name);
    }
  return err;
}
