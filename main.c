#include <stdio.h>
#include <dyn_array.h>

typedef struct a {
	int a;
	int b;
} aa;

int	main(void)
{
	dynarr* in = dynalloc(5, sizeof(aa));
	if (!in)
		return 1;

	for (int i = 0; i < 5; i++)
		dynset(in, &(aa){i, 2}, 1, i);

	(*(aa*)dynload(in, 0)).a += (*(aa*)dynload(in, 0)).b;

	for (int i = 0; i < 5; i++)
		printf("%d %d\n", (*(aa*)dynload(in, i)).a, (*(aa*)dynload(in, i)).b);
	dyn_free(in);
}