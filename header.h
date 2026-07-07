#ifndef HEADER_h
#define HEADER_h
#include<stdio.h>
#include<string.h>

// OperationType will be used in view/eidt return type 
typedef enum{
    is_view,
    is_edit,
    is_other
}Operation_Type;

// OperationType will be used in Success/failure return type 
typedef enum{
    e_success,
    e_failure
}Status;

// User Defined Datatype for reading/view
typedef struct{
    char *mp3_name;
    FILE *fptr_mp3_name;
}Tag_Reader;

// User defined datatype for editing
typedef struct{
    char *tag;
    char *new_data;
    char *mp3_name;
    char *temp_name;
    FILE *fptr_mp3_name;
    FILE *fptr_temp_name;
}Tag_Edit;

// Function Prototypes

// Read and validate from argv in view function
Status read_and_validate(char **,Tag_Reader *);

// Display the Details
Status Display(Tag_Reader*);

// File Opening for viewing 
Status open_file(Tag_Reader*);

// Read and validate from argv in edit function
Status validate(char **,Tag_Edit*);

// Performing the editing operation
Status do_edit(Tag_Edit *);

// File Opening for editing
Status open(Tag_Edit*);

// Copying header file 
Status copy_header(Tag_Edit*);

// Checking the tag is equal to user given inuput tag
char * check_tag(Tag_Edit *);

// Converting Big Endian to Little Endian
unsigned int Big_to_little(unsigned int);

// Converting Little Endian to Big Endian
unsigned int Little_to_big(unsigned int);

// Checking the edit or view option
Operation_Type check_operation_type(char *);

// Copying the data from source file to tempfile
Status Copy(Tag_Edit*);

// Removing the source file 
Status delete(char *);

// Updating the output file as source file
Status edit_name(Tag_Edit*);

#endif