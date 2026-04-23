#ifndef TEGLATEST_H
#define TEGLATEST_H

typedef struct Cuboid
{
	double a;
	double b;
	double c;
} Cuboid;


void set_size(Cuboid* cuboid, double a, double b, double c);

double calc_cuboid_volume(const Cuboid* cuboid);

double calc_cuboid_surface(const Cuboid* cuboid);

int van_negyzet_alaku_lap(const Cuboid* cuboid);

#endif