/* Wavelet Image Transform (HAAR)
 * "A guide to Data Compression Methods", D. Salomon ( H4 p170 )
 * all right reserved - GPL
 */

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>


#include "wlets.h"



//Standard Image Wavelet Transform
void NWTstep( float *a, float *b , int n)
{
  int i2,n2=n>>1;
  const float SQRT2 = sqrt(2.);

  for (int i=0; i<n2; i++) {
    i2=2*i;
    b[i]    = (a[i2]+a[i2+1])/SQRT2;
    b[n2+i] = (a[i2]-a[i2+1])/SQRT2;
  }

  memmove( a,b, sizeof(float)*n );
}
void NWTcalc( float *a, float *b , int n)
{
  int i=0,j=0;

  //a:=a/SQRT(n)
  for (i=0; i<8; i++) a[i]/=sqrt( float(n));
  j=n;
  while (j>=2) {
    NWTstep(a,b,j);
    j>>=1;            //j=j/2
  }
}



void NWTRstep( float *a, float *b , int n)
{
  int i2,n2=n>>1;
  const float SQRT2 = sqrt(2.);

  for (int i=0; i<n2; i++) {
    i2=2*i;
    b[i2]   = (a[i]+a[n2+i])/SQRT2;
    b[i2+1] = (a[i]-a[n2+i])/SQRT2;
  }

  memmove( a,b, sizeof(float)*n );
}
void NWTreconst( float *a, float *b , int n)
{
  int i=0;

  for (i=2; i<=n; i*=2 ) {
    NWTRstep(a,b,i);
  }

  for (i=0; i<8; i++) a[i]*=sqrt( float(n));

}

//m is square matrix! (nxn)
int WITcalc(float **m, int n)
{
  int e=0,i=0,c,r;
  float *tmp=new float[n];
  float *col=new float[n];
  if (!tmp || !col) { e=1; goto err; }  //not enough memory!

  for ( r=0; r<n; ++r) {
    NWTcalc( m[r] , tmp , n ); 
  }
  for ( c=n-1; c>=0; --c) {
    //isolate COLUMN of m[][c]
    for (i=0; i<n; ++i)   col[i]=m[i][c];

    NWTcalc( col ,tmp, n );

    //copy back
    for (i=0; i<n; ++i)   m[i][c]=col[i];
  }

err:
  delete []tmp;
  delete []col;
  return e;
}

int WITreconst(float **m, int n)
{
  int e=0,i=0,c,r;
  float *tmp=new float[n];
  float *col=new float[n];
  rgb_t nieuw [512][512];
  if (!tmp || !col) { e=1; goto err; }  //not enough memory!

  for ( c=n-1; c>=0; --c) {
    //isolate COLUMN of m[][c]
    for (i=0; i<n; ++i)   col[i]=m[i][c];

    NWTreconst( col ,tmp, n );

    //copy back
    for (i=0; i<n; ++i)   m[i][c]=col[i];
  }
  for ( r=0; r<n; ++r) {
    NWTreconst( m[r] , tmp , n ); 
  }
err:
  delete []tmp;
  delete []col;
  return e;
}


int WIT_load_bmp( FILE *fin, int *sz, float ***m,BMPHeader *bmph,BMPHeaderInfo  *bmpi)
{
  fread( bmph, 1, sizeof(BMPHeader), fin);
  fread( bmpi, 1, sizeof(BMPHeaderInfo), fin);

  //check size (2^n)
  if (bmpi->width!=bmpi->height) {
      fprintf(stderr,"illegal file dimension (only squares)\n");
      return 2;
  }
  switch ( bmpi->width )
  {
    case (128):     *sz=128; break;
    case (256):     *sz=256; break;
    case (512):     *sz=512; break;
    default:        
      fprintf(stderr,"illegal file dimension (only 128x128,256x256 or 512x512 allowed)\n");
      return 2;
  };

  //allocate memory
  int i=0,j=0;
  *m=new float*[*sz];
  rgb_t *line=new rgb_t[*sz];
  if (!line || !m) {
      fprintf(stderr,"not enough memory!\n\n");
      return 1;
  };

  for ( i=0; fread(line,*sz,sizeof(rgb_t),fin) && i<(*sz); ++i) {
    (*m)[i]=new float[*sz];
    //if (!(*m)[i]) ...  /check memory.. not done here, since we exit(1) anyway...
    //make grayscale
    for (j=0; j<(*sz); ++j) (*m)[i][j]=(float)(line[j].r+line[j].b+line[j].g)/3.0;
  }

  delete []line;
  return 0;
}


int WIT_save_bmp( FILE *fout,  float **m,int sz,  BMPHeader *bmph,BMPHeaderInfo  *bmpi)
{

  fwrite( bmph, 1, sizeof(BMPHeader), fout);
  fwrite( bmpi, 1, sizeof(BMPHeaderInfo), fout);

  rgb_t *line=new rgb_t[sz];
  if (!line) {
      fprintf(stderr,"not enough memory!\n\n");
      return 1;
  };

  for (int i=0;  i<sz; ++i) {
    //only grayscale!
    for (int j=0; j<sz; ++j) line[j].r=line[j].g=line[j].b=(unsigned char)(m[i][j]);
    fwrite(line, sz,sizeof(rgb_t), fout);
  }

  delete []line;
  return 0;
}



int WIT_exit( float **m, int sz)
{
  for (int i=0; i<sz; ++i) delete []m[i];
  delete []m;
  return 0;
}


void WIT_clear( float **m, int sz)
{
  int i=0;
  for ( i=0; i<sz; ++i) {
    memset( m[i], 0x00, sizeof(float)*sz );
  }
}
