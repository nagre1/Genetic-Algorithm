/*=====================================================
    random_nums contains functions which are used to
    generate random integers, real numbers, and boolean
    values
 
    Written by Nathan Grewal
 ====================================================*/

#include <stdbool.h>
#include <stdlib.h>

/*========================================================
    Returns a random real number on the interval [0, 1]
 =======================================================*/
float random_real (void);

/*===========================================================
    Returns a boolean true value with specified probability
    (a Bernoulli random variable)
 ==========================================================*/
bool flip (const float prob);

/*=======================================================
    Returns a random integer on the interval [low, high]
 ======================================================*/
int random_int (const int low, const int high);
