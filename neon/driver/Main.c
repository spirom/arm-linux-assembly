#include <stdio.h>
#include <sys/time.h>

#include "arm_neon.h"

void
dump_scalar(uint64_t size, uint8_t v[]) 
{
    for (uint64_t i = 0; i < size; i++) {
	printf ("%2x ", v[i]);
	if ((i+1) % 16 == 0) printf("\n");
    }
    printf("\n");
}

void 
vector_from_scalar(uint64_t vector_size, uint8_t s[], uint8x16_t v[])
{
    uint8_t *scalar_pos = s;
    for (uint64_t i = 0; i < vector_size; i++) {
        v[i] = vld1q_u8 (scalar_pos);
	scalar_pos += 16;
    }
}

void 
fill_scalar(uint64_t size, uint8_t v[])
{
    for (uint64_t i = 0; i < size; i++) {
        v[i] = i % 64;
    }   
}

void 
add_scalar(uint64_t size, uint8_t in1[], uint8_t in2[], uint8_t out[])
{
    for (uint64_t i = 0; i < size; i++) {
	out[i] = in1[i] + in2[i];
    }
}

void 
add_vector(uint64_t size, uint8x16_t in1[], uint8x16_t in2[], uint8x16_t out[])
{
    for (uint64_t i = 0; i < size; i++) {
        out[i] = vaddq_u8 (in1[i], in2[i]);
    }
}


extern void 
add_asm(uint64_t size, uint8x16_t in1[], uint8x16_t in2[], uint8x16_t out[]);

double
get_elapsed(struct timeval *before, struct timeval *after) 
{
    double elapsed;
    elapsed = (after->tv_sec - before->tv_sec) * 1000.0; 
    elapsed += (after->tv_usec - before->tv_usec) / 1000.0; 
    return elapsed;
}

_Bool
vector_compare(uint64_t size, uint8x16_t in1[], uint8x16_t in2[])
{
    uint8_t p1[16];
    uint8_t p2[16];

    for (uint64_t i = 0; i < size; i++) {
        vst1q_u8 (p1, in1[i]);
        vst1q_u8 (p2, in2[i]);
	for (int j = 0; j < 16; j++) {
	    if (p1[j] != p2[j]) {
		fprintf(stderr, "At position %ld : %u vs %u\n", i * 16 + j, p1[j], p2[j]);
		return 0;
	    }
	}
    }
    return 1;
}


#define VECTOR_SIZE 4000000ull
uint8_t v_1[VECTOR_SIZE];
uint8_t v_2[VECTOR_SIZE];
uint8_t r[VECTOR_SIZE];

uint8x16_t v128_1[VECTOR_SIZE / 16];
uint8x16_t v128_2[VECTOR_SIZE / 16];
uint8x16_t r128[VECTOR_SIZE / 16];
uint8x16_t r128_asm[VECTOR_SIZE / 16];


int main () {
    struct timeval before, after;
    double elapsed;
    
    fill_scalar(VECTOR_SIZE, v_1);
    fill_scalar(VECTOR_SIZE, v_2);

    vector_from_scalar(VECTOR_SIZE / 16, v_1, v128_1);
    vector_from_scalar(VECTOR_SIZE / 16, v_2, v128_2);

    double total_scalar = 0;
    double total_vector = 0;
    double total_asm = 0;
    const int reps = 10;

    for (int i = 0; i < reps; i++) {
        gettimeofday(&before, NULL);
        add_scalar(VECTOR_SIZE, v_1, v_2, r);
        gettimeofday(&after, NULL);
        total_scalar += get_elapsed(&before, &after);

        gettimeofday(&before, NULL);
        add_vector(VECTOR_SIZE / 16, v128_1, v128_2, r128);
        gettimeofday(&after, NULL);
        total_vector += get_elapsed(&before, &after);

        gettimeofday(&before, NULL);
        add_asm(VECTOR_SIZE / 16, v128_1, v128_2, r128_asm);
        gettimeofday(&after, NULL);
        total_asm += get_elapsed(&before, &after);
    }
    vector_compare(VECTOR_SIZE / 16, r128, r128_asm);
    printf("mean elapsed time (scalar) = %g msec\n", total_scalar / reps);
    printf("mean elapsed time (vector) = %g msec\n", total_vector / reps);
    printf("mean elapsed time (asm) = %g msec\n", total_asm / reps);

    //dump_scalar(VECTOR_SIZE, r);

    return 0;
}

