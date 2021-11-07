//
// Created by Christophe ISHIMWE NGABO on 11/02/2019.
//

#include "domingo-ferrer.h"

int main() {

    /**
     * Define crypto-system parameters
     */
    int d = 3;
    int p = 11;
    int q = 13;
    int rp = 4;
    int rq = 2;
    Config config = {d, p, q, rp, rq};
    /**
    * Print crypto-system parameters
    */
    printf("\n\n\nDomingo-Ferrer Crypto-System Parameters\n");
    printf("---------------------------------------\n");
    printf("d:%d\tp:%d\tq:%d\tn:%d\trp:%d\trq:%d", config.d, config.p, config.q, config.p * config.q, config.rp, config.rq);

    /**
     * Messages to encrypt
     */
    int m1 = 34;
    printf("\n\n-------------- Message 1 --------------\n");
    printf("m1: %d", m1);

    int m2 = 37;
    printf("\n\n-------------- Message 2 --------------\n");
    printf("m2: %d", m2);

    /**
     * Encrypt m1 and print its cipher text
     */
    printf("\n\n------- Encryption of message m1 = %d -------\n", m1);
    int counter;
    int **cipher1;
    cipher1 = encrypt(config, m1);
    printf("c1: (");
    for (counter = 0; counter < config.d ; counter++) {
        printf("[%d,%d]", cipher1[counter][0], cipher1[counter][1]);
        if (counter < config.d - 1) {
            printf(",");
        }
    }
    printf(")");

    /**
     * Encrypt m2 and print its cipher text
     */
    printf("\n\n------- Encryption of message m2 = %d -------\n", m2);
    int **cipher2;
    cipher2 = encrypt(config, m2);
    printf("c2: (");
    for (counter = 0; counter < config.d ; counter++) {
        printf("[%d,%d]", cipher2[counter][0], cipher2[counter][1]);
        if (counter < config.d - 1) {
            printf(",");
        }
    }
    printf(")");

    /**
     * Add two cipher texts
     */
    printf("\n\n------- Addition of cipher texts c = c1 + c2 -------\n");
    int **cipher;
    cipher = cipherSum(config, cipher1, cipher2);
    printf("c = c1 + c2: (");
    for (counter = 0; counter < config.d ; counter++) {
        printf("[%d,%d]", cipher[counter][0], cipher[counter][1]);
        if (counter < config.d - 1) {
            printf(",");
        }
    }
    printf(")");

    /**
     * Decryption of the summed cipher texts
     */
    printf("\n\n------- Decryption of the cipher c -------\n");
    int sum = decrypt(config, cipher);

    /**
     * Print the result of the addition
     */
    printf("m = m1 + m2: %d\n\n", sum);

    return 0;
}
