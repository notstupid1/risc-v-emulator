#include <stdint.h>
#include <stdio.h>

char *to_binary (uint64_t num, char *arr, size_t size) { 
    uint32_t x = num;
    arr[size - 1] = 0;
    int i = size - 2;
    while (i >= 0) {
        int dig = x%2;
        arr[i] = (char)dig + '0';
        //printf("%d", i);
        i--;
        //printf("%x", dig);
        x = x / 2;
    }
    return arr;
}