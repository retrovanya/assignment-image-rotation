#include "bmp.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

uint32_t getPadding(const uint64_t w) {
    const size_t widthSize = w * sizeof(struct pixel);
    if (widthSize % 4 == 0) return 0;
    return 4 - (widthSize % 4);
}

enum read_status from_bmp(FILE* in, struct image* img) {
    struct bmp_header header = {0};
    const uint32_t hSize = sizeof(struct bmp_header);
    const uint32_t pSize = sizeof(struct pixel);
    if (fread(&header, hSize, 1, in) < 1) {
        if (feof(in)) return READ_INVALID_BITS;
        return READ_INVALID_HEADER;
    }
    if (fseek(in, header.bOffBits, SEEK_SET) != 0) return READ_INVALID_BITS;
    if (header.biBitCount != 24) return READ_ERROR;
    if (header.bfType != 0x4D42) return READ_ERROR;

    img->height = header.biHeight;
    img->width = header.biWidth;
    img->data = malloc(img->width * img->height * pSize);
    const uint32_t padding = getPadding(img->width);
    for (size_t j = 0; j < img->height; j++) {
        if (fread(img->data + j * img->width, pSize, img->width, in) < pSize) return READ_INVALID_BITS;
        if (fseek(in, padding, SEEK_CUR) != 0) return READ_INVALID_BITS;
    }
    if (fseek(in, 0, SEEK_SET) != 0) return READ_INVALID_BITS;
    return READ_OK;
}

enum write_status to_bmp(FILE* out, struct image const* img) {
    struct bmp_header header =
            {
                    .bfType = 0x4D42,
                    .biBitCount = 24,
                    .biHeight = img->height,
                    .biWidth = img->width,
                    .bOffBits = sizeof(struct bmp_header),
                    .bfileSize = sizeof(struct bmp_header) + (sizeof(struct pixel) * img->width + img->width % 4) * img->height,
                    .biSizeImage = img->width * img->height * sizeof(struct pixel),
                    .biSize = 40,
                    .biPlanes = 1
            };
    size_t tmp = fwrite(&header, sizeof(struct bmp_header), 1, out);
    if (tmp != 1){
        return WRITE_ERROR;
    }
    const uint32_t hSize = sizeof(struct bmp_header);
    const uint32_t pSize = sizeof(struct pixel);
    if (fwrite(&header, hSize, 1, out) < 1) return WRITE_ERROR;
    char paddingBytes[3] = {0};
    const uint32_t padding = getPadding(img->width);

    for (size_t j = 0; j < img->height; j++) {
        if (fwrite(img->data + j * img->width, pSize, img->width, out) != img->width) return WRITE_ERROR;
        if (fwrite(paddingBytes, padding, 1, out) != 1 && padding != 0) return WRITE_ERROR;
        if (fflush(out) != 0) return WRITE_ERROR;
    }
    if (fseek(out, 0, SEEK_SET) != 0) return WRITE_ERROR;
    return WRITE_OK;
}
