#include<GL/glut.h>
#include<bits/stdc++.h>
#include "bitmap_image.hpp"
using namespace std;

// Point Structure
struct Point3D
{
    double x_val;
    double y_val;
    double z_val;

    // Constructor
    Point3D(){
        x_val = 0;
        y_val = 0;
        z_val = 0;
    }

    Point3D(double x, double y, double z){
        x_val = x;
        y_val = y;
        z_val = z;
    }

    // Copy Constructor
    Point3D(const Point3D& point){
        x_val = point.x_val;
        y_val = point.y_val;
        z_val = point.z_val;
    }

    // Normalize Point
    void normalizePoint(){
        double length = sqrt(x_val * x_val + y_val * y_val + z_val * z_val);
        x_val /= length;
        y_val /= length;
        z_val /= length;
    }
};


// Add Two Points
Point3D addTwoPoints(Point3D point1, Point3D point2){
    Point3D point3 = Point3D(point1.x_val + point2.x_val, point1.y_val + point2.y_val, point1.z_val + point2.z_val);
    return point3;
}


// Subtract Two Points
Point3D subtractTwoPoints(Point3D point1, Point3D point2){
    Point3D point3 = Point3D(point1.x_val - point2.x_val, point1.y_val - point2.y_val, point1.z_val - point2.z_val);
    return point3;
}


// Multiply Point with a Number
Point3D multiplyPointWithNumber(Point3D point1, double num){
    Point3D point2 = Point3D(point1.x_val * num, point1.y_val * num, point1.z_val * num);
    return point2;
}


// Divide Point with a Number
Point3D dividePointByNumber(Point3D point1, double num){
    Point3D point2 = Point3D(point1.x_val / num, point1.y_val / num, point1.z_val / num);
    return point2;
}


// Multiply Two Points
Point3D multiplyTwoPoints(Point3D point1, Point3D point2){
    Point3D point3 = Point3D(point1.y_val * point2.z_val - point1.z_val * point2.y_val, point1.z_val * point2.x_val - point1.x_val * point2.z_val, point1.x_val * point2.y_val - point1.y_val * point2.x_val);
    return point3;
}


// // Global Variables
// int window_width = 800, window_height = 800, window_x_pos = 70, window_y_pos = 70;
// char title[] = "Magic Cube";
// GLfloat rotateAngle = 45.0;
// double maxLength = 1.5, curLength = 1.5, changeRate = 0.1, fixedRadius = 0.577, radius = 0;
double changeRate = 0.1;
// int rowPoints = 100;


// Camera Implementation
struct Camera
{
    Point3D eye_pos, lookat_dir, right_dir, up_dir;

    // Constructor
    Camera(){
        eye_pos = Point3D(300, 50, 100);
        lookat_dir = Point3D(-1, 0, 0);
        right_dir = Point3D(0, 1, 0);
        up_dir = Point3D(0, 0, 1);
    }

    // Look Function
    void startLooking(){
        gluLookAt(eye_pos.x_val, eye_pos.y_val, eye_pos.z_val, eye_pos.x_val + lookat_dir.x_val, eye_pos.y_val + lookat_dir.y_val, eye_pos.z_val + lookat_dir.z_val, up_dir.x_val, up_dir.y_val, up_dir.z_val);
    }

    // Eye Position Change
    void moveForward(){
        eye_pos = addTwoPoints(eye_pos, lookat_dir);
    }

    void moveBackward(){
        eye_pos = subtractTwoPoints(eye_pos, lookat_dir);
    }

    void moveRight(){
        eye_pos = addTwoPoints(eye_pos, right_dir);
    }

    void moveLeft(){
        eye_pos = subtractTwoPoints(eye_pos, right_dir);
    }

    void moveUp(){
        eye_pos = addTwoPoints(eye_pos, up_dir);
    }

    void moveDown(){
        eye_pos = subtractTwoPoints(eye_pos, up_dir);
    }

    // Position Change
    void lookLeft(){
        right_dir = addTwoPoints(multiplyPointWithNumber(right_dir, cos(changeRate)), multiplyPointWithNumber(lookat_dir, sin(changeRate)));
        lookat_dir = subtractTwoPoints(multiplyPointWithNumber(lookat_dir, cos(changeRate)), multiplyPointWithNumber(right_dir, sin(changeRate)));
    }

    void lookRight(){
        right_dir = addTwoPoints(multiplyPointWithNumber(right_dir, cos(-changeRate)), multiplyPointWithNumber(lookat_dir, sin(-changeRate)));
        lookat_dir = subtractTwoPoints(multiplyPointWithNumber(lookat_dir, cos(-changeRate)), multiplyPointWithNumber(right_dir, sin(-changeRate)));
    }

    void lookUp(){
        lookat_dir = addTwoPoints(multiplyPointWithNumber(lookat_dir, cos(changeRate)), multiplyPointWithNumber(up_dir, sin(changeRate)));
        up_dir = subtractTwoPoints(multiplyPointWithNumber(up_dir, cos(changeRate)), multiplyPointWithNumber(lookat_dir, sin(changeRate)));
    }

    void lookDown(){
        lookat_dir = addTwoPoints(multiplyPointWithNumber(lookat_dir, cos(-changeRate)), multiplyPointWithNumber(up_dir, sin(-changeRate)));
        up_dir = subtractTwoPoints(multiplyPointWithNumber(up_dir, cos(-changeRate)), multiplyPointWithNumber(lookat_dir, sin(-changeRate)));
    }

    void tiltCounterClockwise(){
        up_dir = addTwoPoints(multiplyPointWithNumber(up_dir, cos(changeRate)), multiplyPointWithNumber(right_dir, sin(changeRate)));
        right_dir = subtractTwoPoints(multiplyPointWithNumber(right_dir, cos(changeRate)), multiplyPointWithNumber(up_dir, sin(changeRate)));
    }

    void tiltClockwise(){
        up_dir = addTwoPoints(multiplyPointWithNumber(up_dir, cos(-changeRate)), multiplyPointWithNumber(right_dir, sin(-changeRate)));
        right_dir = subtractTwoPoints(multiplyPointWithNumber(right_dir, cos(-changeRate)), multiplyPointWithNumber(up_dir, sin(-changeRate)));
    }

};


// Point Light Object
struct PointLight{
    Point3D light_pos;
    double color[3];

    // Constructor
    PointLight(){
        light_pos = Point3D(0, 0, 0);
        color[0] = 1;
        color[1] = 1;
        color[2] = 1;
    }

    PointLight(Point3D pos, double r, double g, double b){
        light_pos = pos;
        color[0] = r;
        color[1] = g;
        color[2] = b;
    }

    // Draw Point Light
    void drawPointLight(){
        glPushMatrix();
        glPointSize(5);
        glBegin(GL_POINTS);
        glColor3f(color[0], color[1], color[2]);
        glVertex3f(light_pos.x_val, light_pos.y_val, light_pos.z_val);
        glEnd();
        glPopMatrix();
    }

};


// Spot Light Object
struct SpotLight{
    PointLight point_light;
    Point3D light_direction;
    double cutoff_angle;

    // Constructor
    SpotLight(){
        point_light = PointLight();
        light_direction = Point3D(0, 0, 0);
        cutoff_angle = 30;
    }

    SpotLight(PointLight pl, Point3D dir, double angle){
        point_light = pl;
        light_direction = dir;
        cutoff_angle = angle;
    }

    // Draw Spot Light
    void drawSpotLight(){
        point_light.drawPointLight();
    }
};


// Ray Object
struct Ray{
    Point3D start_point;
    Point3D direction;

    // Constructor
    Ray(){
        start_point = Point3D(0, 0, 0);
        direction = Point3D(0, 0, 0);
    }

    Ray(Point3D start, Point3D dir){
        start_point = start;
        dir.normalizePoint();
        direction = dir;
    }

};


class Object;

extern vector <Object*> objects;
extern vector <PointLight*> pointLights;
extern vector <SpotLight*> spotlights;
extern int level;

class Object{
    public:
        Point3D reference_point; // should have x, y, z
        double height, width, length, radius;
        double color[3];
        double coEfficients[4]; //ambient,diffuse,reflection coefficients
        int shine; // exponent term of specular component

        Object(){
            reference_point = Point3D(0,0,0);
            height = 0;
            width = 0;
            length = 0;
            color[0] = 0;
            color[1] = 0;
            color[2] = 0;
            coEfficients[0] = 0;
            coEfficients[1] = 0;
            coEfficients[2] = 0;
            coEfficients[3] = 0;
            shine = 0;
        }

        virtual void draw() = 0;

        void setColor(double color[]){
            for(int i = 0; i < 3; i++){
                this->color[i] = color[i];
            }
        }

        void setShine(int shine){
            this->shine = shine;
        }

        void setCoEfficients(double coEfficients[]){
            for(int i = 0; i < 4; i++){
                this->coEfficients[i] = coEfficients[i];
            }
        }

};


// Triangle class inheriting Object
class Triangle: public Object{
    public:
        Point3D points[3];

        //Constructors
        Triangle(){
            points[0] = Point3D(0, 0, 0);
            points[1] = Point3D(0, 0, 0);
            points[2] = Point3D(0, 0, 0);
        }

        Triangle(Point3D p0, Point3D p1, Point3D p2){
            points[0] = p0;
            points[1] = p1;
            points[2] = p2;
        }

        virtual void draw(){
            glColor3f(color[0], color[1], color[2]);
            glBegin(GL_TRIANGLES);
            {
                glVertex3f(points[0].x_val, points[0].y_val, points[0].z_val);
                glVertex3f(points[1].x_val, points[1].y_val, points[1].z_val);
                glVertex3f(points[2].x_val, points[2].y_val, points[2].z_val);
            }
            glEnd();
        }

};



// Sphere class inheriting Object
class Sphere: public Object{
    public:
        // Constructors
        Sphere(){
            reference_point = Point3D(0, 0, 0);
            radius = 0;
        }

        Sphere(Point3D p, double r){
            reference_point = p;
            radius = r;
        }


        virtual void draw(){
            int stacks = 32, sectors = 16;
            struct Point3D spherePoints[stacks + 1][sectors + 1];

            // Generate Points
            for(int i = 0; i <= stacks; i++)
            {
                double height = radius * sin(((i * 1.0) / stacks) * acos(-1) / 2);
                double r = radius * cos(((i * 1.0) / stacks) * acos(-1) / 2);

                for(int j = 0; j <= sectors; j++)
                {
                    spherePoints[i][j] = Point3D(r * cos(((j * 1.0) / sectors) * 2 * acos(-1)), r * sin(((j * 1.0) / sectors) * 2 * acos(-1)), height);

                }
            }

            // Drawing Sphere Using Quads
            for(int i = 0; i < stacks; i++){
                glPushMatrix();
                glTranslatef(reference_point.x_val, reference_point.y_val, reference_point.z_val);
                glColor3f(color[0], color[1], color[2]);

                for(int j = 0; j < sectors; j++)
                {
                    // For Upper Sphere
                    glBegin(GL_QUADS);
                        glVertex3f(spherePoints[i][j].x_val, spherePoints[i][j].y_val, spherePoints[i][j].z_val);
                        glVertex3f(spherePoints[i][j + 1].x_val, spherePoints[i][j + 1].y_val, spherePoints[i][j + 1].z_val);
                        glVertex3f(spherePoints[i + 1][j + 1].x_val, spherePoints[i + 1][j + 1].y_val, spherePoints[i + 1][j + 1].z_val);
                        glVertex3f(spherePoints[i + 1][j].x_val, spherePoints[i + 1][j].y_val, spherePoints[i + 1][j].z_val);
                    glEnd();

                    // For Lower Sphere
                    glBegin(GL_QUADS);
                        glVertex3f(spherePoints[i][j].x_val, spherePoints[i][j].y_val, -spherePoints[i][j].z_val);
                        glVertex3f(spherePoints[i][j + 1].x_val, spherePoints[i][j + 1].y_val, -spherePoints[i][j + 1].z_val);
                        glVertex3f(spherePoints[i + 1][j + 1].x_val, spherePoints[i + 1][j + 1].y_val, -spherePoints[i + 1][j + 1].z_val);
                        glVertex3f(spherePoints[i + 1][j].x_val, spherePoints[i + 1][j].y_val, -spherePoints[i + 1][j].z_val);
                    glEnd();
                }
                glPopMatrix();
            }     
        }
};


// General Object class inheriting Object
class GeneralObject: public Object{
    public:
        double A, B, C, D, E, F, G, H, I, J;

        // Constructors
        GeneralObject(){
            A = 0;
            B = 0;
            C = 0;
            D = 0;
            E = 0;
            F = 0;
            G = 0;
            H = 0;
            I = 0;
            J = 0;
        }

        // General Object Constructor
        GeneralObject(double a, double b, double c, double d, double e, double f, double g, double h, double i, double j){
            A = a;
            B = b;
            C = c;
            D = d;
            E = e;
            F = f;
            G = g;
            H = h;
            I = i;
            J = j;
        }

        virtual void draw(){
            return;
        }

};


// Floor class inheriting Object
class Floor: public Object{
    public:
        int totalNumberOfTiles;

        //Constructor
        Floor(){
            reference_point = Point3D(0, 0, 0);
            length = 2;
            totalNumberOfTiles = 1;
        }

        Floor(double floorWidth, double tileWidth){
            reference_point = Point3D(-floorWidth / 2, -floorWidth / 2, 0);
            length = tileWidth;
            totalNumberOfTiles = floorWidth / tileWidth;
        }

        virtual void draw(){
            double x = reference_point.x_val, y = reference_point.y_val;
            for(int i = 0; i < totalNumberOfTiles; i++){
                for(int j = 0; j < totalNumberOfTiles; j++){
                    if((i + j) % 2 == 0){
                        glColor3f(1, 1, 1);
                    }
                    else{
                        glColor3f(0, 0, 0);
                    }
                    glBegin(GL_QUADS);
                    {
                        glVertex3f(x, y, 0);
                        glVertex3f(x + length, y, 0);
                        glVertex3f(x + length, y + length, 0);
                        glVertex3f(x, y + length, 0);
                    }
                    glEnd();
                    x += length;
                }
                x = reference_point.x_val;
                y += length;
            }
        }

};