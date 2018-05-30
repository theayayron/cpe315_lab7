#define AMAX 10			/* Maximum (square) array size */
#define CACHESIM 1		/* Set to 1 if simulating Cache */
#define CACHESIZE 16            /* Either 16 or 256 */
#define ASSOCIATIONS 1          /* Set to 1, 2, or 4 */

#include "matmul2.h"
#include <stdio.h>

int num_reads = 0, hits = 0;

static cache_line cache1[CACHESIZE];
static cache_line cache2[CACHESIZE];
static cache_line cache3[CACHESIZE];
static cache_line cache4[CACHESIZE];

/*	memory management, code density, Cache emulation - statistics generation */
/*	Generated for CSC 315 Lab 5 */
void init_cache(void) {
    int i = 0;
    for(; i < CACHESIZE; i++) {
	cache1[i].v = 0;
	cache1[i].m = 0;
	cache1[i].index = i;
	cache1[i].data = 0;
	cache1[i].tag = 0;

	if(CACHESIZE > 1) {
	    cache2[i].v = 0;
	    cache2[i].m = 0;
	    cache2[i].index = i;
	    cache2[i].data = 0;
	    cache2[i].tag = 0;
	}

	if (CACHESIZE == 4) {
	    cache3[i].v = 0;
	    cache3[i].m = 0;
	    cache3[i].index = i;
	    cache3[i].data = 0;
	    cache3[i].tag = 0;

	    cache4[i].v = 0;
	    cache4[i].m = 0;
	    cache4[i].index = i;
	    cache4[i].data = 0;
	    cache4[i].tag = 0;
	}
    }
} 


/* This function gets called with each "read" reference to memory */
mem_read(int *mp){
    num_reads += 1;
    uint8_t index; //
    uint8_t offset; // 2 bits
    uint64_t tag; //

    offset = isolate_bits((int)mp, 1, 0);

    if( sizeof(mp) == 32 ) {
        if (CACHESIZE == 16) {
            // indes size = 4 bits
            index = isolate_bits((int)mp, 5, 2);

            // tag size = 32 - 4 -2
            tag = isolate_bits((int)mp, 31, 6);
        } else {
            // index size = 7 bits
            index = isolate_bits((int)mp, 8, 2);

            // tag = 32 - 7 - 2
            tag = isolate_bits((int)mp, 31, 9);
        }
    } else {
        if (CACHESIZE == 16) {
            // index size = 4 bits
            index = isolate_bits((int)mp, 5, 2);

            // tag size = 64 - 4 -2
            tag = isolate_bits((int)mp, 61, 6);
        } else {
            // index size = 7 bits
            index = isolate_bits((int)mp, 8, 2);

            // tag = 64 - 7 - 2
            tag = isolate_bits((int)mp, 61, 9);
        }
    }
    /* printf("Memory read from location %p\n", mp);  */
}

/* This function gets called with each "write" reference to memory */
mem_write(int *mp) {
    uint8_t index;
    uint8_t offset;
    uint64_t tag;

    if(sizeof(mp) == 32) {

    } else {
	
    }

    /* printf("Memory write to location %p\n", mp); */
}

int isolate_bits(int base, int start, int end) {
    int result, mask = 0, i;

    result = base >> end;
    for (i = 0; i < (start - end + 1); i++) {
        mask <<= 1;
        mask |= 1;
    }
    return result & mask;
}

/* Statically define the arrays a, b, and mult, where mult will become the cross product of a and b, i.e., a x b. */
static int a[AMAX][AMAX], b[AMAX][AMAX], mult[AMAX][AMAX];

void matmul( int r1, int c1, int c2 ) {
   int i,j,k;
   int *mp1, *mp2, *mp3;

/* Initializing elements of matrix mult to 0.*/
    for(i=0; i<r1; ++i) {
        for (j = 0; j < c2; ++j) {
            mult[i][j] = 0;
        }
    }

/* Multiplying matrix a and b and storing in array mult. */

    for(i=0; i<r1; ++i) {
        for (j = 0; j < c2; ++j) {
            for (k = 0; k < c1; ++k) {

#if CACHESIM  /* "Hooks" to measure memory references - enabled if CACHESIM  */

                mp1 = &mult[i][j];
                mp2 = &a[i][k];
                mp3 = &b[k][j];
                mem_read(mp1);
                mem_read(mp2);
                mem_read(mp3);
                mem_write(mp1);
#endif

                mult[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

int main(void) {
    int r1, c1, r2, c2, i, j, k;
    int *mp1, *mp2, *mp3;

    printf("Size of pointer is: %d\n\n", sizeof(mp1));

    printf("Enter rows and column for first matrix: ");
    scanf("%d%d", &r1, &c1);
    printf("Enter rows and column for second matrix: ");
    scanf("%d%d",&r2, &c2);

    /* If column of first matrix in not equal to row of second matrix,
     * asking user to enter the size of matrix again. */
    while (c1 != r2) {
        printf("Error! column of first matrix not equal to row of second.\n");
        printf("Enter rows and column for first matrix: ");
        scanf("%d%d", &r1, &c1);
        printf("Enter rows and column for second matrix: ");
        scanf("%d%d",&r2, &c2);
    }

    /* Storing elements of first matrix. */
    for(i=0; i<r1; ++i) {
        for (j = 0; j < c1; ++j) {
            a[i][j] = i + j; // build sample data

        }
    }

    /* Storing elements of second matrix. */
    for(i=0; i<r2; ++i) {
        for (j = 0; j < c2; ++j) {
            b[i][j] = 10 + i + j;
        }
    }

   matmul(r1, c1, c2);  	/* Invoke matrix multiply function */	

    /* Displaying the multiplication of two matrix. */
    printf("\nOutput Matrix:\n");
    for(i=0; i<r1; ++i) {
        for (j = 0; j < c2; ++j) {
            printf("%d  ", mult[i][j]);
            if (j == c2 - 1)
                printf("\n\n");
        }
    }
    return 0;
}
