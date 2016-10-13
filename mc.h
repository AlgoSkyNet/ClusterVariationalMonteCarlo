/* 
 * File:   mc.h
 * Author: Chris Dahnken
 *
 * Created on November 8, 2011, 4:18 PM
 * 
 * Minimal routines to treat integers up to 128 bit 
 * with native types and with gmp for larger sizes.
 * 
 */

#ifndef MC_H
#define	MC_H

#include <stdio.h>

/*
 * Definition of bit size. Choose one of the following by uncommenting:
 * USE_64BIT - use the 64 native type
 * USE_128BIT - use the 128bit native type (works with GCC only at the moment)
 * USE_GMP - use GNU multi-precision library
 */
//#define USE_128BIT
#define USE_64BIT
//#define USE_GMP

#ifdef USE_GMP
#include <gmp.h>
#include <gmpxx.h>
#endif

/* Primitive type definitions, used throughout the software*/
typedef unsigned long int mc_uint32;
typedef signed long int mc_int32;
typedef unsigned long long int mc_uint64;
typedef signed long long int mc_int64;

/* wrapper for 32bit random function */
mc_uint32 mc_rand32() {
    mc_uint32 a, b;
    a = (mc_uint32) rand();
    b = (mc_uint32) rand();
    return (mc_uint32) (a + b);
}

/* 64bit random function made out of two 32bit random functions */
mc_uint64 mc_rand64() {
    mc_uint64 a = (mc_uint64) mc_rand32();
    mc_uint64 b = (mc_uint64) mc_rand32();
    return (mc_uint64) ((a << ((mc_uint64) 32)) + b);
}

/* popcount64 function wrapper for two 32bit popcounts*/
int mc_popcount64(mc_uint64 s) {
    mc_uint64 mask;
    mask = ((mc_uint64) 1);
    mask = mask << ((mc_uint64) 32);
    mask = mask - ((mc_uint64) 1);
    mc_uint64 a = s & mask;
    mc_uint64 b = (s >> ((mc_uint64) 32)) & mask;
    return __builtin_popcount(a) + __builtin_popcount(b);
}

#ifdef USE_GMP
typedef mpz_class mc_state;

int mc_state_popcount(mpz_class a) {
    return mpz_popcount(a.get_mpz_t());
}
#endif

#ifdef USE_128BIT
#define MC_NBITS 128 
typedef signed __int128 mc_int128;
typedef unsigned __int128 mc_uint128;
typedef unsigned __int128 mc_state;

/* random function for 128bit from two 64bit random functions */
mc_state mc_state_rand() {
    mc_state a = (mc_state) mc_rand64();
    mc_state b = (mc_state) mc_rand64();
    return (a << ((mc_state) 64))+b;
}

/* popcount function for 128bit from two 64bit popcount functions*/
int mc_state_popcount(mc_state s) {
    mc_state mask = (((mc_state) 1) << 64)-((mc_state) 1);
    mc_uint64 a = s & mask;
    mc_uint64 b = (s >> 64) & mask;
    return mc_popcount64(a) + mc_popcount64(b);
}

#endif

#ifdef USE_64BIT
#define MC_NBITS 64
typedef mc_uint64 mc_state;
//    int mc_state_popcount(mc_state a){return __builtin_popcount(a);} 

mc_state mc_state_rand() {
    return mc_rand64();
}

/* 
 * wrapper function for 64bit popcount just returns the original 
 * 64bit function as defined above
 */
int mc_state_popcount(mc_state s) {
    return mc_popcount64(s);
}
#endif

/* 
 */

unsigned int mc_state_get_uli(mc_state a) {
#ifdef USE_GMP
    return (a.get_ui());
#else
    return ((unsigned long int) a);
#endif
}

double mc_state_get_d(mc_state a) {
#ifdef USE_GMP
    return (a.get_d());
#else
    return ((double) a);
#endif
}

mc_state mc_state_set_all_bits(int i) {
    // sets all bits up to the specified position
#ifdef USE_GMP
    mpz_class ONE = 1;
    return ((ONE << i) - ONE);
#else
    if (i < MC_NBITS) {
        return (((mc_state) 1) << ((mc_state) i))-((mc_state) 1);
    } else {
        return ((mc_state_set_all_bits(i - 1) << ((mc_state) 1)) + ((mc_state) 1));
    }
#endif
}

const mc_state ZERO = ((mc_state) 0);

const mc_state ONE = ((mc_state) 1);


/* 
 * mc_state_get_bit 
 * returns the bit at position i in integer s
 */
int mc_state_get_bit(mc_state s, int i) {
    return ((s & (ONE << i)) >> i);
}

/* 
 * mc_state_set_bit
 * returns an integer with the bit in s set at position i
 */
mc_state mc_state_set_bit(mc_state s, int i) {
    mc_state mctmp1 = ((s & (ONE << i)) | (ONE << i));
    mc_state mctmp2 = s + mctmp1;
    return mctmp2;
}

/* 
 * mc_state_set_bit
 * returns an integer with the bit in s cleared at position i
 */
mc_state mc_state_unset_bit(mc_state s, int i) {
    return (s - (s & (ONE << i)));
}

void mc_state_get_binary_char_array(mc_state s, char* c, int l) {
    for (int i = (l - 1); i >= 0; i++) {
        if (mc_state_get_bit(s, i) == 1) {
            c[i] = '1';
        } else {
            c[i] = '0';
        }
    }
}

void mc_state_print_binary_string(mc_state s, int l) {
    char* c = new char[l];
    mc_state_get_binary_char_array(s, c, l);
    printf("%s %llu\n", c, s);
    delete[] c;
}
#endif	/* MC_H */

