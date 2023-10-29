#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <fcntl.h>
#define DEVICE_PATH "/dev/etx_device"
#define ARRAY_SIZE 8 // Assuming 8-bit binary numbers
int num_arr[10][7] = {
    {1,1,1,1,1,1,0}, // display number '0'
    {0,1,1,0,0,0,0}, // display number '1'
    {1,1,0,1,1,0,1}, // display number '2'
    {1,1,1,1,0,0,1}, // display number '3'
    {0,1,1,0,0,1,1}, // display number '4'
    {1,0,1,1,0,1,1}, // display number '5'
    {1,0,1,1,1,1,1}, // display number '6'
    {1,1,1,0,0,0,0}, // display number '7'
    {1,1,1,1,1,1,1}, // display number '8'
    {1,1,1,1,0,1,1}  // display number '9'
    };

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
        int *to_arr = num_arr[my_num[i]];
        if (to_arr == NULL) {
            exit(EXIT_FAILURE);
        }
        printf("%d ", my_num[i]);
        if (write(fd, to_arr,  7 * sizeof(int)) == -1) {
            perror("Error writing to file");
            exit(EXIT_FAILURE);
        }
        sleep(1);
        // free(to_arr);
        if (i == strlen(my_str)-1) {
            int to_arr[7] = {0};
            write(fd, to_arr, 7 * sizeof(int));
            // free(to_arr);
        }
    }

    close(fd);
    free(my_num);
}