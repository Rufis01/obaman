#include "filesystem.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <psxcd.h>


void FS_Init(void)
{
	CdInit();

	printf("FileSystem initialized!\n");
}

char *FS_LoadFileSync(const char *path, int *size)
{
	CdlFILE filePos;
	int numsecs;
	char *buff = 0;
	
	/* locate the file on the CD */
	if(CdSearchFile(&filePos, path))
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

char *FS_GetCorrectPath(const char *path)
{
	char *cpath = malloc(256);
	if(!cpath)
		return 0;

	snprintf(cpath, 255, "\\%s", path);	//Copy path and prepend a backslash

	char *c = cpath;
	while(*c != '\0')	//Replace any forward slash with a backslash
	{
		if(*c == '/') *c = '\\';
		c++;
	}

	return cpath;
}

char *FS_GetDirectoryPath(const char *path)
{
	int length;
	char *dpath = malloc(256);
	if(!dpath)
		return 0;

	const char *lastdir = strrchr(path, '\\');
	if(!lastdir) length = strlen(path);
	else length = (int)(lastdir-path);

	if(length > 255)
		return 0;

	strncpy(dpath, path, length);

	return dpath;
}

char *FS_GetFullPath(const char* directory, const char* filename)
{
	char *path = malloc(256);
	snprintf(path, 255, "%s\\%s", directory, filename);
	return path;
}
