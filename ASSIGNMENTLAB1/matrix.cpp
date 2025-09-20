#include<iostream>
class Matrix
{
 private:
  int M[3][3];
 
 public:
  void matrixInit(){
      for(int i=0;i<3;i++){
             for(int j=0;j<3;j++){
                    std::cin>>M[i][j];
                             }
                          }
                   }
   void matrixPrint(){
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
 A.matrixInit();
 A.matrixPrint();
 return 0;
}