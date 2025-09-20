#include <iostream>
#include <fstream>
#include <pthread.h>
#include <time.h>

class Matrix {
private:
    int N;
    int K;
    int C;
    int BT;
    int* M;
    int* P;
    static int count;

public:
    Matrix() {
        std::ifstream fin;
        fin.open("inp.txt");

        fin >> N >> K >> C >> BT;

        M = new int[N * N];
        P = new int[N * N];

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                fin >> *(M + i * N + j);
            }
        }
        fin.close(); // Close the file after reading
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
    int getN() {
        return N;
    }
    int getC() {
        return C;
    }
    int getBT() {
        return BT;
    }

    void printFinal(std::ostream& out, double cpu_time_used) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                out << *(P + i * N + j) << " ";
            }
            out << "\n";
        }
        out << "CPU time used: " << cpu_time_used << " seconds\n";
    }

    static void* Mixed(void* arg) {
        Matrix* A = static_cast<Matrix*>(arg);
        int K = A->getK();

        int startRow;
        while ((startRow = __sync_fetch_and_add(&count, 1)) < A->N) {
            A->multiply(startRow);
        }

        return nullptr;
    }
};

int Matrix::count = 0;

int main() {
    clock_t start_time = 0, end_time = 0;
    double cpu_time_used;
    start_time = clock();
    Matrix A;
    A.matrixPrint();
    int K = A.getK();
    int N = A.getN();
    int C = A.getC();
    int BT = A.getBT();
        std::ofstream fout("inp.txt");
        fout << N << " ";  
        fout << K << " ";
        fout << C << " ";
        fout << BT << "\n";

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                fout << i + j << " ";
            }
            fout << "\n";
        }
    std::ofstream outputFile("out_B.txt");

    pthread_t pthreads[K];
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    int threadArgs[K];

    for (int i = 0; i < K; i++) {
        threadArgs[i] = i;
        pthread_create(&pthreads[i], &attr, Matrix::Mixed, &A);
    }
   
    cpu_set_t cpuset;
    int b = K / C;
   for (int j = 0, k = 0; j < C && k < BT; j++) {
    for (int i = j * b; i < (j * b) + b && k < BT; i++, k++) {
        CPU_ZERO(&cpuset);
        CPU_SET(j, &cpuset);
        pthread_setaffinity_np(pthreads[k], sizeof(cpu_set_t), &cpuset);
    }
    }

    for (int i = 0; i < K; i++) {
        pthread_join(pthreads[i], nullptr);
    }
   
   
    outputFile.close();
    end_time = clock();
    cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    printf("CPU time used: %f seconds\n", cpu_time_used);

    std::ofstream finalOutputFile("out_B.txt", std::ios::app);
    A.printFinal(finalOutputFile, cpu_time_used);
    finalOutputFile.close();

    return 0;
}
