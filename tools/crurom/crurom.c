const char crurom_rcsid[] = "@(#) $Id$";

/*
 * $Log$
 * Revision 1.1  2003/07/20 19:18:16  haraldkipp
 * First check in
 *
 * Revision 1.5  2002/08/08 16:23:19  harald
 * Command line options for verbose mode, outfile name and recursive
 * directory scan added.
 *
 */

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#ifdef _WIN32
#include <io.h>
#include "dirent.h"
#else
#include <dirent.h>
#endif

#include "getopt.h"

#ifndef O_BINARY
#define O_BINARY 0
#endif
 
#define stricmp strcasecmp
#define strnicmp strncasecmp


#define IDENT   "crurom"
#define VERSION "1.2.2"

static int entryno = 0;
static int verbose = 0;
static int recursive = 0;
static char rootdir[256];
static int rootlen = 0;
static char outname[256];
static FILE *fpout;

int dofile(char *name)
{
    int rc = 0;
    int fd;
    unsigned char buf[512];
	int i;
	int cnt;
	long total = 0;
    char *fsname = name;

    if(strnicmp(fsname, rootdir, rootlen) == 0)
        fsname += rootlen;
        
    if((fd = open(name, O_RDONLY | O_BINARY)) == -1) {
        fprintf(stderr, IDENT ": Error %d opening %s\n", errno, name);
        return -1;
    }
    if(verbose)
        fprintf(stderr, IDENT ": Reading %s\n", name);

	for(;;) {
		if((cnt = read(fd, buf, sizeof(buf))) < 0) {
			fprintf(stderr, IDENT ": Error %d reading %s\n", errno, name);
            rc = -1;
            total = 0;
			break;
		}
		if(total == 0) {
			entryno++;
			fprintf(fpout, "/*\n * File entry %d: %s\n */\n", entryno, fsname);
			fprintf(fpout, "prog_char file%ddata[] = {", entryno);
		}
		if(cnt == 0)
			break;
		total += cnt;
		for(i = 0; i < cnt; i++) {
			if((i % 16) == 0)
				fprintf(fpout, "\n");
			fprintf(fpout, "0x%02x,", buf[i]);
		}
	}
    close(fd);

	fprintf(fpout, "\n};\nstatic ROMENTRY file%dentry = { ", entryno);
	if(entryno > 1)
		fprintf(fpout, "&file%dentry, ", entryno - 1);
	else
		fprintf(fpout, "0, ", entryno - 1);
    fprintf(fpout, "\"%s\", %d, (prog_char *)file%ddata };\n\n", fsname, total, entryno);

    return rc;
}

int dodir(char *dirpath)
{
    int rc = 0;
    char path[256];
	DIR *dir;
	struct dirent *dire;
	struct stat statbuf;

    if((dir = opendir(dirpath)) == NULL) {
        fprintf(stderr, "Failed to scan directory %s\n", dirpath);
        return -1;
    }
    if(verbose)
        fprintf(stderr, "Scan %s\n", dirpath);
    while((dire = readdir(dir)) != NULL && rc == 0) {
        if(dire->d_name[0] == '.' || stricmp(dire->d_name, "cvs") == 0)
            continue;
        strcpy(path, dirpath);
        strcat(path, "/");
        strcat(path, dire->d_name);
        stat(path, &statbuf);
  
        if(statbuf.st_mode & S_IFDIR)
            rc = dodir(path);
        else if(statbuf.st_mode & S_IFREG) 
            rc = dofile(path);
    }
    closedir(dir);
    return rc;
}

void usage(void)
{
    fputs("Usage: crurom OPTIONS DIRECTORY\n"
      "OPTIONS:\n"
      "-ofile  output file\n"
      "-r      recursive\n"
      "-v      verbose\n"
    , stderr);
}

int main(int argc, char **argv)
{
    int option;
    int i;
    int rc = 0;

    while((option = getopt(argc, argv, "o:rv?")) != EOF) {
        switch(option) {
        case 'o':
            strcpy(outname, optarg);
            break;
        case 'r':
            recursive++;
            break;
        case 'v':
            verbose++;
            break;
        default:
            usage();
            return 1;
        }
    }
    argc -= optind;
    argv += optind;

    if(outname[0]) {
        if((fpout = fopen(outname, "w")) == NULL) {
            fprintf(stderr, "Can't write to %s\n", outname);
            return 3;
        }
    }
    else
        fpout = stdout;

    fprintf(fpout, "/*\n");
    fprintf(fpout, " * This file is automatically created by " IDENT " " VERSION "\n");
    fprintf(fpout, " */\n");
    fprintf(fpout, "#include <fs/uromfs.h>\n\n");

    if(argc) {
        for(i = 0; i < argc && rc == 0; i++) {
            strcpy(rootdir, argv[i]);
            strcat(rootdir, "/");
            rootlen = strlen(rootdir);
            rc = dodir(argv[i]);
        }
    }
    else {
        strcpy(rootdir, "./");
        rootlen = 2;
        rc = dodir(".");
    }
	fprintf(fpout, "\nROMENTRY *romEntryList = &file%dentry;\n", entryno);
    if(fpout != stdout)
        fclose(fpout);
    return rc;
}
