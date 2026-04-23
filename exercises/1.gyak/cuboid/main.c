#include "teglatest.h"

#include <stdio.h>


int main()
{
    Cuboid cuboid;
    double volume;
    double surface;

    // Élek beállítása ellenőrzéssel
    set_size(&cuboid, 5, 10, 5);

    // Számítások
    volume = calc_cuboid_volume(&cuboid);
    surface = calc_cuboid_surface(&cuboid);

    // Eredmények kiírása
    printf("Cuboid volume: %.2lf\n", volume);
    printf("Cuboid surface: %.2lf\n", surface);

	if (van_negyzet_alaku_lap(&cuboid))
    {
        printf("A teglatestnek van negyzet alaku lapja.\n");
    }
    else
    {
        printf("A teglatestnek nincs negyzet alaku lapja.\n");
    }

    return 0;
}

