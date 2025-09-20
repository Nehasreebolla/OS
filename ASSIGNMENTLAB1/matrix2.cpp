#include <iostream>
#include <fstream>
#include <pthread.h>

class Matrix {
private:
    int N;
    int K;
    int* M;
    int* P;
    int count=0;
public:
    Matrix() {
        std::ifstream fin;
        fin.open("inp.txt");

        fin >> N >> K;

        M = new int[N * N];
        P = new int[N * N];

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                fin >> *(M + i * N + j);
            }
        }
    }

    void matrixPrint() {
        std::cout << "Number of rows = " << N << "\n";
        std::cout << "Number of Threads = " << K << "\n";
        std::cout << "Matrix = \n";

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                std::cout << *(M + i * N + j) << " ";
            }
            std::cout << "\n";
        }
    }

    void multiply(int row) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                *(P + row * N + j) += (*(M + row * N + k)) * (*(M + k * N + j));
            }
        }
    }

    int getK() {
        return K;
    }
    void set_count(){
      count = 0;
    }
    void printFinal() {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                std::cout << *(P + i * N + j) << " ";
            }
            std::cout << "\n";
        }
    }

    static void* Mixed(void* arg) {
        Matrix* A = static_cast<Matrix*>(arg);
        int K = A->getK();

        for (int j = 0; j < A->N; j += K) {
            A->multiply(A->count + j);
        }
        (A->count)++;
        return nullptr;
    }

    static void* Chunk(void* arg) {
        Matrix* B = static_cast<Matrix*>(arg);
        int K = B->getK();
        int chunkSize = B->N / K;

        int start = B->count * chunkSize;
        int end = (B->count + 1) * chunkSize;

        for (int j = start; j < end && j < B->N; j++) {
            B->multiply(j);
        }
        (B->count)++;
        return nullptr;
    }
};

int main() {
    Matrix A;
    A.matrixPrint();
    int K = A.getK();

    pthread_t pthreads[K];
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    for (int i = 0; i < K; i++) {
        int* p = new int(i);
        pthread_create(&pthreads[i], &attr, Matrix::Mixed, &A);
    }

    for (int i = 0; i < K; i++) {
        pthread_join(pthreads[i], NULL);
    }

    A.printFinal();

    A.set_count();
 
    for (int i = 0; i < K; i++) {
        int* p = new int(i);
        pthread_create(&pthreads[i], &attr, Matrix::Chunk, &A);
    }

    for (int i = 0; i < K; i++) {
        pthread_join(pthreads[i], NULL);
    }

    A.printFinal();
    return 0;
}