#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


// https://stackoverflow.com/questions/12700497/how-to-concatenate-two-integers-in-c
int concatenate(int x, int y){
    int pow = 10;
    while(y >= pow)
            pow*=10;
    return x*pow + y;
}

int main(int argc, char *argv[])
{
    int rank, size;
    MPI_Init(&argc, &argv);       MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // MPI_Wtime stuff
    double start, end;
    start = MPI_Wtime();

    const int MAX_NUMBERS = 90;
    int evenPoddVP[4];        // Array of the goooooods
    int challenger;           // New value

    if (rank!=0) {
        MPI_Recv(evenPoddVP, 4, MPI_INT, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process %d received token from process %d\n",rank, rank-1);
    }
    else {
        int evenPoddVP[4] = {1000, 1001, 0, 0};
        printf("\nProject 1\n\n");
    }

    // Generate random number and make it unique     
    srand(time(NULL)*(rank + 2));
    challenger=rand()%MAX_NUMBERS+10;
    challenger=concatenate(concatenate(1,rank),challenger);
    printf("Process %d generated %d\n", rank, challenger);

    // Compare new random value to existing high even/odd values in the array.        
    if (challenger%2==0 && challenger%100>=evenPoddVP[0]%100) {
        evenPoddVP[0]=challenger;
        evenPoddVP[1]=rank;
    }
    else if (challenger%2 != 0 && challenger%100 >= evenPoddVP[2]%100) {
        evenPoddVP[2]=challenger;
        evenPoddVP[3]=rank;
    }
    // Now send regardless of which number you are
    MPI_Send(evenPoddVP, 4, MPI_INT, (rank+1)%size, 0, MPI_COMM_WORLD);

    // Now process zero can receive from the last process
    if(rank==0) {
        MPI_Recv(evenPoddVP, 4, MPI_INT, size-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        end=MPI_Wtime();
        printf("\n President Process: %d\n President Value: %d\n\n Vice President: %d\n Vice President Value: %d\n", evenPoddVP[1], evenPoddVP[0], evenPoddVP[3], evenPoddVP[2]);
        printf(" \nTotal execution time %f \n\n", end-start);
    }

    MPI_Finalize();
    return 0;
}