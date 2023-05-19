/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dyn_array.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmuzio <lmuzio@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 18:09:53 by lmuzio            #+#    #+#             */
/*   Updated: 2023/05/18 18:45:28 by lmuzio           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dyn_array.h>

PROTO_DYNLOADPTR(void*, dynload);
PROTO_DYNLOAD(short, dynloads);
PROTO_DYNLOADPTR(short*, dynloadsp);
PROTO_DYNLOAD(unsigned short, dynloadus);
PROTO_DYNLOADPTR(unsigned short*, dynloadusp);
PROTO_DYNLOAD(int, dynloadi);
PROTO_DYNLOADPTR(int*, dynloadip);
PROTO_DYNLOAD(unsigned int, dynloadui);
PROTO_DYNLOADPTR(unsigned int*, dynloaduip);
PROTO_DYNLOAD(long, dynloadl);
PROTO_DYNLOADPTR(long*, dynloadlp);
PROTO_DYNLOAD(unsigned long, dynloadul);
PROTO_DYNLOADPTR(unsigned long*, dynloadulp);
PROTO_DYNLOAD(long long, dynloadll);
PROTO_DYNLOADPTR(long long*, dynloadllp);
PROTO_DYNLOAD(unsigned long long, dynloadull);
PROTO_DYNLOADPTR(unsigned long long*, dynloadullp);
PROTO_DYNLOAD(float, dynloadf);
PROTO_DYNLOADPTR(float *, dynloadfp);
PROTO_DYNLOAD(char, dynloadc);
PROTO_DYNLOADPTR(char *, dynloadcp);
PROTO_DYNLOAD(unsigned char, dynloaduc);
PROTO_DYNLOADPTR(unsigned char *, dynloaducp);
PROTO_DYNLOAD(double, dynloadd);
PROTO_DYNLOADPTR(double *, dynloaddp);
PROTO_DYNLOAD(long double, dynloadld);
PROTO_DYNLOADPTR(long double *, dynloadldp);

static void	*dyn_memmove(void *dest, void *src, size_t len)
{
	char	*d;
	char	*s;

	d = dest;
	s = src;
	if (d < s)
		while (len--)
			*d++ = *s++;
	else
	{
		s += (len - 1);
		d += (len - 1);
		while (len--)
			*d-- = *s--;
	}
	return dest;
}

/// @brief Allocates new dynamic array
/// @param size number of elements in the array
/// @param typesize size in bytes of a single element, use sizeof()
/// @return Returns new array on success, 0 on failure
dynarr*	dynalloc(int size, unsigned int typesize)
{
	dynarr*	ret = malloc(sizeof(dynarr));
	if (!ret)
		return (NULL);
	ret->size = typesize;
	ret->ptr = malloc(typesize * (size + 1));
	if (!ret->ptr)
		return (free(ret), NULL);
	ret->length = size;
	ret->cursor = 0;
	while (size--)
		dynsetval(ret, 0, size);
	return ret;
}

/// @brief Reallocates arr with new length and frees arr
/// @param arr original array, will be freed by this function
/// @param length new length of the array
/// @return Resized arr, on fail it returns 0 and sets errno and frees arr
dynarr*	dyn_realloc(dynarr* arr, unsigned int length)
{
	dynarr* ret = dynalloc(length, arr->size);
	if (!ret)
		return (dyn_free(arr), (dynarr *)0);
	ret->cursor = arr->cursor;
	dyn_memmove(ret->ptr, arr->ptr, arr->size * arr->cursor);
	dyn_free(arr);
	return ret;
}

/// @brief Adds a2 at the end of a1, dynamic arrays MUST be of the same type
/// @return 1 on success, 0 on fail and sets errno
int	dynjoin(dynarr** a1, dynarr* a2)
{
	int len = (*a1)->cursor + a2->length;
	dynarr*	tmp = dyn_realloc(*a1, len);
	if (!tmp)
		return (0);
	dynset(tmp, a2->ptr, a2->length, tmp->cursor);
	tmp->cursor = len;
	*a1 = tmp;
}

/// @brief Adds (len * a1.size) elements from a2 at the end of a1, values inside void * MUST be of the same datatype of a1. If len is 0 dynjoin will expect a null-terminated string as a2 and will fail if a1 is not of type char
/// @return 1 on success and a1 is reallocated, 0 on fail
int	dynjoinptr(dynarr** a1, void* a2, unsigned long len)
{
	if (!len)
	{
		if ((*a1)->size != sizeof(char))
			return (0);
		char* s2 = a2;
		while (s2[len])
			len++;
	}
	dynarr*	tmp = dyn_realloc(*a1, (*a1)->cursor + (len * (*a1)->size));
	if (!tmp)
		return (0);
	dynset(tmp, a2, len, tmp->cursor);
	tmp->cursor += len;
	*a1 = tmp;
}

static void	dyn_memset(void *dst, unsigned char value, unsigned long len)
{
	char *s = dst;
	while (len--)
		s[len] = value;
}

/// @brief Acts as `arr[pos] = value` for constant values, use dynset for pointers, does not reallocate
/// @param arr Allocated dynarr*
/// @param value value to write
/// @param pos index where to write
inline void	dynsetval(dynarr* arr, char value, unsigned long pos)
{
	if (pos >= arr->length)
		return ;
	if (arr->cursor < pos)
		arr->cursor = pos;
	dyn_memset(dynload(arr, pos), value, arr->size);
}

/// @brief Copies `len * size of arr data type` bytes from value_addr to arr[pos], use dynsetval for a constant value, does not reallocate
/// @param arr Allocated dynarr*
/// @param value value to write
/// @param pos index where to write
inline void	dynset(dynarr* arr, void* value_addr, unsigned long pos)
{
	if (pos >= arr->length)
		return ;
	dyn_memmove(dynload(arr, pos), value_addr, arr->size);
}

/// @brief Makes deep-copy of arr
/// @param arr array to copy
/// @return pointer to deep-copy of arr, on fail returns 0 and sets errno
dynarr*	dyncopy(dynarr* arr)
{
	dynarr*	ret = dynalloc(arr->length, arr->size);
	if (!ret)
		return ((dynarr *)0);
	dyn_memmove(ret->ptr, arr->ptr, arr->size * arr->length);
	ret->cursor = arr->cursor;
	return ret;
}

/// @brief Creates an instance of dynarr containing a string
/// @param str_addr null-terminated source string
/// @return dynarr pointer to a new dynamic array containing str
dynarr*	dynallocstr(char* str)
{
	int		len = 0;
	while (str[len]) len++;
	dynarr*	ret = dynalloc(len, sizeof(char));
	if (!ret)
		return ((dynarr *)0);
	dyn_memmove(ret->ptr, str, len);
	ret->cursor = len;
	return ret;
}

/// @brief NOT IMPLEMENTED Inserts an element at dst[pos], reallocating the array if necessary
/// @param dst destination array
/// @param value_addr address pointing to the values to be copied
/// @param pos position in the array to insert the element
/// @return returns a positive value on success, 0 on error and sets errno
int	dyninsert(dynarr** dst, void* value_addr, unsigned long pos)
{
	int		reall = ((*dst)->cursor == (*dst)->length);
	dynarr* tmp = 0;
	if (reall)
		tmp = dynalloc((*dst)->length + 1, (*dst)->size);
	else
		tmp = *dst;
	if (!tmp)
		return 0;
	if (pos && reall)
		dyn_memmove(tmp->ptr, (*dst)->ptr, --pos);
	if (tmp->length - pos)
		dyn_memmove(dynload(tmp, pos + 1), dynload(*dst, pos), (tmp->length - pos) * tmp->size);
	dyn_memmove(dynload(tmp, pos), value_addr, tmp->size);
	tmp->cursor = (*dst)->cursor + 1;
	if (reall)
	{
		dyn_free(*dst);
		*dst = tmp;
	}
	return 1;
}

void	dyn_free(dynarr *tofree)
{
	free(tofree->ptr);
	free(tofree);
}
