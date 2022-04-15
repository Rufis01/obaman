#ifndef FILESYSTEM_H
#define FILESYSTEM_H

void FS_Init(void);
char *FS_LoadFileSync(const char *path, int *size);
char *FS_GetCorrectPath(const char *path);
char *FS_GetDirectoryPath(const char *path);
char *FS_GetFullPath(const char* directory, const char* filename);

#endif
