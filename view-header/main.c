#include <stdbool.h>
#include <stdio.h>
#include "bmp.h"
#include "util.h"
#include "rotation.h"


void usage() {
    fprintf(stderr, "Usage: ./print_header BMP_FILE_NAME\n"); 
}

int main( int argc, char** argv ) {
    if (argc != 2) usage();
    if (argc < 2) err("Not enough arguments \n" );
    if (argc > 2) err("Too many arguments \n" );

    FILE *f = fopen(argv[1], "rb");
    if (!f)
        err("Failed to open file\n");
    struct image img = {0};
    enum read_status readStatus = from_bmp(f, &img);
    if (readStatus != READ_OK){
        fclose(f);
        err( "Failed to read image\n" );
    }
    struct image rotated_img = rotate(img);
    fclose(f);
    f = fopen("rotated_picture", "wb");
    enum write_status writeStatus = to_bmp(f, &rotated_img);
    if (writeStatus != WRITE_OK) {
        fclose(f);
        err( "Failed to write image.\n");
    }
    fclose(f);
    return 0;
}
