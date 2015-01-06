
#ifdef WIN32
// http://stackoverflow.com/questions/2038912/how-to-recursively-traverse-directories-in-c-on-windows
// call: FindFilesRecursively(_T("C:\\WINDOWS"), _T("*.wav"))

void
FindFilesRecursively(LPCTSTR lpFolder, LPCTSTR lpFilePattern)
{
    TCHAR szFullPattern[MAX_PATH];
    WIN32_FIND_DATA FindFileData;
    HANDLE hFindFile;

    // first we are going to process any subdirectories
    PathCombine(szFullPattern, lpFolder, _T("*"));
    hFindFile = FindFirstFile(szFullPattern, &FindFileData);
    if(hFindFile != INVALID_HANDLE_VALUE)
    {
        do
        {
            if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                // found a subdirectory; recurse into it
                PathCombine(szFullPattern, lpFolder, FindFileData.cFileName);
                FindFilesRecursively(szFullPattern, lpPattern);
                if (FindFileData.cFileName[0] == '.') continue;
            }
        } while(FindNextFile(hFindFile, &FindFileData));
        FindClose(hFindFile);
    }
    // now we are going to look for the matching files
    PathCombine(szFullPattern, lpFolder, lpFilePattern);
    hFindFile = FindFirstFile(szFullPattern, &FindFileData);
    if(hFindFile != INVALID_HANDLE_VALUE)
    {
        do
        {
            if(!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                // found a file; do something with it
                PathCombine(szFullPattern, lpFolder, FindFileData.cFileName);
                _tprintf_s(_T("%s\n"), szFullPattern);
            }
        } while(FindNextFile(hFindFile, &FindFileData));
        FindClose(hFindFile);
    }
}

#else

#include <sys/types.h>
#include <dirent.h>
#include <fnmatch.h>

// http://compgroups.net/comp.unix.programmer/recursively-search-directories-in-c-c++/172909

void 
FindFilesRecursively(const char *path, char *pattern)
{  
    DIR *dirp = opendir(path);
    while (dirp)
    {
        struct dirent *dp;
        errno = 0;
        if ((dp = readdir(dirp)) != NULL)
        {
            if (dp->d_type == DT_DIR) // go recursive
            {
            }
            else if (dp->d_type == DT_REG)
            {
                if (!fnmatch(dp->d_name, pattern, FNM_PATHNAME))
                {
                }
            }
            closedir(dirp);
        }
    }
}
#endif
