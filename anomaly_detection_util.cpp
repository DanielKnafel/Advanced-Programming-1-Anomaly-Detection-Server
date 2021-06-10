/*
 * anomaly_detection_util.cpp
 *
 * Author: Daniel Knafel
 *         316012624
 */

#include <math.h>
#include "anomaly_detection_util.h"
using namespace std;

 // returns the average of array x.
float avg(float* x, int size) {
    float sum = 0;
    for (int i = 0; i < size; i++) {
        sum += x[i];
    }
    return sum / size;
}

// returns the variance of X and Y
float var(float* x, int size) {
    float mu = avg(x, size);
    float squares[size];

    for (int i = 0; i < size; i++) {
        squares[i] = powf(x[i] - mu, 2);
    }

    return avg(squares, size);
}

// returns the product of 2 arrays.
void multArrays(float* product, float* x, float* y, int size) {
    for (int i = 0; i < size; i++) {
        product[i] = x[i] * y[i];
    }
}

// returns the covariance of X and Y
float cov(float* x, float* y, int size) {
    float product [size];
    float avgX = avg(x, size);
    float avgY = avg(y, size);
    // multiply x and y into product.
    multArrays(product, x, y, size);
    // calculate the average of product.
    float avgXY = avg(product, size);

    return avgXY - (avgX * avgY);
}

// returns the Pearson correlation coefficient of X and Y
float pearson(float* x, float* y, int size) {
    float dX = sqrtf(var(x, size));
    float dY = sqrtf(var(y, size));

    return cov(x, y, size) / (dX * dY);
}

// performs a linear regression and returns the line equation
Line linear_reg(float* x, float* y, int size) {
    float a = cov(x, y, size) / var(x, size);
    float b = avg(y, size) - (a * avg(x, size));

    return Line(a, b);
}

// performs a linear regression and returns the line equation
Line linear_reg(Point** points, int size) {
    float x[size];
    float y[size];

    for (int i = 0; i < size; i++) {
        x[i] = points[i]->x;
        y[i] = points[i]->y;
    }
    return linear_reg(x, y, size);
}


// returns the deviation between point p and a given line.
float dev(Point p, Line l) {
    return fabs(l.f(p.x) - p.y);
}

// returns the deviation between point p and the line equation of the points
float dev(Point p, Point** points, int size) {
    Line l = linear_reg(points, size);
    return dev(p, l);
}
