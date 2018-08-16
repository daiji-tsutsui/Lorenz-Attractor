//
//  make_png.h
//  Lorenz Attractor
//
//  Created by 筒井 大二 on 2018/08/15.
//  Copyright © 2018年 筒井 大二. All rights reserved.
//

#include "png.h"

#ifndef make_png_h
#define make_png_h

void capture(const char filepath[])
{
    png_bytep raw1D;
    png_bytepp raw2D;
    int i;
    int width = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);
    
    // 構造体確保
    FILE *fp = fopen(filepath, "wb");
    png_structp pp = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop ip = png_create_info_struct(pp);
    // 書き込み準備
    png_init_io(pp, fp);
    png_set_IHDR(pp, ip, width, height,
                 8, // 8bit以外にするなら変える
                 PNG_COLOR_TYPE_RGBA, // RGBA以外にするなら変える
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    // ピクセル領域確保
    raw1D = (png_bytep)malloc(height * png_get_rowbytes(pp, ip));
    raw2D = (png_bytepp)malloc(height * sizeof(png_bytep));
    for (i = 0; i < height; i++)
        raw2D[i] = &raw1D[i*png_get_rowbytes(pp, ip)];
    // 画像のキャプチャ
    glReadBuffer(GL_FRONT);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // 初期値は4
    glReadPixels(0, 0, width, height,
                 GL_RGBA, // RGBA以外にするなら変える
                 GL_UNSIGNED_BYTE, // 8bit以外にするなら変える
                 (void*)raw1D);
    // 上下反転
    for (i = 0; i < height/ 2; i++){
        png_bytep swp = raw2D[i];
        raw2D[i] = raw2D[height - i - 1];
        raw2D[height - i - 1] = swp;
    }
    // 書き込み
    png_write_info(pp, ip);
    png_write_image(pp, raw2D);
    png_write_end(pp, ip);
    // 開放
    png_destroy_write_struct(&pp, &ip);
    fclose(fp);
    free(raw1D);
    free(raw2D);
    
    printf("write out screen capture to '%s'\n", filepath);
}

#endif /* make_png_h */
