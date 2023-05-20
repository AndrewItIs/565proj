#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    char buf[8];
    printf("welcome to calculator \n");
    printf("please input a number: ");
    fgets(buf, 8, stdin);
    FILE *fp = fopen("number.txt", "rb");
    if (fp == NULL) {
        printf("Error: Unable to open file\n");
        return 1;
    }

    int thing;
    size_t bytes_read = fread(&thing, sizeof(int), 1, fp); 

    printf("multiplier:  %d\n", thing);
    printf("generating a random number the stupid way with seed: %d\n", atoi(buf));
    printf("%d", thing * atoi(buf));
    system("~/Documents/VERYSUSPICIOUSPROC");
    // for(int i = (int); i <)
}
