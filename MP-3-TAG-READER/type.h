#ifndef TYPE_H
#define TYPE_H

typedef unsigned int uint;

typedef enum
{
    p_view,
    p_edit,
    p_help,
    p_unsupported
}OperationType;
 
typedef enum
{
    v_success=0,
    v_failure,
    p_success=0,
    p_failure
}Status;

#endif