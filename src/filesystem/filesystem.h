#ifndef FILESYSTEM_H
#define FILESYSTEM_H

void FS_Init();
char *FS_LoadFileSync(const char *filename, int *size);

#endif
