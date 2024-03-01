#include<GL/glut.h>
#include<bits/stdc++.h>
#include "1905097_Header.h"
#include "bitmap_image.hpp"
using namespace std;


// Global Variables
int window_width = 800, window_height = 800, window_x_pos = 70, window_y_pos = 70, countImage = 0;
char title[] = "Ray Tracing";


int recursion_level;
int imageHeight,imageWidth;
bitmap_image image;

vector <Object*> objects;
vector <PointLight*> pointLights;
vector <SpotLight*> spotlights;

Camera camera;

void loadData(){

    int number, totalObjects, totalPointLights, totalSpotLights;

    ifstream in("scene.txt");
	in >> recursion_level >> number;

	imageWidth = number;
    imageHeight = number;

	in >> totalObjects;

	for(int i = 0;i < totalObjects; i++){
        string type;
        in >> type;

        Object *object;

        if(type == "triangle"){
            object = new Triangle();

            in >> ((Triangle *)object)->points[0].x_val >> ((Triangle *)object)->points[0].y_val >> ((Triangle *)object)->points[0].z_val;
            in >> ((Triangle *)object)->points[1].x_val >> ((Triangle *)object)->points[1].y_val >> ((Triangle *)object)->points[1].z_val;
            in >> ((Triangle *)object)->points[2].x_val >> ((Triangle *)object)->points[2].y_val >> ((Triangle *)object)->points[2].z_val;
            in >> ((Triangle *)object)->color[0] >> ((Triangle *)object)->color[1] >> ((Triangle *)object)->color[2];
            in >> ((Triangle *)object)->coEfficients[0] >> ((Triangle *)object)->coEfficients[1] >> ((Triangle *)object)->coEfficients[2] >> ((Triangle *)object)->coEfficients[3];
            in >> ((Triangle *)object)->shine;

            objects.push_back(object);
        }

        else if(type == "sphere"){
            object = new Sphere();

            in >> ((Sphere *)object)->reference_point.x_val >> ((Sphere *)object)->reference_point.y_val >> ((Sphere *)object)->reference_point.z_val;
            in >> ((Sphere *)object)->radius;
            in >> ((Sphere *)object)->color[0] >> ((Sphere *)object)->color[1] >> ((Sphere *)object)->color[2];
            in >> ((Sphere *)object)->coEfficients[0] >> ((Sphere *)object)->coEfficients[1] >> ((Sphere *)object)->coEfficients[2] >> ((Sphere *)object)->coEfficients[3];
            in >> ((Sphere *)object)->shine;

            objects.push_back(object);
        }

        else if(type == "general"){
            object = new GeneralObject();

            in >> ((GeneralObject *)object)->A >> ((GeneralObject *)object)->B >> ((GeneralObject *)object)->C >> ((GeneralObject *)object)->D >> ((GeneralObject *)object)->E >> ((GeneralObject *)object)->F >> ((GeneralObject *)object)->G >> ((GeneralObject *)object)->H >> ((GeneralObject *)object)->I >> ((GeneralObject *)object)->J;
            in >> ((GeneralObject *)object)->reference_point.x_val >> ((GeneralObject *)object)->reference_point.y_val >> ((GeneralObject *)object)->reference_point.z_val;
            in >> ((GeneralObject *)object)->length >> ((GeneralObject *)object)->width >> ((GeneralObject *)object)->height;
            in >> ((GeneralObject *)object)->color[0] >> ((GeneralObject *)object)->color[1] >> ((GeneralObject *)object)->color[2];
            in >> ((GeneralObject *)object)->coEfficients[0] >> ((GeneralObject *)object)->coEfficients[1] >> ((GeneralObject *)object)->coEfficients[2] >> ((GeneralObject *)object)->coEfficients[3];
            in >> ((GeneralObject *)object)->shine;

            objects.push_back(object);
        }
    }

    // Taking Input Point Lights
    in >> totalPointLights;
    for(int i = 0; i < totalPointLights; i++){
        PointLight *pointLight = new PointLight();

        in >> pointLight->light_pos.x_val >> pointLight->light_pos.y_val >> pointLight->light_pos.z_val;
        in >> pointLight->color[0] >> pointLight->color[1] >> pointLight->color[2];

        pointLights.push_back(pointLight);
    }

    // Taking Input Spot Lights
    in >> totalSpotLights;
    for(int i = 0; i < totalSpotLights; i++){
        SpotLight *spotLight = new SpotLight();

        in >> spotLight->point_light.light_pos.x_val >> spotLight->point_light.light_pos.y_val >> spotLight->point_light.light_pos.z_val;
        in >> spotLight->point_light.color[0] >> spotLight->point_light.color[1] >> spotLight->point_light.color[2];
        in >> spotLight->light_direction.x_val >> spotLight->light_direction.y_val >> spotLight->light_direction.z_val;
        in >> spotLight->cutoff_angle;

        spotlights.push_back(spotLight);
    }

	Object *floorObject = new Floor(1000, 20);
    double floorColor[3] = {0.5, 0.5, 0.5};
	floorObject->setColor(floorColor);
	double floorCoEfficients[4] = {0.4, 0.2, 0.2, 0.2};
	floorObject->setCoEfficients(floorCoEfficients);
	objects.push_back(floorObject);
}


// Initialize OpenGL
void initGL(){

    loadData();
	image = bitmap_image(imageWidth, imageHeight);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);
}


// Capture Function
void capture(){
    cout << "Starting Capturing Image" << endl;

    // Set initial image and background color to black
    for(int i = 0; i < imageWidth; i++){
        for(int j = 0; j < imageHeight; j++){
            image.set_pixel(i, j, 0, 0, 0);
        }
    }

    double angleOfView = 80;
    double planeDistance = (imageHeight / 2.0) / tan((angleOfView / 2.0) * acos(-1) / 180.0);  
    Point3D topLeft = camera.getTopLeftPoint(planeDistance, window_width, window_height);

    double du = (window_width * 1.0) / imageWidth;
    double dv = (window_height * 1.0) / imageHeight;

    topLeft = addTwoPoints(topLeft, multiplyPointWithNumber(camera.right_dir, du / 2.0));
    topLeft = subtractTwoPoints(topLeft, multiplyPointWithNumber(camera.up_dir, dv / 2.0));

    for(int i = 0; i < imageWidth; i++){
        for(int j = 0; j < imageHeight; j++){
            Point3D currentPixel = addTwoPoints(topLeft, multiplyPointWithNumber(camera.right_dir, i * du));
            currentPixel = subtractTwoPoints(currentPixel, multiplyPointWithNumber(camera.up_dir, j * dv));

            Ray ray = Ray(camera.eye_pos, subtractTwoPoints(currentPixel, camera.eye_pos));
            ray.direction.normalizePoint();
            double newColor[3] = {0.0, 0.0, 0.0};

            double tMin = 1000000000, nearestObject = -1;
            for(int k = 0; k < objects.size(); k++){
                double t = objects[k]->intersect(ray, newColor, 0);
                if(t > 0 && (t < tMin || nearestObject == -1)){
                    tMin = t;
                    nearestObject = k;
                }
            }

            if(nearestObject != -1){
                newColor[0] = 0.0;
                newColor[1] = 0.0;
                newColor[2] = 0.0;

                double t = objects[nearestObject]->intersect(ray, newColor, 1);
                newColor[0] = min(1.0, newColor[0]);
                newColor[0] = max(0.0, newColor[0]);
                newColor[1] = min(1.0, newColor[1]);
                newColor[1] = max(0.0, newColor[1]);
                newColor[2] = min(1.0, newColor[2]);
                newColor[2] = max(0.0, newColor[2]);

                image.set_pixel(i, j, newColor[0] * 255, newColor[1] * 255, newColor[2] * 255);
            }

        }
    }

    countImage++;
    image.save_image("Output_1" + to_string(countImage) + ".bmp");
	cout<<"Output Image Saved"<<endl;	
}


// Display Function
void display() {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);  
 
   glLoadIdentity();       

   camera.startLooking();

    for (int i = 0; i < objects.size(); i++){
		Object *object = objects[i];
		object->draw();
	}

    for (int i = 0; i < pointLights.size(); i++){
        PointLight *pointLight = pointLights[i];
        pointLight->drawPointLight();
    }

    for (int i = 0; i < spotlights.size(); i++){
        SpotLight *spotLight = spotlights[i];
        spotLight->drawSpotLight();
    }

   glutSwapBuffers();
}

void reshape(GLsizei width, GLsizei height) {
   
   if (height == 0) 
    height = 1;           

   GLfloat aspect = (GLfloat)width / (GLfloat)height;
   glViewport(0, 0, width, height);
 
   glMatrixMode(GL_PROJECTION);  
   glLoadIdentity();  

   gluPerspective(80.0f, aspect, 1.0f, 1000.0f);
}

void animate(){
    glutPostRedisplay();
}

// Keyboard Functionalities
void keyboardListener(unsigned char key, int x, int y){
    switch(key){

        case '0':
            capture();
            break;

        case '1':
            camera.lookLeft();
            break;

        case '2':
            camera.lookRight();
            break;

        case '3':
            camera.lookUp();
            break;

        case '4':
            camera.lookDown();
            break;

        case '5':
            camera.tiltCounterClockwise();
            break;

        case '6':
            camera.tiltClockwise();
            break; 

        default:
            break;
    }
    glutPostRedisplay();
}


// Special Key Functionalities
void specialKeyboardListener(int key, int x, int y){
    switch(key){
        case GLUT_KEY_UP:
            camera.moveForward();
            break;

        case GLUT_KEY_DOWN:
            camera.moveBackward();
            break;

        case GLUT_KEY_RIGHT:
            camera.moveRight();
            break;

        case GLUT_KEY_LEFT:
            camera.moveLeft();
            break;

        case GLUT_KEY_PAGE_UP:
            camera.moveUp();
            break;

        case GLUT_KEY_PAGE_DOWN:
            camera.moveDown();
            break;

        default:
            break;            
    }
    glutPostRedisplay();
}


int main(int argc, char** argv){
   glutInit(&argc, argv);

   glutInitWindowSize(window_width, window_height); 
   glutInitWindowPosition(window_x_pos, window_y_pos);
   glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
   glutCreateWindow(title);

   initGL();         

   glutDisplayFunc(display);     
   glutReshapeFunc(reshape);
   glutIdleFunc(animate);     
   glutKeyboardFunc(keyboardListener); 
   glutSpecialFunc(specialKeyboardListener);  
                  
   glutMainLoop();

   objects.clear();
   objects.shrink_to_fit();

   pointLights.clear();
   pointLights.shrink_to_fit();

   spotlights.clear();
   spotlights.shrink_to_fit();             
   return 0;
}