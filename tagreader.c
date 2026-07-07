#include"header.h" // Contains user defined types

// Validating the given file 
Status read_and_validate(char **argv,Tag_Reader *tag_reader)
{
    if(argv[2]==NULL)
    {
        printf("ERROR: No Source file found\n");
        return e_failure;
    }
    // Checking source file is valid or not
    if(strstr(argv[2],".mp3"))
    {
        if(strcmp(strstr(argv[2],".mp3"),".mp3")==0)
        {
            if(argv[2][0]=='.')
            {
                printf("ERROR: Source file is invalid\n");
                return e_failure;
            }
            else
            {
                // Copying the name of source file
                tag_reader->mp3_name=argv[2];
                return 0;
            }
        }   
        else
        {
            printf("ERROR: Source file is invalid\n");
            return e_failure;
        }
    }
    else
    {
        printf("ERROR: Source file is invalid\n");
        return e_failure;
    }
    return e_success;
}

// Opening the Source file
Status open_file(Tag_Reader *tag_reader)
{
    tag_reader->fptr_mp3_name=fopen(tag_reader->mp3_name,"r");
    if(tag_reader->fptr_mp3_name==NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n",tag_reader->mp3_name);
        return e_failure;
    }
    // Checking the source file is empty or not
    char ch=fgetc(tag_reader->fptr_mp3_name);
    if(ch==EOF)
    {
        printf("File is empty\n");
        return e_failure;
    }
    rewind(tag_reader->fptr_mp3_name);
    return e_success;
}

// Displaying the details in the source file
Status Display(Tag_Reader *tag_reader)
{
    if(open_file(tag_reader)==e_failure)
    {
        return 0;
    }
    // Skipping the header files
    fseek(tag_reader->fptr_mp3_name,10,SEEK_SET);
    
    // To compare Tag present in the file
    char *tag[6]={"TIT2","TPE1","TALB","TYER","TCON","COMM"};
    char *compare[6] = {"Title", "Artist", "Album", "Year", "Music", "Comment"};
    int index=-1;
    for(int i=0;i<6;i++)
    {
        char buffer[5];
        fread(buffer, 4, 1, tag_reader->fptr_mp3_name);
        buffer[4] = '\0';
        for(int j=0;j<6;j++)
        {
            if(strcmp(buffer,tag[j])==0)
            {
                index=j;
                break;
            }
        }
        // Printing the Tag details
        printf("%-10s:",compare[index]);
        printf("\t");

        // Storing the big endian from the mp3file
        unsigned int Big=0,size=0;
        fread(&Big,4,1,tag_reader->fptr_mp3_name);
        size=Big_to_little(Big);
        fseek(tag_reader->fptr_mp3_name, 3, SEEK_CUR); 

        // create a buffer of size to store tag data
        char buffer1[size+1]; 
        fread(buffer1, size-1, 1, tag_reader->fptr_mp3_name);
        buffer1[size-1] = '\0'; 
        char *token = strtok(buffer1, "-");
        if (token != NULL) 
        {
            puts(token);
        }
        else 
        {
            puts(buffer1); 
        }
    }
    return e_success;
}

// validating the argumnet for editing
Status validate(char *argv[],Tag_Edit* tag_edit)
{
    if(argv[2]==NULL)
    {
        printf("TAG Edit option not present\n");
        return e_failure;
    }
    // checking for valid tag to edit
    else if(!(strcmp(argv[2],"-t")==0 || strcmp(argv[2],"-T")==0 ||strcmp(argv[2],"-A")==0 || strcmp(argv[2],"-Y")==0 || strcmp(argv[2],"-C")==0 ||strcmp(argv[2],"-c")==0))
    {
        printf("TAG EDIT option invalid\n");
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
        return e_failure;
    }
    tag_edit->tag=argv[2];

    if(argv[3]==NULL)
    {
        printf("No New_data\n");
        return e_failure;
    }
    tag_edit->new_data=argv[3];

    if(argv[4]==NULL)
    {
        printf("ERROR: No Source file found\n");
        return e_failure;
    }
    // checking valid source filename
    if(strstr(argv[4],".mp3"))
    {
        if(strcmp(strstr(argv[4],".mp3"),".mp3")==0)
        {
            if(argv[4][0]=='.')
            {
                printf("ERROR: Source file is invalid\n");
                return e_failure;
            }
            else
            {
                // printf("Source file checked\n");
                tag_edit->mp3_name=argv[4];
            }
        }   
        else
        {
            printf("ERROR: Source file is invalid\n");
            return e_failure;
        }
    }
    else
    {
        printf("ERROR: Source file is invalid\n");
        return e_failure;
    }
    return e_success;
}
// Opening the source file for editing
Status open(Tag_Edit *tag_edit)
{
    tag_edit->fptr_mp3_name=fopen(tag_edit->mp3_name,"r");
    if(tag_edit->fptr_mp3_name==NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n",tag_edit->mp3_name);
        return e_failure;
    }
    // Checking Source file is empty
    char ch=fgetc(tag_edit->fptr_mp3_name);
    if(ch==EOF)
    {
        printf("File is empty\n");
        return e_failure;
    }
    rewind(tag_edit->fptr_mp3_name);
    // Opening the temp file to store data.
    tag_edit->temp_name="temp.mp3";
    tag_edit->fptr_temp_name=fopen(tag_edit->temp_name,"w");
    if(tag_edit->fptr_temp_name==NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n",tag_edit->temp_name);
        return e_failure;
    }
    return e_success;
}

// Copying the header file
Status copy_header(Tag_Edit* tag_edit)
{
    char buffer[11];
    fread(buffer,10,1,tag_edit->fptr_mp3_name);
    buffer[10]='\0';
    fwrite(buffer,10,1,tag_edit->fptr_temp_name);
    return e_success;
}

// Checking the tag is equal to user given inuput tag
char * check_tag(Tag_Edit *tag_edit)
{
    if(strcmp(tag_edit->tag,"-t")==0)
    {
        return "TIT2";
    }
    else if(strcmp(tag_edit->tag,"-T")==0)
    {
        return "TPE1";
    }
    else if(strcmp(tag_edit->tag,"-A")==0)
    {
        return "TALB";
    }
    else if(strcmp(tag_edit->tag,"-Y")==0)
    {
        return "TYER";
    }
    else if(strcmp(tag_edit->tag,"-c")==0)
    {
        return "TCON";
    }
    else if(strcmp(tag_edit->tag,"-C")==0)
    {
        return "COMM";
    }
}

// Convert Big Endian to Little Endian
unsigned int Big_to_little(unsigned int Big)
{
    return ((Big & 0x000000FF) << 24) | ((Big & 0x0000FF00) << 8)  |((Big & 0x00FF0000) >> 8)  |((Big & 0xFF000000) >> 24);
}

// Convert Little Endian to Big Endian
unsigned int Little_to_big(unsigned int little)
{
    return ((little >> 24) & 0x000000FF) |((little >> 8)  & 0x0000FF00) |((little << 8)  & 0x00FF0000) |((little << 24) & 0xFF000000);
}

// Copying the data from source file to tempfile
Status copy(Tag_Edit *tag_edit)
{
    char ch=fgetc(tag_edit->fptr_mp3_name);
    while(1)
    {
        if(feof(tag_edit->fptr_mp3_name)!=0) 
        {
            break;
        }
        // copying the remaining data from source to output file
        fputc(ch,tag_edit->fptr_temp_name);
        ch=fgetc(tag_edit->fptr_mp3_name);
    }
    return e_success;
}

// Removing the source file
Status delete(char * filename)
{
    if(remove(filename)==0)
    {
        return e_success;
    }
    return e_failure;
}

// Updating the output file as source file
Status edit_name(Tag_Edit *tag_edit)
{
    if(rename(tag_edit->temp_name,tag_edit->mp3_name)==0)
    {
        return e_success;
    }
    return e_failure;
}

// Performing the editing operation
Status do_edit(Tag_Edit *tag_edit)
{
    if(open(tag_edit)==e_failure)
    {
        return e_failure;
    }
    if(copy_header(tag_edit)!=e_success)
    {
        return e_failure;
    }
    for(int i=0;i<6;i++)
    {
        char buffer[5];
        fread(buffer,4,1,tag_edit->fptr_mp3_name);
        buffer[4]='\0';
        // Changing the tag data with new data into temp file
        if(strcmp(buffer,check_tag(tag_edit))==0)
        {
            fwrite(buffer,4,1,tag_edit->fptr_temp_name);
            unsigned int size_data=strlen(tag_edit->new_data);
            // printf("%d\n",size);
            unsigned Big=Little_to_big(size_data+1);
            // printf("%d\n",Big);
            fwrite(&Big,4,1,tag_edit->fptr_temp_name);
            unsigned int size_source=0;
            fread(&size_source,4,1,tag_edit->fptr_mp3_name);
            unsigned int Little=Big_to_little(size_source);

            char buffer1[4];
            fread(buffer,3,1,tag_edit->fptr_mp3_name);
            buffer1[3]='\0';
            fwrite(buffer,3,1,tag_edit->fptr_temp_name);

            fwrite(tag_edit->new_data,size_data,1,tag_edit->fptr_temp_name);
            fseek(tag_edit->fptr_mp3_name,Little-1,SEEK_CUR);
            ftell(tag_edit->fptr_mp3_name);
            break;
        }
        // Copying the tag details into temp file
        else
        {
            fwrite(buffer,4,1,tag_edit->fptr_temp_name);

            unsigned int size=0;
            fread(&size,4,1,tag_edit->fptr_mp3_name);
            unsigned int little=Big_to_little(size);
            fwrite(&size,4,1,tag_edit->fptr_temp_name);

            char flag[4];
            fread(flag,3,1,tag_edit->fptr_mp3_name);
            flag[3]='\0';
            fwrite(flag,3,1,tag_edit->fptr_temp_name);

            char buffer1[little];
            fread(buffer1,little-1,1,tag_edit->fptr_mp3_name);
            buffer1[little-1]='\0';
            fwrite(buffer1,little-1,1,tag_edit->fptr_temp_name);
        }
    }
    // Copying the Remaining data 
    if(copy(tag_edit)!=e_success)
    {
        return e_failure;
    } 
    // Removing the source file
    if(delete(tag_edit->mp3_name)==e_failure)
    {
        return e_failure;
    }
    // Rename the temp file to source file
    if(edit_name(tag_edit)==e_failure)
    {
        return e_failure;
    }   
}