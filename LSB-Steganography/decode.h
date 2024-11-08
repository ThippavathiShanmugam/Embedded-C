#ifndef DECODE_H
#define DECODE_H
#include<stdio.h>
#include "types.h" // Contains user defined types

/*
 * Structure to store information required for
 * decoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)

typedef struct _DecodeInfo
{
    /* Stego image Info */
    char *d_src_image_fname;//store the source file name[stego.bmp]
    FILE *fptr_d_src_image;//store the address of the source file

    int size_secret_file;//store file size

    char *d_secret_fname;//store the secret file name[decode.txt]
    FILE *fptr_d_secret;//store the address of the secret file
} DecodeInfo;


/* Decoding Function Prototypes */

/* Read and validate decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status open_files_dec(DecodeInfo *decInfo);

/* Decode Magic String */
Status decode_magic_string(DecodeInfo *decInfo);

/* Decode data from image */
Status decode_data_from_image(int size, FILE *fptr_d_src_image,char *str);

/* Decode byte from lsb */
Status decode_byte_from_lsb(char *data, char *image_buffer);

/* Decode file extn size */ 
Status decode_file_extn_size (int size, FILE *fptr_d_src_image);

/* Decode size from lsb */
Status decode_size_from_lsb (char *buffer, int *size);

/* Decode secret file extn */
Status decode_secret_file_extn (char *file_ext, DecodeInfo *decInfo) ;

/* Decode secret file size */
Status decode_secret_file_size (int file_size, DecodeInfo *decInfo);

/* Decode secret file data */
Status decode_secret_file_data (DecodeInfo *decInfo);

#endif
