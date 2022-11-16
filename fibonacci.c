void fibonacci(Monitor *mon, int s) {
	unsigned int i=0, n=0, nx, ny, nw, nh;
	Client *c;
	
	wl_list_for_each(c, &clients, link)
		if (VISIBLEON(c, mon) && !c->isfloating)
			n++;
	if(n == 0)
		return;
	
	nx = mon->w.x;
	ny = 0;
	nw = mon->w.width;
	nh = mon->w.height;
	
	wl_list_for_each(c, &clients, link)
		if (VISIBLEON(c, mon) && !c->isfloating){
		if((i % 2 && nh / 2 > 2 * c->bw)
		   || (!(i % 2) && nw / 2 > 2 * c->bw)) {
			if(i < n - 1) {
				if(i % 2)
					nh /= 2;
				else
					nw /= 2;
				if((i % 4) == 2 && !s)
					nx += nw;
				else if((i % 4) == 3 && !s)
					ny += nh;
			}
			if((i % 4) == 0) {
				if(s)
					ny += nh;
				else
					ny -= nh;
			}
			else if((i % 4) == 1)
				nx += nw;
			else if((i % 4) == 2)
				ny += nh;
			else if((i % 4) == 3) {
				if(s)
					nx += nw;
				else
					nx -= nw;
			}
			if(i == 0)
			{
				if(n != 1)
					nw = mon->w.width * mon->mfact;
				ny = mon->w.y;
			}
			else if(i == 1)
				nw = mon->w.width - nw;
			i++;
		}
		resize(c, (struct wlr_box){.x = nx, .y = ny,
			.width = nw - 2 * c->bw, .height = nh - 2 * c->bw}, 0);
	}
}

void
dwindle(Monitor *mon) {
	fibonacci(mon, 1);
}

void
spiral(Monitor *mon) {
	fibonacci(mon, 0);
}
