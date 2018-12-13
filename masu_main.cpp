#include <cstdio>
#include "masu.h"

int main(void) {
	masu m;
	m.startGame(10, 10, 10);
	for (;;) {
		printf("%d target(s) left.\n", m.getNumTargetsLeft());
		printf("  ");
		for (int x = 0; x < 10; x++) printf("%2d", x);
		printf("\n");
		for (int y = 0; y < 10; y++) {
			printf("%2d", y);
			for (int x = 0; x < 10; x++) {
				int stat = m.getStatus(x, y);
				printf(" %c",
					stat == 0 ? '.' :
					0 < stat && stat < 9 ? stat + '0' :
					stat == 9 ? '#' :
					stat == 10 ? 'F' :
					stat == 11 ? '*' :
					stat == 12 ? 'f' :
					'?');
			}
			printf("\n");
		}
		printf("\n");
		if (m.isActive()) {
			int x, y, a;
			printf("x y 0(hit)/1(flag) > ");
			fflush(stdout);
			if (scanf("%d%d%d", &x, &y, &a) != 3) return 1;
			if (a) m.toggleFlag(x, y); else m.hit(x, y);
		} else {
			break;
		}
	}
	if (m.isDead()) {
		printf("ouch!\n");
	} else if (m.isSolved()) {
		printf("congratulations!\n");
	}
	return 0;
}
