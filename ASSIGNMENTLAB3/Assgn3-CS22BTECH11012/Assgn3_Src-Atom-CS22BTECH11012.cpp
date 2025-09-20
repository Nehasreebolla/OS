#include <iostream>
#include <fstream>
#include <thread>
#include <atomic>
#include <algorithm> // For std::min
#include <vector>

int **matrixA;
int **resultMatrix;
int N, K, rowInc;
std::atomic<int> C(0);

int **allocateMatrix(int N) {
    int **matrix = new int*[N];
    for (int i = 0; i < N; ++i) {
        matrix[i] = new int[N]();
    }
    return matrix;
}

void deallocateMatrix(int **matrix, int N) {
    for (int i = 0; i < N; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

void multiply(int startRow, int endRow) {
    for (int i = startRow; i < endRow; ++i) {
        for (int j = 0; j < N; ++j) {
            for (int k = 0; k < N; ++k) {
                resultMatrix[i][j] += matrixA[i][k] * matrixA[k][j];
            }
        }
    }
}

void matrixPrint() {
    std::cout << "Number of rows = " << N << ", ";
    std::cout << "Number of Threads = " << K << ", ";
    std::cout << "Dynamic row = " << rowInc << "\n";
    std::cout << "Matrix A:\n";

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            std::cout << matrixA[i][j] << " ";
        }
        std::cout << "\n";
    }
}

void matrixinit(){
    std::ifstream fin("inp.txt");
    fin >> N >> K >> rowInc;
    fin.close();
    std::ofstream fout("inp.txt");
    fout << N << " " << K << " " << rowInc << "\n";
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            fout << (i + j) << " ";
        }
        fout << "\n";
    }   
}

int main() {
    matrixinit();
    std::ifstream fin("inp.txt");
    fin >> N >> K >> rowInc;

    matrixA = allocateMatrix(N);
    resultMatrix = allocateMatrix(N);

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            fin >> matrixA[i][j];
        }
    }

    fin.close();

  //  matrixPrint();
    clock_t start_time=0, end_time=0;
    double cpu_time_used;
    start_time = clock();
    std::vector<std::thread> threads;
    while (C < N) {
        threads.emplace_back([&]() {
            int start, end;
            while (true) {
                start = C.fetch_add(rowInc);
                if (start >= N) {
                    break;
                }
                end = std::min(start + rowInc, N);
                multiply(start, end);
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }
        end_time = clock();
  cpu_time_used = ((double) (end_time - start_time))/ CLOCKS_PER_SEC;

    printf("CPU time used: %f seconds\n", cpu_time_used);
    std::ofstream outfile("output.txt");

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            outfile << resultMatrix[i][j] << " ";
        }
        outfile << std::endl;
    }

    outfile.close();

    deallocateMatrix(matrixA, N);
    deallocateMatrix(resultMatrix, N);

    return 0;
}
