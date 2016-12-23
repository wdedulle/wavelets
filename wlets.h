/* Wavelet Image Transform (HAAR)
 * File: wlets.h
 * "A guide to Data Compression Methods", D. Salomon ( H4 p170 )
 */

#ifndef _HAAR_H_
#define _HAAR_H_

struct rgb_t {
  unsigned char b,g,r;         //waarde RGB worden in RGB opgeslaan als BGR (??)

  rgb_t(int r_,int g_,int b_) : r(r_),g(g_),b(b_) {}
  rgb_t() : r(0),g(0),b(0) {}
};

//class Bitmap
/* http://members.fortunecity.com/shetzl/bmpffrmt.html
   HEADER
   HEADERINFO
   RGBDATA
*/


/* BMPHeader
2 bfType 19778 must always be set to 'BM' to declare that this is a .bmp-file.
4 bfSize ??     specifies the size of the file in bytes.
2 bfReserved1 0 must always be set to zero.
2 bfReserved2 0 must always be set to zero.
4 bfOffBits 0x36 specifies the offset from the beginning of the file to the bitmap data.
*/
struct BMPHeader /*STRCUTURES MUST BE PACKED*/
{
 unsigned char type[2];
 unsigned sz;
 unsigned short resv1,resv2;
 unsigned offset;
};


/*
4 biSize 40 specifies the size of the BITMAPINFOHEADER structure, in bytes.
4 biWidth ?? specifies the width of the image, in pixels.
4 biHeight ?? specifies the height of the image, in pixels.
2 biPlanes 1 specifies the number of planes of the target device, must be set to zero.
2 biBitCount 24 specifies the number of bits per pixel (2/8/16/24).
4 biCompression 0 Specifies the type of compression, usually set to zero (no compression).
4 biSizeImage 0 specifies the size of the image data, in bytes. If there is no compression, it is valid to set this member to zero.
4 biXPelsPerMeter 0 specifies the the horizontal pixels per meter on the designated targer device, usually set to zero.
4 biYPelsPerMeter 0 specifies the the vertical pixels per meter on the designated targer device, usually set to zero.
4 biClrUsed 0 specifies the number of colors used in the bitmap, if set to zero the number of colors is calculated using the biBitCount member.
4 biClrImportant 0 specifies the number of color that are 'important' for the bitmap, if set to zero, all colors are important.
*/
struct BMPHeaderInfo /*STRCUTURES MUST BE PACKED*/
{
  unsigned size;
  unsigned width;
  unsigned height;
  unsigned short planes;
  unsigned short bitcount;
  unsigned compression;
  unsigned sizecompression;
  unsigned x_pxls_p_m;
  unsigned y_pxls_p_m;
  unsigned clrused;
  unsigned clrimportant;
};




//Wavelet Image Transform  interface  functions
void WIT_clear( float **m, int sz);
int  WIT_exit( float **m, int sz);
int  WIT_save_bmp( FILE *fout,  float **m,int sz,  BMPHeader *bmph,BMPHeaderInfo  *bmpi);
int  WIT_load_bmp( FILE *fin, int *sz, float ***m,BMPHeader *bmph,BMPHeaderInfo  *bmpi);

int  WITreconst(float **m, int n);
int  WITcalc(float **m, int n);

#endif
