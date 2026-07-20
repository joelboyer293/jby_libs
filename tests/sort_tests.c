#define SORT_IMPLEMENTATION
#include "sort.h"

#include <assert.h>
#include <stdio.h>

#define GREEN   "\033[32m"
#define RESET   "\033[0m"
#define PRINT_SUCCESS printf(GREEN "Test '%s' sucessful\n" RESET, __func__);

typedef struct {
    float x;
    char* str;
} Struct;

char compare_struct(const void* struct1, const void* struct2) {
    Struct* s1 = (Struct*)struct1;
    Struct* s2 = (Struct*)struct2;

    char c1 = s1->str[0];
    char c2 = s2->str[0];

    if (c1 > c2) { return -1; }
    if (c2 > c1) { return  1;}

    // Tie breaker
    if (s1->x > s2->x) { return -1; } 
    if (s2->x > s1->x) { return 1; }
    return 0; 
}

char compare_numbers(const void* n1, const void* n2) {
    if ( *((int*)n1) > *((int*)n2) ) { return -1; } else { return 1; }
}

void test_bubble_sort_single_pass_primitives() {
    int buf[5] = { 10, 15, 5, 10, 2 };
    bubble_sort_single_pass(buf, 5, sizeof(int), compare_numbers);

    assert(buf[0] == 10);
    assert(buf[1] == 5);
    assert(buf[2] == 10);
    assert(buf[3] == 2);
    assert(buf[4] == 15);

    PRINT_SUCCESS
}

void test_bubble_sort_primitives() {
   int buf[10] = {9,8,7,6,5,4,10,122,3,2};
   bubble_sort(buf, 10, sizeof(int), compare_numbers);

   int expected[10] = {2,3,4,5,6,7,8,9,10,122};
   assert( !memcmp(buf, expected, 10*sizeof(int)) );
   PRINT_SUCCESS
}

void test_bubble_sort_single_pass_struct() {
    Struct structs[10] = {
        (Struct){3, "Beta"},
        (Struct){1, "Beta"},
        (Struct){2, "Beta"},
        (Struct){1, "Alpha"},
        (Struct){150, "Delta"},
        (Struct){1, "Omega"},
        (Struct){3, "Alpha"},
        (Struct){100, "Hello"},
        (Struct){12, "Houston"},
        (Struct){11, "Help"}
    };

    bubble_sort_single_pass(structs, 10, sizeof(Struct), compare_struct);

    Struct expected[10] = { 
        (Struct){1, "Beta"},
        (Struct){2, "Beta"},
        (Struct){1, "Alpha"},
        (Struct){3, "Beta"},
        (Struct){150, "Delta"},
        (Struct){3, "Alpha"},
        (Struct){100, "Hello"},
        (Struct){12, "Houston"},
        (Struct){11, "Help"},
        (Struct){1, "Omega"}
    };
    
    for (int i = 0; i < 10; i++) {
        assert(structs[i].x == expected[i].x);
        assert(strcmp(structs[i].str, expected[i].str) == 0);
    }
    PRINT_SUCCESS
}


void test_bubble_sort_struct() {
    Struct structs[10] = {
        (Struct){3, "Beta"},
        (Struct){1, "Beta"},
        (Struct){2, "Beta"},
        (Struct){1, "Alpha"},
        (Struct){150, "Delta"},
        (Struct){1, "Omega"},
        (Struct){3, "Alpha"},
        (Struct){100, "Hello"},
        (Struct){12, "Houston"},
        (Struct){11, "Help"}
    };

    bubble_sort(structs, 10, sizeof(Struct), compare_struct);

    Struct expected[10] = { 
        (Struct){1, "Alpha"},
        (Struct){3, "Alpha"},
        (Struct){1, "Beta"},
        (Struct){2, "Beta"},
        (Struct){3, "Beta"},
        (Struct){150, "Delta"},
        (Struct){11, "Help"},
        (Struct){12, "Houston"},
        (Struct){100, "Hello"},
        (Struct){1, "Omega"}
    };

    for (int i = 0; i < 10; i++) {
        assert(structs[i].x == expected[i].x);
        assert(strcmp(structs[i].str, expected[i].str) == 0);
    }
    PRINT_SUCCESS
}



int main() {
    test_bubble_sort_single_pass_primitives(); 
    test_bubble_sort_primitives();
    test_bubble_sort_single_pass_struct();
    test_bubble_sort_struct();
}
