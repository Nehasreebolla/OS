#include <iostream>
#include <fstream>
#include <pthread.h>

class Matrix {
private:
    int N;
    int K;
    int* M;
    int* P;
    int count = 0;

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

    ~Matrix() {
        delete[] M;
        delete[] P;
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

    void printFinal() {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                std::cout << *(P + i * N + j) << " ";
            }
            std::cout << "\n";
        }
    }

    void set_count() {
        count = 0;
    }

    static void* Chunk(void* arg) {
        Matrix* B = static_cast<Matrix*>(arg);
        int K = B->getK();
        int chunkSize = (B->N / K) + 1;

        while (true) {
            int start = B->count * chunkSize;
            int end = std::min((B->count + 1) * chunkSize, B->N);
            int currentCount = B->count;
            (B->count)++;

            if (currentCount >= K)
                break;

            for (int j = start; j < end; j++) {
                B->multiply(j);
            }
        }

        return nullptr;
    }

    static void* Mixed(void* arg) {
        Matrix* A = static_cast<Matrix*>(arg);
        int K = A->getK();

        while (true) {
            int currentCount = A->count;
            (A->count)++;

            if (currentCount >= K)
                break;

            for (int j = 0; j < A->N; j += K) {
                A->multiply(currentCount + j);
            }
        }

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

    int threadArgs[K];

    for (int i = 0; i < K; i++) {
        threadArgs[i] = i;
        pthread_create(&pthreads[i], &attr, Matrix::Chunk, &A);
    }

    for (int i = 0; i < K; i++) {
        pthread_join(pthreads[i], nullptr);
    }

    A.printFinal();
    A.set_count();
    Matrix B;
    B.matrixPrint();
    for (int i = 0; i < K; i++) {
        threadArgs[i] = i;
        pthread_create(&pthreads[i], &attr, Matrix::Mixed, &B);
    }

    for (int i = 0; i < K; i++) {
        pthread_join(pthreads[i], nullptr);
    }

    A.printFinal();

    return 0;
}
