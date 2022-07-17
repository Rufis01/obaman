#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "behaviour.h"

#ifdef NO_REALLOC
static void *realloc(void *ptr, size_t size)
{
	void *ret = malloc(size);
	if(ret == NULL)
		return NULL;
	if(ptr)
		memcpy(ret, ptr, size);
	free(ptr);
	return ret;
}
#endif

bool E_AddBehaviour(G_Entity **ent_p, G_E_BehaviourHeader *behaviour)
{
	printf("Behaviour %p size %d FUN: %p\n", behaviour, behaviour->size, behaviour->behaviour);
	unsigned short oldSize = (*ent_p)->size;
	(*ent_p)->size += behaviour->size;

	G_Entity *ent = (G_Entity *) realloc(*ent_p, (*ent_p)->size);
	if(ent == NULL)
	{
		printf("Error!\n");
		(*ent_p)->size -= behaviour->size;
		return false;
	}
	
	*ent_p = ent;
	memcpy(((char *)(ent)) + oldSize, behaviour, behaviour->size);
	ent->nBehaviour++;

	return true;
}