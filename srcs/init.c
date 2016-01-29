/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndatin <ndatin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/13 13:37:18 by ndatin            #+#    #+#             */
/*   Updated: 2016/01/29 18:22:40 by marene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include "libft.h"
#include "ft_malloc.h"

extern metadata_t	malloc_data_g;

static int	data_init(blocksize_t blk_size)
{
	int	size;
	int	pagesize;

	pagesize = getpagesize();
	if (blk_size == TINY)
	{
		size = TINY_PAGES_NB * pagesize;
		malloc_data_g.data_tiny = mmap(0, size, MMAP_PROT, MMAP_FLAGS, -1, 0);
		if (malloc_data_g.data_tiny == MAP_FAILED)
		{
			return (M_NOK);
		}
		ft_bzero(malloc_data_g.data_tiny, size);
		malloc_data_g.datas_len[TINY] = size;
		malloc_data_g.datas[TINY] = malloc_data_g.data_tiny;
		malloc_data_g.datas_end[TINY] = malloc_data_g.data_tiny + size - sizeof(void*); 
		/*
			^ attention: GROSSE modif, les 8 derniers bytes de chaque segments data serviront a present d'adresse pour le morceau de segment suivant
		*/
	}
	if (blk_size == SMALL)
	{
		size = SMALL_PAGES_NB * pagesize;
		malloc_data_g.data_small = mmap(0, size, MMAP_PROT, MMAP_FLAGS, -1, 0);
		if (malloc_data_g.data_small == MAP_FAILED)
		{
			return (M_NOK);
		}
		ft_bzero(malloc_data_g.data_small, size);
		malloc_data_g.datas_len[SMALL] = size;
		malloc_data_g.datas[SMALL] = malloc_data_g.data_small;
		malloc_data_g.datas_end[SMALL] = malloc_data_g.data_small + size - sizeof(void*);
	}
	return (M_OK);
}

int		pages_init(blocksize_t blk_size)
{
	if (data_init(blk_size) == M_OK /*&& metadata_init(blk_size) == M_OK*/)
		return (M_OK);
	else
		return (M_NOK);
}
