#include <bits/stdc++.h>
using namespace std;

const int SIZES[] = {100, 500, 1000, 2000}; // Tamaños de matrices para pruebas

void multiplicacionClasica(int size) {
    // Reservar memoria dinámica
    int** A = new int*[size];
    int** B = new int*[size];
    int** C = new int*[size];
    for (int i = 0; i < size; ++i) {
        A[i] = new int[size]();
        B[i] = new int[size]();
        C[i] = new int[size]();
    }

    // Inicialización de matrices (puedes poner valores aquí si es necesario)
    // ...

    // Cronometrar
    auto start = chrono::high_resolution_clock::now();

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            C[i][j] = 0;
            for (int k = 0; k < size; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << "Multiplicacion Clasica (size=" << size << ") Time: " << duration << " ms" << endl;

    // Liberar memoria
    for (int i = 0; i < size; ++i) {
        delete[] A[i];
        delete[] B[i];
        delete[] C[i];
    }
    delete[] A;
    delete[] B;
    delete[] C;
}

void multiplicacionLibro1(int size) {
    // Reservar memoria dinámica
    int** A = new int*[size];
    int* x = new int[size];
    int* y = new int[size]();
    for (int i = 0; i < size; ++i) {
        A[i] = new int[size]();
        x[i] = 0;
    }

    // Cronometrar
    auto start = chrono::high_resolution_clock::now();

    for (int i = 0; i < size; i++) {
        y[i] = 0;
    }

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            y[i] += A[i][j] * x[j];
        }
    }

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << "Multiplicacion 1er par de bucles anidados (size=" << size << ") Time: " << duration << " ms" << endl;

    // Liberar memoria
    for (int i = 0; i < size; ++i) {
        delete[] A[i];
    }
    delete[] A;
    delete[] x;
    delete[] y;
}

void multiplicacionLibro2(int size) {
    // Reservar memoria dinámica
    int** A = new int*[size];
    int* x = new int[size];
    int* y = new int[size]();
    for (int i = 0; i < size; ++i) {
        A[i] = new int[size]();
        x[i] = 0;
    }

    // Cronometrar
    auto start = chrono::high_resolution_clock::now();

    for (int i = 0; i < size; i++) {
        y[i] = 0;
    }

    for (int j = 0; j < size; j++) {
        for (int i = 0; i < size; i++) {
            y[i] += A[i][j] * x[j];
        }
    }

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << "Multiplicacion 2do par de bucles anidados (size=" << size << ") Time: " << duration << " ms" << endl;

    // Liberar memoria
    for (int i = 0; i < size; ++i) {
        delete[] A[i];
    }
    delete[] A;
    delete[] x;
    delete[] y;
}

void multiplicacionPorBloques(int size) {
    // Reservar memoria dinámica
    int** A = new int*[size];
    int** B = new int*[size];
    int** C = new int*[size];
    int bloque = 2;
    for (int i = 0; i < size; ++i) {
        A[i] = new int[size]();
        B[i] = new int[size]();
        C[i] = new int[size]();
    }

    // Inicialización de matrices (puedes poner valores aquí si es necesario)
    // ...

    // Cronometrar
    auto start = chrono::high_resolution_clock::now();

    for (int i = 0; i < size; i += bloque) {
        for (int j = 0; j < size; j += bloque) {
            for (int k = 0; k < size; k += bloque) {
                for (int ii = i; ii < min(i + bloque, size); ii++) {
                    for (int jj = j; jj < min(j + bloque, size); jj++) {
                        for (int kk = k; kk < min(k + bloque, size); kk++) {
                            C[ii][jj] += A[ii][kk] * B[kk][jj];
                        }
                    }
                }
            }
        }
    }

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << "Multiplicacion Por Bloques (size=" << size << ") Time: " << duration << " ms" << endl;

    // Liberar memoria
    for (int i = 0; i < size; ++i) {
        delete[] A[i];
        delete[] B[i];
        delete[] C[i];
    }
    delete[] A;
    delete[] B;
    delete[] C;
}

int main() {
    for (int size : SIZES) {
        cout << "Tamanio de la matriz de multiplicacion: " << size << endl;
        multiplicacionLibro1(size);
        multiplicacionLibro2(size);
        multiplicacionClasica(size);
        multiplicacionPorBloques(size);
        cout << endl;
    }
    return 0;
}
