#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define POISON 0xEBA1CB0E04E10

// #define ON_DEBUG

#ifdef ON_DEBUG
    #define PRINTF(...) printf(__VA_ARGS__)
    #define GETCHAR() getchar()
#endif
#ifndef ON_DEBUG
    #define PRINTF(...) (void(0))
    #define GETCHAR()   (void(0))
#endif

enum DATATYPES {
    INT          = 1,
    UNSIGNED_INT = 2,
    FLOAT        = 3,
    DOUBLE       = 4,
    SIZE_T       = 5,
    CHAR         = 6,
    POINTER      = 7,
    STRING       = 8,
};

// struct stat file_info;

struct str_info {
    size_t str_len;
    const char* ptr_on_str;
};

struct eaten_array {
    char*  raw_buffer;
    size_t length_written;
    unsigned int amount_of_read_strings;
    struct str_info* array_strings_info;
};

void   quick_sort(void* array, 
                  int (*comparator) (const void* address_a, const void* address_b), 
                  void (*swapper) (int index_a, int index_b, void* source_array, int array_element_size), 
                  int array_element_size,
                  int left_border, int right_border);

void   my_bubble_sort(void* array, int left_border, int right_border, 
                      int array_element_size,
                      int (*comparator) (const void* address_a, const void* address_b),
                      void (*swapper) (int index_a, int index_b, void* source_array, int array_element_size)); 

void   swap_ints(int a, int b, void* array, int array_element_size);
void   swap_doubles(int a, int b, void* array, int array_element_size);
void   swap_chars(int a, int b, void* array, int array_element_size);
void   swap_pointers(int a, int b, void* array, int array_element_size);
int    partition(void* array, int array_element_size, int left_border, int right_border,
                 int (*comparator) (const void* address_a, const void* address_b), 
                 void (*swapper) (int index_a, int index_b, void* source_array, int array_element_size));

int    compare_ints(const void* address_a, const void* address_b);
int    my_strcompare_start(const void* string_a_raw, const void* string_b_raw);
int    my_strcompare_end(const void* string_a_raw, const void* string_b_raw);

int    maximum(int i, int j);
int    minimum(int i, int j);

char*  initialize_raw_buffer(const char* filename, struct stat* metadata);
size_t fill_raw_buffer(char* raw_buffer, const char* filename, unsigned int* strings_counter, struct stat* ptr_file_info);

struct str_info* fill_structures_array(char* raw_buffer, unsigned int strings_counter, size_t length_written);

void   print_strings_by_ptr_array(char** pointers_array, int pointers_array_size);
void   fill_sorted_strings_in_txt_file(const char* filename, struct str_info* array_strings_info, unsigned int strings_counter);
void   raspechatka(int* array, int size); // сделать войдовской!!!!!

struct eaten_array* eat_txt_file(const char* filename_from);