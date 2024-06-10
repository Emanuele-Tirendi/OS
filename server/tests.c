#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "test.h"
/*
for all tests it is important that the example.html is the same as the original.html
*/
char* read_html_example(){
    FILE *html_file;
    html_file = fopen("example.html", "r");
    static char buffer[10000];
    int i = 0;
    char c;
    while((c = fgetc(html_file)) != EOF){
        buffer[i] = c;
        i++;
    }
    buffer[i] = '\0';
    fclose(html_file);
    return buffer;
}

/*
Test functions fffor the first suite. They test if the number of lines are correct and that the files is in correct order.
*/ 
void test_getLength(void) {
    CU_ASSERT_EQUAL(get_length("original.html"), 11);
}
void test_insert(void) {
    initialize_html();
    insert(3, "hahaha\n");
    CU_ASSERT_EQUAL(get_length("example.html"), 12);
    char* actual = read_html_example();
    char* expected = "<!DOCTYPE html>\n<html lang=\"en\">\nhahaha\n<head>\n    <meta charset=\"UTF-8\">\n    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n    <title>Hello, World!</title>\n</head>\n<body>\n    <h1>Hello, World!</h1>\n    <p>This is a simple \"Hello, World!\" webpage written in HTML.</p>\n</body>\n</html>";
    CU_ASSERT_STRING_EQUAL(actual, expected);
}
void test_delete(void) {
    delete(3);
    CU_ASSERT_EQUAL(get_length("example.html"), 11);
    char* actual = read_html_example();
    char* expected = "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n    <meta charset=\"UTF-8\">\n    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n    <title>Hello, World!</title>\n</head>\n<body>\n    <h1>Hello, World!</h1>\n    <p>This is a simple \"Hello, World!\" webpage written in HTML.</p>\n</body>\n</html>";
    CU_ASSERT_STRING_EQUAL(actual, expected);
}
void test_change(void){
    change(3, "hahaha\n");
    CU_ASSERT_EQUAL(get_length("example.html"), 11);
    char* actual = read_html_example();
    char* expected = "<!DOCTYPE html>\n<html lang=\"en\">\nhahaha\n    <meta charset=\"UTF-8\">\n    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n    <title>Hello, World!</title>\n</head>\n<body>\n    <h1>Hello, World!</h1>\n    <p>This is a simple \"Hello, World!\" webpage written in HTML.</p>\n</body>\n</html>";
    CU_ASSERT_STRING_EQUAL(actual, expected);
    change(3, "<head>\n");
}
/*
Test functions for test suite2. They test if the functions works when the lines are inserted changes or deleted at the first posistion of the html file.
*/
void test_insert_start(void) {
    initialize_html();
    insert(1, "hahaha\n");
    char* actual = read_html_example();
    char* expected = "hahaha\n<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n    <meta charset=\"UTF-8\">\n    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n    <title>Hello, World!</title>\n</head>\n<body>\n    <h1>Hello, World!</h1>\n    <p>This is a simple \"Hello, World!\" webpage written in HTML.</p>\n</body>\n</html>";
    CU_ASSERT_STRING_EQUAL(actual, expected);
}
void test_delete_start(void) {
    delete(1);
    char* actual = read_html_example();
    char* expected = "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n    <meta charset=\"UTF-8\">\n    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n    <title>Hello, World!</title>\n</head>\n<body>\n    <h1>Hello, World!</h1>\n    <p>This is a simple \"Hello, World!\" webpage written in HTML.</p>\n</body>\n</html>";
    CU_ASSERT_STRING_EQUAL(actual, expected);
}
void test_change_start(void){
    change(1, "hahaha\n");
    char* actual = read_html_example();
    char* expected = "hahaha\n<html lang=\"en\">\n<head>\n    <meta charset=\"UTF-8\">\n    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n    <title>Hello, World!</title>\n</head>\n<body>\n    <h1>Hello, World!</h1>\n    <p>This is a simple \"Hello, World!\" webpage written in HTML.</p>\n</body>\n</html>";
    CU_ASSERT_STRING_EQUAL(actual, expected);
    change(1, "<!DOCTYPE html>\n");
}

/*
Test functions for test suite3. They test if the functions works when the lines are inserted changes or deleted at the end of the html file.
*/
void test_insert_end(void) {
    initialize_html();
    insert(13, "\nhahaha\n");
    char* actual = read_html_example();
    char* expected = "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n    <meta charset=\"UTF-8\">\n    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n    <title>Hello, World!</title>\n</head>\n<body>\n    <h1>Hello, World!</h1>\n    <p>This is a simple \"Hello, World!\" webpage written in HTML.</p>\n</body>\n</html>\nhahaha\n";
    CU_ASSERT_STRING_EQUAL(actual, expected);
}
void test_delete_end(void) {
    delete(13);
    char* actual = read_html_example();
    char* expected = "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n    <meta charset=\"UTF-8\">\n    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n    <title>Hello, World!</title>\n</head>\n<body>\n    <h1>Hello, World!</h1>\n    <p>This is a simple \"Hello, World!\" webpage written in HTML.</p>\n</body>\n</html>\n";
    CU_ASSERT_STRING_EQUAL(actual, expected);
}
void test_change_end(void){
    change(12, "hahaha\n");
    char* actual = read_html_example();
    char* expected = "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n    <meta charset=\"UTF-8\">\n    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n    <title>Hello, World!</title>\n</head>\n<body>\n    <h1>Hello, World!</h1>\n    <p>This is a simple \"Hello, World!\" webpage written in HTML.</p>\n</body>\nhahaha\n";
    CU_ASSERT_STRING_EQUAL(actual, expected);
    change(12, "</html>\n");
}

/*
Test functions for suite4. They check if the insert change and delete functions still work when the number is larger than the actualy size of the file.
*/
void test_insert_high(void) {
    initialize_html();
    insert(20, "\nhahaha\n");
    char* actual = read_html_example();
    char* expected = "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n    <meta charset=\"UTF-8\">\n    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n    <title>Hello, World!</title>\n</head>\n<body>\n    <h1>Hello, World!</h1>\n    <p>This is a simple \"Hello, World!\" webpage written in HTML.</p>\n</body>\n</html>\nhahaha\n";
    CU_ASSERT_STRING_EQUAL(actual, expected);
}
void test_delete_high(void) {
    delete(20);
    char* actual = read_html_example();
    char* expected = "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n    <meta charset=\"UTF-8\">\n    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n    <title>Hello, World!</title>\n</head>\n<body>\n    <h1>Hello, World!</h1>\n    <p>This is a simple \"Hello, World!\" webpage written in HTML.</p>\n</body>\n</html>\nhahaha\n";
    CU_ASSERT_STRING_EQUAL(actual, expected);
}
void test_change_high(void){
    change(20, "hahaha\n");
    char* actual = read_html_example();
    char* expected = "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n    <meta charset=\"UTF-8\">\n    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n    <title>Hello, World!</title>\n</head>\n<body>\n    <h1>Hello, World!</h1>\n    <p>This is a simple \"Hello, World!\" webpage written in HTML.</p>\n</body>\n</html>\nhahaha\nhahaha\n";
    CU_ASSERT_STRING_EQUAL(actual, expected);
    delete(13);
    delete(13);
}

// Main function setting up and running tests
int main() {
    if (CUE_SUCCESS != CU_initialize_registry()) {
        return CU_get_error();
    }

    CU_pSuite pSuite1 = NULL;
    pSuite1 = CU_add_suite("Suite_1", 0, 0);
    if (NULL == pSuite1) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if ((NULL == CU_add_test(pSuite1, "test of getLength()", test_getLength)) ||
        (NULL == CU_add_test(pSuite1, "test of insert()", test_insert)) ||
        (NULL == CU_add_test(pSuite1, "test of delete()", test_delete)) ||
        (NULL == CU_add_test(pSuite1, "test of change()", test_change))) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_pSuite pSuite2 = NULL;
    pSuite2 = CU_add_suite("Suite_2", 0, 0);
    if (NULL == pSuite2) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if ((NULL == CU_add_test(pSuite2, "test of insert()", test_insert_start)) ||
        (NULL == CU_add_test(pSuite2, "test of delete()", test_delete_start)) ||
        (NULL == CU_add_test(pSuite2, "test of change()", test_change_start))) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_pSuite pSuite3 = NULL;
    pSuite3 = CU_add_suite("Suite_3", 0, 0);
    if (NULL == pSuite3) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if ((NULL == CU_add_test(pSuite3, "test of insert()", test_insert_end)) ||
        (NULL == CU_add_test(pSuite3, "test of delete()", test_delete_end)) ||
        (NULL == CU_add_test(pSuite3, "test of change()", test_change_end))) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_pSuite pSuite4 = NULL;
    pSuite4 = CU_add_suite("Suite_4", 0, 0);
    if (NULL == pSuite4) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if ((NULL == CU_add_test(pSuite4, "test of insert()", test_insert_high)) ||
        (NULL == CU_add_test(pSuite4, "test of delete()", test_delete_high)) ||
        (NULL == CU_add_test(pSuite4, "test of change()", test_change_high))) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
