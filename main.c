/* ======================================================================================
    GENETIC ALGORITHM - IMPLEMENTED BY NATHAN GREWAL
 
    Uses a genetic algorithm to solve an optimization problem
 
    Encodes integers as binary strings, which we treat as chromosomes
    The population reproduces several time, applying Darwin's "survival of the fittest"
    (meaning more optimal solutions have a greater chance of reproducing).
 
    Over time, we expect the population to contain more optimal solutions.
 
    In this case, the genetic algorithm attempts to maximize the function f(x) = x^2
        on the interval [0, 31]
   =======================================================================================  */

#include "random_nums.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

/* ================================
    GLOBAL VARIABLES AND CONSTANTS
   ================================ */
int curr_gen;
int sumfitness;
int xover_count, mut_count; // track # of crossovers & mutations
const int POPULATION_SIZE = 40; // must be even
const int STRING_LENGTH = 5;
const int MAX_GEN = 30;
const float PMUTATION = 0.0001;

/* ================================
    TYPE AND STRUCTURE DEFINITIONS
   ================================ */
typedef bool Allele;
typedef Allele Chromosome[STRING_LENGTH]; // bit string
struct individual {
    Chromosome chrom;
    int phenotype; // decoded bit string
    int fitness; // fitness function evaluated for this chromosome
};
typedef struct individual Population[POPULATION_SIZE];
Population oldpop, newpop;

/* =============================================================================
    USES ROULETTE WHEEL SELECTION TO SELECT A CHROMOSOME FROM oldpop FOR
        REPRODUCTION
    REQUIRES: oldpop POINTS TO A VALID POPULATION
              sumfitness IS THE SUM OF THE FITNESS OF EACH INDIVIDUAL IN oldpop
   ============================================================================= */
int select_indiv (void) {
    float partial_sum = 0;
    float rand = random_real() * sumfitness;
    int j = 0;
    
    while (partial_sum < rand && j != POPULATION_SIZE) {
        j++;
        partial_sum += oldpop[j].fitness;
    }
    
    return j;
}

/* ======================================================================================
    USING PROBABILITY pmutation, MAY FLIP THE VALUE OF parentval AND RETURNS THE RESULT.
   ====================================================================================== */
Allele mutate (Allele parentval) {
    if (flip(PMUTATION)) {
        mut_count++;
        if (parentval) return 0;
        else return 1;
    }
    return parentval;
}

/* ==============================================================================
    GIVEN PARENT CHROMOSOMES parent1 AND parent2, GENERATES 2 OF THEIR OFFSPRING
    AND STORES THE CHILDREN IN newpop AT index AND index+1
   ============================================================================== */
void crossover (Chromosome parent1, Chromosome parent2, int index) {
    
    int jcross = random_int (1, STRING_LENGTH-1);
    
    // Copy string1 to child1, string2 to child2
    for (int j = 0; j < jcross; ++j) {
        newpop[index].chrom[j] = mutate(parent1[j]);
        newpop[index+1].chrom[j] = mutate(parent2[j]);
    }
    // Copy string1 to child2, string2 to child1
    for (int j = jcross; j < STRING_LENGTH; ++j) {
        newpop[index].chrom[j] = mutate(parent2[j]);
        newpop[index+1].chrom[j] = mutate(parent1[j]);
    }

}

/* ======================================================================
    TREATS A CHROMOSOME AS A BINARY STRING, AND DECODES IT AS AN INTEGER
   ====================================================================== */
int decode (Chromosome chrom) {
    int accum = 0;
    int base = 1;
    for (int i = STRING_LENGTH-1; i >= 0; i--) {
        accum += chrom[i] * base;
        base *= 2;
    }
    return accum;
}

/* ==================================================================================================
    THE FITNESS FUNCTION USED TO DETERMINE FITNESS OF AN INDIVIDUAL; HIGHER OUTPUTS ARE MORE OPTIMAL
    IN THIS CASE, THE FITNESS FUNCTION IS f(x) = x^2
   ================================================================================================== */
int fitness (int x) {
    return x * x;
}

/* =======================================================================
 FOR EACH INDIVIDUAL IN oldpop, CALCULATES THEIR PHENOTYPE AND FITNESS
 AND STORES THE DATA IN THE STRUCTURE
 ======================================================================= */
void update_fitness (void) {
    sumfitness = 0;
    for (int i = 0; i < POPULATION_SIZE; ++i) {
        oldpop[i].phenotype = decode(oldpop[i].chrom);
        oldpop[i].fitness = fitness(oldpop[i].phenotype);
        sumfitness += oldpop[i].fitness;
    }
}

/* ==================================================================
    USING oldpop, GENERATES A NEW POPULATION AND STORES IT IN newpop
        NOTE: Assumes POPULATION_SIZE is even
   ================================================================== */
void generation (void) {
    int mate1, mate2;
    for (int j = 0; j < POPULATION_SIZE; j+=2) {
        
        mate1 = select_indiv();
        mate2 = select_indiv();
        
        // crossover and mutation:
        crossover(oldpop[mate1].chrom, oldpop[mate2].chrom, j);
    }
    
    sumfitness = 0;
    // new generation is in newpop; copy it to oldpop and update fitnesses/phenotypes:
    for (int i = 0; i < POPULATION_SIZE; i++) {
        for (int j = 0; j < STRING_LENGTH; j++) {
            oldpop[i].chrom[j] = newpop[i].chrom[j];
        }
    }
    
    update_fitness();
    
    curr_gen++;
}

/* ===========================================================
    INITIALIZES PROGRAM BY ALLOCATING HEAP MEMORY AND FILLING
    THE FIRST GENERATION WITH RANDOM CHROMOSOMES
    HALTS PROGRAM IF MEMORY ALLOCATION FAILS
   =========================================================== */
void initialize (void) {
    for (int i = 0; i < POPULATION_SIZE; i++) {
        for (int j = 0; j < STRING_LENGTH; j++) {
            oldpop[i].chrom[j] = flip(0.5); // may be 0 or 1
        }
    }
    curr_gen = 1;
    update_fitness();
    xover_count = 0;
    mut_count = 0;
}

/* =============================================
    OUTPUTS A SUMMARY OF THE CURRENT POPULATION
   ============================================= */
void print_curr_gen (void) {
    for (int i = 0; i < 33; ++i) printf("-");
    printf("\nCurrent generation: %d\n", curr_gen);
    int index_max = 0;
    int index_min = 0;
    int tot_fitness = 0;
    
    for (int i = 0; i < POPULATION_SIZE; i++) {
        printf("%d: ", i+1);
        for (int j = 0; j < STRING_LENGTH; j++) {
            if (oldpop[i].chrom[j]) printf("1");
            else printf("0");
        }
        printf("\tPhenotype: %d\t ", oldpop[i].phenotype);
        printf("Fitness: %d\t\n", oldpop[i].fitness);
        if (oldpop[i].fitness > oldpop[index_max].fitness) index_max = i;
        if (oldpop[i].fitness < oldpop[index_min].fitness) index_min = i;
        tot_fitness += oldpop[i].fitness;
    }
    printf("Maximum fitness: %d\n", oldpop[index_max].fitness);
    printf("Minimum fitness: %d\n", oldpop[index_min].fitness);
    printf("Average fitness: %f\n", (float)tot_fitness / (float)POPULATION_SIZE);
    printf("Num mutations: %d\n", mut_count);
}

int main(void) {
    initialize();
    print_curr_gen();
    
    // Population replaces itself MAX_GEN times
    for (int i = 1; i < MAX_GEN; ++i) {
        generation();
        print_curr_gen();
    }
}
