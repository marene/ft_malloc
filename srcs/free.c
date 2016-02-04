/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndatin <ndatin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/21 12:02:33 by marene            #+#    #+#             */
/*   Updated: 2016/02/04 19:23:54 by marene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ft_malloc.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>

extern metadata_t	malloc_data_g;

static blocksize_t	get_blk_size(void* meta_ptr)
{
	if (meta_ptr >= malloc_data_g.datas[TINY] && meta_ptr <= malloc_data_g.datas_end[TINY])
		return (TINY);
	if (meta_ptr >= malloc_data_g.datas[SMALL] && meta_ptr <= malloc_data_g.datas_end[SMALL])
		return (SMALL);
	return (LARGE);
}

void				free(void* usr_ptr)
{
	void*			meta_ptr;
	int				to_unmap;
	int				alloced;
	int				page_size;
	blocksize_t		blk_size;

	meta_ptr = usr_ptr - sizeof(int32_t);
	blk_size = get_blk_size(meta_ptr);//*(int32_t*)meta_ptr; // On initialse par defaut a TINY
	if (usr_ptr != NULL && meta_ptr != NULL && *(int32_t*)meta_ptr > 0 && blk_size < LARGE)
	{
		if (*(int32_t*)meta_ptr > 0)
			*(int32_t*)meta_ptr *= -1;
		defragment_memory(blk_size);
	}
	else
	{
		to_unmap = 0;
		page_size = getpagesize();
		alloced = *(int32_t*)meta_ptr;
		to_unmap = alloced;
		if (alloced % page_size > 0)
			to_unmap += (page_size - (alloced % page_size));
		munmap(usr_ptr, to_unmap);
	}
}
