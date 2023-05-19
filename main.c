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
	dynarr* s = dynallocstr("ciao"), *s2 = dynallocstr(" porcodio");
	if (!s)
		return (dyn_free(in), 1);
	dynjoin(&s, s2);
	dynjoinptr(&s, " lol", 0);
	dynarr* copy = dyncopy(s);
	dyn_free(s);
	s = copy;
	dyninsert(&s, "a", 0);
	dyninsert(&s, "a", 5);
	dyninsert(&s, "a", s->cursor + 1);
	for (int i = 0; i < 5; i++)
		dynset(in, &(aa){i, 2}, 1, i);

	(*(aa*)dynload(in, 0)).a += (*(aa*)dynload(in, 0)).b;

	for (int i = 0; i < 5; i++)
		printf("%d %d\n", (*(aa*)dynload(in, i)).a, (*(aa*)dynload(in, i)).b);
	printf("%s\n", dynloadcp(s, 0));
	dyn_free(in);
	dyn_free(s);
	dyn_free(s2);
}