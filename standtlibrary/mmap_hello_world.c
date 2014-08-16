#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#define FILEPATH "mmapped.bin"
#define NUMINTS  (10)
#define FILESIZE (NUMINTS * sizeof(int))



int main(int argc, char *argv[])
{
    int i;
    int fd;
    int result;
    unsigned char *map;  /* mmapped array of int's */

    /* Open a file for writing.
     *  - Creating the file if it doesn't exist.
     *  - Truncating it to 0 size if it already exists. (not really needed)
     *
     * Note: "O_WRONLY" mode is not sufficient when mmaping.
     */
    //fd = open(FILEPATH, O_RDWR | O_CREAT | O_TRUNC, (mode_t)0600);
    fd = open(FILEPATH, O_RDWR, (mode_t)0600);
    if (fd == -1) {
		printf("Error opening file for writing");
		exit(EXIT_FAILURE);
    }

    /* Stretch the file size to the size of the (mmapped) array of ints
     */
    result = lseek(fd, FILESIZE-1, SEEK_SET);
    if (result == -1) {
		close(fd);
		printf("Error calling lseek() to 'stretch' the file");
		exit(EXIT_FAILURE);
    }
    
    /* Something needs to be written at the end of the file to
     * have the file actually have the new size.
     * Just writing an empty string at the current file position will do.
     *
     * Note:
     *  - The current position in the file is at the end of the stretched 
     *    file due to the call to lseek().
     *  - An empty string is actually a single '\0' character, so a zero-byte
     *    will be written at the last byte of the file.
     */
    //result = write(fd, "", 1);
	// if (result != 1) {
	//	close(fd);
	//	printf("Error writing last byte of the file");
	//	exit(EXIT_FAILURE);
	//}

    /* Now the file is ready to be mmapped.
     */
    map = mmap(0, FILESIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) {
		close(fd);
		printf("Error mmapping the file");
		exit(EXIT_FAILURE);
    }
    
    /* Now write int's to the file as if it were memory (an array of ints).
     */
	 printf("NUMINTS = %d\n", NUMINTS);
    for (i = 0; i <=NUMINTS; ++i) {
		//map[i] = 2 * i; 
		printf("map[i] = %c\n", map[i]);
    }
	
	//modify
	printf("Modify address 0 from %c to %c\n", 	map[0],  map[0] - 32);
	map[0] = map[0] - 32;
	
	
    for (i = 0; i <=NUMINTS; ++i) {
		//map[i] = 2 * i; 
		printf("affter change map[i] = %c\n", map[i]);
    }
	sleep(100);
    /* Don't forget to free the mmapped memory
     */
    if (munmap(map, FILESIZE) == -1) {
		printf("Error un-mmapping the file");
	/* Decide here whether to close(fd) and exit() or not. Depends... */
    }

    /* Un-mmaping doesn't close the file, so we still need to do that.
     */
    close(fd);
    return 0;
}

//gcc -o mmap_hello_world mmap_hello_world.c

/*
input: map.bin
	map.bin -> qwe rty uio op[ ]as dfg ghj kkl l;' ''\ zxc vbn nm, ./- 123 456 789 0-= =-0 987 765 443 321 /., mnb vcx z\' ;lk jhg fds a][ poi uyt rew qaz wsx edc rfv tgb yhn ujm ik, ol. p;/ [']
Output:
	[uydang@sv-linuxcentos71 testlibrary]$ ./mmap_hello_world
	NUMINTS = 10
	map[i] = q
	map[i] = w
	map[i] = e
	map[i] =  
	map[i] = r
	map[i] = t
	map[i] = y
	map[i] =  
	map[i] = u
	map[i] = i
	map[i] = o
	[uydang@sv-linuxcentos71 testlibrary]$ 

*/

