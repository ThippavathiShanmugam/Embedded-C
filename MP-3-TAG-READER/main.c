#include <string.h>
#include "view.h"
#include "edit.h"
#include "type.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc >= 2)
    {
        int option = check_operation_type(argv);
        if (option == p_view)
        {
            ViewInfo vInfo;
            printf("\033[1;32m<--------Started View-------->\033[0m\n");
            if (argc == 3)
            {
                if (read_and_validate_view_args(argv, &vInfo) == v_success)
                {
                    // printf("\033[1;34mread and validate done\033[0m\n");
                    if (do_view(&vInfo) == v_success)
                    {
                        printf("\033[1;32m<--------Completed viewing-------->\033[0m\n");
                    }
                    else
                    {
                        printf("\033[1;31mFailed to viewing\033[0m\n");
                        return 0;
                    }
                }
                else
                {
                    printf("\033[1;31mread and validate not done\033[0m\n");
                }
            }
            else
            {
                printf("INFO: For Viewing the Tags -> ./a.out -v <file_name.mpeg>\n");
            }
        }
        else if (option == p_edit)
        {
            EditInfo eInfo;
            printf("\033[1;32m<--------Started Edit-------->\033[0m\n");
            if (argc == 5)
            {
                if (read_and_validate_mp3_file_args(argv, &eInfo) == p_success)
                {
                    //printf("\033[1;34mread and validate done\033[0m\n");
                    if (edit_tag(argv,&eInfo) == p_success)
                    {
                        printf("\033[1;32m<--------Completed editng-------->\033[0m\n");
                    }
                    else
                    {
                        printf("\033[1;31mFailed to editing\033[0m\n");
                        return 0;
                    }
                }
                else
                {
                    printf("\033[1;31mread and validate not done\033[0m\n");
                }
            }
            else
            {
                printf("\033[1;31mINFO: For Editing the Tags -> ./a.out -e <modifier> \"New_Value\" <file_name.mpeg>\033[0m\n");
            }
        }
        else if (option == p_help)
        {
            printf("\033[1;32mINFO: Help Menu for Tag Reader and Editor:\n");
            printf("INFO: For Viewing the Tags -> ./a.out -v <file_name.mpeg>\n");
            printf("INFO: For Editing the Tags -> ./a.out -e <modifier> \"New_Value\" <file_name.mpeg>\n");
            printf("INFO: Modifier Functions:\n");
            printf("-t\tModify Title Tag\n-A\tModify Artist Tag\n-a\tModify Album Tag\n-y\tModify Year Tag\n-m\tModify Content Type Tag\n-c\tModify Comments Tag\033[0m\n");
        }
        else
        {
            printf("\033[1;31mInvalid option\nKindly pass\nView: ./a.out -v mp3.mpeg\nEdit: ./a.out -e mp3.mpeg\nHelp: ./a.out --help\033[0m\n");
        }
    }
    else
    {
        printf("\033[1;31mError : Kindly pass\nView: 3 arguments\nEdit: 3 arguments\nHelp: 2 arguments\033[0m\n");
    }
    return 0;
}

OperationType check_operation_type(char *argv[])
{
    if (!(strcmp(argv[1], "-v")))
        return p_view;
    else if (!(strcmp(argv[1], "-e")))
        return p_edit;
    else if (!(strcmp(argv[1], "--help")))
        return p_help;
    else
        return p_unsupported;
}
