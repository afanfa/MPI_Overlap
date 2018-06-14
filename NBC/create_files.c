#include <stdio.h>
#include <stdlib.h>

int main(void)
{
  FILE *fp;
  int i,j,n;
  double *x;

  n = 9000;

  x = (double *)malloc(n*n*sizeof(double));

  for(i=0;i<n;i++)
    {
      for(j=0;j<n;j++)
	{
	  x[i*n + j] = (i + j) * 0.1;
	}
    }

  fp = fopen("./matrix_1.dat","wb");

  for(i=0;i<n*n;i+=n)
    fwrite(&x[i],sizeof(double),n,fp);

  fclose(fp);

  for(i=0;i<n;i++)
    {
      for(j=0;j<n;j++)
	{
	  x[i*n + j] = (i + j) * 0.2;
	}
    }

  fp = fopen("./matrix_2.dat","wb");

  for(i=0;i<n*n;i+=n)
    fwrite(&x[i],sizeof(double),n,fp);

  fclose(fp);

  for(i=0;i<n;i++)
    {
      for(j=0;j<n;j++)
	{
	  x[i*n + j] = (i + j) * 0.3;
	}
    }

  fp = fopen("./matrix_3.dat","wb");

  for(i=0;i<n*n;i+=n)
    fwrite(&x[i],sizeof(double),n,fp);

  fclose(fp);

  for(i=0;i<n;i++)
    {
      for(j=0;j<n;j++)
	{
	  x[i*n + j] = (i + j) * 0.4;
	}
    }

  fp = fopen("./matrix_4.dat","wb");

  for(i=0;i<n*n;i+=n)
    fwrite(&x[i],sizeof(double),n,fp);

  fclose(fp);

  for(i=0;i<n;i++)
    {
      for(j=0;j<n;j++)
	{
	  x[i*n + j] = (i + j) * 0.5;
	}
    }

  fp = fopen("./matrix_5.dat","wb");

  for(i=0;i<n*n;i+=n)
    fwrite(&x[i],sizeof(double),n,fp);

  fclose(fp);

  return 0;
}
