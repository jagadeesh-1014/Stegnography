/*
Started Date  : 15/09/2025
Completed Date: 25/09/2025
Student Name  : C.Jagadeesh
Project Name  : Steganography
Description   : 1. Embedding (Hiding) Secret Data into Image
                    i. Read source image (BMP/PNG format)
                    ii. Read secret data (Text file / Message)
                    iii. Encode secret data into image pixels (LSB method)
                    iv. Save the new stego-image
                2. Extracting (Retrieving) Secret Data from Stego-Image
                    i. Read stego-image
                    ii. Extract hidden bits from pixels
                    iii. Reconstruct the secret data
                    iv. Save/Display the extracted secret
                3. Options Menu
                    i. Encode data into image
                    ii. Decode data from image
                    iii. Show image properties (width, height, size, bits/pixel)
                    iv. List supported file formats
                    v. Exit
                4. File Handling
                    i. Read/Write binary data from files
                    ii. Handle errors for invalid image or oversized secret
                5. Security
                    i. Password-based protection (optional)
                    ii. Verify integrity of extracted data
*/

#include <stdio.h>
#include "encode.h"
#include "types.h"
#include<string.h>
#include "decode.h"
void help(void);

OperationType check_operation_type(char *argv[]);

int main(int argc , char *argv[])
{
    EncodeInfo encInfo;
    DecodeInfo decodeInfo;
    EncodeInfo encInfo_local;
    memset(&encInfo_local, 0, sizeof(encInfo_local));
   
    if(argc < 2)
    {
        printf("INFO : Please pass valid arguments.\n");
        printf("INFO : Encoding -Minimum 4 arguments.\n");
        printf("INFO : Decoding -Minimum 3 arguments.\n");


    }
    
    OperationType res  = check_operation_type(&argv[1]);

    if(res == e_encode)
    {

        printf("-----------------START ENCODING---------------\n");

        if(read_and_validate_encode_args(argc, argv, &encInfo) == e_success)
        {
            printf("read sucessfully\n");
                
                // Step 3: Call do_encoding
            if (do_encoding(&encInfo) == e_success)
            {
                printf("INFO: Encoding completed successfully\n");
                printf ("----------------------------------------------\n");
            }
            else
            {
                printf("ERROR: Encoding failed\n");
                return 1;
            }

        }
        else
        {
            printf("ERROR : INVALID ARUGUMENT \n");
            help();
            
        }
    }
    else if (res == e_decode)
    {
        printf("---------------START DECODING---------------\n");

        if(read_and_validate_decode_args(argc,argv,&decodeInfo) == e_success)
        {
            printf("read sucessfully\n");
            if(do_decoding(&decodeInfo) == e_success)
            {
                 printf("INFO: Decoding completed successfully\n");
                 printf ("----------------------------------------------\n");
            }
            else{
                printf("error ");
            }
        }
        else
        {
        fprintf(stderr, "ERROR : \n"); 
        help();
        return e_failure;
        }
    }
    else
    {
        help();
    }
    
    return 0;
}
OperationType check_operation_type(char *argv[])
{
    

    if (strcasecmp(argv[0], "-e") == 0)
    {
        return e_encode;
    }
    else if (strcasecmp(argv[0], "-d") == 0)
    {
        return e_decode;
    }
    else{
        return e_unsupported;
    }
    

}
