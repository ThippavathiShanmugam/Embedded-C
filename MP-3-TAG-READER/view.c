#include "type.h"
#include "view.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

Status read_and_validate_view_args(char *argv[], ViewInfo *vInfo)
{
    if (!strcmp(strstr(argv[2], ".mp"), ".mpeg"))
    {
        vInfo->src_image_fname = argv[2];
    }
    else if (!strcmp(strstr(argv[2], ".mp"), ".mp3"))
    {
        vInfo->src_image_fname = argv[2];
    }
    else
    {
        printf("\033[1;31mInvalid file format\033[0m\n");
        return v_failure;
    }
    return v_success;
}

Status open_file(ViewInfo *vInfo)
{
    vInfo->fptr_src_image = fopen(vInfo->src_image_fname, "r");
    if (vInfo->fptr_src_image == NULL)
    {
        perror("\033[1;31mfopen"); // tell why the file is not opening
        fprintf(stderr, "ERROR: Unable to open file %s\033[0m\n", vInfo->src_image_fname);
        return v_failure;
    }
    else
    {
        char str[3];
        fread(str, 1, 3, vInfo->fptr_src_image);
        //printf("%s\n",str);
        if (!(strncmp(str, "ID3", 3)))
        {
            printf("\033[1;34m<---------Version ID3: v2.3--------->\033[0m\n");
        }
        else
        {
            printf("\033[1;31mERROR: The file Signature is not matching with that of a '.mpeg' file.\033[0m\n");
            return v_failure;
        }
    }
    return v_success;
}

Status get_and_display_data(char *str_frame, char *str_Id, char *frame_Id, uint *size, char *tag, FILE *fptr)
{
    // printf ("MP3 position = %d.\n", ftell (fptr));
    if (fread(frame_Id, 1, 4, fptr) == 4)
    {
        // printf ("MP3 position = %d.\n", ftell (fptr));
        // printf("%s\n",frame_Id);
        if (!(strncmp(frame_Id, str_Id, 4)))
        {
            if (fread(size, 4, 1, fptr) == 1)
            {
                //printf ("MP3 position = %d.\n", ftell (fptr));
                //printf("%u\n",*size);
                char *p = (char *)size;
                for (int i = 0; i < 2; i++)
                {
                    p[i] = p[i] ^ p[3- i];
                    p[3- i] = p[i] ^ p[3- i];
                    p[i] = p[i] ^ p[3- i];
                }
                // printf("%u\n",*size);
                tag = malloc(((*size) * sizeof(char)));
                fseek(fptr, 3, SEEK_CUR);
                // printf ("MP3 position = %d.\n", ftell (fptr));
                if (fread(tag, 1, (*size - 1), fptr) == (*size - 1))
                {
                    tag[*size - 1] = '\0';
                    printf("\033[1;33m%-15s\033[1;37m%-10s\033[1;35m%s\033[0m\n", str_frame, ":", tag);
                    free(tag);
                    tag = NULL;
                }
                else
                {
                    printf("\033[1;31mERROR: Unable to read from the MP3 file.\033[0m\n");
                    return v_failure;
                }
            }
            else
            {
                printf("\033[1;31mERROR: Unable to read from the MP3 file.\033[0m\n");
                return v_failure;
            }
        }
        else
        {
            printf("\033[1;31mERROR: %s Frame ID not found.\033[0m\n", str_Id);
            return v_failure;
        }
    }
    else
    {
        printf("\033[1;31mERROR: Unable to read from the MP3 file.\033[0m\n");
        return v_failure;
    }
    return v_success;
}

Status do_view(ViewInfo *vInfo)
{
    if (open_file(vInfo) == v_success)
    {
        // printf("\033[1;32mFile opened successfully\033[0m\n");
        fseek(vInfo->fptr_src_image, 10, SEEK_SET);
        if (get_and_display_data("Title ", "TIT2", vInfo->frame_Id, &vInfo->title_tag_size, vInfo->title_tag, vInfo->fptr_src_image) == v_success)
        {
            if (get_and_display_data("Artist ", "TPE1", vInfo->frame_Id, &vInfo->artist_tag_size, vInfo->artist_tag, vInfo->fptr_src_image) == v_success)
            {
                if (get_and_display_data("Album ", "TALB", vInfo->frame_Id, &vInfo->album_tag_size, vInfo->album_tag, vInfo->fptr_src_image) == v_success)
                {
                    if (get_and_display_data("Year ", "TYER", vInfo->frame_Id, &vInfo->year_size, vInfo->year, vInfo->fptr_src_image) == v_success)
                    {
                        if (get_and_display_data("Content ", "TCON", vInfo->frame_Id, &vInfo->content_type_size, vInfo->content_type, vInfo->fptr_src_image) == v_success)
                        {
                            if (get_and_display_data("Comment ", "COMM", vInfo->frame_Id, &vInfo->comment_size, vInfo->comments, vInfo->fptr_src_image) == v_success)
                            {
                                return v_success;
                            }
                            else
                            {
                                printf("\033[1;31mERROR: Comments Frame ID Contents cannot be displayed.\033[0m\n");
                                return v_failure;
                            }
                        }
                        else
                        {
                            printf("\033[1;31mERROR: Content Frame ID Contents cannot be displayed.\033[0m\n");
                            return v_failure;
                        }
                    }
                    else
                    {
                        printf("\033[1;31mERROR: Year Frame ID Contents cannot be displayed.\033[0m\n");
                        return v_failure;
                    }
                }
                else
                {
                    printf("\033[1;31mERROR: Album Frame ID Contents cannot be displayed.\033[0m\n");
                    return v_failure;
                }
            }
            else
            {
                printf("\033[1;31mERROR: Artist Frame ID Contents cannot be displayed.\033[0m\n");
                return v_failure;
            }
        }
        else
        {
            printf("\033[1;31mERROR: Title Frame ID Contents cannot be displayed.\033[0m\n");
            return v_failure;
        }
    }
    else
    {
        printf("\033[1;31mInvalid operation\033[0m\n");
        return v_failure;
    }
}