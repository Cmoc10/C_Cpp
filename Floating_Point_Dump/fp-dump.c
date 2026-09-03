#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

void printSpecial(int binary, int exponent, int mantissa, int numBits) {
    int sign = (binary >> (numBits - 1)) & 1;
    int exp_rep = (binary >> (numBits - 1 - exponent)) & ((1 << exponent) - 1);
    int mant_rep = binary & ((1 << (numBits - 1 - exponent)) - 1);
    if(exp_rep == (1 << exponent) - 1){
        for (int j = numBits - 1; j >= 0; j--){
            printf("%d", (binary >> j) & 1);
        }
        //infinity or NaN
        if(mant_rep == 0){
            if(sign == 0){
                printf(" inf\n");
            }
            else{
                printf(" -inf\n");
            }
        }
        else{
            printf(" nan\n");
        }
    }
}

void printNormalize(int binary, int exponent, int mantissa, int numBits) {
    int sign = (binary >> (numBits - 1)) & 1;
    int exp_rep = (binary >> (numBits - 1 - exponent)) & ((1 << exponent) - 1);
    int mant_rep = binary & ((1 << (numBits - 1 - exponent)) - 1);
    if(!(exp_rep == 0) && !(exp_rep == (1 << exponent) - 1)){
        for (int j = numBits - 1; j >= 0; j--) {
                printf("%d", (binary >> j) & 1);
        }
        double floating_point_value = pow(-1, sign) * pow(2.0, exp_rep - (1 << (exponent - 1))) * (1.0 + (mant_rep / pow(2.0, numBits - 1 - exponent))) * 2;
        printf(" %.255f\n", floating_point_value);
    }
}

void printDenormalized(int binary, int exponent, int mantissa, int numBits) {
    int sign = (binary >> (numBits - 1)) & 1;
    int exp_rep = (binary >> (numBits - 1 - exponent)) & ((1 << exponent) - 1);
    double mant_rep = binary & ((1 << (numBits - 1 - exponent)) - 1);
    if(exp_rep == 0){
        for (int j = numBits - 1; j >= 0; j--) {
            printf("%d", (binary >> j) & 1);
        }
        //denormalized
        double floating_point_value = pow(-1, sign) * pow(2.0, 1 - (1 << (exponent - 1))) * (mant_rep / pow(2.0, numBits - 1 - exponent)) * 2;
        printf(" %.255f\n", floating_point_value);
    }
}

void printFloatingPoint(int binary, int exponent, int mantissa, int numBits) {
    int sign = (binary >> (numBits - 1)) & 1;
    int exp_rep = (binary >> (numBits - 1 - exponent)) & ((1 << exponent) - 1);
    double mant_rep = binary & ((1 << (numBits - 1 - exponent)) - 1);
    if(exp_rep == 0){
        //denormalized
        double floating_point_value = pow(-1, sign) * pow(2.0, 1 - (1 << (exponent - 1))) * (mant_rep / pow(2.0, numBits - 1 - exponent)) * 2;
        printf(" %.255f\n", floating_point_value);
    }
    else if(exp_rep == (1 << exponent) - 1){
        //infinity or NaN
        if(mant_rep == 0){
            if(sign == 0){
                printf(" inf\n");
            }
            else{
                printf(" -inf\n");
            }
        }
        else{
            printf(" nan\n");
        }
    }
    else{
        //normalized
        double floating_point_value = pow(-1, sign) * pow(2.0, exp_rep - (1 << (exponent - 1))) * (1.0 + (mant_rep / pow(2.0, numBits - 1 - exponent))) * 2;
        printf(" %.255f\n", floating_point_value);
    }
}

void generateBinaryCombinations(int numBits, int exponent, int mantissa, int type) {
    int maxNum = 1 << numBits;
    // 0 all 1 normalized 2 denormalized 3 special
    if(type == 0){
        for (int i = 0; i < maxNum; i++) {
            for (int j = numBits - 1; j >= 0; j--) {
                printf("%d", (i >> j) & 1);
            }
            printFloatingPoint(i, exponent, mantissa, numBits);
        }
    }
    else if(type == 1){
        for (int i = 0; i < maxNum; i++) {
            printNormalize(i, exponent, mantissa, numBits);
        }
    }
    else if(type == 2){
        for (int i = 0; i < maxNum; i++) {
            printDenormalized(i, exponent, mantissa, numBits);
        }
    }
    else if(type == 3){
        for (int i = 0; i < maxNum; i++) {
            printSpecial(i, exponent, mantissa, numBits);
        }
    }
}

int main(int argc, char **argv) {
    int exponent = atoi(argv[1]);
    int mantissa = atoi(argv[2]);
    int numBits = exponent + mantissa + 1;
    if(strcmp(argv[3], "d") == 0){
        generateBinaryCombinations(numBits, exponent, mantissa, 2);
    }
    else if(strcmp(argv[3], "n") == 0){
        generateBinaryCombinations(numBits, exponent, mantissa, 1);
    }
    else if(strcmp(argv[3], "s") == 0){
        generateBinaryCombinations(numBits, exponent, mantissa, 3);
    }
    else if(strcmp(argv[3], "a") == 0){
        generateBinaryCombinations(numBits, exponent, mantissa, 0);
    }
    else{
        printf("Error: Invalid argument\n");
        return 1;
    }
    return 0;
}
