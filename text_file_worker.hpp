#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>


///@brief Max size of filenames
const int FLEN = 300;

/// @brief The main part of my programm!!!
/// file_info contains info about:
/// 1)size of file
/// 2) number of symbols
/// 3) pointer to array with text from file
/// 4) number of strings
/// 5) array of struct containing pointers to the beginning and the end of each string
/// @note Have only one constructor!!! Look at this!!!
/// @note Destructor free memory
struct file_info;

///@brief Struct containing 2 pointers: pointer to the beginning of string and pointer to the ending of the same string
struct pointer;

/// @brief Function calculates size of file
/// @param *stream Stream with file
/// @return Size of file
long  size_of_file ( FILE* stream);

/// @brief Function counts strings in array with text
/// @param *poem_arr Array containing text
/// @return Number of strings
long  stringcount ( char* poem_arr );

/// @brief Function assigns pointers to every string in array with text
/// @param *poem_arr Array containing text
/// @param **pointers Array with pointers
/// @param number_of_strings Number of strings
void  makeptr ( char* poem_arr, pointer* pointers, long number_of_strings);

/// @brief Compare 2 strings
/// @return Returns difference between first mismatching symbols in strings or returns 0 if first string if equal second one
int   direct_strcmp ( const pointer string1, const pointer string2 );

/// @brief Swap 2 strings in an array
/// @param mySwappingArray Array with strings
/// @param a Index of first string
/// @param b Index of second string
void  mySwap ( pointer* mySwappingArray, long a, long b);

/// @brief An usual quicksort, but for strings:)
/// @param **arr Array for sorting
/// @param left Lower edge for sort
/// @param right Upper edge for sort
void  quicksort ( pointer* arr, long left, long right, int (*strcmp) ( const pointer string1, const pointer string2 ));

/// @brief Opens file (You must write the name of file)
/// @return Stream with file
FILE* open_file ();

/// @brief Writes strings in file
/// @param **stringpointer Array containing pointers to strings
/// @param **number_of_strings Number of strings;
void write_in_file ( pointer* stringpointer, const long number_of_strings);

/// @param  [in] ch A symbol
/// @return ch if it is a lowercase letter or lowercase analogue of ch
char lowercase_letter ( char ch);

/// @brief Compare 2 strings from its end ( Ignores symbol if it's not letter)
/// @return Returns difference between first mismatching symbols in strings or returns 0 if first string if equal second one
int reversed_strcmp ( const pointer string1, const pointer string2);

/// @brief Writes information about file to struct file_info \n
/// file_info contains info about: \n
/// 1)size of file \n
/// 2) number of symbols \n
/// 3) pointer to array with text from file \n
/// 4) number of strings \n
/// 5) array of struct containing pointers to the beginning and the end of each string
file_info file_worker ();


/// @brief Complex of tests for direct_strcmp
void test_direct_strcmp ();

/// @brief Complex of tests for reversed_strcmp
void test_reversed_strcmp ();

/// @brief Complex of tests for lowercase_letter
void test_lowercase_letter ();

/// @brief Uses all the tests
void tests();




struct pointer
{
    const char* b_ptr;
    const char* e_ptr;
};


struct file_info
{
    long file_size = 0;
    long number_of_symbols = 0;
    char* text_arr = nullptr;
    long number_of_strings = 0;
    pointer* stringpointer = nullptr;

    file_info (const long size_of_file, const long symbols_number, char* poem_arr, const long stringnumber, pointer* strptr)
    {
            assert (poem_arr);
            assert (strptr);

            file_size = size_of_file;
            number_of_symbols = symbols_number;
            text_arr = poem_arr;
            number_of_strings = stringnumber;
            stringpointer = strptr;
    }

    ~file_info()
    {
        free (stringpointer);
        stringpointer = nullptr;

        free (text_arr);
        text_arr = nullptr;
    }
};

file_info file_worker ()
{
    FILE* stream = open_file ();
    assert (stream);

    const long file_size = size_of_file ( stream );

    char* poem_arr = (char*) calloc ( file_size+2, sizeof(char) );
    if (!poem_arr)
    {
        printf ("Memory can't be allocated\n");
        assert (poem_arr);
    }

    *poem_arr = '\0';
    poem_arr++;

    const long number_of_symbols = fread ( poem_arr, sizeof(char), file_size, stream );
    poem_arr[number_of_symbols] = '\0';

    fclose (stream);

    const long number_of_strings = stringcount ( poem_arr );

    pointer* stringpointer = (pointer*)calloc ( number_of_strings, sizeof (pointer) );
    makeptr (poem_arr, stringpointer, number_of_strings);

    file_info information (file_size, number_of_symbols, poem_arr, number_of_strings, stringpointer );
    return information;
}


FILE* open_file()                            
{
    FILE* stream = NULL;
    char INPUT_FILE_NAME[FLEN] = "";
    printf ("Enter input file name: ");
    scanf("%s", INPUT_FILE_NAME);
    if ( !( stream = fopen ( INPUT_FILE_NAME, "r") ) )
    {
        fprintf (stderr, "Input file is not open\n");   
        return nullptr;
    }
    return stream;
}

void write_in_file( pointer* stringpointer, const long number_of_strings)
{
    assert (stringpointer);
    assert ( 0 < number_of_strings );

    FILE* stream = NULL;
    char OUTPUT_FILE_NAME[FLEN] = "";
    scanf ("%s", OUTPUT_FILE_NAME);
    if ( !( stream = fopen ( OUTPUT_FILE_NAME, "w") ) )
    {
        fprintf (stderr, "Input file is not open\n");
        return;
    }

    for ( int i = 0; i < number_of_strings; i++)
    {
        fprintf(stream, "%s\n", stringpointer->b_ptr );
        stringpointer++;
    }

    fclose (stream);
}

long size_of_file (FILE* stream)
{
    assert ( stream );

    fseek ( stream, 0, SEEK_END);
    const long filesize = ftell (stream);
    fseek ( stream, 0, SEEK_SET);
    return filesize;
}

long stringcount ( char* poem_arr )     
{
    assert (  poem_arr );
    assert ( *poem_arr );

    long scount = 0;
    while (*poem_arr)
    {
       if ( *poem_arr == '\n' )
        {
            *poem_arr = '\0';
            scount++;
        }
        poem_arr++;
    }
    return ++scount;
}

void makeptr ( char* poem_arr, pointer* pointers, long number_of_strings)
{
    assert ( poem_arr );
    assert ( *poem_arr );
    assert ( pointers );
    assert ( 0 < number_of_strings );

    long i = 1;
    pointers->b_ptr=poem_arr++;
    pointers++;
    while ( i < number_of_strings)
    {
        if ( !(*poem_arr) )
        {
            (pointers-1)->e_ptr = poem_arr - 1;
            pointers->b_ptr = poem_arr+1;
            i++;
            if ( i!= number_of_strings )
                {
                    pointers++;
                    poem_arr++;
                }
        }
        else poem_arr++;
    }
    while (*poem_arr) poem_arr++;
    pointers->e_ptr = poem_arr - 1;
}

void mySwap ( pointer* swap_array, long a, long b)
{
    assert (isfinite(a));   //
    assert (isfinite(b));   //
    assert (swap_array != NULL);

    pointer temp = swap_array[a];
    swap_array[a] = swap_array[b];
    swap_array [b] = temp;
}

void quicksort (pointer* arr, long left, long right, int (*strcmp) ( const pointer string1, const pointer string2 ))
{
    assert (arr);
    assert ( isfinite (left) ); //
    assert ( isfinite (right) ); //

    if ( right <= left ) return;
    if ( right - left == 1 )
    {
        if ( strcmp( arr[left], arr[right]) > 0 )   mySwap ( arr, left, right );
        return;
    }                                                                                     
    const long beginning = left;
    const long ending = right;
    const pointer mid = arr[ (left + right) / 2];
    while (left < right)
    {
        while ( strcmp( arr[left], mid ) < 0 ) left++;
        while ( strcmp( arr[right], mid) > 0 ) right--;
        if ( left < right )
        {
            mySwap (arr, left, right);
            left++;
            right--;
        }
    }
    quicksort ( arr, beginning, right, strcmp);
    quicksort ( arr, left, ending, strcmp);
}

char lowercase_letter (const char ch)
{
    if ( ch >= 'A' && ch <= 'Z' ) return ch + ('a' - 'A');
    return ch;
}

int direct_strcmp ( const pointer string1, const pointer string2 )
{
    assert ( string1.b_ptr );
    assert ( string2.b_ptr );

    int i = 0;
    while ( string1.b_ptr [i] != '\0' && string2.b_ptr [i] != '\0' )
    {
        if ( lowercase_letter (string1.b_ptr [i]) != lowercase_letter (string2.b_ptr[i]) ) return lowercase_letter (string1.b_ptr [i]) - lowercase_letter (string2.b_ptr [i]);
        i++;
    }
    return lowercase_letter (string1.b_ptr[i]) - lowercase_letter (string2.b_ptr[i]);
}

int reversed_strcmp ( const pointer string1, const pointer string2)
{
    assert ( string1.e_ptr );
    assert ( string2.e_ptr );

    int i = 0, j = 0;
    while ( string1.e_ptr [i] != '\0' && string2.e_ptr [j] != '\0' )
    {
        while ( (lowercase_letter (string1.e_ptr [i]) < 'a' || lowercase_letter (string1.e_ptr [i]) > 'z') && lowercase_letter (string1.e_ptr [i]) != '\0' ) i--;
        while ( (lowercase_letter (string2.e_ptr [j]) < 'a' || lowercase_letter (string2.e_ptr [j]) > 'z') && lowercase_letter (string2.e_ptr [j]) != '\0') j--;
        if ( lowercase_letter (string1.e_ptr [i]) != lowercase_letter (string2.e_ptr[j]) ) return lowercase_letter (string1.e_ptr [i]) - lowercase_letter (string2.e_ptr [j]);
        i--;
        j--;
    }
    return lowercase_letter (string1.e_ptr[i]) - lowercase_letter (string2.e_ptr[j]);
}




void tests()
{
    char wanna_test = 0;
    printf ("Wanna test? (Y/N): ");
    scanf ("%c", &wanna_test);
    printf ("\n");
    if ( wanna_test == 'Y' || wanna_test == 'y')
    {
        test_direct_strcmp ();
        test_reversed_strcmp ();
        test_lowercase_letter ();
    }
}




void test_direct_strcmp1 ()
{
    const char* string1 = "abcd";
    const char* string2 = "bcde";
    pointer ptr1, ptr2;
    ptr1.b_ptr = string1;
    ptr2.b_ptr = string2;
    assert( direct_strcmp (ptr1, ptr2) == -1 );
    assert( direct_strcmp (ptr2, ptr1) == 1 );
    printf ("first direct_strcmp test done!!!\n");
}

void test_direct_strcmp2 ()
{
    const char* string1 = "rdgs";
    const char* string2 = "r";
    pointer ptr1, ptr2;
    ptr1.b_ptr = string1;
    ptr2.b_ptr = string2;
    assert( direct_strcmp (ptr1, ptr2) == 'd' );
    assert( direct_strcmp (ptr2, ptr1) == -'d' );
    printf ("second direct_strcmp test done!!!\n");
}

void test_direct_strcmp3 ()
{
    const char* string1 = "";
    const char* string2 = "f";
    pointer ptr1, ptr2;
    ptr1.b_ptr = string1;
    ptr2.b_ptr = string2;
    assert( direct_strcmp (ptr1, ptr2) == -'f' );
    assert( direct_strcmp (ptr2, ptr1) == 'f' );
    printf ("third direct_strcmp test done!!!\n");
}

void test_direct_strcmp4 ()
{
    const char* string1 = "fesfesfesf";
    const char* string2 = "fesfesfesf";
    pointer ptr1, ptr2;
    ptr1.b_ptr = string1;
    ptr2.b_ptr = string2;
    assert( direct_strcmp (ptr1, ptr2) == 0 );
    assert( direct_strcmp (ptr2, ptr1) == 0 );
    printf ("fourth direct_strcmp test done!!!\n");
}

void test_direct_strcmp ()
{
    test_direct_strcmp1 ();
    test_direct_strcmp2 ();
    test_direct_strcmp3 ();
    test_direct_strcmp4 ();
    printf ("DIRECT_STRCMP WORKS\n\n");
}






void test_reversed_strcmp1 ()
{
    const char* string1 = "abcd";
    const char* string2 = "abcc";
    pointer ptr1, ptr2;
    for (int i = 0; string1[i]; i++) ptr1.e_ptr = &string1[i];
    for (int i = 0; string2[i]; i++) ptr2.e_ptr = &string2[i];
    assert( reversed_strcmp (ptr1, ptr2) == 1 );
    assert( reversed_strcmp (ptr2, ptr1) == -1 );
    printf ("first reversed_strcmp test done!!!\n");
}

void test_reversed_strcmp2 ()
{
    const char* string1 = "abcccd";
    const char* string2 = "abcccd";
    pointer ptr1, ptr2;
    for (int i = 0; string1[i]; i++) ptr1.e_ptr = &string1[i];
    for (int i = 0; string2[i]; i++) ptr2.e_ptr = &string2[i];
    assert( reversed_strcmp (ptr1, ptr2) == 0 );
    assert( reversed_strcmp (ptr2, ptr1) == 0 );
    printf ("second reversed_strcmp test done!!!\n");
}

void test_reversed_strcmp3 ()
{
    const char* string1 = "a";
    const char* string2 = "red'";
    pointer ptr1, ptr2;
    for (int i = 0; string1[i]; i++) ptr1.e_ptr = &string1[i];
    for (int i = 0; string2[i]; i++) ptr2.e_ptr = &string2[i];
    assert( reversed_strcmp (ptr1, ptr2) == -3 );
    assert( reversed_strcmp (ptr2, ptr1) == 3 );
    printf ("third reversed_strcmp test done!!!\n");
}

void test_reversed_strcmp4 ()
{
    const char* string1 = "aesgsrfs-3";
    const char* string2 = "asgfsfsdr65";
    pointer ptr1, ptr2;
    for (int i = 0; string1[i]; i++) ptr1.e_ptr = &string1[i];
    for (int i = 0; string2[i]; i++) ptr2.e_ptr = &string2[i];
    assert( reversed_strcmp (ptr1, ptr2) == 1 );
    assert( reversed_strcmp (ptr2, ptr1) == -1 );
    printf ("first reversed_strcmp test done!!!\n");
}

void test_reversed_strcmp ()
{
    test_reversed_strcmp1 ();
    test_reversed_strcmp2 ();
    test_reversed_strcmp3 ();
    test_reversed_strcmp4 ();
    printf ("REVERSED_STRCMP WORKS\n\n");
}







void test_lowercase_letter1 ()
{
    assert (lowercase_letter ('L') == 'l');
    assert (lowercase_letter ('f') == 'f');
    assert (lowercase_letter ('D') == 'd');
    assert (lowercase_letter ('T') == 't');
    assert (lowercase_letter ('r') == 'r');
    printf ("first lowercase_letter test done!!!\n");
}

void test_lowercase_letter2 ()
{
    assert (lowercase_letter ('t') == 't');
    assert (lowercase_letter ('p') == 'p');
    assert (lowercase_letter ('o') == 'o');
    assert (lowercase_letter ('i') == 'i');
    assert (lowercase_letter ('k') == 'k');
    printf ("second lowercase_letter test done!!!\n");
}

void test_lowercase_letter3 ()
{
    assert (lowercase_letter ('j') == 'j');
    assert (lowercase_letter ('J') == 'j');
    assert (lowercase_letter ('k') == 'k');
    assert (lowercase_letter ('K') == 'k');
    assert (lowercase_letter ('l') == 'l');
    printf ("third lowercase_letter test done!!!\n");
}

void test_lowercase_letter4 ()
{
    assert (lowercase_letter ('A') == 'a');
    assert (lowercase_letter ('S') == 's');
    assert (lowercase_letter ('D') == 'd');
    assert (lowercase_letter ('E') == 'e');
    assert (lowercase_letter ('R') == 'r');
    printf ("fourth lowercase_letter test done!!!\n");
}

void test_lowercase_letter ()
{
    test_lowercase_letter1 ();
    test_lowercase_letter2 ();
    test_lowercase_letter3 ();
    test_lowercase_letter4 ();
    printf ("LOWERCASE_LETTER WORKS\n\n");
}
