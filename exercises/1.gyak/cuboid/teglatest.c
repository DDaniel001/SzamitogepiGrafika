#include "teglatest.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


void set_size(Cuboid* cuboid, double a, double b, double c)
{
    if (a <= 0 || b <= 0 || c <= 0) 
    {
        printf("Hiba: Minden él hosszának pozitívnak kell lennie!\n");
        exit(1); // Kilépünk a programból hiba esetén
    }

    cuboid->a = a;
    cuboid->b = b;
    cuboid->c = c;
}


double calc_cuboid_volume(const Cuboid* cuboid)
{
	double volume = cuboid->c * cuboid->b * cuboid->a;
	return volume;
}

double calc_cuboid_surface(const Cuboid* cuboid) 
{
	double surface = 2 * (cuboid->a * cuboid->b + cuboid->a * cuboid->c + cuboid->b * cuboid->c);
	return surface;
}

int van_negyzet_alaku_lap(const Cuboid* cuboid)
{
	if (cuboid->a == cuboid->b || cuboid->a == cuboid->c || cuboid->b == cuboid->c)
	{
		return 1;
	}
	return 0;
}

