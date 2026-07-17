#ifndef TREES_H
#define TREES_H

#include <stdint.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

// N-ary tree with fixed branching factor
typedef struct {
    size_t count;
    size_t elem_size;
    uint8_t branching;
    void* pool;
} NTree;

void ntree_init(NTree* tree, size_t elem_size, size_t count, uint8_t branching);
void ntree_fill(NTree* tree, void* data);
size_t ntree_get_first_child(NTree* tree, size_t parent);
size_t ntree_get_parent(NTree* tree, size_t child);
void ntree_free(NTree* tree);

// Generic tree structure
typedef struct {
    size_t count;
    size_t elem_size;
    size_t* parents;
    void* pool;
} Tree;

void tree_clear(Tree* tree);
void tree_init(Tree* tree, size_t elem_size, size_t count);
void tree_fill(Tree* tree, void* data);
size_t tree_get_parent(Tree* tree, size_t child); 
void tree_get_children(Tree* tree, size_t parent, size_t* children_indices, size_t buf_size);
bool tree_is_leaf(Tree* tree, size_t node);
size_t tree_find_free_node(Tree* tree);
size_t tree_add_child(Tree* tree, size_t parent);
void tree_remove_leaf_node(Tree* tree, size_t node);
void tree_remove_branch(Tree* tree, size_t node);
void tree_free(Tree* tree);

#endif 


#ifdef TREES_IMPLEMENTATION

void ntree_init(NTree* tree, size_t elem_size, size_t count, uint8_t branching) {
    assert(tree != NULL);
    assert(branching > 0);
    tree->pool = (void*)malloc(elem_size * count); 
    tree->elem_size = elem_size;
    tree->branching = branching;
    tree->count = count;
}

// Fill the nodes with given source data
void ntree_fill(NTree* tree, void* data) {
    assert(tree != NULL);
    assert(data != NULL);
    uint8_t* start = (uint8_t*)(tree->pool);
    uint8_t* end   = start + (tree->count * tree->elem_size);
    for (uint8_t* dest = start; dest < end; dest += tree->elem_size) {
        memcpy((void*)dest, data, tree->elem_size);
    }
}

// Get the index of the first child. Return parent if children are out of bound
size_t ntree_get_first_child(NTree* tree, size_t parent) {
    assert(tree != NULL);

    size_t first = (tree->branching * parent) + 1;
    size_t last = (tree->branching * parent) + tree->branching;
    
    if (first > tree->count || last > tree->count) {
        return parent;
    } else {
        return first;
    }
}

// Get the parent of the child. Return the child if it is the root node
size_t ntree_get_parent(NTree* tree, size_t child) {
    assert(tree != NULL);
    if (child == 0) { 
        return child; 
    }
    return (child - 1) / tree->branching; 
}

void ntree_free(NTree* tree) {
    assert(tree != NULL);
    free(tree->pool);
}


// The root has itself as parent. The rest are given invalid index
void tree_clear(Tree* tree) { 
    tree->parents[0] = 0;
    for (size_t i = 1; i < tree->count; i++) {
        tree->parents[i] = UINT16_MAX;
    }
}

void tree_init(Tree* tree, size_t elem_size, size_t count) {
    assert(tree != NULL);
    tree->pool    = (void*)  malloc(elem_size * count);
    tree->parents = (size_t*)malloc(sizeof(size_t) * count);
    tree->elem_size = elem_size;
    tree->count = count;

    tree_clear(tree);
}

// Fill the nodes with given source data
void tree_fill(Tree* tree, void* data) {
    assert(tree != NULL);
    assert(data != NULL);
    uint8_t* start = (uint8_t*)(tree->pool);
    uint8_t* end   = start + (tree->count * tree->elem_size);
    for (uint8_t* dest = start; dest < end; dest += tree->elem_size) {
        memcpy((void*)dest, data, tree->elem_size);
    }
}

size_t tree_get_parent(Tree* tree, size_t child) {
    assert(tree != NULL);
    return tree->parents[child];
}

// Fill a buffer with all the children indices of a given node
void tree_get_children(Tree* tree, size_t parent, size_t* children_indices, size_t buf_size) {
    assert(tree != NULL);
    assert(children_indices != NULL);
    assert(buf_size > 0);

    size_t j = 0;
    for (size_t i = 1; i < tree->count; i++) { // Skip the root index 0
        if (tree->parents[i] == parent) {
            children_indices[j++] = i;
        }
    }
    for ( ; j < buf_size; j++) {
        children_indices[j] = UINT16_MAX; 
    }
}

bool tree_is_leaf(Tree* tree, size_t node) {
    for (size_t i = 0; i < tree->count; i++) {
        if (tree->parents[i] == node) {
            return false;
        }
    }
    return true;
}

size_t tree_find_free_node(Tree* tree) {
    for (size_t i = 0; i < tree->count; i++) {
        if (tree->parents[i] == UINT16_MAX) {
            return i;
        }
    }
    return UINT16_MAX;
}

// Add and return a child to a node
size_t tree_add_child(Tree* tree, size_t parent) {
    assert(tree != NULL);

    size_t free_node = tree_find_free_node(tree);
    assert(free_node != UINT16_MAX);
    tree->parents[free_node] = parent;
    return free_node;
}

void tree_remove_leaf_node(Tree* tree, size_t node) {
    assert(tree != NULL);
    if (!tree_is_leaf(tree, node)) { return; }
    tree->parents[node] = UINT16_MAX;
}

void tree_remove_branch(Tree* tree, size_t node) {
    assert(tree != NULL);

    size_t to_remove[256];
    size_t temp_children[64];
    memset(to_remove, UINT16_MAX, 256 * sizeof(size_t));
    memset(temp_children, UINT16_MAX, 64  * sizeof(size_t));
    to_remove[0] = node;
    
    size_t read_idx  = 0; 
    size_t write_idx = 1; 

    // If the read catches up to the write, then we stopped writing and there are no more children
    while (write_idx != read_idx) {

        // Get children at current read index
        tree_get_children(tree, to_remove[read_idx++], temp_children, 64);

        // Append the children to the buffer
        size_t child_idx = 0;
        while (temp_children[child_idx] != UINT16_MAX) {
            to_remove[write_idx++] = temp_children[child_idx++];
        }
    }

    // Remove all indices found to delete the branch
    for (size_t i = 0; i < write_idx; i++) {
        if (to_remove[i] == UINT16_MAX) { break; }
        tree->parents[to_remove[i]] = UINT16_MAX;
    }

}

void tree_free(Tree* tree) {
    free(tree->parents);
    free(tree->pool);
}

#endif
