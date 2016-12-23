/* Wavelet Image Transform (HAAR)
 * "A guide to Data Compression Methods", D. Salomon ( H4 p170 )
 * all right reserved - GPL
 */

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>


#include "wlets.h"


int WIT_save_wit( FILE *fout,  float **m,int sz)
{

  for (int i=0;  i<sz/2; ++i) {
    fwrite(m[i], sz,sizeof(float), fout);
  }

  return 0;
}

int WIT_load_wit( FILE *fin,  float **m,int sz)
{
  for (int i=0;  i<sz/2; ++i) {
    fread(m[i], sz,sizeof(float), fin);
  }

  return 0;
}

//Test WIT_ image functions....
int main(int argc,char **argv)
{
  BMPHeader      bmph;
  BMPHeaderInfo  bmpi;

  printf("WIT performs standard image wavelet transform (using HAAR transform)\n");
  if (argc!=2) {
    fprintf(stderr,"use: wit <filename.raw>\n");
    return 1;
  }

  //take care of da files...
  FILE *fin_bmp  = fopen(argv[1],"rb");
  FILE *fout_bmp = fopen("oh.bmp","wb+");
  FILE *fin_cmp  = 0L;
  FILE *fout_cmp = 0L;
  if (!fin_bmp || !fout_bmp) {
    fprintf(stderr,"filename not found or couldnot open output file ('out.bmp'). Cannot proceed...\n");
    return 1;
  }


   //Do the transform
   // FIRST  Load Bitmap & build matrix of it
   // SECOND Make transform & save data in a file (xxx.wit)

   // To complete the test...

   // THIRTH Load data in a file (xxx.wit)
   // FOUR   Make reverse transform & Save back the result as a bmp
   // FIFTH  Cleanup memory & byebye


  int    sz=0; //size of matrix
  float **m;  //matrix of pixels  
  if (0!=WIT_load_bmp( fin_bmp, &sz, &m, &bmph,&bmpi ))  exit(1);

  //make transform ....
  WITcalc( m , sz );


  fout_cmp = fopen("compress.wit","wb+");
  if (fout_cmp) {
    //save as compressed/transformed data
    WIT_save_wit( fout_cmp, m ,sz );
    fclose(fout_cmp);
  } else fprintf(stderr,"Error save transformed file..\n");


  //clear matrix ... no tricks up the sleeves....
  WIT_clear(m,sz);

  //now reload the transformed data & start rebuilding the image....
  fin_cmp = fopen("compress.wit","rb");
  if (fin_cmp) {
    //save as compressed/transformed data
    WIT_load_wit( fin_cmp, m ,sz );
    fclose(fin_cmp);
  } else fprintf(stderr,"Could not load transformed file..\n");


  WITreconst( m , sz ); 

  //save file & free memory
  WIT_save_bmp( fout_bmp, m, sz, &bmph,&bmpi );
  WIT_exit( m, sz);

  fclose(fin_bmp);
  fclose(fout_bmp);

  return 0;
}

