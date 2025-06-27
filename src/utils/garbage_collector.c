/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 15:27:22 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/27 15:16:59 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Ajoute un noeud en tete de liste GC
static int	gc_add_node(t_gc *gc, void *ptr, t_gc_type type)
{
	t_gc_node	*new;

	new = ft_calloc(1, sizeof(t_gc_node));
	if (!new)
		return (-1);
	new->ptr = ptr;
	new->type = type;
	new->next = gc->head;
	gc->head = new;
	return (1);
}

// Alloue avec ft_calloc et enregistre
static void	*gc_alloc(t_gc *gc, size_t size, t_gc_type type)
{
	void	*ptr;

	ptr = ft_calloc(1, size);
	if (!ptr)
		return (NULL);
	if (gc_add_node(gc, ptr, type) == -1)
	{
		free (ptr);
		return (NULL);
	}
	return (ptr);
}

// Libère un pointeur donné
static void	gc_free_one(t_gc *gc, void *ptr)
{
	t_gc_node	*curr;
	t_gc_node	*prev;

	curr = gc->head;
	prev = NULL;
	while (curr)
	{
		if (curr->ptr == ptr)
		{
			if (prev)
				prev->next = curr->next;
			else
				gc->head = curr->next;
			free(curr->ptr);
			free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}

// Libère tout ou par type
static void	gc_free_all(t_gc *gc, t_gc_type type)
{
	t_gc_node	*curr;
	t_gc_node	*tmp;
	t_gc_node	*prev;

	curr = gc->head;
	prev = NULL;
	while (curr)
	{
		if (type == GC_NONE || curr->type == type)
		{
			tmp = curr;
			if (prev)
				prev->next = curr->next;
			else
				gc->head = curr->next;
			curr = curr->next;
			free(tmp->ptr);
			free(tmp);
		}
		else
		{
			prev = curr;
			curr = curr->next;
		}
	}
}

// Gestion memoire
void	*gc_mem(t_gc_action op, size_t size, void *ptr, t_gc_type type)
{
	t_gc	*gc;

	gc = &get_shell()->gc;
	if (op == GC_ALLOC)
		return (gc_alloc(gc, size, type));
	else if (op == GC_FREE_ONE)
		return (gc_free_one(gc, ptr), NULL);
	else if (op == GC_FREE_ALL)
		return (gc_free_all(gc, type), NULL);
	return (NULL);
}

/* Exemple pour appeler fonctions :
char *s1 = gc_mem(GC_ALLOC, 100, NULL, GC_TMP);		// alloue 100 octets
gc_mem(GC_FREE_ONE, 0, s1, 0);						// libère s1
gc_mem(GC_FREE_ALL, 0, NULL, GC_CMD);				// libère tous les GC_CMD
gc_mem(GC_FREE_ALL, 0, NULL, GC_CMD);				// libère tout
*/

/* int main(void)
{
	init_shell();

	// Alloc pour unr str
	char *str1 = gc_mem(GC_ALLOC, 50, NULL, GC_TMP);
	if (!str1)
	{
		fprintf(stderr, "Echec de alloc pour str1\n");
		return EXIT_FAILURE;
	}
	strcpy(str1, "Coucou Thomas, c'est str1.");
	printf("str1: %s\n", str1);

	// Alloc pour str2
	char *str2 = gc_mem(GC_ALLOC, 100, NULL, GC_ENV);
	if (!str2)
	{
		fprintf(stderr, "Echec de alloc pour str2\n");
		return EXIT_FAILURE;
	}
	strcpy(str2, "C'est str2, alloue avec GC_ENV.");
	printf("str2: %s\n", str2);

	// Free de str1
	gc_mem(GC_FREE_ONE, 0, str1, 0);
	printf("str1 a ete libere.\n");


	// Free de toutes les alloc type GC_ENV
	gc_mem(GC_FREE_ALL, 0, NULL, GC_ENV);
	printf("toutes les allocs type GC_ENV liberes.\n");

	// Free de toutes les alloc restantes
	gc_mem(GC_FREE_ALL, 0, NULL, GC_CMD);
	printf("toutes les allocs restantes liberes.\n");

	return EXIT_SUCCESS;
} */
