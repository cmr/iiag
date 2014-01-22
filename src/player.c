//
// player.c
//

#include <stdlib.h>
#include "player.h"
#include "display.h"

#define PLYRT (PLYR.z->tiles[PLYR.x][PLYR.y])

// this probably should not be here
static void redraw(void)
{
	wclear(dispscr);
	zone_draw(PLYR.z);
	wrefresh(dispscr);
}

void plyr_act_pickup(void)
{
	int i, j;

	if (PLYRT.inv->weight == 0) {
		memo("There is nothing here to pick up.");
	} else {
		i = inv_prompt("Pick up what?", PLYRT.inv);

		if (PLYRT.inv->size > i && PLYRT.inv->itms[i] != NULL) {
			if ((j = inv_add(PLYR.inv, PLYRT.inv->itms[i])) != INVALID) {
				PLYRT.inv->itms[i]->i = j;
				inv_rm(PLYRT.inv, i);
				memo("You pick up the %s", PLYR.inv->itms[j]->f->name);
			} else {
				memo("You cannot pick up the %s.", PLYRT.inv->itms[i]->f->name);
			}
		} else {
			memo("There is no such item.");
		}

		redraw();
	}
}

void plyr_act_drop(void)
{
	int i,j;

	i = inv_prompt("You dropped what?", PLYR.inv);

	if(PLYR.inv->size > i && PLYR.inv->itms[i]!=NULL){
		if((j=inv_add(PLYRT.inv,PLYR.inv->itms[i]))!=INVALID){
			PLYR.inv->itms[i]->i=j;
			inv_rm(PLYR.inv,i);
			memo("You Dropped %s",PLYRT.inv->itms[j]->f->name);
		}else{
			memo("You cannot drop the %s.",PLYR.inv->itms[i]->f->name);
		}
	}else{
		memo("There is no such item.");
	}

	redraw();
}

void plyr_act_inv(void)
{
	inv_prompt("Your inventory:", PLYR.inv);
	redraw();
}

void plyr_act_move(int dx, int dy)
{
	int dam;
	tile * t;

	if (!crtr_move(&PLYR, dx, dy)) {
		t = zone_at(PLYR.z, PLYR.x + dx, PLYR.y + dy);

		// auto attack
		if (t != NULL && t->crtr != NULL) {
			switch (dam = crtr_attack(&PLYR, t->crtr)) {
			case DEAD:
				memo("You kill the %s", t->crtr->f->name);
				crtr_free(t->crtr);
				t->crtr = NULL;

				zone_update(PLYR.z, PLYR.x + dx, PLYR.y + dy);
				wrefresh(dispscr);
				break;
			case 0:
				memo("You miss");
				break;
			default:
				memo("You hit for %d damage", dam);
				break;
			}
		}
	}
}

void plyr_ev_death(void)
{
	memo("You are dead, how unfortunate.");
	wgetch(memoscr);
	end_disp();
	exit(0);
}

void plyr_ev_lvlup(void)
{
	memo("Level up!");
}