#include <iostream>
#include <assert.h>

//Write a function that tests if a Point falls within a specified distance(“dist”) of any part of a solid, 2D rectangle.The rectangle is specified 
//by the bottom left corner, a width, and a height.

struct Point
{
	float x;
	float y;
};

struct Rectangle
{
	Point bottomLeft;
	float width;
	float height;
};

int IsPointWithinDistOfRectangle(const Rectangle& rect, const Point& point, float dist)
{
	//Checking to see if the point is on the rectangle
	if (point.x <= rect.bottomLeft.x + rect.width && point.x >= rect.bottomLeft.x) {//Checking the x.
		if (point.y <= rect.bottomLeft.y + rect.height && point.y >= rect.bottomLeft.y) {//Checking the y.
			return 1;
		}
	}

	//Checking to see if point is within distance of the rectangle.
	else if (point.x <= (rect.bottomLeft.x + rect.width + dist) && point.x >= rect.bottomLeft.x - dist) {//Checking the x.
		if (point.y <= rect.bottomLeft.y + rect.height + dist && point.y >= rect.bottomLeft.y - dist) {//Checking the y.
			return 2;
		}
	}

	//If not on rectangle and not within distance, return 0 for the failure
	else {
		return 0;
	}
}

int main() {
	Point p;
	p.x = 0.f;
	p.y = 0.f;

	Rectangle a;
	a.bottomLeft = p;
	a.height = 5.f;
	a.width = 5.f;

	Point test;
	test.x = 1.f;
	test.y = 1.f;

	float dist = 5.f;

	/*
	a.x = 0;
	a.y = 0;
	a.width = 5;
	a.height = 5;

	test.x = 1;
	test.y = 1;

	dist = 5;
	*/
	assert(IsPointWithinDistOfRectangle(a, test, dist) == 1);	//Testing to see if the function returns in rectangle. PASS

	test.x = 10.f;
	test.y = 10.f;

	/*
	a.x = 0;
	a.y = 0;
	a.width = 5;
	a.height = 5;

	test.x = 10;
	test.y = 10;

	dist = 5;
	*/

	assert(IsPointWithinDistOfRectangle(a, test, dist) == 2);	//Testing to see if point is within distance of rectangle. PASS
	
	/*
	a.x = 0;
	a.y = 0;
	a.width = 5;
	a.height = 5;

	test.x = 10;
	test.y = 10;

	dist = 1;
	*/

	dist = 1.f;
	assert(IsPointWithinDistOfRectangle(a, test, dist) == 0);	//Testing for failure. PASS

	system("pause");
	return 0;
}