#include "apue.h"
#include <dirent.h>
#include <limits.h>

/*	function type that is  called for each filename		*/
typedef int Myfunc(const char *, const struct stat *, int);

static Myfunc myfunc;
static int	myftw(char *, Myfunc *);
static int	dopath(Myfunc *);
static long nreg, ndir, nblk, nchr, nfifo, nslink, nsock, ntot;

int
main(int argc, char *argv[])
{
	int	ret;
	if (argc != 2)
		err_quit("usage: ftw <starting-pathname>");
	ret = myftw(argv[1], myfunc);
	ntot = nreg + ndir + nblk + nchr + nfifo + nslink + nsock ;
	if ( ntot == 0)
		ntot = 1;	// avoid divide by 0; print 0 for all counts
	printf("regular files = %7ld,  %5.2f %%\n", nreg, nreg*100.0/ntot);
	
	printf("directories   = %7ld,  %5.2f %%\n", ndir, ndir*100.0/ntot);
	
	printf("block special = %7ld,  %5.2f %%\n", nblk, nblk*100.0/ntot);
	
	printf("char special  = %7ld,  %5.2f %%\n", nchr, nchr*100.0/ntot);
	
	printf("FIFOs         = %7ld,  %5.2f %%\n", nfifo, nfifo*100.0/ntot);
	
	printf("symbolic links= %7ld,  %5.2f %%\n", nslink, nslink*100.0/ntot);
	
	printf("sockets       = %7ld,  %5.2f %%\n", nsock, nsock*100.0/ntot);
	
	exit(ret);
}


/*
* Descend through the hierarchy, starting at "pathname".
* The caller's func() is called for every file.
*/
#define FTW_F 1
#define FTW_D 2
#define FTW_DNR 3
#define FTW_NS 4

static char *fullpath;		//contains full pathname for every file
static size_t pathlen;

static int 
myftw(char *pathname, Myfunc *func)
{
	fullpath = path_alloc(&pathlen);	//alloc PATH_MAX+1 bytes
	
	if (pathlen <= strlen(pathname)) {
		pathlen = strlen(pathname) * 2;
		if ((fullpath = realloc(fullpath, pathlen)) == NULL)
			err_sys("realloc failed");
	}
	strcpy(fullpath, pathname);
	return(dopath(func));
}

/*
 * Descend through the hierarchy, starting at "pathname".
 * If "fullpath" is anything other than a directory, we lstat() it,
 * call func(), and return. For a directory , we call outself
 * recursively for each name in the directory.
 */

static int 
dopath(Myfunc* func)
{
	struct stat	statbuf;
	struct dirent	*dirp;
	DIR		*dp;
	int 		ret,n;

	if(lstat(fullpath,&statbuf) < 0)	//stat error
		return(func(fullpath,&statbuf,FTW_NS));
	if(S_ISDIR(statbuf.st_mode) == 0)	//not a directory
		return(func(fullpath,&statbuf,FTW_F));
	/*
	 * It is a directory. FIrst call func() for the directory,
	 */

}
