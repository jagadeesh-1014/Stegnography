#ifndef DECODE_H
#define DECODE_H


#include "types.h" // Contains user defined types

/* 
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

 typedef struct _DecodeInfo
{
    /* Secret File Info */
    char secret_fname[100];
    FILE *fptr_secret;
    
    long size_secret_file;
    int sec_file_extn_size;
    int sec_file_size ;
    char sec_file_extn[10];

    /* Stego Image Info */
    char *stego_image_fname;
    FILE *fptr_stego_image;

} DecodeInfo;

/* -----Decoding function prototype-------- */

/* Check operation type */
OperationType check_operation_type(char *argv[]);

/* Read and validate Encode args from argv */
Status read_and_validate_decode_args(int argc,char *argv[], DecodeInfo *decodecInfo);


/* Perform the decoding */
Status do_decoding(DecodeInfo *decodeInfo);

/* Get File pointers for i/p and o/p files */
Status open_decode_files(DecodeInfo *decodeInfo);

/*Perform the decode magic string*/
Status decode_magic_string(const char *magic_string,DecodeInfo * decodeInfo);

/*Perform decode secret file extention size*/
Status decode_sec_extn_size(DecodeInfo *decodeInfo);

/*Perform decode secrect file size */
Status decode_sec_file_size(DecodeInfo * decodeInfo);

/* Perform decode secrect file extention*/
Status decode_sec_extn(DecodeInfo *decodeInfo);

/*Perform decode size from lsb*/
Status decode_size_from_lsb(char *image_buffer, int *size);

/*Perform decode secrect file data*/
Status decode_sec_file_data(DecodeInfo * decodeInfo);

/*Perform the lsb from byte*/
Status decode_lsb_from_byte(char *data,char *ch);


#endif
