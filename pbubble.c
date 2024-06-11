#include <stdio.h>
#include <omp.h>
#include <stdbool.h>
#define MAX_SIZE 65800 // Adjust max size as needed

void OddEvenBubbleSort(int n, int arr[]) {
    bool sorted = false;
    int i;
    while (!sorted) {
        sorted = true;
        #pragma omp parallel for shared(arr, n) private(i)
        // Odd phase
        for (int i = 1; i <= n-2; i += 2) {
            if (arr[i] > arr[i+1]) {
                int temp = arr[i];
                arr[i] = arr[i+1];
                arr[i+1] = temp;
                sorted = false;
            }
        }
        #pragma omp parallel for shared(arr, n) private(i)
        // Even phase
        for (int i = 0; i <= n-2; i += 2) {
            if (arr[i] > arr[i+1]) {
                int temp = arr[i];
                arr[i] = arr[i+1];
                arr[i+1] = temp;
                sorted = false;
            }
        }
    }
}


int main() {
    double start, end, cpu_time_used;
    FILE* input_file = fopen("ip256.txt", "r");
    if (input_file == NULL) {
        perror("Error opening input file");
        return 1;
    }

    FILE* output_file = fopen("output.txt", "w");
    if (output_file == NULL) {
        perror("Error opening output file");
        fclose(input_file);
        return 1;
    }

    int data[MAX_SIZE];
    int size = 0;

    // Read data from the input file
    while (fscanf(input_file, "%d", &data[size]) != EOF) {
        size++;
    }

    start = omp_get_wtime(); // Record the start time

    OddEvenBubbleSort(size,data);

    end = omp_get_wtime(); // Record the end time

    for (int i = 0; i < size; i++) {
        fprintf(output_file, "%d ", data[i]);
    }

    cpu_time_used = end - start;
    printf("Elapsed Wall Clock Time: %f seconds\n", cpu_time_used);

    // Close the files
    fclose(input_file);
    fclose(output_file);

    return 0;
}
