#include <stdio.h>
#include <cstring>

#include "scene_parser.h"

#include "orthographic_camera.h"  
#include "sphere.h"
#include "group.h"

// ====================================================================
// ====================================================================
// CONSTRUCTORS, DESTRUCTOR & INITIALIZE

SceneParser::SceneParser() {
  initialize();
}

SceneParser::SceneParser(const char* filename) {
  initialize();

  // open the file
  assert(filename != NULL);
  const char *ext = &filename[strlen(filename)-4];
  assert(!strcmp(ext,".txt"));
  file = fopen(filename,"r");
  assert (file != NULL);

  // parse the scene
  parseFile();

  // close the file
  fclose(file); 
  file = NULL;
}

SceneParser::~SceneParser() {
  if (group != NULL) 
    delete group;
  if (camera != NULL) 
    delete camera;
}

void SceneParser::initialize() {
  // initialize some reasonable default values
  group = NULL;
  camera = NULL;
  background_color = Vec3f(0.5,0.5,0.5);
  current_object_color = Vec3f(1,1,1);
  file = NULL;
}

// ====================================================================
// ====================================================================

void SceneParser::parseFile() {
  //
  // at the top level, the scene can have a camera, 
  // background color and a group of objects
  // (we will add lights and other things in future assignments)
  //
  char token[MAX_PARSER_TOKEN_LENGTH];    
  while (getToken(token)) { 
    if (!strcmp(token, "OrthographicCamera")) {
      camera = parseOrthographicCamera();
    } else if (!strcmp(token, "Background")) {
      parseBackground();
    } else if (!strcmp(token, "Group")) {
      group = parseGroup();
    } else {
      printf ("Unknown token in parseFile: '%s'\n", token);
      exit(0);
    }
  }
}

Group* SceneParser::parseGroup() {
  //
  // each group starts with an integer that specifies
  // the number of objects in the group
  //
  // the material node sets the color of all objects which 
  // follow, until the next material node
  //
  char token[MAX_PARSER_TOKEN_LENGTH];
  getToken(token); assert (!strcmp(token, "{"));

  // read in the number of objects
  int num_objects;
  getToken(token); assert (!strcmp(token, "num_objects"));
  fscanf(file,"%d", &num_objects);

  // ++++++++++++++++++++++++++++++++++++++++++++++++++++
  // CALLING ASSIGNMENT 1 CODE
  // create a new Group with the right number of objects
  Group *answer = new Group(num_objects);
  // ++++++++++++++++++++++++++++++++++++++++++++++++++++

  // read in the objects
  int count = 0;
  while (num_objects > count) {
    getToken(token); 
    if (!strcmp(token, "Material")) {
      parseMaterial();
    } else if (!strcmp(token, "Sphere")) {      
      Sphere *sphere = parseSphere();

      // ++++++++++++++++++++++++++++++++++++++++++++++++
      // CALLING ASSIGNMENT 1 CODE
      // add this object to the group
      answer->addObject(count,sphere);
      // ++++++++++++++++++++++++++++++++++++++++++++++++

      count++;
    } else {
      printf ("Unknown token in parseGroup: '%s'\n", token);
      exit(0);
    }   
  }
  getToken(token); assert (!strcmp(token, "}"));

  // return the group
  return answer;
}

// ====================================================================
// ====================================================================

Camera* SceneParser::parseOrthographicCamera() {
  char token[MAX_PARSER_TOKEN_LENGTH];
  float c0,c1,c2;
  float d0,d1,d2;
  float u0,u1,u2;
  float size;

  // read in the camera parameters
  getToken(token); assert (!strcmp(token, "{"));
  getToken(token); assert (!strcmp(token, "center"));
  fscanf(file,"%f %f %f", &c0,&c1,&c2);
  getToken(token); assert (!strcmp(token, "direction"));
  fscanf(file,"%f %f %f", &d0,&d1,&d2);
  getToken(token); assert (!strcmp(token, "up"));
  fscanf(file,"%f %f %f", &u0,&u1,&u2);
  getToken(token); assert (!strcmp(token, "size"));
  fscanf(file,"%f", &size);
  getToken(token); assert (!strcmp(token, "}"));

  Vec3f center = Vec3f(c0,c1,c2);
  Vec3f direction = Vec3f (d0,d1,d2);
  Vec3f up = Vec3f(u0,u1,u2);

  // ++++++++++++++++++++++++++++++++++++++++++++++++++++
  // CALLING ASSIGNMENT 1 CODE
  // create a new orthographic camera and return it
  return new OrthographicCamera(center,direction,up,size);
  // ++++++++++++++++++++++++++++++++++++++++++++++++++++
}


void SceneParser::parseBackground() {
  char token[MAX_PARSER_TOKEN_LENGTH];
  float c0,c1,c2;

  // read in the background color
  getToken(token); assert (!strcmp(token, "{"));  
  getToken(token); assert (!strcmp(token, "color"));

  fscanf(file,"%f %f %f",&c0,&c1,&c2);
  getToken(token); assert (!strcmp(token, "}"));  

  // set the background color
  background_color = Vec3f(c0,c1,c2);
}

// ====================================================================
// ====================================================================

void SceneParser::parseMaterial() {
  char token[MAX_PARSER_TOKEN_LENGTH];
  float c0,c1,c2;

  // read in the material color
  getToken(token); assert (!strcmp(token, "{"));
  getToken(token); assert (!strcmp(token, "diffuseColor"));
  fscanf(file,"%f %f %f", &c0,&c1,&c2);
  getToken(token); assert (!strcmp(token, "}"));

  // change the current object color
  // scoping for the materials is very simplistic,
  // and essentially ignores any tree hierarchy
  current_object_color = Vec3f(c0,c1,c2);
}


Sphere* SceneParser::parseSphere() {
  char token[MAX_PARSER_TOKEN_LENGTH];
  float c0,c1,c2;
  float radius;

  // read in the sphere parameters
  getToken(token); assert (!strcmp(token, "{"));
  getToken(token); assert (!strcmp(token, "center"));
  fscanf(file,"%f %f %f", &c0,&c1,&c2);
  getToken(token); assert (!strcmp(token, "radius"));
  fscanf(file,"%f", &radius);
  getToken(token); assert (!strcmp(token, "}"));

  Vec3f center = Vec3f(c0,c1,c2);
  Vec3f color = current_object_color;

  // ++++++++++++++++++++++++++++++++++++++++++++++++++++
  // CALLING ASSIGNMENT 1 CODE
  // create a new sphere object and return it
  return new Sphere(center,radius,color);
  // ++++++++++++++++++++++++++++++++++++++++++++++++++++
}

// ====================================================================
// ====================================================================

int SceneParser::getToken(char token[MAX_PARSER_TOKEN_LENGTH]) {
  // for simplicity, tokens must be separated by whitespace
  assert (file != NULL);
  int success = fscanf(file,"%s ",token);
  if (success == EOF) {
    token[0] = '\0';
    return 0;
  }
  return 1;
}

// ====================================================================
// ====================================================================

