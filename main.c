/**********************************************************************************************************
    Name :: Chandeesh K M
    Date :: 30/05/2026

    Description :: This code is the main code for implementation to reads, displays, and modifies
    the metadata (ID3v2 tags) stored in MP3 audio files. MP3 files contain metadata that 
    stores information about a song, such as the Title, Artist, Album, Year, Genre, Composer, 
    Comments, and other details, which are displayed by music players. It has two part view 
    and edit the song details. In view Option it display the details of song such as Title,
    Artist, Album, Year, Genre, Composer and Comments. In edit Option getting the which tag 
    to be replace with the given new data form user. The user selects the tag that needs 
    to be changed, such as the Title or Artist, and enters the new value. The program reads
    the existing data, replaces only the selected tag with the new information, and writes 
    the updated data back to the MP3 file while keeping all other data and the audio content 
    unchanged. In addition to this a help menu also provided for the user whenever user 
    entered invalid details or help option from the arguments. 

***********************************************************************************************************/

#include"header.h" // Contains user defined types

int main(int arg,char *argv[])
{
    // If no arguments passed printing Usage details
    if(argv[1]==NULL)
    {
        printf("ERROR: No Arguments Passed\n");
        printf("Usage: ./a.out <option> <file>\n");
        printf("       <option> Menu\n");
        printf("       -v ->To view\n");
        printf("       -e ->To edit\n");
        printf("To edit, pass argument as:\n");
        printf("Usage: ./a.out -e <Tag_list> <New_data> <file>\n");
        printf("        <Tag_list> Menu\n");
        printf("         -t ->Title\n");
        printf("         -T ->Artist\n");
        printf("         -A ->Album\n");
        printf("         -Y ->Year\n");
        printf("         -c ->COmposer\n");
        printf("         -C ->Comment\n");
        return 0;
    }

    // help menu for the  arguument
    if(strcmp(argv[1],"-help")==0)
    {
        printf("ERROR: No Arguments Passed\n");
        printf("Usage: ./a.out <option> <file>\n");
        printf("       <option> Menu\n");
        printf("       -v ->To view\n");
        printf("       -e ->To edit\n");
        printf("To edit, pass argument as:\n");
        printf("Usage: ./a.out -e <Tag_list> <New_data> <file>\n");
        printf("        <Tag_list> Menu\n");
        printf("         -t ->Title\n");
        printf("         -T ->Artist\n");
        printf("         -A ->Album\n");
        printf("         -Y ->Year\n");
        printf("         -c ->COmposer\n");
        printf("         -C ->Comment\n");
        return 0;
    }

    // To check it is view option or not
    else if(check_operation_type(argv[1])==is_view)
    {
        // Declaration of structure variable for reading
        Tag_Reader tag_reader;

        // Validating the file for reading 
        if(read_and_validate(argv,&tag_reader)==e_success)
        {
            printf("-------------------------------------------------------\n");
            printf("MP3 Tag Reader and Editor for ID3v2.4\n");
            printf("-------------------------------------------------------\n");
            // Displaying the details
            Display(&tag_reader);
            printf("-------------------------------------------------------\n");
        }
    }
    // To check it is edit option or not
    else if(check_operation_type(argv[1])==is_edit)
    {
        // Declaration of structur varaiable for editing
        Tag_Edit tag_edit;

        // Valiadting for Editing
        if(validate(argv,&tag_edit)==e_success)
        {
            // Performing for Edit 
            do_edit(&tag_edit);
            printf("Updated Successfully\n");
        }
    }
    // If it is neither edit nor view display the menu
    else
    {
        printf("ERROR: Invalid Arguments\n");
        printf("Usage: ./a.out <option> <file>\n");
        printf("       <option> Menu\n");
        printf("       -v ->To view\n");
        printf("       -e ->To edit\n");
        printf("To edit, pass argument as:\n");
        printf("Usage: ./a.out -e <Tag_list> <New_data> <file>\n");
        printf("        <Tag_list> Menu\n");
        printf("         -t ->Title\n");
        printf("         -T ->Artist\n");
        printf("         -A ->Album\n");
        printf("         -Y ->Year\n");
        printf("         -c ->COmposer\n");
        printf("         -C ->Comment\n");
        return 0;
    }
    return 0;
}
Operation_Type check_operation_type(char *symbol)
{
    //To check its type of argument for edit or view
    if(strcmp(symbol,"-e")==0) 
    {
        return is_edit;
    }
    else if(strcmp(symbol,"-v")==0) 
    {
        return is_view;
    }
    return is_other;
}
