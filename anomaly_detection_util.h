
#ifndef ANOMALYDETECTORUTIL_H_
#define ANOMALYDETECTORUTIL_H_

#include <iostream>
#include <math.h>

using namespace std;
// returns the avarage of array x
float avg(float* x, int size);

// returns the variance of X and Y
float var(float* x, int size);

// returns the covariance of X and Y
float cov(float* x,  float* y, int size);

// returns the Pearson correlation coefficient of X and Y
float pearson(float* x, float* y, int size);

class Line{
public:
	float a,b;
	Line():a(0),b(0){};
	Line(float a, float b):a(a),b(b){}
	float f(float x){
		return a*x+b;
	}

	void print() {
		cout << a << "x+" << b<< endl;
	}
};

class Point{
public:
	float x,y;
	Point(float x, float y):x(x),y(y){}

	// calculates the distance between 2 points.
	const float distance(const Point& p) const {
		return sqrt(pow(x - p.x, 2) + pow(y - p.y, 2));
	}
	void print() {
		cout << "(" << x << ", " << y << ")" << endl;
	}
};

Line linear_reg(float* x, float* y, int size);

// performs a linear regression and returns the line equation
Line linear_reg(Point** points, int size);

// returns the deviation between point p and the line equation of the points
float dev(Point p, Point** points, int size);

// returns the deviation between point p and the line
float dev(Point p,Line l);

#endif
