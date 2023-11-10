#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <fcntl.h>
#define DEVICE_PATH "/dev/seven_seg"

char *seg_for_c[27] = {
    "1111001100010001", // A(0)
    "0000011100000101", // b(1)
    "1100111100000000", // C(2)
    "0000011001000101", // d(3)
    "1000011100000001", // E(4)
    "1000001100000001", // F
    "1001111100010000", // G
    "0011001100010001", // H
    "1100110001000100", // I
    "1100010001000100", // J
    "0000000001101100", // K
    "0000111100000000", // L
    "0011001110100000", // M
    "0011001110001000", // N
    "1111111100000000", // O
    "1000001101000001", // P
    "0111000001010000", //q
    "1110001100011001", //R
    "1101110100010001", //S
    "1100000001000100", //T
    "0011111100000000", //U
    "0000001100100010", //V
    "0011001100001010", //W
    "0000000010101010", //X
    "0000000010100100", //Y
    "1100110000100010", //Z(25)
    "0000000000000000"  //no light(26)
};


int main(int argc, char *argv[]) {
    char *fromusr = argv[1];
    int fd = open("/dev/seven_seg", O_RDWR);
    printf("%d\n", fd);
    int numOfChar = strlen(fromusr);
    char **gotodriver;

    gotodriver = (char **)malloc(numOfChar * sizeof(char *));
    if (gotodriver == NULL) {
        printf("Memory allocation failed for rows.\n");
        return -1;
    }
    // 分配每行的内存
    for (int i = 0; i < numOfChar; i++) {
        gotodriver[i] = (char *)malloc(16 * sizeof(char));//16 because there are 16 characters, we dont need to send \0
        if (gotodriver[i] == NULL) {
            printf("Memory allocation failed for columns.\n");
            return -1;
        }
    }

    for (int i=0; i<numOfChar; ++i) {
        if (fromusr[i]<=90 && fromusr[i]>=65) {
            for (int j=0; j<16; ++j) {
                gotodriver[i][j] = seg_for_c[fromusr[i]-65][j];
            }
        }
        else if (fromusr[i]<=122 && fromusr[i]>=97) {
            for (int z=0; z<16; ++z) {
                gotodriver[i][z] = seg_for_c[fromusr[i]-97][z];
            }
        }
        for (int k=0; k<16; ++k) {
            printf("%c", gotodriver[i][k]);
        }
        printf("\n");
    }
    char *buffer = (char *)malloc(numOfChar * 16 * sizeof(char));
    if (buffer == NULL) {
        printf("Memory allocation failed for buffer.\n");
        return -1;
    }

    for (int i=0; i<numOfChar; i++) {
        for (int j=0; j<16; j++) {
            buffer[i*16 + j] = gotodriver[i][j];
        }
    }

    if (write(fd, buffer, numOfChar * 16 * sizeof(char)) == -1) {
        perror("Error writing to file");
        exit(EXIT_FAILURE);
    }
    
    for (int i=0; i < numOfChar; i++) {
        free(gotodriver[i]);
    }
    free(buffer);
    free(gotodriver);
    close(fd);
}