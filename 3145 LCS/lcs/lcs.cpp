#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include "omploop.hpp"

#ifdef __cplusplus
extern "C" {
#endif

  void generateLCS(char* X, int m, char* Y, int n);
  void checkLCS(char* X, int m, char* Y, int n, int result);

#ifdef __cplusplus
}
#endif

int LCS(char* X, int m, char* Y, int n) {

  int** C = new int*[m+1];
  for (int i=0; i<=m; ++i) {
    C[i] = new int[n+1];
    C[i][0] = 0;
  }
  for (int j=0; j<=n; ++j) {
    C[0][j] = 0;
  }

  for (int a=1; a<=m; ++a) {
    for (int b=1; b<=n; ++b) {
      if (X[a-1] == Y[b-1]) {
        C[a][b] = C[a-1][b-1] + 1; 
      } else {
        C[a][b] = std::max(C[a-1][b], C[a][b-1]);
      }
    }
  }

  int result = C[m][n];

  for (int i=0; i<=m; ++i) { 
    delete[] C[i];
  }
  delete[] C;
  
  return result;
}



int main (int argc, char* argv[]) {
OmpLoop om;

  if (argc < 4) { std::cerr<<"usage: "<<argv[0]<<" <m> <n> <nbthreads>"<<std::endl;
    return -1;
  }

  int m = atoi(argv[1]);
  int n = atoi(argv[2]);
  int nThreads = atoi(argv[3]);
  // get string data 
  char *X = new char[m];
  char *Y = new char[n];
  generateLCS(X, m, Y, n);

  
  //insert LCS code here.
  int result = -1; // length of common subsequence
  int final = 0;
  om.parfor<double>(0, 1, 1, nThreads,
		 [&](double& tls){
		 tls = 0;

		 },
		[&](int i, double& tls) ->void {
		tls += LCS(X, m, Y, n);
		},
		 [&](double& tls) -> void{
		   final += result;
		 }
		 );

  checkLCS(X, m, Y, n, result);


  return 0;
}
