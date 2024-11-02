#include <stdio.h>
#include <string.h>
#include "type.h"
#include "edit.h"

Status read_and_validate_mp3_file_args(char *argv[], EditInfo *eInfo)
{
    if (argv[4] != NULL)
    {
        if ((strcmp(strstr(argv[4], ".mp"), ".mpeg")) && (strcmp(strstr(argv[4], ".mp"), ".mp3")))
        {
            printf("\033[1;31mInvalid file format\033[0m\n");
            return p_failure;
        }
    }
    if (argv[2] == NULL)
    {
        printf("\033[1;31mINFO: For Editing the Tags -> ./mp3_tag_reader -e <modifier> \"New_Value\" <file_name.mp3>\n");
        printf("INFO: Modifier Functions:\n");
        printf("-t\tModify Title Tag\n-A\tModify Artist Tag\n-a\tModify Album Tag\n-y\tModify Year Tag\n-G\tModify Content Type Tag\n-c\tModify Comments Tag\033[0m\n");
        return p_failure;
    }
    else
    {
        if ((strncmp(argv[2], "-t", 2) == 0))
        {
            strncpy(eInfo->frame_Id, "TIT2", 5);
        }
        else if ((strncmp(argv[2], "-A", 2) == 0))
        {
            strncpy(eInfo->frame_Id, "TPE1", 5);
        }
        else if ((strncmp(argv[2], "-a", 2) == 0))
        {
            strncpy(eInfo->frame_Id, "TALB", 5);
        }
        else if ((strncmp(argv[2], "-y", 2) == 0))
        {
            strncpy(eInfo->frame_Id, "TYER", 5);
        }
        else if ((strncmp(argv[2], "-G", 2) == 0))
        {
            strncpy(eInfo->frame_Id, "TCON", 5);
        }
        else if ((strncmp(argv[2], "-c", 2) == 0))
        {
            strncpy(eInfo->frame_Id, "COMM", 5);
        }
        else
        {
            printf("\033[1;31mERROR: Unsupported Modifier.\n");
            printf("INFO: Modifier Functions:\n");
            printf("-t\tModify Title Tag\n-A\tModify Artist Tag\n-a\tModify Album Tag\n-y\tModify Year Tag\n-G\tModify Content Type Tag\n-c\tModify Comments Tag\033[0m\n");
            return p_failure;
        }
        printf("\033[1;33mFrame ID to be changed:\033[1;37m %s.\033[0m\n", eInfo->frame_Id);
    }

    if (argv[3] == NULL)
    {
        printf("\033[1;31mERROR: New_Value to be updated on the Frame ID %s is Empty.\n", eInfo->frame_Id);
        printf("INFO: For Editing the Tags -> ./mp3_tag_reader -e <modifier> \"New_Value\" <file_name.mp3>\033[0m\n");
        return p_failure;
    }
    else
    {
        eInfo->frame_Id_size = strlen(argv[3]) + 1;
        if (eInfo->frame_Id_size < 100)
        {
            printf("\033[1;33mLength of the Data in CLA = \033[1;37m%d.\033[0m\n", eInfo->frame_Id_size);
            strncpy(eInfo->frame_Id_value, argv[3], eInfo->frame_Id_size);
            printf("\033[1;33mUpdated Value of Frame ID is\033[1;37m %s.\033[0m\n", eInfo->frame_Id_value);
        }
        else
        {
            printf("\033[1;31mERROR: Length of the Data is too Long!.\033[0m\n");
            return p_failure;
        }
    }

    if (argv[4] == NULL)
    {
        printf("\033[1;31mINFO: For Editing the Tags -> ./mp3_tag_reader -e <modifier> \"New_Value\" <file_name.mp3>\033[0m\n");
        return p_failure;
    }
    else
    {
        eInfo->fptr_mp3 = fopen(argv[4], "r");
        if (eInfo->fptr_mp3 != NULL)
        {
            char str[3];
            fread(str, 1, 3, eInfo->fptr_mp3);
            if ((strncmp(str, "ID3", 3)) == 0)
            {
                printf("\033[1;34m<---------Version ID3: v2.3--------->\033[0m\n");
            }
            else
            {
                printf("\033[1;31mERROR: The file Signature is not matching with that of a '.mp3' file.\n");
                printf("INFO: For Editing the Tags -> ./mp3_tag_reader -e <modifier> \"New_Value\" <file_name.mp3>\033[0m\n");
                return p_failure;
            }
        }
        else
        {
            printf("\033[1;31mERROR: Unable to Open the %s file.\n", argv[4]);
            printf("INFO: For Editing the Tags -> ./mp3_tag_reader -e <modifier> \"New_Value\" <file_name.mp3>\033[0m\n");
            return p_failure;
        }
        fclose(eInfo->fptr_mp3);
    }

    // No p-failure returned above, then return p_success.
    return p_success;
}

Status edit_tag(char *argv[], EditInfo *eInfo)
{
    eInfo->fptr_mp3 = fopen(argv[4], "r");
    eInfo->fptr_temp = fopen("temp.mp3", "w");

    fseek(eInfo->fptr_mp3, 0L, SEEK_SET);
    fseek(eInfo->fptr_temp, 0L, SEEK_SET);

    char str[10];
    if (fread(str, 1, 10, eInfo->fptr_mp3) < 10)
    {
        printf("\033[1;31mERROR: Unable to read from the MP3 file.\033[0m\n");
        return p_failure;
    }
    else
    {
        if (fwrite(str, 1, 10, eInfo->fptr_temp) < 10)
        {
            printf("\033[1;31mERROR: Unable to write on the MP3 file.\033[0m\n");
            return p_failure;
        }
    }

    char *frame_tag[6] = {"TIT2", "TPE1", "TALB", "TYER", "TCON", "COMM"};
    for (int i = 0; i < 6; i++)
    {
        if (fread(str, 1, 4, eInfo->fptr_mp3) < 4)
        {
            printf("\033[1;31mERROR: Unable to read from the MP3 file.\033[0m\n");
            return p_failure;
        }
        else
        {
            if (fwrite(str, 1, 4, eInfo->fptr_temp) < 4)
            {
                printf("\033[1;31mERROR: Unable to write on the MP3 file.\033[0m\n");
                return p_failure;
            }
            else
            {
                if ((strncmp(eInfo->frame_Id, frame_tag[i], 4)) == 0)
                {
                    if (copy_data(eInfo, 1) == p_failure)
                    {
                        printf("\033[1;31mERROR: Unable to copy the updated Data from Source to Destination '.mp3' file.\033[0m\n");
                        return p_failure;
                    }
                }
                else
                {
                    if (copy_data(eInfo, 0) == p_failure)
                    {
                        printf("\033[1;31mERROR: Unable to copy the remaining Data from Source to Destination '.mp3' file.\033[0m\n");
                        return p_failure;
                    }
                }
            }
        }
    }

    if (copy_remaining_data(eInfo) == p_failure)
    {
        printf("\033[1;31mERROR: Unable to copy the remaining Data from Source to Destination '.mp3' file.\033[0m\n");
        return p_failure;
    }

    fclose(eInfo->fptr_mp3);
    fclose(eInfo->fptr_temp);

    eInfo->fptr_mp3 = fopen(argv[4], "w");
    eInfo->fptr_temp = fopen("temp.mp3", "r");

    if (copy_back_to_src(eInfo) == p_failure)
    {
        printf("\033[1;31mERROR: Unable to copy the remaining Data from Destination to Source '.mp3' file.\033[0m\n");
        return p_failure;
    }

    // No p-failure returned above, then return p_success.
    return p_success;
}

Status copy_data(EditInfo *eInfo, int flag)
{
    if (flag == 1)
    {
        uint old_size, new_size;
        if (fread(&old_size, 4, 1, eInfo->fptr_mp3) < 1)
        {
            printf("\033[1;31mERROR: Unable to read from the MP3 file.\033[0m\n");
            return p_failure;
        }
        else
        {
            char *pos = (char *)&old_size;
            converter(pos);
        }
        printf("\033[1;33mOld Tag Data Size =\033[1;37m %d.\033[0m\n", old_size);

        new_size = eInfo->frame_Id_size;
        char *pos = (char *)&eInfo->frame_Id_size;
        converter(pos);
        printf("\033[1;33mNew Tag Data Size =\033[1;37m %d.\033[0m\n", eInfo->frame_Id_size);

        if (fwrite(&eInfo->frame_Id_size, 4, 1, eInfo->fptr_temp) < 1)
        {
            printf("\033[1;31mERROR: Unable to write on the MP3 file.\033[0m\n");
            return p_failure;
        }

        char f[3];
        if (fread(f, 1, 3, eInfo->fptr_mp3) < 3)
        {
            printf("\033[1;31mERROR: Unable to read from the MP3 file.\033[0m\n");
            return p_failure;
        }
        else
        {
            if (fwrite(f, 1, 3, eInfo->fptr_temp) < 3)
            {
                printf("\033[1;31mERROR: Unable to write on the MP3 file.\033[0m\n");
                return p_failure;
            }
        }

        if (fwrite(eInfo->frame_Id_value, 1, (new_size - 1), eInfo->fptr_temp) < (new_size - 1))
        {
            printf("\033[1;31mERROR: Unable to write on the MP3 file.\033[0m\n");
            return p_failure;
        }
        fseek(eInfo->fptr_mp3, (long)(old_size - 1), SEEK_CUR);
        printf("\033[1;33mNew Data =\033[1;37m %s.\033[0m\n", eInfo->frame_Id_value);
        printf("\033[1;33mMP3 position =\033[1;37m %lu.\033[0m\n", ftell(eInfo->fptr_mp3));
        printf("\033[1;33mTemp position =\033[1;37m %lu.\033[0m\n", ftell(eInfo->fptr_temp));
    }
    else
    {
        uint size;
        if (fread(&size, 4, 1, eInfo->fptr_mp3) < 1)
        {
            printf("\033[1;31mERROR: Unable to read from the MP3 file.\033[0m\n");
            return p_failure;
        }
        else
        {
            if (fwrite(&size, 4, 1, eInfo->fptr_temp) < 1)
            {
                printf("\033[1;31mERROR: Unable to write on the MP3 file.\033[0m\n");
                return p_failure;
            }
        }
        char *pos = (char *)&size;
        converter(pos);
        printf("\033[1;33mSize of the Frame ID content =\033[1;37m %d.\033[0m\n", size);

        char f[3];
        if (fread(f, 1, 3, eInfo->fptr_mp3) < 3)
        {
            printf("\033[1;31mERROR: Unable to read from the MP3 file.\033[0m\n");
            return p_failure;
        }
        else
        {
            if (fwrite(f, 1, 3, eInfo->fptr_temp) < 3)
            {
                printf("\033[1;31mERROR: Unable to write on the MP3 file.\033[0m\n");
                return p_failure;
            }
        }

        char str[size - 1];
        if (fread(str, 1, (size - 1), eInfo->fptr_mp3) < (size - 1))
        {
            printf("\033[1;31mERROR: Unable to read from the MP3 file.\033[0m\n");
            return p_failure;
        }
        else
        {
            str[size - 1] = '\0';
            printf("\033[1;33mFrame ID content =\033[1;37m %s.\033[0m\n", str);
            if (fwrite(str, 1, (size - 1), eInfo->fptr_temp) < (size - 1))
            {
                printf("\033[1;31mERROR: Unable to write on the MP3 file.\033[0m\n");
                return p_failure;
            }
        }

        printf("\033[1;33mMP3 position =\033[1;37m %lu.\033[0m\n", ftell(eInfo->fptr_mp3));
        printf("\033[1;33mTemp position =\033[1;37m %lu.\033[0m\n", ftell(eInfo->fptr_temp));
    }

    // No p-failure returned above, then return p_success.
    return p_success;
}

void converter(char *pos)
{
    for (int i = 0; i < 2; i++) // Reversal of the Bytes after reading to get the correct value of the 'Size'.
    {
        pos[i] = pos[i] ^ pos[3 - i];
        pos[3 - i] = pos[i] ^ pos[3 - i];
        pos[i] = pos[i] ^ pos[3 - i];
    }
}
Status copy_remaining_data(EditInfo *eInfo)
{
    printf("\033[1;34mCopy Remaining Data:\033[0m\n");
    char ch[1024];
    long start = ftell(eInfo->fptr_mp3);
    printf("\033[1;33mMP3 position =\033[1;37m %lu.\033[0m\n", start);
    printf("\033[1;33mTemp position =\033[1;37m %lu.\033[0m\n", ftell(eInfo->fptr_temp));

    fseek(eInfo->fptr_mp3, 0L, SEEK_END);
    long end = ftell(eInfo->fptr_mp3);
    printf("\033[1;33mMP3 position =\033[1;37m %lu.\033[0m\n", end);

    fseek(eInfo->fptr_mp3, start, SEEK_SET);
    printf("\033[1;33mMP3 position =\033[1;37m %lu.\033[0m\n", ftell(eInfo->fptr_mp3));

    while ((ftell(eInfo->fptr_mp3)) != end)
    {
        if (fread(ch, 1, 1024, eInfo->fptr_mp3) < 0)
        {
            printf("\033[1;31mERROR: Unable to read from the MP3 file.\033[0m\n");
            return p_failure;
        }
        else
        {
            fwrite(ch, 1, 1024, eInfo->fptr_temp);
        }
    }

    printf("\033[1;33mMP3 position =\033[1;37m %lu.\033[0m\n", ftell(eInfo->fptr_mp3));
    printf("\033[1;33mTemp position =\033[1;37m %lu.\033[0m\n", ftell(eInfo->fptr_temp));

    // No p-failure returned above, then return p_success.
    return p_success;
}

Status copy_back_to_src(EditInfo *eInfo)
{
    printf("\033[1;34mCopy Data Back to Source:\033[0m\n");
    fseek(eInfo->fptr_mp3, 0L, SEEK_SET);
    fseek(eInfo->fptr_temp, 0L, SEEK_SET);
    char ch[1024];
    long start = ftell(eInfo->fptr_temp);
    printf("\033[1;33mTemp position =\033[1;37m %lu.\033[0m\n", start);
    printf("\033[1;33mMP3 position =\033[1;37m %lu.\033[0m\n", ftell(eInfo->fptr_mp3));

    fseek(eInfo->fptr_temp, 0L, SEEK_END);
    long end = ftell(eInfo->fptr_temp);
    printf("\033[1;33mTemp position =\033[1;37m %lu.\033[0m\n", end);

    fseek(eInfo->fptr_temp, 0L, SEEK_SET);
    printf("\033[1;33mTemp position =\033[1;37m %lu.\033[0m\n", ftell(eInfo->fptr_temp));

    while ((ftell(eInfo->fptr_temp)) != end)
    {
        if (fread(ch, 1, 1024, eInfo->fptr_temp) < 0)
        {
            printf("\033[1;31mERROR: Unable to read from the MP3 file.\033[0m\n");
            return p_failure;
        }
        else
        {
            fwrite(ch, 1, 1024, eInfo->fptr_mp3);
        }
    }

    printf("\033[1;33mMP3 position =\033[1;37m %lu.\033[0m\n", ftell(eInfo->fptr_mp3));
    printf("\033[1;33mTemp position =\033[1;37m %lu.\033[0m\n", ftell(eInfo->fptr_temp));
    // No p-failure returned above, then return p_success.
    return p_success;
}