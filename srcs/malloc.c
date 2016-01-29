/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marene <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/19 17:18:11 by marene            #+#    #+#             */
/*   Updated: 2016/01/29 18:29:04 by marene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <sys/mman.h>
#include "libft.h"
#include "ft_malloc.h"

extern metadata_t		malloc_data_g;
size_t					alloc_maxsize_g;


static void*			alloc(size_t size, blocksize_t blk_size)
{
	int		i;
	int32_t	len;
	void*	data;
	void*	data_end;
	void*	ret;

	i = 0;
	if (blk_size == LARGE)
	{
		ret = mmap(0, size, MMAP_PROT, MMAP_FLAGS, -1, 0);
		if (ret != MAP_FAILED)
		{
			*(int32_t*)ret = (int32_t)size;
			return (ret + sizeof(int32_t));
		}
		else
		{
			return (NULL);
		}
	}
	data = malloc_data_g.datas[blk_size];
	data_end = malloc_data_g.datas_end[blk_size];
	while (data != NULL)
	{
		while (data + i < data_end)
		{
			len = *(int32_t*)(data + i);
			if (len > 0)
			{
				i += len + sizeof(int32_t);
			}
			else if ((len == 0 && data + i + sizeof(int32_t) + size < data_end) || -1 * len >= (int32_t)size)
			{
				if (size > alloc_maxsize_g)
					alloc_maxsize_g = size;
				*(int32_t*)(data + i) = (int32_t)size;
				return (data + i + sizeof(int32_t));
			}
			else
			{
				i += (-1 * len) + sizeof(int32_t);
			}
		}
		data = *(void**)data_end;
		if (data != NULL)
		{
			data_end = data + malloc_data_g.datas_len[blk_size] - sizeof(void*);
		}
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
	blocksize_t		blk_size;
	static int		init[2] = {0, 0};

	if (size != 0)
	{
		blk_size = get_blk_size(size);
		if (init[blk_size] == 0)
		{
			alloc_maxsize_g = 0;
			if (pages_init(blk_size) == M_OK)
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
	else
		return (NULL);
}
