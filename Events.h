#pragma once

typedef struct{
	uint num;
	uint sel;
	Coord *arr;
}Points;

void events(const Ticks frameEnd, Points *p)
{
	i32 ticksLeft = frameEnd - getTicks();
	do{
		Event event;
		if(!SDL_WaitEventTimeout(&event, ticksLeft))
			return;
		Key key;
		Coord relpos;
		Coord *newArr;
		uint pnumprev;
		static bool clickState = false;
		switch(event.type){
		case SDL_QUIT:
			printf("Quitting now!\n");
			exit(0);
			break;
		case SDL_KEYDOWN:
			switch (key = event.key.keysym.sym) {
			case SDLK_ESCAPE:
				free(p->arr);
				printf("Quitting now!\n");
				exit(0);
				break;
			case SDLK_UP:
				if(p->num >= 10){
					printf("p->num >= 10(%02u)\tSkipping!\n", p->num);
					break;
				}
				pnumprev = p->num;
				p->num++;
				printf("num:%02u -> num:%02u\n", pnumprev, p->num);
				newArr = malloc(sizeof(Coord)*p->num);
				memcpy(newArr, p->arr, sizeof(Coord)*(p->num));
				free(p->arr);
				p->arr = newArr;
				p->arr[p->num-1].x = gfx.xlen/2;
				p->arr[p->num-1].y = gfx.ylen/2;
				break;
			case SDLK_DOWN:
				if(p->num <= 3){
					printf("p->num <=  3(%02u)\tSkipping!\n", p->num);
					break;
				}
				pnumprev = p->num;
				p->num--;
				printf("num:%02u -> num:%02u\n", pnumprev, p->num);
				newArr = malloc(sizeof(Coord)*p->num);
				memcpy(newArr, p->arr, sizeof(Coord)*(p->num));
				free(p->arr);
				p->arr = newArr;
				break;
			case SDLK_LEFT:
				p->sel = wrap(p->sel-1,0,p->num);
				printf("sel: %u\n", p->sel);
				break;
			case SDLK_RIGHT:
				p->sel = wrap(p->sel+1,0,p->num);
				printf("sel: %u\n", p->sel);
				break;
			}
			break;
		case SDL_MOUSEMOTION:
			if(
				SDL_GetRelativeMouseState(&relpos.x,&relpos.y)&SDL_BUTTON(SDL_BUTTON_LEFT)
				&& clickState
			){
				if(coordNz(relpos)){
					// printf("(%03d,%03d)\n",relpos.x, relpos.y);
					p->arr[p->sel]=coordClamp(
						coordOffset(p->arr[p->sel],relpos),
						(Coord){0,gfx.xlen},
						(Coord){0,gfx.ylen}
					);
				}
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			switch (event.button.button) {
				case SDL_BUTTON_LEFT:
					clickState = true;
					uint closestin = 0;
					uint closestsq = coordDistSq(
						p->arr[0],
						(Coord){event.button.x,event.button.y}
					);
					for(uint i = 1; i < p->num; i++){
						uint distsq = coordDistSq(
							p->arr[i],
							(Coord){event.button.x,event.button.y}
						);
						if(distsq < closestsq){
							closestsq = distsq;
							closestin = i;
						}
					}
					p->sel = closestin;
					break;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			switch (event.button.button) {
				case SDL_BUTTON_LEFT:
					clickState = false;
					break;
			}
			break;

		}
		ticksLeft = frameEnd - getTicks();
	}while(ticksLeft > 0);
}
