/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_allocable_segment.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marene <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/05 17:17:24 by marene            #+#    #+#             */
/*   Updated: 2016/05/18 17:08:31 by marene           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/mman.h>
#include <unistd.h>
#include "libft.h"
#include "ft_malloc.h"

extern t_metadata	g_malloc_data;

void				*extend_blk(t_blocksize blk_size, t_header *head)
{
	void		*new_data;
	int			size;
	int			pagesize;

	PROFILE_BASIC;
	pagesize = getpagesize();
	size = (blk_size == TINY) ? TINY_PAGES_NB * pagesize
		: SMALL_PAGES_NB * pagesize;
	new_data = mmap(0, size, MMAP_PROT, MMAP_FLAGS, -1, 0);
	if (new_data == MAP_FAILED)
		return (NULL);
	ft_bzero(new_data, size);
	((t_header*)new_data)->magic = M_MAGIC ^ (unsigned long int)new_data;
	((t_header*)new_data)->prev = (head == NULL) ? NULL : (void*)(head + 1);
	((t_header*)new_data)->next = NULL;
	if (head != NULL)
		head->next = new_data + sizeof(t_header);
	else
		g_malloc_data.datas[blk_size] = new_data + sizeof(t_header);
	defragment_memory(blk_size, new_data + g_malloc_data.datas_len[blk_size]);
	return (new_data + sizeof(t_header));
}

static int32_t		find_next_segment(void *data, void *data_end, size_t size)
{
	int32_t		len;

	len = *(int32_t*)(data);
	if (len > 0)
		return (len + sizeof(int32_t));
	else if ((len == 0 && data + sizeof(int32_t) + size <= data_end)
			|| -1 * len > (int32_t)(size + sizeof(int32_t)))
		return (0);
	else
		return ((-1 * len) + sizeof(int32_t));
}

void				*find_allocable_segment(size_t size, t_blocksize blk_size)
{
	void		*data;
	void		*d_end;
	int			i;
	int32_t		len;
	t_header	*head;

	PROFILE_BASIC;
	data = g_malloc_data.datas[blk_size];
	head = NULL;
	while (data != NULL)
	{
		head = header_change_segment((t_header**)&data, SEG_NONE, ORIGIN);
		d_end = data + g_malloc_data.datas_len[blk_size];
		i = 0;
		while (data + i < d_end)
		{
			len = find_next_segment(data + i, d_end, size);
			if (len != 0)
				i += len;
			else
				return (data + i);
		}
		data = header_change_segment(&head, SEG_NEXT, ORIGIN);
	}
	return (extend_blk(blk_size, head));
}
