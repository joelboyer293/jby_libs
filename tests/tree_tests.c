#define TREES_IMPLEMENTATION
#include "trees.h"
#include "stdio.h"


#define GREEN   "\033[32m"
#define RESET   "\033[0m"
#define PRINT_SUCCESS printf(GREEN "Test '%s' sucessful\n" RESET, __func__);

typedef struct {
    int x;
    int y;
} Struct;

void test_fill_primitives() {
    Tree tree;
    int16_t data = 125;
    tree_init(&tree, sizeof(int16_t), 100); 
    tree_fill(&tree, &data);

    int16_t* pool = (int16_t*)tree.pool;
    for (size_t i = 0; i < 100; i++) {
        assert( pool[i] == 125 ); 
    }

    PRINT_SUCCESS
}

void test_fill_struct() {
    Tree tree;
    Struct s = {123, 321};
    tree_init(&tree, sizeof(Struct), 100); 
    tree_fill(&tree, &s);

    Struct* pool = (Struct*)tree.pool;
    for (size_t i = 0; i < 100; i++) {
        assert( pool[i].x == 123 ); 
        assert( pool[i].y == 321 ); 
    }
    PRINT_SUCCESS
}

void test_get_root_parent() {
    Tree tree;
    tree_init(&tree, sizeof(Struct), 100); 
   
    size_t parent = tree_get_parent(&tree, 0);
    assert( parent == 0 );
    assert(tree.parents[1] == UINT16_MAX);
    assert(tree.parents[99] == UINT16_MAX);

    PRINT_SUCCESS
}

void test_add_child() {
    Tree tree;
    tree_init(&tree, sizeof(Struct), 100); 
  
    size_t c1 = tree_add_child(&tree, 0);
    tree_add_child(&tree, 0);
    tree_add_child(&tree, c1);

    assert(tree.parents[0] == 0);
    assert(tree.parents[1] == 0);
    assert(tree.parents[2] == 0);
    assert(tree.parents[3] == 1);
    assert(tree.parents[4] == UINT16_MAX);
    assert(tree.parents[99] == UINT16_MAX);

    PRINT_SUCCESS
}

void test_get_parent() {
    Tree tree;
    tree_init(&tree, sizeof(Struct), 50); 
   
    size_t c1 = tree_add_child(&tree, 0);
    size_t c2 = tree_add_child(&tree, 0);
    size_t gc = tree_add_child(&tree, c1);
    
    assert(tree_get_parent(&tree,0) == 0);
    assert(tree_get_parent(&tree,c1) == 0);
    assert(tree_get_parent(&tree,c2) == 0);
    assert(tree_get_parent(&tree,gc) == 1);
    assert(tree_get_parent(&tree,gc+1) == UINT16_MAX);
    assert(tree_get_parent(&tree,49) == UINT16_MAX);

    PRINT_SUCCESS
}

void test_get_children() {
    Tree tree;
    tree_init(&tree, sizeof(Struct), 50); 
   
    size_t c1 = tree_add_child(&tree, 0);
    size_t c2 = tree_add_child(&tree, 0);
    size_t c3 = tree_add_child(&tree, 0);
    size_t c4 = tree_add_child(&tree, 0);
    size_t gc1 = tree_add_child(&tree, c1);
    size_t gc2 = tree_add_child(&tree, c1);

    size_t children[50];
    tree_get_children(&tree, 0, children, 50);
    assert(children[0] == c1);
    assert(children[1] == c2);
    assert(children[2] == c3);
    assert(children[3] == c4);
    assert(children[4] == UINT16_MAX);
    assert(children[49] == UINT16_MAX);

    tree_get_children(&tree, c1, children, 50);
    assert(children[0] == gc1);
    assert(children[1] == gc2);
    assert(children[2] == UINT16_MAX);
    assert(children[49] == UINT16_MAX);

    PRINT_SUCCESS
}

void test_remove_leaf_node() {
    Tree tree;
    tree_init(&tree, sizeof(Struct), 50); 
   
    size_t c1 = tree_add_child(&tree, 0);
    size_t c2 = tree_add_child(&tree, 0);
    size_t gc = tree_add_child(&tree, c1);
 
    // Not removed because isn't a leaf
    tree_remove_leaf_node(&tree,c1);
    assert(tree.parents[c1] == 0);

    tree_remove_leaf_node(&tree,c2);
    tree_remove_leaf_node(&tree,gc);
    tree_remove_leaf_node(&tree,c1);
    assert(tree.parents[c2] == UINT16_MAX);
    assert(tree.parents[gc] == UINT16_MAX);
    assert(tree.parents[c1] == UINT16_MAX);

    PRINT_SUCCESS
}

void test_remove_branch() {
    Tree tree;
    tree_init(&tree, sizeof(Struct), 50); 
   
    size_t c1 = tree_add_child(&tree, 0);
    size_t c2 = tree_add_child(&tree, 0);
    size_t c3 = tree_add_child(&tree, 0);
    size_t c4 = tree_add_child(&tree, 0);
    size_t gc1 = tree_add_child(&tree, c1);
    size_t gc2 = tree_add_child(&tree, c1);
    assert(tree.parents[c2] == 0);

    tree_remove_branch(&tree, c1);
    assert(tree.parents[c1] == UINT16_MAX);
    assert(tree.parents[gc1] == UINT16_MAX);
    assert(tree.parents[gc2] == UINT16_MAX);
    assert(tree.parents[c2] == 0);
    assert(tree.parents[c3] == 0);
    assert(tree.parents[c4] == 0);

    PRINT_SUCCESS
}

int main() {
    test_fill_primitives();
    test_fill_struct();
    test_get_root_parent();
    test_add_child();
    test_get_parent();
    test_get_children();
    test_remove_leaf_node();
    test_remove_branch();
}
