/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dyn_array.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmuzio <lmuzio@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 18:15:29 by lmuzio            #+#    #+#             */
/*   Updated: 2023/05/18 18:44:52 by lmuzio           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DYN_ARRAY_H
# define DYN_ARRAY_H

# include <stdlib.h>

typedef struct dyn_arr {
	int			size;
	void		*ptr;
	int			cursor;
	int			length;
}	dynarr;

# define PROTO_DYNLOADPTR(type, name) \
	inline type	name(dynarr* arr, int pos) \
	{ \
		return (type)(arr->ptr + (pos * arr->size)); \
	}
# define PROTO_DYNLOAD(type, name) \
	inline type	name(dynarr* arr, int pos) \
	{ \
		return *(type*)(arr->ptr + (pos * arr->size)); \
	}
# define DECLARE_DYNLOAD(type, name) \
	type name(dynarr* arr, int pos);

DECLARE_DYNLOAD(void*, dynload);
DECLARE_DYNLOAD(short, dynloads);
DECLARE_DYNLOAD(short*, dynloadsp);
DECLARE_DYNLOAD(unsigned short, dynloadus);
DECLARE_DYNLOAD(unsigned short*, dynloadusp);
DECLARE_DYNLOAD(int, dynloadi);
DECLARE_DYNLOAD(int*, dynloadip);
DECLARE_DYNLOAD(unsigned int, dynloadui);
DECLARE_DYNLOAD(unsigned int*, dynloaduip);
DECLARE_DYNLOAD(long, dynloadl);
DECLARE_DYNLOAD(long*, dynloadlp);
DECLARE_DYNLOAD(unsigned long, dynloadul);
DECLARE_DYNLOAD(unsigned long*, dynloadulp);
DECLARE_DYNLOAD(long long, dynloadll);
DECLARE_DYNLOAD(long long*, dynloadllp);
DECLARE_DYNLOAD(unsigned long long, dynloadull);
DECLARE_DYNLOAD(unsigned long long*, dynloadullp);
DECLARE_DYNLOAD(float, dynloadf);
DECLARE_DYNLOAD(float*, dynloadfp);
DECLARE_DYNLOAD(char, dynloadc);
DECLARE_DYNLOAD(char*, dynloadcp);
DECLARE_DYNLOAD(unsigned char, dynloaduc);
DECLARE_DYNLOAD(unsigned char*, dynloaducp);
DECLARE_DYNLOAD(double, dynloadd);
DECLARE_DYNLOAD(double*, dynloaddp);
DECLARE_DYNLOAD(long double, dynloadld);
DECLARE_DYNLOAD(long double*, dynloadldp);

dynarr*			dyn_malloc(int size, unsigned int typesize);
dynarr*	dyn_realloc(dynarr* arr, unsigned int size);
inline void		dynsetval(dynarr* arr, long value, unsigned long pos);
inline void		dynset(dynarr* arr, void* value_addr, unsigned long len, unsigned long pos);
dynarr*			dyncopy(dynarr* arr);
void			dyn_free(dynarr *tofree);

#endif