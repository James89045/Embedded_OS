#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define LED "/dev/led_device"
#define seg "/dev/seg_device"
int price_table[3][2] = {{60, 80}, {40, 70}, {120, 50}};
int distance_arr[] = {3, 5, 8};
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

void show_menu() {
    printf("1. shop list\n");
    printf("2. order\n");
}
void show_list() {
    printf("Dessert shop: 3km\n");
    printf("Beverage shop: 5km\n");
    printf("Diner: 8km\n");
}
void show_option() {
    printf("Please choose from 1~3\n");
    printf("1. Dessert shop\n");
    printf("2. Beverage Shop\n");
    printf("3. Diner\n");
}
void show_dessert() {
    printf("Please choose from 1~4\n");
    printf("1. cookie: $60\n");
    printf("2. cake: $80\n");
    printf("3. confirm\n");
    printf("4. cancel\n");
}
void show_beverage() {
    printf("Please choose from 1~4\n");
    printf("1. tea: $40\n");
    printf("2. boba: $70\n");
    printf("3. confirm\n");
    printf("4. cancel\n");
}
void show_diner() {
    printf("Please choose from 1~4\n");
    printf("1. fried rice: $120\n");
    printf("2. egg-drop soup: $50\n");
    printf("3. confirm\n");
    printf("4. cancel\n");
}
void usr_show_menu() {
    int tmp = 0;
    while(1) {
        show_menu();
        scanf("%d", &tmp);
        if (tmp == 1) {
            show_list();
            getchar();  // Wait for Enter key press
            getchar();  // Waut for any key
        }
        if (tmp == 2) {
            break;
        }
        if (tmp != 2 && tmp != 1) { //temp now has to be 1 or 2
            printf("please input valid number\n");
            while (getchar() != '\n') {}
        }
    }
}//邏輯是一直執行迴圈，直到tmp是2
int usr_show_shop_menu(int shopnumber) {
    int tmp = 0;
    if (shopnumber == 0) {//這格判斷式是用來確定有沒有show過餐廳選單了
        while(1) {
        show_option();
        scanf("%d", &tmp);
        if (tmp == 1 || tmp == 2 || tmp == 3) {
            if (tmp == 1) {
                show_dessert();
            }
            if (tmp == 2) {
                show_beverage();
            }
            if (tmp == 3) {
                show_diner();
            }
            return tmp;//tmp是餐廳代號
            break;
        }
        printf("please input valid number\n");
        while (getchar() != '\n') {} // handle character issue
        }
    }
    if (shopnumber == 1) {
        show_dessert();
    }
    if (shopnumber == 2) {
        show_beverage();
    }
    if (shopnumber == 3) {
        show_diner();
    }
    return shopnumber;
}
int *sumup() {
    int opt = 0;
    int total = 0;
    int shop_number = 0;
    // static int return_arr[2];
    while(1) {
        int servings = 0;
        shop_number = usr_show_shop_menu(shop_number);
        scanf("%d", &opt);
        if (opt == 1 || opt == 2) {
           printf("How many?\n");
           scanf("%d", &servings);
           total += servings * price_table[shop_number-1][opt-1];
        }
        if (opt == 3 && total == 0) {
            continue;
        }
        if (opt == 3 || opt == 4) {
            break;
        }
    }
    if (opt == 3) {
        int *return_arr = (int *)malloc(2 * sizeof(int));
        return_arr[0] = total;
        return_arr[1] = shop_number;
        return return_arr;
    }
    if (opt == 4) {
        usr_show_menu();
        sumup();
    }
    
}
void show_LED(int dis) {
    int num = dis;
    int led_fd = open(LED, O_RDWR);
    while(num >= 0) {
        write(led_fd, &num, sizeof(int));
        sleep(1);
        --num;
    }
}
void show_seg(int num) {
    int tmp = num;
    int seg_fd = open(seg, O_RDWR);
    char buf[20];
    int *todriver;
    int all_zero[7] = {0,0,0,0,0,0,0};
    sprintf(buf, "%d", num);
    for (int i=0; buf[i] != '\0'; ++i) {
        todriver = num_arr[buf[i] - '0'];
        // printf("%d\n", buf[i] - '0');
        write(seg_fd, todriver, 7 * sizeof(int)); 
        usleep(500000);
    }
    write(seg_fd, all_zero, 7 * sizeof(int)); 
}
int main(int argc, char *argv[]) {
    while(1) {
        int tmp=1;
        int total = 0;
        // show menu
        usr_show_menu();
        //放行，到下一關，可以選餐廳
        int *result = sumup();
        if (result) {
            printf("sum: %d, restaurant: %d\n", result[0], result[1]);
        }
        printf("Please wait for a few minutes...\n");
        show_seg(result[0]);
        show_LED(distance_arr[result[1] - 1]);
        free(result);
        printf("Please pick up your meal\n");
        getchar();
        getchar();
    }  
}