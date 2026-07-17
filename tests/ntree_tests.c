
#define TREES_IMPLEMENTATION
#include "trees.h"
#include <stdio.h>

#define GREEN   "\033[32m"
#define RESET   "\033[0m"
#define PRINT_SUCCESS printf(GREEN "Test '%s' sucessful\n" RESET, __func__);

typedef struct {
    int x;
    int y;
} Struct;

void test_fill_primitive() {
    NTree tree;
    int data = 1235;
    ntree_init(&tree, sizeof(int), 13, 3);
    ntree_fill(&tree, &data);

    int* pool = (int*)tree.pool;
    for (int i = i; i < 13; i++) {
        assert( pool[i] == 1235 );
    }
    ntree_free(&tree);
    PRINT_SUCCESS
}

void test_fill_struct() {
    NTree tree;
    Struct s = {4550, 12321};
    ntree_init(&tree, sizeof(s), 21, 4);
    ntree_fill(&tree, &s); 

    Struct* pool = (Struct*)tree.pool;
    for (int i = i; i < 21; i++) {
        assert( pool[i].x == 4550  );
        assert( pool[i].y == 12321 );
    }
    ntree_free(&tree);
    PRINT_SUCCESS
}

void test_get_parent() {
    NTree tree;
    Struct s = {0, 1};
    ntree_init(&tree, sizeof(s), 15, 2);
    ntree_fill(&tree, &s); 

    size_t parent = ntree_get_parent(&tree, 10);
    assert(parent == 4);
    ntree_free(&tree);
    PRINT_SUCCESS
}

void test_get_root_parent() {
    NTree tree;
    Struct s = {45, 40};
    ntree_init(&tree, sizeof(s), 21, 4);
    ntree_fill(&tree, &s); 

    size_t parent = ntree_get_parent(&tree, 0);
    assert(parent == 0);
    ntree_free(&tree);
    PRINT_SUCCESS
}

void test_get_first_child() {
    NTree tree;
    int data = 420;
    ntree_init(&tree, sizeof(int), 40, 3); 
    ntree_fill(&tree, &data);

    size_t child = ntree_get_first_child(&tree, 2);
    assert(child == 7);
    ntree_free(&tree);
    PRINT_SUCCESS
}

void test_get_leaf_first_child() {
    NTree tree;
    Struct s = {666, 67};
    ntree_init(&tree, sizeof(s), 364, 3);
    ntree_fill(&tree, &s); 

    size_t child = ntree_get_first_child(&tree, 300);
    assert(child == 300);
    ntree_free(&tree);
    PRINT_SUCCESS
}

int main() {
    test_fill_primitive(); 
    test_fill_struct();
    test_get_parent();
    test_get_root_parent();
    test_get_first_child();
    test_get_leaf_first_child();
}
