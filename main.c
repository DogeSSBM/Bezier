#include "Includes.h"

int main(int argc, char const *argv[])
{
	const Length window = {800, 800};
	init(window);

	Points p = {
		.num = 4,
		.sel = 0,
		.arr = malloc(sizeof(Coord)*4)
	};
	for(uint i = 0; i < 4; i++){
		p.arr[i]=coordShift(coordDiv(window,2),i,100);
	}

	while(1){
		Ticks frameStart = getTicks();
		clear();

		setColor(WHITE);
		bezier(p.arr, p.num, 5);
		for(uint i = 0; i < p.num; i++){
			setColor(i == p.sel? BLUE : RED);
			fillCircleCoord(p.arr[i], 5);
		}

		draw();
		events(frameStart + TPF, &p);
	}
	return 0;
}
