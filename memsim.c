#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
        int pageNo;
        int modified;
} page;

typedef struct{
	int modified;
	int frameno;
	int present;
	char AccessInfo;
} PTE;


enum    repl { random, fifo, lru, clock};
int     createMMU( int);
int     checkInMemory( int ) ;
int     allocateFrame( int ) ;
page    selectVictim( int, enum repl) ;
const   int pageoffset = 12;            /* Page size is fixed to 4 KB */
int     numFrames ;
const   int maxpages = 100;
PTE pageTable[maxpages];
int* frameTable;


/* Creates the page table structure to record memory allocation */
int createMMU (int frames)
{
	numFrames = frames;
	frameTable = (int*) malloc(numFrames* sizeof(int));
	for(int i=0; i<numFrames; i++){
		frameTable[i] = -1; //all slots are free initially
	}

	for(int j= 0; j<maxpages; j++){
		pageTable[j].frameno = -1;
		pageTable[j].modified = 0;
		pageTable[j].present = 0;
		pageTable[j].AccessInfo = 0;
	}
	
	return 0;
}

/* Checks for residency: returns frame no or -1 if not found */
int checkInMemory( int page_number)
{
	int result = -1;
	if(pageTable[page_number].present){
		result = pageTable[page_number].frameno;
	}
	return result;
}

/* allocate page to the next free frame and record where it put it */
int allocateFrame( int page_number)
{
		int free = -1;
		for(int i=0; i<numFrames; i++){
			if(frameTable[i] == -1){
				free = i;
				frameTable[i] = page_number;
				break;
			}
		}

		if(free == -1){
			//call selectVictim to free up space 
		}
		pageTable[page_number].present = 1;
		pageTable[page_number].frameno = free;
		

        // to do
        return free;
}

/* Selects a victim for eviction/discard according to the replacement algorithm,  returns chosen frame_no  */
page    selectVictim(int page_number, enum repl  mode )
{
        page    victim;
        // to do
		if(mode == random){

		}else if(mode == fifo){

		}else if(mode == lru){

		}else if(mode == clock){
			
		}

        victim.pageNo = 0;
        victim.modified = 0;
        return (victim) ;
}

		
main(int argc, char *argv[])
{
  
	char	*tracename;
	int	page_number,frame_no, done ;
	int	do_line, i;
	int	no_events, disk_writes, disk_reads;
	int     debugmode;
 	enum	repl  replace;
	int	allocated=0; 
	int	victim_page;
        unsigned address;
    	char 	rw;
	page	Pvictim;
	FILE	*trace;


        if (argc < 5) {
             printf( "Usage: ./memsim inputfile numberframes replacementmode debugmode \n");
             exit ( -1);
	}
	else {
        tracename = argv[1];	
	trace = fopen( tracename, "r");
	if (trace == NULL ) {
             printf( "Cannot open trace file %s \n", tracename);
             exit ( -1);
	}
	numFrames = atoi(argv[2]);
        if (numFrames < 1) {
            printf( "Frame number must be at least 1\n");
            exit ( -1);
        }
        if (strcmp(argv[3], "lru\0") == 0)
            replace = lru;
	    else if (strcmp(argv[3], "rand\0") == 0)
	     replace = random;
	          else if (strcmp(argv[3], "clock\0") == 0)
                       replace = clock;		 
	               else if (strcmp(argv[3], "fifo\0") == 0)
                             replace = fifo;		 
        else 
	  {
             printf( "Replacement algorithm must be rand/fifo/lru/clock  \n");
             exit ( -1);
	  }

        if (strcmp(argv[4], "quiet\0") == 0)
            debugmode = 0;
	else if (strcmp(argv[4], "debug\0") == 0)
            debugmode = 1;
        else 
	  {
             printf( "Replacement algorithm must be quiet/debug  \n");
             exit ( -1);
	  }
	}
	
	done = createMMU (numFrames);
	if ( done == -1 ) {
		 printf( "Cannot create MMU" ) ;
		 exit(-1);
        }
	no_events = 0 ;
	disk_writes = 0 ;
	disk_reads = 0 ;

        do_line = fscanf(trace,"%x %c",&address,&rw);
	while ( do_line == 2)
	{
		page_number =  address >> pageoffset;
		frame_no = checkInMemory( page_number) ;    /* ask for physical address */

		if ( frame_no == -1 )
		{
		  disk_reads++ ;			/* Page fault, need to load it into memory */
		  if (debugmode) 
		      printf( "Page fault %8d \n", page_number) ;
		  if (allocated < numFrames)  			/* allocate it to an empty frame */
		   {
                     frame_no = allocateFrame(page_number);
		     allocated++;
                   }
                   else{
		      Pvictim = selectVictim(page_number, replace) ;   /* returns page number of the victim  */
		      frame_no = checkInMemory( page_number) ;    /* find out the frame the new page is in */
		   if (Pvictim.modified)           /* need to know victim page and modified  */
	 	      {
                      disk_writes++;			    
                      if (debugmode) printf( "Disk write %8d \n", Pvictim.pageNo) ;
		      }
		   else
                      if (debugmode) printf( "Discard    %8d \n", Pvictim.pageNo) ;
		   }
		}
		if ( rw == 'R'){
		    if (debugmode) printf( "reading    %8d \n", page_number) ;
		}
		else if ( rw == 'W'){
		    // mark page in page table as written - modified  
		    if (debugmode) printf( "writting   %8d \n", page_number) ;
		}
		 else {
		      printf( "Badly formatted file. Error on line %d\n", no_events+1); 
		      exit (-1);
		}

		no_events++;
        	do_line = fscanf(trace,"%x %c",&address,&rw);
	}

	printf( "total memory frames:  %d\n", numFrames);
	printf( "events in trace:      %d\n", no_events);
	printf( "total disk reads:     %d\n", disk_reads);
	printf( "total disk writes:    %d\n", disk_writes);
	printf( "page fault rate:      %.4f\n", (float) disk_reads/no_events);
}
				
