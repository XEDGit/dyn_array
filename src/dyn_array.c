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
/// @param size 
/// @param typesize 
/// @return 
dynarr*	dynalloc(int size, unsigned int typesize)
{
	dynarr*	ret = malloc(sizeof(dynarr));
	if (!ret)
		return (NULL);
	ret->size = typesize;
	ret->ptr = malloc(typesize * size);
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
/// @return Resized arr, on fail it returns 0, sets errno and frees arr
dynarr*	dyn_realloc(dynarr* arr, unsigned int length)
{
	dynarr* ret = dynalloc(length, arr->size);
	if (!ret)
		return (dyn_free(arr), (dynarr *)0);
	dyn_memmove(ret->ptr, arr->ptr, arr->size * arr->length);
	dyn_free(arr);
	return ret;
}

/// @brief Acts as `arr[pos] = value` for constant values, use dynset for pointers
/// @param arr Allocated dynarr*
/// @param value value to write
/// @param pos index where to write
inline void	dynsetval(dynarr* arr, long long value, unsigned long pos)
{
	if (pos >= arr->length)
		return ;
	dyn_memmove(dynload(arr, pos), &value, arr->size);
}

/// @brief Copies `len * size of arr data type` bytes from value_addr to arr[pos], use dynsetval for constant values
/// @param arr 
/// @param value_addr 
/// @param len 
/// @param pos 
inline void	dynset(dynarr* arr, void* value_addr, unsigned long len, unsigned long pos)
{
	if (pos >= arr->length)
		return ;
	dyn_memmove(dynload(arr, pos), value_addr, (arr->size * len));
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
	return ret;
}

/// @brief NOT IMPLEMENTED Inserts an element at dst[pos], reallocating the array if necessary
/// @param dst destination array
/// @param value_addr address pointing to the values to be copied
/// @param pos position in the array to insert the element
/// @return returns a positive value on success, 0 on error and sets errno
int	dynadd(dynarr* dst, void* value_addr, unsigned long pos)
{
	return 0;
}

void	dyn_free(dynarr *tofree)
{
	free(tofree->ptr);
	free(tofree);
}
