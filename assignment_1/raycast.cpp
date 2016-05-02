#include <stdio.h>
#include <stdlib.h>
#include "Ray.h"
#include "hit.h"
#include "camera.h"
#include "group.h"
#include "scene_parser.h"
#include "image.h"

void dispSceneSummary(char *scene_file_name, char *output_file_name, int x_res, int y_res, char *depth_file_name, int min_depth, int max_depth)
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
  printf("\n==============================");
  printf("\n");
}

int main (int argc, char** argv)
{
  int err = 0;

  if (argc < 8)
    {
      printf("Usage: raycast scene_file outputfile x_res y_res depth_file min_depth max_depth\n");
    }
  else
    {
      char * scene_file_name = argv[1];
      char * output_file_name = argv[2];
      char * depth_file_name = argv[5];
      int x_res = atoi(argv[3]);
      int y_res = atoi(argv[4]);
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
      Camera* cam = sp.getCamera();
      Group* group = sp.getGroup();
      Vec3f bg_color = sp.getBackgroundColor();
      Image img(x_res, y_res);
      img.SetAllPixels(bg_color);
      Image depth_img(x_res, y_res);
      depth_img.SetAllPixels(Vec3f(0.0, 0.0, 0.0));

      int i, j;
      int precalc = max_depth - min_depth;
      if (precalc == 0) precalc = 1;
      dispSceneSummary(scene_file_name, output_file_name, x_res, y_res, depth_file_name, min_depth, max_depth);

      // Rendering Start
      for ( j = 0; j < y_res; j++)
	{
	  for ( i = 0; i < x_res; i++)
	    {
	      float x = (float) i * 2.0 / x_res - 1.0;
	      float y = (float) j * 2.0 / y_res -1.0;
	      Ray ray = cam->generateRay(Vec2f(x, y));
	      Hit h(INFINITY, bg_color);
	      float tmin = - INFINITY;
	      if (group->Intersect(ray, h, tmin))
		{
		  img.SetPixel(i, j, h.getColor());
		  
		  float t = h.getT();
		  if (t >= min_depth && t <= max_depth)
		    { 
		      float volumn = (max_depth - t) / precalc;
		      depth_img.SetPixel(i, j, Vec3f(volumn, volumn, volumn));
		    }
		}
	    }
	}
      // Rendering End

      // Output
      img.SaveTGA(output_file_name);
      depth_img.SaveTGA(depth_file_name);
      
    }
  return err;
}
