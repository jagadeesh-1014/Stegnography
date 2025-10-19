#include <stdio.h>
#include "decode.h"
#include "types.h"
#include<string.h>
#include "common.h"

DecodeInfo  decodeInfo;
// Reead and validate arguments
Status read_and_validate_decode_args(int argc,char *argv[], DecodeInfo *decodeInfo)
{
    if(argc < 3)
    {
        printf("ERROR : enter the valid arguments");
        return e_failure;
    }

       char *ext1 = strrchr(argv[2],'.');
       if(ext1 != NULL && strcasecmp(ext1,".bmp") == 0)
       {
        decodeInfo -> stego_image_fname = argv[2];
       }
       else{
        return e_failure;
       }

   
        if(argv[3] != NULL)
        {
            strcpy(decodeInfo->secret_fname, argv[3]);
        }
        else{
            strcpy(decodeInfo->secret_fname, "output"); 
            
            //char decode_sec_fname;
            //decodeInfo -> secret_fname = "decode_sec_fname";
        }
    

    return e_success;

}

//open files
Status open_decode_files(DecodeInfo *decodeInfo)

{
     decodeInfo -> fptr_stego_image = fopen(decodeInfo->stego_image_fname,"rb");

    if(decodeInfo -> fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file stego_image_fptr\n");
        return e_failure;
    }
    
    printf("INFO : All files open sucessfully!!!\n");
        // No failure return e_success
        return e_success;
}

Status decode_lsb_from_byte(char *data, char *ch)
{
    *ch = 0;
    for (int i = 0; i < 8; i++)
    {
        int bit = data[i] & 1;         // get LSB
        *ch |= (bit << (7 - i));       // put it back MSB-first
    }
    return e_success;
}

Status decode_size_from_lsb(char *image_buffer, int *size)
{
    *size = 0;
     int i;
    for(i=0; i < 32; i++)
    {
        int bit = image_buffer[i] & 1;
        *size = (*size << 1) | bit;
    }
    return e_success;
}

Status decode_magic_string(const char *magic_string,DecodeInfo * decodeInfo)
{
    fseek(decodeInfo -> fptr_stego_image,54,SEEK_SET);
    
    int size = strlen(MAGIC_STRING);
    int i;
    char image_buffer[8];
    char ch;
    char str[100];
    for(i = 0; i < size; i++)
    {
        fread(image_buffer, 1, 8, decodeInfo->fptr_stego_image);

           decode_lsb_from_byte(image_buffer, &ch) ;
        str[i] = ch; 
    }
    str[i] = '\0';
       printf("INFO : Decoded Magic String: %s\n", str);
    if(strcmp(str,MAGIC_STRING) == 0)
    return e_success;
    else
    return e_failure;
}

Status decode_sec_extn_size(DecodeInfo *decodeInfo)
{
    char image_buffer[32];
    int extn_size;
    fread(image_buffer,1,32, decodeInfo -> fptr_stego_image);
    decode_size_from_lsb(image_buffer, &extn_size);

    decodeInfo -> sec_file_extn_size = extn_size;
    printf("INFO : Decoded extn size is %d\n",decodeInfo -> sec_file_extn_size);
    
    return e_success;
}

Status decode_sec_extn(DecodeInfo *decodeInfo)
{
    int size = decodeInfo -> sec_file_extn_size;
    char ch;
    char ext[10];
    char image_buffer[8];
    int i;
    for (i=0;i<size;i++)
    {
        fread(image_buffer, 1, 8, decodeInfo->fptr_stego_image);
        decode_lsb_from_byte(image_buffer, &ch) ;
        ext[i] = ch;
    }
    ext[i] = '\0';


    //decodeInfo -> sec_file_extn = ext;
    
   // strcat(decodeInfo->secret_fname, ext);   
    //printf("INFO : Decoded extn is %s\n", decodeInfo->sec_file_extn);
    printf("INFO : Decoded extn is %s\n", ext);

    
    strcat(decodeInfo -> secret_fname , ext);

    decodeInfo->fptr_secret = fopen(decodeInfo->secret_fname, "wb");   
    if (decodeInfo->fptr_secret == NULL)
    {
        perror("fopen");
        return e_failure;
    }

    return e_success;
}
Status decode_sec_file_size(DecodeInfo * decodeInfo)
{

    char image_buffer[32];
    int size;
   fread(image_buffer,1,32, decodeInfo -> fptr_stego_image);
    decode_size_from_lsb(image_buffer, &size);
        
    decodeInfo -> sec_file_size = size ;
    printf("INFO : Decoded sec_file_size is %d\n",decodeInfo -> sec_file_size);
  
    return e_success;
}

Status decode_sec_file_data(DecodeInfo * decodeInfo)
{
    int size = decodeInfo -> sec_file_size;
    char ch;
    char data[100];
    char image_buffer[8];
    int i;
    for (i=0;i<size;i++)
    {
        fread(image_buffer, 1, 8, decodeInfo->fptr_stego_image);
        decode_lsb_from_byte(image_buffer, &ch) ;
        data[i] = ch;
        fwrite(&ch,1, 1, decodeInfo -> fptr_secret);
    }
    data[i] = '\0';
    printf("INFO : data is %s\n",data);
    
    return e_success;
}

/*-------------------DO DECODING----------------------*/

Status do_decoding(DecodeInfo *decodeInfo)
{
    if(open_decode_files(decodeInfo) == e_failure)
    {
        fprintf(stderr, "open_files failed\n");
        return e_failure;
    }
    else{
        printf("INFO : files opened sucessfully\n");
    }
    
    if(decode_magic_string(MAGIC_STRING,decodeInfo) == e_failure)
    {
        // print error msg
        fprintf(stderr, "decode_magic_string failed\n");
    }
    else
    {
        printf("INFO : sucessfully decoded magic string\n"); 
    }
    if(decode_sec_extn_size(decodeInfo) == e_failure)
    {
        // print error msg
        fprintf(stderr, "INFO : decode_extn_size failed\n");
    }
    else
    {
        printf("INFO : sucessfully decoded extn size\n"); 
    }

    if(decode_sec_extn(decodeInfo) == e_failure)
    {
        fprintf(stderr,"INFO : decode ext is faied\n");
    }
    else{
        printf("INFO : sucessfully decoded the extn \n");
    }

      


    if(decode_sec_file_size(decodeInfo) == e_failure)
    {
        fprintf(stderr,"INFO : decode sec file size is faied\n");
    }
    else{
        printf("INFO : sucessfully decode sec file size\n");
    }
    if(decode_sec_file_data(decodeInfo) == e_failure)
    {
        fprintf(stderr,"INFO : decode sec file data is faied\n");
    }
    else{
        printf("INFO : sucessfully decode sec file data\n");
    }

    
     
    fclose(decodeInfo -> fptr_stego_image);
    fclose(decodeInfo -> fptr_secret);
    return e_success;
}