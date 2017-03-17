 #include <sys/types.h>
       #include <sys/stat.h>
       #include <limits.h>
       #include <stdio.h>
       #include <stdlib.h>
       #include <unistd.h>

       int
       main(int argc, char *argv[])
       {
           struct stat sb;
           char *linkname;
           ssize_t r, bufsiz;

           if (argc != 2) {
               fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
               exit(EXIT_FAILURE);
           }

           if (lstat(argv[1], &sb) == -1) {
               perror("lstat");
               exit(EXIT_FAILURE);
           }

           bufsiz = sb.st_size + 1;

           /* Some magic symlinks under (for example) /proc and /sys
              report 'st_size' as zero. In that case, take PATH_MAX as
              a "good enough" estimate */

           if (sb.st_size == 0)
               bufsiz = PATH_MAX;

           printf("%zd\n", bufsiz);

           linkname = malloc(bufsiz);
           if (linkname == NULL) {
               perror("malloc");
               exit(EXIT_FAILURE);
           }

           r = readlink(argv[1], linkname, bufsiz);
           if (r == -1) {
               perror("readlink");
               exit(EXIT_FAILURE);
           }

           linkname[r] = '\0';

           printf("'%s' points to '%s'\n", argv[1], linkname);

           if (r == bufsiz)
               printf("(Returned buffer may have been truncated)\n");

           free(linkname);
           exit(EXIT_SUCCESS);
       }
