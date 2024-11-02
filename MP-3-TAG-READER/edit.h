
#ifndef EDIT_H
#define EDIT_H
#include<stdio.h>
#include "type.h"      //Contains User-defined Data Types.

/*
 * Structure to store information required for storing the Updated Fram Content details
 * Information abouth the Source and Temporary MP3 files
 */

typedef struct _EditInfo
{
    /* Source MP3 file Info */
    FILE* fptr_mp3;

	/* Temporary MP3 file Info */
    FILE* fptr_temp;

	/* Updated value Info passed in the Command Line */
    char frame_Id [5];
    char frame_Id_value [100];
    uint frame_Id_size;

} EditInfo;

/* Editing MP3 details Function Prototype */

/* Perform Validation of the Arguments passed and store in the Structure */
Status read_and_validate_mp3_file_args (char* argv[], EditInfo* eInfo);

/* Perform the Editing Tag details Operation */
Status edit_tag (char* argv[], EditInfo* eInfo);

/* Copy the Frame Contents from the Source to Temporary MP3 file */
Status copy_data (EditInfo* eInfo, int flag);

/* Copy the contents after the Frames have been copied from Source to Temporary MP3 file */
Status copy_remaining_data (EditInfo* eInfo);

/* Copy the complete Data from the Temporary to Source MP3 file */
Status copy_back_to_src (EditInfo* eInfo);

void converter(char *pos);

#endif
