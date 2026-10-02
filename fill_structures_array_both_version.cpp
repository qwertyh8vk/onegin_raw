
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
//             }

//             raw_buffer_previous_string = (raw_buffer_current_string + 1);
//         }

//         raw_buffer_current_string++;
//     }

//     return strings_info_arr;
// }
