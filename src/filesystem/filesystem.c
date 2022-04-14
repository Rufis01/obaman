#include <stdlib.h>
#include <psxcd.h>

#include "filesystem.h"

void FS_Init()
{
	CdInit();
}

char *FS_LoadFileSync(const char *filename, int *size)
{
	CdlFILE filePos;
	int numsecs;
	char *buff = 0;
	
	/* locate the file on the CD */
	if(CdSearchFile(&filePos, filename))
	{
		if(size)
			*size = filePos.size;
		// CdRead reads 1 block (2048 bytes) at a time
		// We need to make sure we allocate enough space
		numsecs = (filePos.size+2047)>>11;
		
		buff = (char *)malloc(numsecs<<11);
		
		// Seek to file
		CdControl(CdlSetloc, (u_char*)&filePos.pos, 0);
		
		CdRead(numsecs, (u_long*)buff, CdlModeSpeed);
		
		// Wait for read to end
		CdReadSync( 0, 0 );
	}
	
	return(buff);
}
