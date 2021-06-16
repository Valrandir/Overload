#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <shellapi.h>

void ShellOpen(const char* filename)
{
	ShellExecuteA(0, "open", filename, 0, 0, SW_SHOW);
}

void ShellExplore(const char* path)
{
	ShellExecuteA(0, "explore", path, 0, 0, SW_SHOW);
}

void ShellExploreCurrentFolder()
{
	ShellExplore(".");
}
