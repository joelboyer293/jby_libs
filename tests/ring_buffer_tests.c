#define RING_BUFFER_IMPLEMENTATION
#include "ring_buffer.h"

#include <stdio.h>

#define GREEN   "\033[32m"
#define RESET   "\033[0m"
#define PRINT_SUCCESS printf(GREEN "Test '%s' sucessful\n" RESET, __func__);

typedef struct {
    int x;
    int y;
} Struct;

void test_rb_fill_primitives() {
    RingBuffer rb;
    int data = 155;
    ring_buffer_init(&rb, sizeof(int), 100);
    ring_buffer_fill(&rb, &data);
    
    assert(rb.pos == 99);

    int* buffer = (int*)rb.buffer;
    for (size_t i = 0; i < 100; i++) {
        assert(buffer[i] == 155);
    }
    
    ring_buffer_free(&rb);
    PRINT_SUCCESS
}

void test_rb_fill_struct() {
    RingBuffer rb;
    Struct s = {120, 360};
    ring_buffer_init(&rb, sizeof(Struct), 250);
    ring_buffer_fill(&rb, &s);

    assert(rb.pos == 249);

    Struct* buffer = (Struct*)rb.buffer;
    for (size_t i = 0; i < 250; i++) {
        assert(buffer[i].x == 120);
        assert(buffer[i].y == 360);
    }

    ring_buffer_free(&rb);
    PRINT_SUCCESS
}

void test_rb_moving_average() {
    RingBuffer rb;
    Struct s = {0,0};
    ring_buffer_init(&rb, sizeof(Struct), 10);
    ring_buffer_fill(&rb, &s); 

    int X[15] = {5,5,5,5,5,5,5,5,5,5,10,15,20,25,30};
    int Y[15] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

    float expect_x[15] = {0.5,1,1.5,2,2.5,3,3.5,4,4.5,5,5.5,6.5,8,10,12.5};
    float expect_y[15] = {0.1,0.3,0.6,1,1.5,2.1,2.8,3.6,4.5,5.5,6.5,7.5,8.5,9.5,10.5};
    
    Struct* buffer = (Struct*)rb.buffer;
    for (size_t i = 0; i < 15; i++) {

        // Write data to the ring buffer
        size_t write_idx = ring_buffer_get_next(&rb); 
        buffer[write_idx].x = X[i];
        buffer[write_idx].y = Y[i];

        // Calculate the average
        float average_x = 0;
        float average_y = 0;
        for (size_t j = 0; j < rb.size; j++) {
            average_x += buffer[j].x;
            average_y += buffer[j].y;
        }
        average_x /= (float)rb.size;
        average_y /= (float)rb.size;

        assert(average_x == expect_x[i]);
        assert(average_y == expect_y[i]);
    } 

    ring_buffer_free(&rb);
    PRINT_SUCCESS;
}


int main() {
    test_rb_fill_primitives();
    test_rb_fill_struct();
    test_rb_moving_average();
}

