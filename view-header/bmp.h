#ifndef _BMP_H_
#define _BMP_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#pragma pack(push, 1)
struct bmp_header {
    uint16_t bfType;
    uint32_t bfileSize;
    uint32_t bfReserved;
    uint32_t bOffBits;
    uint32_t biSize;
    uint32_t biWidth;
    uint32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    uint32_t biXPelsPerMeter;
    uint32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
};
#pragma pack(pop)

struct pixel { uint8_t b,g,r; };
struct image {
    uint64_t width, height;
    struct pixel* data;
};
enum read_status {
    READ_OK = 0,
    READ_ERROR,
    READ_INVALID_HEADER,
    READ_INVALID_BITS
};

enum write_status {
    WRITE_OK = 0,
    WRITE_ERROR
};

enum read_status from_bmp(FILE* in, struct image* pic);
enum write_status to_bmp(FILE* out, struct image const* pic);

#endif
