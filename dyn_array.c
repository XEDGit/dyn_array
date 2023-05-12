#include <inttypes.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

typedef union	dyn_ptr {
	uintptr_t*	p;
	char		c;
	char*		cp;
	double		d;
	double*		dp;
	int			i;
	int*		ip;
	long		l;
	long*		lp;
	float		f;
	float*		fp;
}	dynptr;

typedef struct dyn_arr {
	int			size;
	dynptr		ptr;
	int			cursor;
	int			length;
}	dynarr;

inline dynptr	dynload(dynarr* arr, int pos);
dynarr*			dyn_malloc(int size, unsigned int typesize);
static dynarr*	dyn_realloc(dynarr* arr, unsigned int size);
inline void		dynset(dynarr* arr, long value_addr, unsigned long pos);
inline void		dynsetp(dynarr* arr, void* value_addr, unsigned long len, unsigned long pos);
dynarr*			dyncopy(dynarr* arr);
void			dyn_free(dynarr *tofree);

dynptr	dynload(dynarr* arr, int pos)
{
	return (dynptr){ .p=(uintptr_t *)(arr->ptr.p + (pos * arr->size)) };
}


dynarr*	dyn_malloc(int size, unsigned int typesize)
{
	dynarr*	ret = malloc(sizeof(dynarr));
	if (!ret)
		return (NULL);
	ret->size = typesize;
	ret->ptr.p = malloc(typesize * size);
	if (!ret->ptr.p)
		return (free(ret), NULL);
	ret->length = size;
	ret->cursor = 0;
	while (size--)
		dynset(ret, 0, size);
	return ret;
}

static dynarr*	dyn_realloc(dynarr* arr, unsigned int size)
{
	dynarr* ret = dyn_malloc(size, arr->size);
	if (!ret)
		return (dyn_free(arr), (dynarr *)0);
	size = arr->length;
	while (size-- >= 0)
		if (size < arr->length)
			memmove(dynload(ret, size).p, dynload(arr, size).p, arr->size);
	dyn_free(arr);
	return ret;
}

void	dynset(dynarr* arr, long value_addr, unsigned long pos)
{
	if (pos >= arr->length && (arr = dyn_realloc(arr, pos + 1)))
		return (perror("dynset failed to reallocate: "), exit(1));
	memmove(dynload(arr, pos).p, &value_addr, sizeof(long));
}

void	dynsetp(dynarr* arr, void* value_addr, unsigned long len, unsigned long pos)
{
	if (pos >= arr->length && (arr = dyn_realloc(arr, pos + 1)))
		return (perror("dynset failed to reallocate: "), exit(1));
	memmove(dynload(arr, pos).p, value_addr, (arr->size * len));
}

dynarr*	dyncopy(dynarr* arr)
{
	dynarr*	ret = dyn_malloc(arr->length, arr->size);
	int	i = 0;

	memmove(dynload(ret, 0).p, dynload(arr, 0).p, arr->size * arr->length);
	return ret;
}

int	dynadd(dynarr* dst, uintptr_t value)
{
	return 0;
}

void	dyn_free(dynarr *tofree)
{
	free(tofree->ptr.p);
	free(tofree);
}

char dyn_types[4][7] = {
	"char",
	"int",
	"float",
	"double"
};

int	main(void)
{
	dynarr* in = dyn_malloc(5, sizeof(char));
	for (int i = 0; i < 5; i++)
	{
		dynset(in, 'a', i);
	}
	for (int i = 0; i < 5; i++)
		printf("%c", dynload(in, i).c);
}