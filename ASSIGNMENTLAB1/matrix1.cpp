#include<iostream>
#include<fstream>

class Matrix
{
  int N;
  int K;
  int M[3][3];
 
 public:
  Matrix(){
  std::ifstream fin;
  fin.open("inp.txt");
  
  fin >> N >> K;
  
        for(int i=0;i<N;i++){
             for(int j=0;j<N;j++){
                   fin >> M[i][j];
                                 }         
                         }
           }
    
         
   void matrixPrint(){
     std::cout<<"Number of rows = ";
     std::cout<<N;
     std::cout<<"\n";
     std::cout<<"Number of Threads = ";
     std::cout<<K;
     std::cout<<"\n"; 
     std::cout<<"Matrix = \n";
     for(int i=0;i<3;i++){
             for(int j=0;j<3;j++){
             std::cout<<M[i][j];
             std::cout<<" ";
                             }
             std::cout<<"\n";            
                         }
                     }
};

int main()
{ 
 Matrix A; 
 A.matrixPrint();
 return 0;
}