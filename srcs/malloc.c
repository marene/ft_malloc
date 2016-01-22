/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marene <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/19 17:18:11 by marene            #+#    #+#             */
/*   Updated: 2016/01/22 20:05:04 by marene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <sys/mman.h>
#include "libft.h"
#include "ft_malloc.h"

extern metadata_t		malloc_data_g;


static void*			alloc(size_t size, blocksize_t blk_size)
{
	void*	data;
	void*	end;
	void*	ret;
	int32_t	len;

	data = malloc_data_g.datas[blk_size];
	end = malloc_data_g.datas_end[blk_size];
	if (blk_size == LARGE)
	{
		ret = mmap(NULL, size + sizeof(int32_t), MMAP_PROT, MMAP_FLAGS, -1, 0);
		if (ret == MAP_FAILED)
		{
			ret = NULL;
		}
		return (ret);
	}
	while (data < end)
	{
		len = *(int32_t*)data;
		if (len <= 0 && ((len * -1) >= (int)size || len == 0))
		{
			ret = data + sizeof(int32_t);
			if (metadata_add(ret, blk_size) == M_OK)
			{
				*(int32_t*)data = size; // PROBLEME -> On stock la taille comme un int, alors qu'a la base c'est un size_t! :/
				return (data);
			}
			else
			{
				return (NULL);
			}
		}
		else
			data += sizeof(int32_t) + len;
	}
	return (NULL);
}

static blocksize_t		get_blk_size(size_t size)
{
	if (size <= TINY_MAX_SIZE)
	{
		return (TINY);
	}
	else if (size <= SMALL_MAX_SIZE)
	{
		return (SMALL);
	}
	else
	{
		return (LARGE);
	}
}

void*					malloc(size_t size)
{
	static char		init[3] = {0, 0, 0};
	blocksize_t		blk_size;

	blk_size = get_blk_size(size);
	if (init[blk_size] == 0)
	{
		if (pages_init(&blk_size) == M_OK)
		{
			init[blk_size] = 1;
		}
		else
		{
			return (NULL);
		}
	}
	return (alloc(size, blk_size));
}
