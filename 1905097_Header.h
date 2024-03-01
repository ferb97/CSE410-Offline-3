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

// Dot product of two points
double dotProductOfTwoPoints(Point3D point1, Point3D point2){
    return (point1.x_val * point2.x_val + point1.y_val * point2.y_val + point1.z_val * point2.z_val);
}


// // Global Variables
double changeRate = 0.1;


// Camera Implementation
struct Camera
{
    Point3D eye_pos, lookat_dir, right_dir, up_dir;

    // Constructor
    Camera(){
        eye_pos = Point3D(300, 50, 100);
        lookat_dir = Point3D(-1 / sqrt(2), -1 / sqrt(2), 0);
        right_dir = Point3D(-1 / sqrt(2), 1 / sqrt(2), 0);
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

    // Get Top Left Point
    Point3D getTopLeftPoint(double planeDistance, double window_width, double window_height){
        Point3D topLeftPoint = eye_pos;
        topLeftPoint = addTwoPoints(topLeftPoint, multiplyPointWithNumber(lookat_dir, planeDistance));
        topLeftPoint = addTwoPoints(topLeftPoint, multiplyPointWithNumber(right_dir, -window_width / 2));
        topLeftPoint = addTwoPoints(topLeftPoint, multiplyPointWithNumber(up_dir, window_height / 2));
        return topLeftPoint;
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

// Import necessary vectors
extern vector <Object*> objects;
extern vector <PointLight*> pointLights;
extern vector <SpotLight*> spotlights;
extern int recursion_level;


// Object Class
class Object{
    public:
        Point3D reference_point; // reference point of the object
        double height, width, length, radius;
        double color[3];
        double coEfficients[4]; // 0 = ambient, 1 = diffuse, 2 = specular, 3 = reflection
        int shine; // exponent term of specular component

        // Constructors
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

        // Virtual Functions
        virtual void draw() = 0;
        virtual Ray normalRayAtPoint(Point3D point, Ray ray) = 0;
        virtual double getIntersectingTValue(Ray ray) = 0;

        // The virtual functions getColorAt should be implemented in the floor class
        virtual double getColorRedAt(Point3D point){
            return color[0];
        }

        virtual double getColorGreenAt(Point3D point){
            return color[1];
        }  

        virtual double getColorBlueAt(Point3D point){
            return color[2];
        }

        // Setters
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

        // Intersect Function, which is common for all object classes
        double intersect(Ray ray, double (&newColor)[3], int level){

            // Get value of t
            double t = getIntersectingTValue(ray);

            if(t < 0){
                return -1;
            }

            // If level equals to zero, return the value of t
            if(level == 0){
                return t;
            }

            // Get the intersecting point
            Point3D intersectingPoint = addTwoPoints(ray.start_point, multiplyPointWithNumber(ray.direction, t));

            // Get the color at the intersecting point
            double colorAtIntersectingPoint[3];
            colorAtIntersectingPoint[0] = getColorRedAt(intersectingPoint);
            colorAtIntersectingPoint[1] = getColorGreenAt(intersectingPoint);
            colorAtIntersectingPoint[2] = getColorBlueAt(intersectingPoint);

            // Setting up ambient color
            for(int i = 0; i < 3; i++){
                newColor[i] = coEfficients[0] * colorAtIntersectingPoint[i];
            }

            // Calculate for each point light
            for(int i = 0; i < pointLights.size(); i++){

                // Get the direction of the light
                Point3D lightDirection = subtractTwoPoints(intersectingPoint, pointLights[i]->light_pos);
                double distanceWithLight = sqrt(lightDirection.x_val * lightDirection.x_val + lightDirection.y_val * lightDirection.y_val + lightDirection.z_val * lightDirection.z_val);
                if(distanceWithLight < 0.00001){
                    continue;
                }

                lightDirection.normalizePoint();
                Ray lightRay = Ray(pointLights[i]->light_pos, lightDirection);
                bool isShadow = false;

                for(int j = 0; j < objects.size(); j++){
                    double t1 = objects[j]->getIntersectingTValue(lightRay);
                    if(t1 > 0 && t1 + 0.00001 < distanceWithLight){
                        isShadow = true;
                        break;
                    }
                }

                if(!isShadow){
                    double lambert = dotProductOfTwoPoints(lightRay.direction, normalRayAtPoint(intersectingPoint, lightRay).direction) * (-1);
                    if(lambert < 0){
                        lambert = 0;
                    }

                    for(int j = 0; j < 3; j++){
                        newColor[j] += coEfficients[1] * colorAtIntersectingPoint[j] * lambert * pointLights[i]->color[j];
                    }

                    Point3D reflectionRayDirection = subtractTwoPoints(lightRay.direction, multiplyPointWithNumber(normalRayAtPoint(intersectingPoint, lightRay).direction, 2 * dotProductOfTwoPoints(normalRayAtPoint(intersectingPoint, lightRay).direction, lightRay.direction)));
                    Ray reflectionRay = Ray(intersectingPoint, reflectionRayDirection);
                    reflectionRay.direction.normalizePoint();

                    double phong = dotProductOfTwoPoints(reflectionRay.direction, ray.direction) * (-1);
                    if(phong < 0){
                        phong = 0;
                    }

                    for(int j = 0; j < 3; j++){
                        newColor[j] += coEfficients[2] * pow(phong, shine) * pointLights[i]->color[j] * colorAtIntersectingPoint[j];
                    }
                }
            }

            for(int i = 0; i < spotlights.size(); i++){
                Point3D lightDirection = subtractTwoPoints(intersectingPoint, spotlights[i]->point_light.light_pos);
                double distanceWithLight = sqrt(lightDirection.x_val * lightDirection.x_val + lightDirection.y_val * lightDirection.y_val + lightDirection.z_val * lightDirection.z_val);
                if(distanceWithLight < 0.00001){
                    continue;
                }

                lightDirection.normalizePoint();

                double numerator = dotProductOfTwoPoints(lightDirection, spotlights[i]->light_direction);
                double denominator = sqrt(lightDirection.x_val * lightDirection.x_val + lightDirection.y_val * lightDirection.y_val + lightDirection.z_val * lightDirection.z_val);
                denominator *= sqrt(spotlights[i]->light_direction.x_val * spotlights[i]->light_direction.x_val + spotlights[i]->light_direction.y_val * spotlights[i]->light_direction.y_val + spotlights[i]->light_direction.z_val * spotlights[i]->light_direction.z_val);

                double angle = acos(numerator / denominator) * (180 / acos(-1));

                if(fabs(angle) >= spotlights[i]->cutoff_angle){
                    continue;
                }
                Ray lightRay = Ray(spotlights[i]->point_light.light_pos, lightDirection);
                bool isShadow = false;

                for(int j = 0; j < objects.size(); j++){
                    double t1 = objects[j]->getIntersectingTValue(lightRay);
                    if(t1 > 0 && t1 + 0.00001 < distanceWithLight){
                        isShadow = true;
                        break;
                    }
                }

                if(!isShadow){
                    double lambert = dotProductOfTwoPoints(lightRay.direction, normalRayAtPoint(intersectingPoint, lightRay).direction) * (-1);
                    if(lambert < 0){
                        lambert = 0;
                    }

                    for(int j = 0; j < 3; j++){
                        newColor[j] += coEfficients[1] * colorAtIntersectingPoint[j] * lambert * spotlights[i]->point_light.color[j];
                    }

                    Point3D reflectionRayDirection = subtractTwoPoints(lightRay.direction, multiplyPointWithNumber(normalRayAtPoint(intersectingPoint, lightRay).direction, 2 * dotProductOfTwoPoints(normalRayAtPoint(intersectingPoint, lightRay).direction, lightRay.direction)));
                    Ray reflectionRay = Ray(intersectingPoint, reflectionRayDirection);
                    reflectionRay.direction.normalizePoint();

                    double phong = dotProductOfTwoPoints(reflectionRay.direction, ray.direction) * (-1);
                    if(phong < 0){
                        phong = 0;
                    }

                    for(int j = 0; j < 3; j++){
                        newColor[j] += coEfficients[2] * pow(phong, shine) * spotlights[i]->point_light.color[j] * colorAtIntersectingPoint[j];
                    }
                }

            }

            if(level < recursion_level){
                Point3D reflectionRayDirection = subtractTwoPoints(ray.direction, multiplyPointWithNumber(normalRayAtPoint(intersectingPoint, ray).direction, 2 * dotProductOfTwoPoints(normalRayAtPoint(intersectingPoint, ray).direction, ray.direction)));
                Ray reflectionRay = Ray(intersectingPoint, reflectionRayDirection);
                reflectionRay.direction.normalizePoint();

                reflectionRay.start_point = addTwoPoints(reflectionRay.start_point, multiplyPointWithNumber(reflectionRay.direction, 0.00001));

                double newColorAtIntersectingPoint[3] = {0.0, 0.0, 0.0};
                double t1 = -1;
                double minT = 1000000000;
                int minIndex = -1;

                for(int i = 0; i < objects.size(); i++){
                    t1 = objects[i]->intersect(reflectionRay, newColor, 0);
                    if(t1 > 0 && t1 < minT){
                        minT = t1;
                        minIndex = i;
                    }
                }

                if(minIndex != -1){
                    double t2 = objects[minIndex]->intersect(reflectionRay, newColorAtIntersectingPoint, level + 1);
                    for(int i = 0; i < 3; i++){
                        newColor[i] += coEfficients[3] * newColorAtIntersectingPoint[i];
                    }
                }
            }

            return t;
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

        // Draw Function for triangle
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

        // Calculating Normal at a point for a triangle
        virtual Ray normalRayAtPoint(Point3D point, Ray ray){
            Point3D normal = multiplyTwoPoints(subtractTwoPoints(points[1], points[0]), subtractTwoPoints(points[2], points[0]));
            normal.normalizePoint();

            if(dotProductOfTwoPoints(normal, ray.direction) < 0){
                normal = multiplyPointWithNumber(normal, -1);
            }

            // Normal Ray
            Ray normalRay = Ray(point, normal);
            return normalRay;
        }

        virtual double getIntersectingTValue(Ray ray){

            // All the matrices needed for intersection point calculation
            double betaMatrix[3][3], gammaMatrix[3][3], tMatrix[3][3], aMatrix[3][3];

            // Forming beta matrix
            betaMatrix[0][0] = points[0].x_val - ray.start_point.x_val;
            betaMatrix[0][1] = points[0].x_val - points[2].x_val;
            betaMatrix[0][2] = ray.direction.x_val;
            betaMatrix[1][0] = points[0].y_val - ray.start_point.y_val;
            betaMatrix[1][1] = points[0].y_val - points[2].y_val;
            betaMatrix[1][2] = ray.direction.y_val;
            betaMatrix[2][0] = points[0].z_val - ray.start_point.z_val;
            betaMatrix[2][1] = points[0].z_val - points[2].z_val;
            betaMatrix[2][2] = ray.direction.z_val;

            // Forming gamma matrix
            gammaMatrix[0][0] = points[0].x_val - points[1].x_val;
            gammaMatrix[0][1] = points[0].x_val - ray.start_point.x_val;
            gammaMatrix[0][2] = ray.direction.x_val;
            gammaMatrix[1][0] = points[0].y_val - points[1].y_val;
            gammaMatrix[1][1] = points[0].y_val - ray.start_point.y_val;
            gammaMatrix[1][2] = ray.direction.y_val;
            gammaMatrix[2][0] = points[0].z_val - points[1].z_val;
            gammaMatrix[2][1] = points[0].z_val - ray.start_point.z_val;
            gammaMatrix[2][2] = ray.direction.z_val;

            // Forming t matrix
            tMatrix[0][0] = points[0].x_val - points[1].x_val;
            tMatrix[0][1] = points[0].x_val - points[2].x_val;
            tMatrix[0][2] = points[0].x_val - ray.start_point.x_val;
            tMatrix[1][0] = points[0].y_val - points[1].y_val;
            tMatrix[1][1] = points[0].y_val - points[2].y_val;
            tMatrix[1][2] = points[0].y_val - ray.start_point.y_val;
            tMatrix[2][0] = points[0].z_val - points[1].z_val;
            tMatrix[2][1] = points[0].z_val - points[2].z_val;
            tMatrix[2][2] = points[0].z_val - ray.start_point.z_val;

            // Forming a matrix
            aMatrix[0][0] = points[0].x_val - points[1].x_val;
            aMatrix[0][1] = points[0].x_val - points[2].x_val;
            aMatrix[0][2] = ray.direction.x_val;
            aMatrix[1][0] = points[0].y_val - points[1].y_val;
            aMatrix[1][1] = points[0].y_val - points[2].y_val;
            aMatrix[1][2] = ray.direction.y_val;
            aMatrix[2][0] = points[0].z_val - points[1].z_val;
            aMatrix[2][1] = points[0].z_val - points[2].z_val;
            aMatrix[2][2] = ray.direction.z_val;

            // Get value of beta determinant
            double betaDeterminant = betaMatrix[0][0] * (betaMatrix[1][1] * betaMatrix[2][2] - betaMatrix[1][2] * betaMatrix[2][1]);
            betaDeterminant -= betaMatrix[0][1] * (betaMatrix[1][0] * betaMatrix[2][2] - betaMatrix[1][2] * betaMatrix[2][0]);
            betaDeterminant += betaMatrix[0][2] * (betaMatrix[1][0] * betaMatrix[2][1] - betaMatrix[1][1] * betaMatrix[2][0]);

            // Get value of gamma determinant
            double gammaDeterminant = gammaMatrix[0][0] * (gammaMatrix[1][1] * gammaMatrix[2][2] - gammaMatrix[1][2] * gammaMatrix[2][1]);
            gammaDeterminant -= gammaMatrix[0][1] * (gammaMatrix[1][0] * gammaMatrix[2][2] - gammaMatrix[1][2] * gammaMatrix[2][0]);
            gammaDeterminant += gammaMatrix[0][2] * (gammaMatrix[1][0] * gammaMatrix[2][1] - gammaMatrix[1][1] * gammaMatrix[2][0]);

            // Get value of t determinant
            double tDeterminant = tMatrix[0][0] * (tMatrix[1][1] * tMatrix[2][2] - tMatrix[1][2] * tMatrix[2][1]);
            tDeterminant -= tMatrix[0][1] * (tMatrix[1][0] * tMatrix[2][2] - tMatrix[1][2] * tMatrix[2][0]);
            tDeterminant += tMatrix[0][2] * (tMatrix[1][0] * tMatrix[2][1] - tMatrix[1][1] * tMatrix[2][0]);

            // Get value of a determinant
            double aDeterminant = aMatrix[0][0] * (aMatrix[1][1] * aMatrix[2][2] - aMatrix[1][2] * aMatrix[2][1]);
            aDeterminant -= aMatrix[0][1] * (aMatrix[1][0] * aMatrix[2][2] - aMatrix[1][2] * aMatrix[2][0]);
            aDeterminant += aMatrix[0][2] * (aMatrix[1][0] * aMatrix[2][1] - aMatrix[1][1] * aMatrix[2][0]);

            // Get value of t
            double t = tDeterminant / aDeterminant;
            double beta = betaDeterminant / aDeterminant;
            double gamma = gammaDeterminant / aDeterminant;

            // Check Conditions
            if(beta + gamma < 1 && beta > 0 && gamma > 0 && t > 0){
                return t;
            }
            else{
                return -1;
            }   
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


        // Draw Function for Sphere
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

        // Normal Ray at a point for a sphere
        virtual Ray normalRayAtPoint(Point3D point, Ray radius){
            Point3D normal = subtractTwoPoints(point, reference_point);
            normal.normalizePoint();
            Ray normalRay = Ray(point, normal);
            return normalRay;
        }

        // Get Intersecting T Value for a sphere
        virtual double getIntersectingTValue(Ray ray){
            double a = 1;
            double b = 2 * (ray.direction.x_val * (ray.start_point.x_val - reference_point.x_val) + ray.direction.y_val * (ray.start_point.y_val - reference_point.y_val) + ray.direction.z_val * (ray.start_point.z_val - reference_point.z_val));
            double c = (ray.start_point.x_val - reference_point.x_val) * (ray.start_point.x_val - reference_point.x_val) + (ray.start_point.y_val - reference_point.y_val) * (ray.start_point.y_val - reference_point.y_val) + (ray.start_point.z_val - reference_point.z_val) * (ray.start_point.z_val - reference_point.z_val) - radius * radius;

            // Calculate the discriminant
            double d = b * b - 4 * a * c;

            // If the discriminant is negative, there are no real roots
            if(d < 0){
                return -1;
            }
            // return the value of intersecting t
            else{
                double t1 = (-b + sqrt(d)) / (2 * a);
                double t2 = (-b - sqrt(d)) / (2 * a);
                if(t1 < 0 && t2 < 0){
                    return -1;
                }
                else if(t1 < 0){
                    return t2;
                }
                else if(t2 < 0){
                    return t1;
                }
                else{
                    return min(t1, t2);
                }
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

        // No Drawing as it is a general object
        virtual void draw(){
            return;
        }

        // Normal Ray at a point for a general object
        virtual Ray normalRayAtPoint(Point3D point, Ray ray){
            Point3D normal = Point3D(2 * A * point.x_val + D * point.y_val + E * point.z_val + G, 2 * B * point.y_val + D * point.x_val + F * point.z_val + H, 2 * C * point.z_val + E * point.x_val + F * point.y_val + I);
            normal.normalizePoint();
            Ray normalRay = Ray(point, normal);
            return normalRay;
        }

        virtual double getIntersectingTValue(Ray ray){

            double a = A * ray.direction.x_val * ray.direction.x_val; 
            a += B * ray.direction.y_val * ray.direction.y_val; 
            a += C * ray.direction.z_val * ray.direction.z_val;
            a += D * ray.direction.x_val * ray.direction.y_val;
            a += E * ray.direction.x_val * ray.direction.z_val;
            a += F * ray.direction.y_val * ray.direction.z_val;

            double b = 2 * (A * ray.start_point.x_val * ray.direction.x_val + B * ray.start_point.y_val * ray.direction.y_val + C * ray.start_point.z_val * ray.direction.z_val);
            b += D * (ray.start_point.x_val * ray.direction.y_val + ray.start_point.y_val * ray.direction.x_val);
            b += E * (ray.start_point.x_val * ray.direction.z_val + ray.start_point.z_val * ray.direction.x_val);
            b += F * (ray.start_point.y_val * ray.direction.z_val + ray.start_point.z_val * ray.direction.y_val);
            b += G * ray.direction.x_val;
            b += H * ray.direction.y_val;
            b += I * ray.direction.z_val;

            double c = A * ray.start_point.x_val * ray.start_point.x_val;
            c += B * ray.start_point.y_val * ray.start_point.y_val;
            c += C * ray.start_point.z_val * ray.start_point.z_val;
            c += D * ray.start_point.x_val * ray.start_point.y_val;
            c += E * ray.start_point.x_val * ray.start_point.z_val;
            c += F * ray.start_point.y_val * ray.start_point.z_val;
            c += G * ray.start_point.x_val;
            c += H * ray.start_point.y_val;
            c += I * ray.start_point.z_val;
            c += J;

            double d = b * b - 4 * a * c;
            if(d < 0){
                return -1;
            }

            if(fabs(a) < 0.00001){
                return -c / b;
            }
                double t1 = (-b + sqrt(d)) / (2 * a);
                double t2 = (-b - sqrt(d)) / (2 * a);
                double tMin = min(t1, t2);
                double tMax = max(t1, t2);

                if(tMin < 0 && tMax < 0){
                    return -1;
                }

                else if(tMin > 0){
                    Point3D intersectingPoint1 = addTwoPoints(ray.start_point, multiplyPointWithNumber(ray.direction, tMin));
                    if((length > 0.00001 && (intersectingPoint1.x_val < reference_point.x_val || intersectingPoint1.x_val > reference_point.x_val + length)) || (width > 0.00001 && (intersectingPoint1.y_val < reference_point.y_val || intersectingPoint1.y_val > reference_point.y_val + width)) || (height > 0.00001 && (intersectingPoint1.z_val < reference_point.z_val || intersectingPoint1.z_val > reference_point.z_val + height))){
                        Point3D intersectingPoint2 = addTwoPoints(ray.start_point, multiplyPointWithNumber(ray.direction, tMax));
                        if((length > 0.00001 && (intersectingPoint2.x_val < reference_point.x_val || intersectingPoint2.x_val > reference_point.x_val + length)) || (width > 0.00001 && (intersectingPoint2.y_val < reference_point.y_val || intersectingPoint2.y_val > reference_point.y_val + width)) || (height > 0.00001 && (intersectingPoint2.z_val < reference_point.z_val || intersectingPoint2.z_val > reference_point.z_val + height))){
                            return -1;
                        }
                        else{
                            return tMax;
                        }
                    }
                    else{
                        return tMin;
                    }
                }

                else{
                    Point3D intersectingPoint = addTwoPoints(ray.start_point, multiplyPointWithNumber(ray.direction, tMax));
                    if((length > 0.00001 && (intersectingPoint.x_val < reference_point.x_val || intersectingPoint.x_val > reference_point.x_val + length)) || (width > 0.00001 && (intersectingPoint.y_val < reference_point.y_val || intersectingPoint.y_val > reference_point.y_val + width)) || (height > 0.00001 && (intersectingPoint.z_val < reference_point.z_val || intersectingPoint.z_val > reference_point.z_val + height))){
                        return -1;
                    }
                    else{
                        return tMax;
                    }
                }
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
            reference_point = Point3D(-floorWidth / 2.0, -floorWidth / 2.0, 0);
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

        virtual Ray normalRayAtPoint(Point3D point, Ray ray){
            if(ray.direction.z_val > 0){
                Point3D normal = Point3D(0, 0, 1);
                normal.normalizePoint();
                Ray normalRay = Ray(point, normal);
                return normalRay;
            }
            else{
                Point3D normal = Point3D(0, 0, -1);
                normal.normalizePoint();
                Ray normalRay = Ray(point, normal);
                return normalRay;
            }   
        }

        virtual double getIntersectingTValue(Ray ray){
            if(ray.direction.z_val == 0){
                return -1;
            }
            else{
                double t = -ray.start_point.z_val / ray.direction.z_val;
                if(t < 0){
                    return -1;
                }
                else{
                    return t;
                }
            }
        }

        virtual double getColorRedAt(Point3D point){
            int tile_x = (point.x_val - reference_point.x_val) / length;
            int tile_y = (point.y_val - reference_point.y_val) / length;
            if((tile_x + tile_y) % 2 != 0 || tile_x < 0 || tile_y < 0 || tile_x >= totalNumberOfTiles || tile_y >= totalNumberOfTiles){
                return 0;
            }
            else{
                return 1;
            }
        }

        virtual double getColorGreenAt(Point3D point){
            int tile_x = (point.x_val - reference_point.x_val) / length;
            int tile_y = (point.y_val - reference_point.y_val) / length;
            if((tile_x + tile_y) % 2 != 0 || tile_x < 0 || tile_y < 0 || tile_x >= totalNumberOfTiles || tile_y >= totalNumberOfTiles){
                return 0;
            }
            else{
                return 1;
            }
        }

        virtual double getColorBlueAt(Point3D point){
            int tile_x = (point.x_val - reference_point.x_val) / length;
            int tile_y = (point.y_val - reference_point.y_val) / length;
            if((tile_x + tile_y) % 2 != 0 || tile_x < 0 || tile_y < 0 || tile_x >= totalNumberOfTiles || tile_y >= totalNumberOfTiles){
                return 0;
            }
            else{
                return 1;
            }
        }

};