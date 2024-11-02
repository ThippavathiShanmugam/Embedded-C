#include <stdio.h>
#include "decode.h"
#include "types.h"
#include <string.h>
#include "common.h"
#include <stdlib.h>
// Function definition for read and validate decode args
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    if (!(strcmp(strstr(argv[2], "."), ".bmp")))
    {
        decInfo->d_src_image_fname = argv[2];
    }
    else
    {
        return d_failure;
    }
    if (argv[3])
    {
	    if(!(strcmp(strstr(argv[3],"."),".txt")))
	    {
		    decInfo->d_secret_fname=argv[3];
        }
	    else
	    {
		    decInfo->d_secret_fname="decode.txt";
	    }
    }

    else
    {
        decInfo->d_secret_fname = "decode.txt";
    }
    return d_success;
}

// Function definition for open files for decoding
Status open_files_dec(DecodeInfo *decInfo)
{
    decInfo->fptr_d_src_image = fopen(decInfo->d_src_image_fname, "r");

    if (decInfo->fptr_d_src_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->d_src_image_fname);
        return d_failure;
    }
   
    decInfo->fptr_d_secret = fopen(decInfo->d_secret_fname, "w");

    if (decInfo->fptr_d_secret == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->d_secret_fname);
        return d_failure;
    }
    return d_success;
}

// Function definition for decode magic string
Status decode_magic_string(DecodeInfo *decInfo)
{
    fseek(decInfo->fptr_d_src_image, 54, SEEK_SET);
    char *str= malloc(strlen(MAGIC_STRING) + 1);
    decode_data_from_image(strlen(MAGIC_STRING), decInfo->fptr_d_src_image, str);
    str[strlen(MAGIC_STRING)]='\0';
    
    if (!(strcmp(str, MAGIC_STRING)))
        return d_success;
    else
        return d_failure;
}

// Function definition for decoding data fom image
Status decode_data_from_image(int size, FILE *fptr_d_src_image, char *str)
{
    char image_buffer[8];
    for (int i = 0; i < size; i++)
    {
        fread(image_buffer, 8, sizeof(char), fptr_d_src_image);
        decode_byte_from_lsb(&str[i], image_buffer);
    }
    return d_success;
}

// Function definition for decode byte from lsb
Status decode_byte_from_lsb(char *data, char *image_buffer)
{
    unsigned char ch = 0x00;
    for (int i = 0; i < 8; i++)
    {
        ch = ((image_buffer[i] & 1) << (7-i)) | ch;
    }
    *data = ch;
}

// Function definition for decode file extn size
Status decode_file_extn_size(int size, FILE *fptr_d_src_image)
{
    char str[32];
    int length;

    fread(str, 32, sizeof(char), fptr_d_src_image);
    decode_size_from_lsb(str, &length);

    if (length == size)
        return d_success;
    else
        return d_failure;
}

// Function definition decode size from lsb
Status decode_size_from_lsb(char *buffer, int *size)
{
    int num = 0x00;
    for (int i = 0; i < 32; i++)
    {
        num = ((buffer[i] & 1) << (31-i) )| num;
    }
    *size = num;
}

// Function definition for decode secret file extn
Status decode_secret_file_extn(char *file_ext, DecodeInfo *decInfo)
{
    char *str= malloc(strlen(file_ext) + 1);
    decode_data_from_image(strlen(file_ext), decInfo->fptr_d_src_image, str);
    str[strlen(file_ext)] = '\0';

    if (!(strcmp(str, file_ext)))
        return d_success;
    else
        return d_failure;
}

// Function definition for decode secret file size
Status decode_secret_file_size(int file_size, DecodeInfo *decInfo)
{
    char str[32];
    fread(str, 32, sizeof(char), decInfo->fptr_d_src_image);
    decode_size_from_lsb(str, &file_size);
    decInfo->size_secret_file = file_size;
    return d_success;
}

// Function definition for decode secret file data
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char ch;
    for (int i = 0; i < decInfo->size_secret_file; i++)
    {
        fread (decInfo->d_src_image_fname, 8, sizeof(char), decInfo->fptr_d_src_image);
        decode_byte_from_lsb(&ch, decInfo->d_src_image_fname);
        fwrite(&ch,sizeof(char),1, decInfo->fptr_d_secret);
    }
    return d_success;
}

// Function definition for do decoding
Status do_decoding(DecodeInfo *decInfo)
{
    if (open_files_dec(decInfo) == d_success)
    {
        printf("\033[1;34mOpen files is a success\033[0m\n");
        if (decode_magic_string(decInfo) == d_success)
        {
            printf("\033[1;34mDecoded magic string Successfully\033[0m\n");
            if (decode_file_extn_size(strlen(".txt"), decInfo->fptr_d_src_image) == d_success)
            {
                printf("\033[1;34mDecoded file extension size Succesfully\033[0m\n");
                if (decode_secret_file_extn(".txt", decInfo) == d_success)
                {
                    printf("\033[1;34mDecoded Secret File Extension Succesfully\033[0m\n");
                    if (decode_secret_file_size(decInfo->size_secret_file, decInfo) == d_success)
                    {
                        printf("\033[1;34mDecoded secret file size Successfully\033[0m\n");
                        if (decode_secret_file_data(decInfo) == d_success)
                        {
                            printf("\033[1;34mDecoded secret file data Succuessfully\033[0m\n");
                        }
                        else
                        {
                            printf("\033[1;31mDecoding of secret file data is a failure\033[0m\n");
                        }
                    }
                    else
                    {
                        printf("\033[1;31mDecode of secret file size is a failure\033[0m\n");
                        return d_failure;
                    }
                }
                else
                {
                    printf("\033[1;31mDecode of Secret file extension is a failure\033[0m\n");
                    return d_failure;
                }
            }
            else
            {
                printf("\033[1;31mDecoded of file extension size is a failure\033[0m\n");
                return d_failure;
            }
        }
        else
        {
            printf("\033[1;31mDecoding of magic string is a failure\033[0m\n");
            return d_failure;
        }
    }
    else
    {
        printf("\033[1;31mOpen files is a failure\033[0m\n");
        return d_failure;
    }
    return d_success;
}
