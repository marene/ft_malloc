#ifndef FT_MALLOC_H
# define FT_MALLOC_H

# include <stdint.h>
# include <stdlib.h>

# define M_OK				0x00
# define M_NOK				0x01

# define MMAP_PROT			PROT_READ | PROT_WRITE
# define MMAP_FLAGS			MAP_ANON | MAP_PRIVATE

# define TINY_PAGES_NB		250
# define SMALL_PAGES_NB		400 // SMALL_MAX_SIZE 4 * plus grnd que TINY_MAX_SIZE, donc 4 * plus de pages?

# define TINY_ATOMIC		16
# define SMALL_ATOMIC		997 // <- Bidouiller le nombre de pages / les atomic pour que ces derniers soient des puissance de deux (je pense que ce sera plus propre)

# define TINY_MAX_SIZE		1024
# define SMALL_MAX_SIZE		4096

/**
 * \struct metadata_s
 * \brief Structure permettant le stockage des metadonnees relatives aux blocs de
 *        memoire ayant ete malloc \n
 *
 *        void* data_tiny -> Pointeur sur le debut de la premiere page de la region
 *        "tiny" \n
 *        void* data_small -> Idem, pour la region small (duh) \n
 *        void* meta_tiny -> pointeur sur le debut de la (les) page(s) de metadonnees
 *        relatives a la region "tiny" \n
 *        void* meta_small -> idem que meta_tiny, pour la region "small" \n
 *        void* meta_large -> C'est bon, t'as compris maintenant?
 */

// TODO: Ajouter une variable global metadata_t malloc_meta_g dans le fichier qui contiendra la definition de la fonction malloc(size_t size)

typedef enum	blocksize_s
{
	TINY = 0,
	SMALL,
	LARGE,
}				blocksize_t;

typedef struct	metadata_s
{
	void*		data_tiny;
	void*		data_tiny_end;
	void*		data_small;
	void*		data_small_end;
	void*		datas[2];
	void*		datas_end[2];
	void**		meta_tiny;
	void**		meta_tiny_end;
	void**		meta_small;
	void**		meta_small_end;
	void**		meta_large; // On a meme pas besoin de tenir des metadata sur les larges /!
	void		**meta_pages_start[2];
//	void		*meta_pages_end[2];
	size_t		meta_len[2];
}				metadata_t; // <- globale

metadata_t		malloc_data_g;

int				pages_init(blocksize_t* blk_size);	// <- initialise metadata_t
int				create_meta(void);		// |
int				create_data(void);		// |
int				metadata_init(blocksize_t* blk_size);
size_t			get_metapagesize(blocksize_t size);
size_t			get_metapagelen(blocksize_t size);
void*			metadata_retrieve(void* usr_ptr, blocksize_t* blk_size);
int				metadata_add(void* usr_ptr, blocksize_t blk_size);
int				metadata_remove(void* usr_ptr, blocksize_t blk_size);

void*	ft_malloc(size_t size);
void	ft_free(void* ptr);

#endif
