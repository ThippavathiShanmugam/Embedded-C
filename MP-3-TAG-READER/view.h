#ifndef VIEW_H
#define VIEW_H
#include <stdio.h>
#include "type.h"

typedef unsigned int uint;

typedef struct _ViewInfo
{
    char *src_image_fname; // store the source file name[mp3.mpeg]
    FILE *fptr_src_image;  // store the address of source file
  
    char frame_Id[4]; // store tags(TIT2,TPE1,TALB,TYER)

    /* Title Info */
    char *title_tag;
    uint title_tag_size;

    /* Artist Info */
    char *artist_tag;
    uint artist_tag_size;

    /* Album Info */
    char *album_tag;
    uint album_tag_size;

    /* Year of Release Info */
    char *year;
    uint year_size;

    /* Content Type Info */
    char *content_type;
    uint content_type_size;

    /* Comments Info */
    char *comments;
    uint comment_size;

} ViewInfo;

/* Viewing function prototype */

/* Check operation type */
OperationType check_operation_type(char *argv[]);

/* Read and validate view args from argv */
Status read_and_validate_view_args(char *argv[], ViewInfo *vInfo);

/*open files*/
Status open_file(ViewInfo *vInfo);

/*get and print data*/
Status get_and_display_data(char *str_frame, char *str_Id, char *frame_Id, uint *size, char *tag, FILE *fptr);

/*do viewing*/
Status do_view(ViewInfo *vInfo);

#endif