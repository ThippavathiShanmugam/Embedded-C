#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "common.h"
#include <string.h>
/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("\033[1;33mwidth =\033[0m \033[1;35m%u\033[0m\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("\033[1;33mheight =\033[0m \033[1;35m%u\033[0m\n", height);

    // Return image capacity
    return width * height * 3;
}
/*
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
        perror("\033[1;31mfopen"); // tell why the file is not opening
        fprintf(stderr, "ERROR: Unable to open file %s\033[0m\n", encInfo->src_image_fname);

        return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
        perror("\033[1;31mfopen");
        fprintf(stderr, "ERROR: Unable to open file %s\033[0m\n", encInfo->secret_fname);

        return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
        perror("\033[1;31mfopen");
        fprintf(stderr, "ERROR: Unable to open file %s\033[0m\n", encInfo->stego_image_fname);

        return e_failure;
    }

    // No failure return e_success
    return e_success;
}
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    if (!(strcmp(strstr(argv[2], "."), ".bmp")))
    {
        encInfo->src_image_fname = argv[2];
    }
    else
    {
        printf("\033[1;31mInvalid file extention!\033[0m\n");
        return e_failure;
    }
    if (!(strcmp(strstr(argv[3], "."), ".txt")))
    {
        encInfo->secret_fname = argv[3];
    }
    else
    {
        printf("\033[1;31mInvalid file extention!\033[0m\n");
        return e_failure;
    }
    if (argv[4] != NULL)
    {
        if (!(strcmp(strstr(argv[4], "."), ".bmp")))
        {
            encInfo->stego_image_fname = argv[4];
        }
        else
        {
            encInfo->stego_image_fname = "stego.bmp";
        }
    }
    else
    {
        encInfo->stego_image_fname = "stego.bmp";
    }
    return e_success;
}

Status check_capacity(EncodeInfo *encInfo)
{
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);
    // printf("%d\n",encInfo->size_secret_file);
    // printf("%d\n",encInfo->image_capacity);
    if (encInfo->image_capacity > ((strlen(MAGIC_STRING) + 4 + 4 + 4 + encInfo->size_secret_file) * 8))
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}

long get_file_size(FILE *fptr)
{
    fseek(fptr, 0, SEEK_END); // set end posission
    return ftell(fptr);
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    char buffer[54];
    // Setting pointer to point to 0th position
    fseek(fptr_src_image, 0, SEEK_SET); // rewind(fptr_src_image);
    // Reading 54 bytes from beautiful.bmp
    fread(buffer, 54, 1, fptr_src_image);
    // Writing 54 bytes to str
    fwrite(buffer, 54, 1, fptr_dest_image);
    return e_success;
}

Status encode_magic_string(char *magic_string, EncodeInfo *encInfo)
{
    encode_data_to_image(magic_string, 2, encInfo->fptr_src_image, encInfo->fptr_stego_image, encInfo);
    return e_success;
}

Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image, EncodeInfo *encInfo)
{
    // read the 8bytes from fptr_src_image strore image_buffer
    // call byte to lsb function(data[0],image_buffer)
    // write the 8 bytes to stego bmp
    // char image_buffer[8];
    for (int i = 0; i < size; i++)
    {
        fread(encInfo->image_data, 8, 1, fptr_src_image);
        encode_byte_to_lsb(data[i], encInfo->image_data);
        fwrite(encInfo->image_data, 8, 1, fptr_stego_image);
    }
}
Status encode_byte_to_lsb(char data, char *image_buffer)
{
    // clear the image buffer in lsb bit
    // get the bit from msb in ch var
    // set the bit lsb side
    for (int i = 0; i < 8; i++)
    {
        image_buffer[i] = (image_buffer[i] & 0xFE) | ((data & (1 << (7 - i))) >> (7 - i));
    }
}

Status encode_secret_file_extn_size(int extn_size, EncodeInfo *encInfo)
{
    char image_buffer[32];
    fread(image_buffer, 32, 1, encInfo->fptr_src_image);
    encode_size_to_lsb(extn_size, image_buffer);
    fwrite(image_buffer, 32, 1, encInfo->fptr_stego_image);
    return e_success;
}

Status encode_size_to_lsb(int extn_size, char *image_buffer)
{
    for (int i = 0; i < 32; i++)
    {
        image_buffer[i] = (image_buffer[i] & 0xFE) | ((extn_size & ((1 << (31 - i)))) >> (31 - i));
    }
}

Status encode_secret_file_extn(char *file_extn, EncodeInfo *encInfo)
{
    encode_data_to_image(file_extn, strlen(file_extn), encInfo->fptr_src_image, encInfo->fptr_stego_image, encInfo);
    return e_success;
}

Status encode_secret_file_size(int file_size, EncodeInfo *encInfo)
{
    char str[32];
    fread(str, 32, 1, encInfo->fptr_src_image);
    encode_size_to_lsb(file_size, str);
    fwrite(str, 32, 1, encInfo->fptr_stego_image);
    return e_success;
}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    fseek(encInfo->fptr_secret, 0, SEEK_SET);
    char str[encInfo->size_secret_file];
    fread(str, encInfo->size_secret_file, 1, encInfo->fptr_secret);
    encode_data_to_image(str, strlen(str), encInfo->fptr_src_image, encInfo->fptr_stego_image, encInfo);
    return e_success;
}

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char ch;
    while ((fread(&ch, 1, 1, fptr_src)) > 0)
    {
        fwrite(&ch, 1, 1, fptr_dest);
    }
    return e_success;
}
Status do_encoding(EncodeInfo *encInfo)
{
    if (open_files(encInfo) == e_success)
    {
        printf("\033[1;34mFiles open successfully\033[0m\n");
        if (check_capacity(encInfo) == e_success)
        {
            printf("\033[1;34mCheck capacity is success\033[0m\n");
            if (copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
            {
                printf("\033[1;34mCopied bmp header successfully\033[0m\n");
                if (encode_magic_string(MAGIC_STRING, encInfo) == e_success)
                {
                    printf("\033[1;34mEncoded magic string successfully\033[0m\n");
                    strcpy(encInfo->extn_secret_file, strstr(encInfo->secret_fname, "."));
                    if (encode_secret_file_extn_size(strlen(encInfo->extn_secret_file), encInfo) == e_success)
                    {
                        printf("\033[1;34mEncoded secret file extn size\033[0m\n");
                        if (encode_secret_file_extn(encInfo->extn_secret_file, encInfo) == e_success)
                        {
                            printf("\033[1;34mEncoded secret file extn\033[0m\n");
                            if (encode_secret_file_size(encInfo->size_secret_file, encInfo) == e_success)
                            {
                                printf("\033[1;34mEncoded secret file size\033[0m\n");
                                if (encode_secret_file_data(encInfo) == e_success)
                                {
                                    printf("\033[1;34mEncoded secret file data\033[0m\n");
                                    if (copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
                                    {
                                        printf("\033[1;34mCopied remaining data\033[0m\n");
                                    }
                                    else
                                    {
                                        printf("\033[1;31mFailed to copy remaining data\033[0m\n");
                                        return e_failure;
                                    }
                                }
                                else
                                {
                                    printf("\033[1;31mFailed to encode secret file data\033[0m\n");
                                    return e_failure;
                                }
                            }
                            else
                            {
                                printf("\033[1;31mFailed to encode secret file size\033[0m\n");
                                return e_failure;
                            }
                        }
                        else
                        {
                            printf("\033[1;31mFailed to encode secret file extn\033[0m\n");
                            return e_failure;
                        }
                    }
                    else
                    {
                        printf("\033[1;31mFailed to encoded secret file extn size\033[0m\n");
                        return e_failure;
                    }
                }
                else
                {
                    printf("\033[1;31mFailed to encode magic string\033[0m\n");
                    return e_failure;
                }
            }
            else
            {
                printf("\033[1;31mFailed to copy bmp header\033[0m\n");
                return e_failure;
            }
        }
        else
        {
            printf("\033[1;31mCheck capacity is a failure\033[0m\n");
            return e_failure;
        }
    }
    else
    {
        printf("\033[1;31mOpen files is a failure\033[0m\n");
        return e_failure;
    }
    printf("\033[1;32mEncoding Completed Successfully\033[0m\n");
    return e_success;
}
