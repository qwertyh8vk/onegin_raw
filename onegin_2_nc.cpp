#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define POISON 0xEBA1BCE

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

struct stat  file_info;
struct stat* ptr_file_info = &file_info;

struct str_info {
    size_t str_len;
    const char* ptr_on_str;
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
void   swap_pointers(int a, int b, void* array, int array_element_size); // Деду не забыть показать 21 варнинг если поставить unsigned int
int    partition(void* array, int array_element_size, int left_border, int right_border,
                 int (*comparator) (const void* address_a, const void* address_b), 
                 void (*swapper) (int index_a, int index_b, void* source_array, int array_element_size));

int    compare_ints(const void* address_a, const void* address_b);
int    my_strcompare_start(const void* string_a_raw, const void* string_b_raw);
int    my_strcompare_end(const void* string_a_raw, const void* string_b_raw);

int    maximum(int i, int j);
int    minimum(int i, int j);

char*  initialize_raw_buffer(const char* filename, struct stat* metadata);
size_t fill_raw_buffer(char* raw_buffer, const char* filename, unsigned int* strings_counter);

// char** fill_indexes_array(char* raw_buffer, unsigned int strings_counter, size_t length_written);
struct str_info* fill_structures_array(char* raw_buffer, unsigned int strings_counter, size_t length_written);

void   print_strings_by_ptr_array(char** pointers_array, int pointers_array_size);

void   fill_sorted_strings_in_txt_file(const char* filename, struct str_info* array_strings_info, unsigned int strings_counter);
void   raspechatka(int* array, int size); // сделать войдовской!!!!!

int main () {
    unsigned int amount_of_read_strings = 0;

    char*  raw_buffer = initialize_raw_buffer("formatted_onegin.txt", &file_info);
    size_t length_written = fill_raw_buffer(raw_buffer, "formatted_onegin.txt", &amount_of_read_strings);
    struct str_info* array_strings_info = fill_structures_array(raw_buffer, amount_of_read_strings, length_written);

    for (unsigned int i = 0; i < amount_of_read_strings; i++) {
        printf("contains: <%s>, address: <%p>, structaddress: <%p>, length: <%zu>, string number: <%u>", 
               array_strings_info[i].ptr_on_str, 
               array_strings_info[i].ptr_on_str, 
               array_strings_info + i,
               array_strings_info[i].str_len, 
               i+1);
            
        printf("\n");
    }   


    printf("sizeof strinfo: <%zu>, sizeof strlen'a: <%zu>, sizeof char*: <%zu>\n", sizeof(array_strings_info[0]), sizeof(array_strings_info[0].str_len), sizeof(array_strings_info[0].ptr_on_str));
    getchar();
    printf("lengthwritten: <%zu>\n", length_written);


    printf("<%u> amount of read strings\n", amount_of_read_strings);
    printf("---------------------------------\n");

    quick_sort(array_strings_info, my_strcompare_start, swap_pointers, sizeof(struct str_info), 0, amount_of_read_strings - 1);

    fill_sorted_strings_in_txt_file("output.txt", array_strings_info, amount_of_read_strings);
    printf("нажми гетчар\n");

    getchar();

    const char* ptr_on_str_const_1 = "To Peter Alexandrovich Pletnev";
    const char* ptr_on_str_const_2 = "Heedless of the proud world's enjoyment";

    struct str_info debug[2] {
        {  
        strlen(ptr_on_str_const_1),
        ptr_on_str_const_1,
        },

        {   
        strlen(ptr_on_str_const_2),
        ptr_on_str_const_2,
       }
    };

    int comparator_result = my_strcompare_start(&debug[0], &debug[1]);

    // getchar();

    // qsort(array_strings_info, amount_of_read_strings, sizeof(struct str_info), my_strcompare_start); 

    // // my_bubble_sort(array_strings_info, 0, amount_of_read_strings - 1, sizeof(struct str_info), my_strcompare_start, swap_pointers);

    // for (unsigned int i = 0; i < amount_of_read_strings; i++) {
    //     printf("contains: <%s>, address: <%p>, length: <%zu>, string number: <%u>", 
    //            array_strings_info[i].ptr_on_str, 
    //            array_strings_info[i].ptr_on_str, 
    //            array_strings_info[i].str_len, 
    //            i+1);
            
    //     printf("\n");
    // }   

    // fill_sorted_strings_in_txt_file("output.txt", array_strings_info, amount_of_read_strings);

    free(raw_buffer);
    raw_buffer = (char *)POISON; // увидел в инете и пздц понравилось

    return 0;
}

void quick_sort(void* array, 
                int (*comparator) (const void* address_a, const void* address_b), 
                void (*swapper) (int index_a, int index_b, void* source_array, int array_element_size),
                int array_element_size,
                int left_border, int right_border) 
{
    assert(array);
    // printf("<%d> right border, <%d> left border\n", right_border, left_border);

    if (right_border - left_border <= 3) {
        my_bubble_sort(array, left_border, right_border, array_element_size, comparator, swapper);
        // printf("ВОТ , ОДИН ЭЛЕМЕНТ, СРАВНИВАТЬ САМ С СОБОЙ НЕ НАДО!!!\n");
        return;
    }

    int pivot = partition(array, array_element_size, left_border, right_border, comparator, swapper);
    
    quick_sort(array, comparator, swapper, array_element_size, left_border, pivot);    
    quick_sort(array, comparator, swapper, array_element_size, pivot + 1, right_border);

}

int partition(void* array, int array_element_size, int left_border, int right_border,
              int (*comparator) (const void* address_a, const void* address_b), 
              void (*swapper) (int index_a, int index_b, void* source_array, int array_element_size)) 
{   
    assert(array);

    int pivot_index = (left_border + right_border) / 2;
    void *pivot = calloc(1, array_element_size);

    assert(pivot);

    memcpy(pivot, (char *)array + pivot_index * array_element_size, array_element_size);

    int i = left_border;
    int j = right_border;

    while (true) {
        while (comparator((char *)array + i * array_element_size, pivot) < 0) {
            i++;
        }

        while (comparator((char *)array + j * array_element_size, pivot) > 0) {
            j--;
        }

        if (i >= j) {
            free(pivot);
            pivot = (void *)POISON;
            
            return j;
        }

        swapper(i, j, array, array_element_size);
        i++;
        j--;
    }
}

char* initialize_raw_buffer(const char* filename, struct stat* metadata) {
    int stat_status = stat(filename, metadata);

    if (stat_status == 0) {
        assert (metadata);
        off_t raw_buffer_size  = (*metadata).st_size + 1;

        if (raw_buffer_size) {
            char* raw_buffer = (char *)calloc((size_t)raw_buffer_size, sizeof(char)); // скорее варнинг мьют, чем обязательное действие (size_t)
            assert(raw_buffer);
            return raw_buffer;
        }

        else {
            printf("ошибка во время определения размера файла.\nбуфер не инициализирован.\n"); 

            return NULL;
        }
    }

    else {

        return NULL;
    }
}

size_t fill_raw_buffer(char* raw_buffer, const char* filename, unsigned int* strings_counter) {
    assert(raw_buffer);
    assert(strings_counter);

    int fd = open(filename, O_RDONLY);
    size_t  maximum_to_write = ptr_file_info->st_size;
    ssize_t bytes_written = 0;

    if (fd > 0) {
        bytes_written = read(fd, raw_buffer, maximum_to_write);
    }

    for (ssize_t i = 0; i < bytes_written; i++) {
        if (raw_buffer[i] == '\n') {
            raw_buffer[i] = '\0';
            // if (i > 0 && (raw_buffer[i - 1] != '\0')) {
            //     (*strings_counter)++; // сделать скип воит здесь для правильного счёчтика слеш каунтер
            // }
            (*strings_counter)++;
        }
    }

    printf("<%zd>\n", bytes_written);
    
    raw_buffer[bytes_written] = '\0';
    if (raw_buffer[bytes_written - 1] != '\0') {
        (*strings_counter)++;
    }
    bytes_written++;

    return bytes_written;

    // assert(raw_buffer);
    // assert(strings_counter);

    // FILE*    file_pointer = fopen(filename, "r");
    // size_t   length_written = 0;

    // unsigned long symbol = 0;

    // if (file_pointer) {
    //     while ((symbol = fread(raw_buffer + length_written, sizeof(char), 1, file_pointer)) == 1) { // переделать полностью чтение файла
    //         length_written++;

    //         if (raw_buffer[length_written - 1] == '\n') {
    //             (*strings_counter)++;
    //             raw_buffer[length_written - 1] = '\0'; 
    //         }
    //     }
    // }

    // length_written++;
    // raw_buffer[length_written - 1] = '\0';
    // (*strings_counter)++;

    // return length_written;
}

// struct str_info* fill_structures_array(char* raw_buffer, unsigned int strings_counter, size_t length_written) {
//     assert(raw_buffer);

//     struct str_info* strings_info_arr = (struct str_info *)calloc(strings_counter, sizeof(struct str_info));

//     assert(strings_info_arr); 

//     unsigned int j = 0;

//     char* raw_buffer_current_string  = raw_buffer;
//     char* raw_buffer_previous_string = raw_buffer;
    
//     for (size_t i = 0; i < length_written && j < strings_counter; i++) {
//         printf("<%zu:%c:%d>", i, raw_buffer[i], raw_buffer[i]);
//         printf("\n");

//         if ((*raw_buffer_current_string) == '\0') {
//             if ((raw_buffer_current_string - raw_buffer_previous_string) > 0) {
//                 strings_info_arr[j].ptr_on_str = raw_buffer_previous_string;
//                 strings_info_arr[j].str_len = raw_buffer_current_string - raw_buffer_previous_string;
//                 j++;
//                 raw_buffer_previous_string = (raw_buffer_current_string + 1);
//                 raw_buffer_current_string++; // вот эта строчка починила мою запись.
//             }

//             else {
//                 printf("попал на слеш0\n");
//                 printf("rbcs1.0 <%c>, i <%zu>\n", *raw_buffer_current_string, i);
//                 getchar();
            
//                 while (*raw_buffer_current_string == '\0') {
//                     raw_buffer_current_string++;
//                 }

//                 raw_buffer_previous_string = raw_buffer_current_string;
//                 printf("rbcs <%c>, i <%zu>\n", *raw_buffer_current_string, i);
//             }

//         }

//         else {
//             raw_buffer_current_string++;
//         }
//     }

//     return strings_info_arr;
// }

struct str_info* fill_structures_array(char* raw_buffer, unsigned int strings_counter, size_t length_written) {
    assert(raw_buffer);

    struct str_info* strings_info_arr = (struct str_info *)calloc(strings_counter, sizeof(struct str_info));

    assert(strings_info_arr); 

    unsigned int j = 0;

    char* raw_buffer_current_string  = raw_buffer;
    char* raw_buffer_previous_string = raw_buffer;
    
    for (size_t i = 0; i < length_written && j < strings_counter; i++) {
        printf("<%zu:%c:%d>", i, raw_buffer[i], raw_buffer[i]);
        printf("\n");

        if ((*raw_buffer_current_string) == '\0') {
            if ((raw_buffer_current_string - raw_buffer_previous_string) > 0) {
                strings_info_arr[j].ptr_on_str = raw_buffer_previous_string;
                strings_info_arr[j].str_len = raw_buffer_current_string - raw_buffer_previous_string;
                j++;
            }

            raw_buffer_previous_string = (raw_buffer_current_string + 1);
        }

        raw_buffer_current_string++;
    }

    return strings_info_arr;
}


void fill_sorted_strings_in_txt_file(const char* filename, struct str_info* array_strings_info, unsigned int strings_counter) {
    assert(array_strings_info);

    FILE* file_pointer = fopen(filename, "w"); // OPEN или FOPEN?
    // int fd = open(filename, O_WRONLY);

    if (file_pointer) {
        for (unsigned int i = 0; i < strings_counter - 1; i++) {
            size_t len_of_one_string = array_strings_info[i].str_len;
            fwrite(array_strings_info[i].ptr_on_str, sizeof(char), len_of_one_string, file_pointer);
            // if (i != (strings_counter - 1)) {
                fputc(10,  file_pointer);
            // }
            printf("записал\n");
        }

        size_t len_of_one_string = array_strings_info[strings_counter - 1].str_len;
        fwrite(array_strings_info[strings_counter-1].ptr_on_str, sizeof(char), len_of_one_string, file_pointer);
    }

    else {
        printf("не удалось открыть файл.(\n");
    }
}


void print_strings_by_ptr_array(char** pointers_array, int pointers_array_size) {
    assert(pointers_array);

    printf("начало принта\n");
    printf("pointers_array_size: <%d>\n", pointers_array_size);
    
    for (int i = 0; i < pointers_array_size; i++) {
        printf("<%s>\n", pointers_array[i]);
    }

    printf("конец принта\n"); 
}

void my_bubble_sort(void* array, int left_border, int right_border, 
                    int array_element_size,
                    int (*comparator) (const void* address_a, const void* address_b),
                    void (*swapper) (int index_a, int index_b, void* source_array, int array_element_size)) 
{
    assert(array);

    for (int i = left_border; i < right_border; right_border--) {
        for (int j = left_border + 1; j < right_border + 1; j++) {

            assert((char *)array + i * array_element_size);
            assert((char *)array + j * array_element_size);

            if (comparator((char *)array + i * array_element_size, (char *)array + j * array_element_size) > 0) {
                swapper(i , j, array, array_element_size);
            }

            i++;
        }

        i = left_border;
    }
}

void swap_ints(int a, int b, void* array, int array_element_size) {
    assert(array);

    int* element_a = (int *)((char *)array + a * array_element_size);
    int* element_b = (int *)((char *)array + b * array_element_size);

    int temp   = *element_a;
    *element_a = *element_b;
    *element_b = temp;
}

void swap_pointers(int a, int b, void* array, int array_element_size) {
    assert(array);

    struct str_info* struct_a = ((struct str_info*)((char *)array + a * array_element_size));
    struct str_info* struct_b = ((struct str_info*)((char *)array + b * array_element_size));
    assert(struct_a);
    assert(struct_b);

    size_t temp_length = struct_a->str_len;
    struct_a->str_len = struct_b->str_len;
    struct_b->str_len = temp_length;

    const char* temp = struct_a->ptr_on_str;
    struct_a->ptr_on_str = struct_b->ptr_on_str;
    struct_b->ptr_on_str = temp;
}

/*  Хернища хернищевская которую надо переделать


// void swap_doubles(int a, int b, void* array, int array_element_size) {
//     double temp = *(double *)((char *)array + a * array_element_size);
//     *(double *)((char*)array + a * array_element_size) = *(double *)((char *)array + b * array_element_size);
//     *(double *)((char*)array + b * array_element_size) = temp;
// }

// void swap_chars(int a, int b, void* array, int array_element_size) {
//     char temp = *(char *)((char*)array + a * array_element_size);
//     *(char *)((char*)array + a * array_element_size) = *(char *)((char*)array + b * array_element_size);
//     *(char *)((char*)array + b * array_element_size) = temp;
// }


*/
int maximum(int i, int j) {
    if (i > j) {
        return i;
    }
    
    return j;
}

int minimum(int i, int j) {
    if (i < j) {
        return i;
    }

    return j;
}

int compare_ints(const void* address_a, const void* address_b) {
    const int num_a = *(const int *)(address_a);
    const int num_b = *(const int *)(address_b);

    if (num_a > num_b) {
        return 1;
    }
    
    else if (num_a < num_b) {
        return -1;
    }

    return 0;
}

int my_strcompare_start(const void* string_a_raw, const void* string_b_raw) {
    assert(string_a_raw);
    assert(string_b_raw);

    // printf("\n<%p>, <%p> до преобразований! string a string b\n", string_a_raw, string_b_raw);
    // getchar();

    

    const char* string_a = ((const str_info *)(string_a_raw)) -> ptr_on_str;
    const char* string_b = ((const str_info *)(string_b_raw)) -> ptr_on_str;

    // printf("<%p> адрес структуры а из массива структур\n", ((const str_info *)(string_a_raw)));
    // printf("<%p> адрес строки структуры а из массива структур\n", ((const str_info *)(string_a_raw)) -> ptr_on_str);
    // printf("<%s> первая строка (стринга)\n", ((const str_info *)(string_a_raw)) -> ptr_on_str);
    // printf("<%zu> длина первой строки\n", ((const str_info *)(string_a_raw)) -> str_len);

    // printf("------------------------------------------\n");

    // printf("<%p> адрес структуры б из массива структур\n", ((const str_info *)(string_b_raw)));
    // printf("<%p> адрес строки структуры б а из массива структур\n", ((const str_info *)(string_b_raw)) -> ptr_on_str);
    // printf("<%s> вторая строка (стринга)\n", ((const str_info *)(string_b_raw)) -> ptr_on_str);
    // printf("<%zu> длина второй строки\n", ((const str_info *)(string_b_raw)) -> str_len);;


    if (string_a == string_b) {
        // printf("сравниваем сам с собой. это партишин\n");

        return 0;
    }
   
    // getchar();

    size_t len_string_a = ((const str_info *)(string_a_raw)) -> str_len;
    size_t len_string_b = ((const str_info *)(string_b_raw)) -> str_len;

    size_t len_a_copy = len_string_a;
    size_t len_b_copy = len_string_b;

    size_t i = 0;
    size_t j = 0; 

    // printf("<%zu>, <%zu>\n", len_string_a, len_string_b);

    while (i < len_string_a && j < len_string_b) {

        // printf("строки: <%s> <%s>\nэлемент сравнения: <%c> <%c>\n", string_a, string_b, string_a[i], string_b[j]);
        // getchar();

        if (!isalpha(string_a[i])) {
            i++;
            len_a_copy--;
        }

        else if (!isalpha(string_b[j])) {
            j++;
            len_b_copy--;
        }

        else if (tolower(string_a[i]) - tolower(string_b[j]) != 0) {

            return (tolower(string_a[i]) - tolower(string_b[j]));
        }

        else {
            i++;
            j++;
        }
    }

    while (i < len_string_a) {
        if (!isalpha(string_a[i])) {
            len_a_copy--;
        }

        i++;
    }  


    while (j < len_string_b) {
        if (!isalpha(string_b[j])) {
            len_b_copy--;
        }

        j++;
    }

    return (int) (len_a_copy - len_b_copy);
}

int my_strcompare_end(const void* string_a_raw, const void* string_b_raw) {
    assert(string_a_raw);
    assert(string_b_raw);

    const char* string_a = ((const str_info *)(string_a_raw)) -> ptr_on_str;
    const char* string_b = ((const str_info *)(string_b_raw)) -> ptr_on_str;

    size_t len_string_a = ((const str_info *)(string_a_raw)) -> str_len;
    size_t len_string_b = ((const str_info *)(string_b_raw)) -> str_len;

    size_t len_a_copy = len_string_a;
    size_t len_b_copy = len_string_b;

    size_t i = len_a_copy;
    size_t j = len_b_copy;

    while (i > 0 && j > 0) {
        
        if (!isalpha(string_a[i - 1])) {
            i--;
            len_a_copy--;
        }

        else if (!isalpha(string_b[j - 1])) {
            j--;
            len_b_copy--;
        }

        else if (tolower(string_a[i - 1]) - tolower(string_b[j - 1]) != 0) {

            return (tolower(string_a[i - 1]) - tolower(string_b[j - 1]));
        }

        else {
            i--;
            j--;
        }
    }

    while (j > 0) {
        if (!isalpha(string_b[j - 1])) {
            len_b_copy--;
        }

        j--;
    }  
    
    while (i > 0) {
        if (!isalpha(string_a[i - 1])) {
            len_a_copy--;
        }

        i--;
    }   

    return (int) (len_a_copy - len_b_copy);
}

void raspechatka(int* array, int size) {
    assert(array);

    for (int i = 0; i < size; i++) {
        printf("[%d] ", *(int *)((char *)array + i * sizeof(int)));
    }

    printf("\n");
}
