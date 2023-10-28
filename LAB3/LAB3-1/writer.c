#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <fcntl.h>
#define DEVICE_PATH "/dev/etx_device"
#define ARRAY_SIZE 8 // Assuming 8-bit binary numbers

int *decimalToBinary(int decimal) {
    int *binary_arr = malloc(ARRAY_SIZE * sizeof(int));
    if (binary_arr == NULL) {
        printf("Memory allocation failed\n");
        return NULL;
    }

    for (int i = 0; i < ARRAY_SIZE; i++) {
        binary_arr[i] = decimal % 2;
        decimal = decimal / 2;
    }

    return binary_arr;
}

int *stringToArray(char *str) {
    int str_long = strlen(str);
    int *my_number = malloc(str_long * sizeof(int));
    
    for (int i = 0; i < str_long; i++) {
        my_number[i] = str[i] - '0';
    }

    return my_number;
}

int main() {
    char *my_str = "311605011";
    int *my_num = stringToArray(my_str);
    int fd = open(DEVICE_PATH, O_RDWR);
    for (int i = 0; i < strlen(my_str); i++) {
        int *to_decimal = decimalToBinary(my_num[i]);
        if (to_decimal == NULL) {
            exit(EXIT_FAILURE);
        }
        printf("%d ", my_num[i]);
        if (write(fd, to_decimal, ARRAY_SIZE * sizeof(int)) == -1) {
            perror("Error writing to file");
            exit(EXIT_FAILURE);
        }
        sleep(1);
        free(to_decimal);
        if (i == strlen(my_str)-1) {
            to_decimal = decimalToBinary(0);
            write(fd, to_decimal, ARRAY_SIZE * sizeof(int));
            free(to_decimal);
        }
    }

    close(fd);
    free(my_num);
}
