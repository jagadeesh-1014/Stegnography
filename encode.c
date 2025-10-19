#if 1
#include <stdio.h>
#include "encode.h"
#include "types.h"
#include<string.h>
#include "common.h"
void help(void);


void help()
{
   printf("TO ENCODE : pass aruguments like \n");
   printf("           :./a.out -e <file_name.bmp> <file_name.txt> [optional]\n");
   printf("TO DECODE : pass aruguments like \n");
   printf("           :./a.out -d <file_name.bmp> [optional]\n");
   
}

EncodeInfo *encInfo;
/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */

uint get_image_size_for_bmp(FILE *fptr_image)
{
    //fptr_image = encInfo->fptr_src_image;
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("INFO : width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("INFO : height = %u\n", height);

    // Return image capacity
    long int src_file_size = width * height * 3;
    printf("INFO : size = %lu\n",src_file_size);
    return src_file_size;
}

Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "rb");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);
    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "rb");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "wb");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }
     
    printf("all files open sucessfully!!!\n");
    // No failure return e_success
    return e_success;
}
Status read_and_validate_encode_args(int argc,char *argv[],EncodeInfo * encInfo)
{
     if(argc < 4)
     {
        fprintf(stderr, "ERROR: Insufficient arguments\n");
        printf("INFO : Please pass valid arguments.\n");
        printf("INFO : Encoding -Minimum 4 arguments.\n");
        printf("INFO : Decoding -Minimum 3 arguments.\n");
        return e_failure;
     }

    char *ext1 = strrchr(argv[2],'.');
    if(ext1 != NULL && strcasecmp(ext1, ".bmp") == 0)
    {
        encInfo->src_image_fname = argv[2];
    }
    else
    {
        return e_failure;
    }

    char *ext2 = strrchr(argv[3],'.');
    if(ext2 != NULL && strcasecmp(ext2, ".txt") == 0)
    {
        encInfo -> secret_fname = argv[3];
      strcpy( encInfo -> sec_file_extn ,".txt");
    }
    else if(ext2 != NULL && strcasecmp(ext2, ".csv") == 0)
    {
        encInfo -> secret_fname = argv[3];
       strcpy( encInfo -> sec_file_extn ,".csv");
    }
    else
    {
        return e_failure;
    }
    
     // Optional stego image
    if (argc > 4)
    {
        char *ext3 = strrchr(argv[4], '.');
        if (ext3 != NULL && strcasecmp(ext3, ".bmp") == 0)
        {
            encInfo->stego_image_fname = argv[4];
        }
        else
        {
            fprintf(stderr, "ERROR: Output file must be a .bmp file\n");
            return e_failure;
        }
    }
    else
    {
        encInfo->stego_image_fname = "stego.bmp";
    }
    return e_success;
}

uint get_file_size(FILE *fptr)
{
   // if (fptr == NULL) return 0;   // safety check

    fseek(fptr, 0, SEEK_END);       // move pointer to end   
    long size = ftell(fptr);  
        printf("file size 1: %ld\n",size);    
      // get file size

    rewind(fptr);           // reset pointer for reading again          
    return (uint)size; 
}

Status check_capacity(EncodeInfo *encInfo)
{
    int src_file_size = get_image_size_for_bmp(encInfo -> fptr_src_image);

    encInfo -> size_secret_file =(long) get_file_size(encInfo -> fptr_secret);

     printf("file size 2: %ld\n",encInfo->size_secret_file); 

    int required_size = 54 + (int)strlen(MAGIC_STRING)*8 + 32 + (int)strlen(encInfo -> sec_file_extn)*8+ 32+ (int) encInfo -> size_secret_file * 8;
    if(src_file_size > required_size)
    {
        return e_success;
    }
    else
    {
        e_failure;
    }
}
Status copy_bmp_header(FILE *src ,FILE *dest)
{
    char  image_buffer[54];
    rewind(src);
    fread (image_buffer,1,54,src);

    rewind(dest);
    fwrite(image_buffer,1,54,dest);
    return e_success;
}

Status encode_byte_to_lsb(char data, char *image_buffer)
{
    int i;
    for (i=0;i<8;i++)
    {
    int bit = data >>(7-i) & 1;
    //image_buffer[i] = image_buffer[i] & 0XFE;
     //image_buffer[i] = image_buffer[i] | bit;  
       image_buffer[i] = (image_buffer[i] & 0xFE) | bit;
    }
    return e_success;
}

Status encode_size_to_lsb(int data, char *image_buffer)
{
int i;
    for (i=0;i< 32;i++)
    {
     int bit = data >> (31 - i) & 1;
     //image_buffer[i] = image_buffer[i] & 0XFE;
     //image_buffer[i] | bit;
     image_buffer[i] = (image_buffer[i] & 0xFE) | bit;
    }
    return e_success;
}

Status encode_data_to_image(const char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
     char image_buffer[8];
     for(int i=0;i<size ;i++)
     {
      if(fread (image_buffer,1,8,fptr_src_image) != 8) return e_failure;
      encode_byte_to_lsb(data[i],image_buffer);
      if(fwrite(image_buffer,1,8,fptr_stego_image) != 8) return e_failure;
     }  
     return e_success;
}

Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    return encode_data_to_image(magic_string,strlen(magic_string),encInfo-> fptr_src_image,encInfo -> fptr_stego_image);
}

Status encode_secret_file_ext_size(long extrn_size,EncodeInfo * encInfo)
{
    char image_buffer[32];
      if(fread (image_buffer,1,32,encInfo -> fptr_src_image) != 32) return e_failure;
      encode_size_to_lsb(extrn_size,image_buffer);
      if(fwrite(image_buffer,1,32,encInfo -> fptr_stego_image) !=32) return e_failure;
     return e_success;

}

Status encode_secret_file_ext(const char *extn,EncodeInfo *encInfo)
{
    encode_data_to_image((char *)extn,strlen(extn), encInfo -> fptr_src_image, encInfo -> fptr_stego_image);
    return e_success;
}

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    char image_buffer[32];
  //  printf("file size %lf",file_size);
      if(fread (image_buffer,1,32,encInfo -> fptr_src_image) !=32) return e_failure;
      encode_size_to_lsb((int)file_size, image_buffer);  
      if(fwrite(image_buffer,1,32,encInfo ->fptr_stego_image) !=32) return e_failure;
     return e_success;
}
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    char data;
    rewind(encInfo -> fptr_secret);
    char image_buffer[8];
   while(fread(&data, 1, 1, encInfo->fptr_secret) == 1)
   {
    if(fread (image_buffer,1,8,encInfo ->fptr_src_image) != 8) return e_failure;
    encode_byte_to_lsb(data, image_buffer);
   if( fwrite(image_buffer,1,8, encInfo ->fptr_stego_image) !=8) return e_failure;
   }
    return e_success;
}
Status copy_remaining_img_data(FILE *src , FILE *dest)
{
    char ch_buffer[4096];
    size_t n;
    while ((n = fread(ch_buffer, 1, sizeof(ch_buffer), src)) > 0)
        {
           if( fwrite(ch_buffer, 1, n,dest) != n) return e_failure;
        }
        return e_success;
}

/*--------------------DO ENCODING--------------------*/
  
Status do_encoding(EncodeInfo *encInfo)
{
    if(open_files(encInfo) == e_failure)
    {
        fprintf(stderr, "INFO : open_files failed\n");
        return e_failure;
    }
    else{
        printf("INFO : files opened sucessfully\n");
    }
    if(check_capacity(encInfo) == e_failure)
    {
        fprintf(stderr, "INFO : check_capacity failed\n"); 
        return e_failure;
    }
    else
    {
        printf("INFO : capacity is sufficient\n");
    }

    if(copy_bmp_header(encInfo -> fptr_src_image,encInfo -> fptr_stego_image) == e_failure)
    {
        // print error msg
         fprintf(stderr, "copy_bmp_header failed\n");
        fclose(encInfo->fptr_src_image);
        fclose(encInfo->fptr_secret);
        fclose(encInfo->fptr_stego_image);
        return e_failure;
    }
    else
    {
         printf("INFO : Header copied successfully\n");
    }
    if(encode_magic_string (MAGIC_STRING,encInfo) == e_failure)
    {
        // print error msg
        fprintf(stderr, "INFO : encode_magic_string failed\n");
    }
    else
    {
        printf("INFO : sucessfully encoded magic string\n"); 
    }

    if(encode_secret_file_ext_size (strlen(encInfo -> sec_file_extn), encInfo) == e_failure)
     {
        fprintf(stderr, "INFO : encode_sec file extn size failed\n");
        return e_failure;
    }
    else
    {
        printf("INFO : sec_ext_size sucessfully encoded\n");
    }

    if(encode_secret_file_ext(encInfo -> sec_file_extn,encInfo) == e_failure)
    {
        fprintf(stderr, "INFO : encode_sec_file extn failed\n");
    }
    else
    {
        printf("INFO : sec_file_extn sucessfully encoded\n");
    }

    if(encode_secret_file_size(encInfo -> size_secret_file,encInfo) == e_failure)
     {
        fprintf(stderr, "INFO : encode_secrect file  failed\n");
    }
    else
    {
        printf("INFO : sec_file_size sucessfully encoded\n");
    }

    if(encode_secret_file_data(encInfo) == e_failure)
     {
        fprintf(stderr, "INFO : encode_secrect data failed\n");
        return e_failure;
    }
    else
    {
        printf("INFO : sec_file_data sucessfully encoded\n");
    }

    if(copy_remaining_img_data(encInfo -> fptr_src_image,encInfo -> fptr_stego_image) == e_failure)
    {
        fprintf(stderr, "INFO : encode remaing data failed\n");
    }
    else
    {
        printf("INFO : sucessfully copied remaing data \n");
    }
    // close all files 
     fclose(encInfo->fptr_src_image);
    fclose(encInfo->fptr_secret);
    fclose(encInfo->fptr_stego_image);

    return e_success; 
}
#endif 




