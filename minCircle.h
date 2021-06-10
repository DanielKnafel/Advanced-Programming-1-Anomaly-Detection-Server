// 31578622		31601262
// We used Welzl's algorithm, taken from Wikipedia.
/* 
	algorithm welzl:
		input: Finite sets P and R of points in the plane |R|≤ 3.
		output: Minimal disk enclosing P with R on the boundary.

		if P is empty or |R| = 3 then
			return trivial(R)
		choose p in P (randomly and uniformly)
		D := welzl(P - { p }, R)
		if p is in D then
			return D

		return welzl(P - { p }, R ∪ { p })
*/
#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <math.h>
#include <vector>
#include "anomaly_detection_util.h"

using namespace std;

class Circle {
	Point center;
	float radius;
public:
	Circle(): center(0,0), radius(0){}
	Circle(Point c,float r):center(c),radius(r){}
	const Point getCenter() const { return center; };
	void setCenter(Point newCenter) { center = newCenter; };
	const float getRadius() const { return radius; };
	void setRadius(float newRadius) { radius = newRadius; };
	const bool isPointInCircle(Point *p) const;
};

Circle findMinCircle(Point** points, size_t size);

#endif /* MINCIRCLE_H_ */