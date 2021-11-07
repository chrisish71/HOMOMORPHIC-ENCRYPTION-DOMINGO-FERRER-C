//
// Created by Christophe ISHIMWE NGABO on 11/02/2019.
//

#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "configuration.h"

/**
 * Floor Modulus Function
 * @param k: dividend number
 * @param m: divisor number
 * @return: floor modulus
 */
int floorModulus(int k, int m) {
    return (k % m + m) % m;
}

/**
 * Reverse Floor Modulus Function
 * @param k: dividend number
 * @param m: divisor number
 * @return: reverse floor modulus
 */
int reverseFloorModulus(int k, int m) {
    int counter;
    for (counter = 0; counter < m; counter++) {
        if (floorModulus(counter * k, m) == 1) {
            return counter;
        }
    }
    return 1;
}

/**
 * Randomly Message Splitter Function
 * @param config: a structure representing crypto-system parameters
 * @param message: message which is going to be fragmented so that m = (m1 + m2 + … + md) mod n
 * @return: array of fragments [m1,m2,…,md]
 */
int * split(Config config, int message) {
    int counter;
    int sum = 0;
    int *fragment;
    fragment = malloc(sizeof(int*) * config.d);
    srand((unsigned) time(NULL));
    for (counter = 0; counter < config.d - 1; counter++) {
        int random = rand();
        int randomM = floorModulus(random, (config.p * config.q));
        *(fragment + counter) = randomM;
        sum += randomM;
    }
    for (counter = 0; counter < (config.p * config.q); counter++) {
        if (floorModulus(sum + counter, (config.p * config.q)) == message) {
            *(fragment + config.d - 1) = counter;
            break;
        }
    }
    return fragment;
}

/**
 * Domingo-Ferrer Encryption Function
 * @param config: a structure representing crypto-system parameters
 * @param message: message to encrypt
 * @return: matrix of ciphertext
 */
int ** encrypt(Config config, int message) {
    /**
     * Split randomly the message so that the message m = (m1 + m2 + … + md) mod n
     */
    int *fragment;
    fragment = split(config, message);

    /**
     * Compute the ciphertext
     */
    int counter;
    int **cipher;
    cipher = malloc(sizeof(int*) * config.d);
    for (counter = 0; counter < config.d; counter++) {
        cipher[counter] = malloc(sizeof(int*) * 2);
    }
    for (counter = 0; counter < config.d; counter++) {
        cipher[counter][0] = floorModulus(fragment[counter] * (int) pow(config.rp, counter + 1), config.p);
        cipher[counter][1] = floorModulus(fragment[counter] * (int) pow(config.rq, counter + 1), config.q);
    }

    /**
     * Print the cipher text
     */
    printf("Fragments: (");
    for (counter = 0; counter < config.d ; counter++) {
        printf("%d", fragment[counter]);
        if (counter < config.d - 1) {
            printf(",");
        }
    }
    printf(")\n");
    return cipher;
}

/**
 * Cipher Text Adder Function
 * @param config: a structure representing crypto-system parameters
 * @param cipher1
 * @param cipher2
 * @return:
 */
int ** cipherSum(Config config, int **cipher1, int **cipher2) {
    int counter;
    int **cipher;
    cipher = malloc(sizeof(int*) * config.d);
    for (counter = 0; counter < config.d; counter++) {
        cipher[counter] = malloc(sizeof(int*) * 2);
    }
    for (counter = 0; counter < config.d; counter++) {
        cipher[counter][0] = floorModulus(cipher1[counter][0] + cipher2[counter][0], config.p);
        cipher[counter][1] = floorModulus(cipher1[counter][1] + cipher2[counter][1], config.q);
    }
    return cipher;
}

/**
 * Domingo-Ferrer Decryption Function
 * @param config: a structure representing crypto-system parameters
 * @param cipher: cipher text to decrypt
 * @return: plain text message
 */
int decrypt(Config config, int **cipher) {
    /**
     * Compute rrp and rrq respectively reverse of rp and rq
     */
    int rrp = reverseFloorModulus(config.rp, config.p);
    int rrq = reverseFloorModulus(config.rq, config.q);

    /**
     * Perform the scalar product and add them to obtain  [m mod p, m mod q]
     */
    int counter;
    int scalarSum[2] = {0,0};
    for (counter = 0; counter < config.d; counter++) {
        scalarSum[0] += floorModulus((int) (cipher[counter][0] * pow(rrp, counter + 1)), config.p);
        scalarSum[1] += floorModulus((int) (cipher[counter][1] * pow(rrq, counter + 1)), config.q);
    }

    /**
     * Apply chinese remainder theorem to retrieve the message m = (m1 + m2) mod n
     */
    int message1 = floorModulus(scalarSum[0], config.p) * config.q * floorModulus(reverseFloorModulus(config.q, config.p), config.p);
    int message2 = floorModulus(scalarSum[1], config.q) * config.p * floorModulus(reverseFloorModulus(config.p, config.q), config.q);
    int message = floorModulus(message1 + message2, (config.p * config.q));

    /**
     * Print the sum of the scalar product
     */
    printf("Scalar Product: (");
    for (counter = 0; counter < 2; counter++) {
        printf("%d", scalarSum[counter]);
        if (counter < 1) {
            printf(",");
        }
    }
    printf(")\n");

    return message;
}