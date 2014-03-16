#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>     // provides int8_t, uint8_t, int16_t etc.
#include <stdlib.h>
#include <time.h>

typedef struct Particle
{
    int8_t v_x, v_y, v_z;
} ;

/* Ba alocarea asta nu a mersi de kkt...am dat skip si n-am apucat sa mai fac */

struct Particle **allocMat(long w, long h)
{
    struct Particle **mat;
    struct Particle *buf;
    buf = (struct Particle *) calloc(h * w, sizeof(struct Particle));
    long i;
    mat = (struct Particle **) calloc(h, sizeof(struct Particle *));
    for (i = 0; i < h; i++)
        mat[i] = buf + i * w;
    return mat;
}


int main(int argc, char* argv[])
{

    srand(time(NULL));
    if(argc < 2)
    {
        printf("apelati cu %s <n>\n", argv[0]);
        return -1;
    }

    long n = atol(argv[1]);
    int i;

    struct Particle **vect;



    vect = allocMat(n,n);

    // TODO
    // alocati dinamic o matrice de n x n elemente de tip struct particle
    // verificati daca operatia a reusit

    // vect = (struct Particle**) calloc(n,sizeof(struct Particle*));
    // if (vect == NULL){
    //     printf("Eroare la alocare\n");
    //     exit(EXIT_FAILURE);
    // }

    // for (i = 0; i < n ; i++){
    //     vect[i] = (struct Particle*) calloc(n,sizeof(struct Particle));
    //     if (vect[i] == NULL){
    //         printf("Eroare la alocare in for\n");
    //         exit(EXIT_FAILURE);
    //     }
    // }



    // TODO
    // populati matricea alocata astfel:
    // *liniile pare contin particule cu toate componentele vitezei pozitive
    //   -> folositi modulo 128 pentru a limita rezultatului lui rand()
    // *liniile impare contin particule cu toate componentele vitezi negative
    //   -> folositi modulo 129 pentru a limita rezultatului lui rand()

    int r,j;

    for(i=0; i < n ; i++){
        for(j = 0 ; j < n ; j++){
            if(i%2 == 0){
                r = rand()%128;
                vect[i][j].v_x = r;
                r = rand()%128;
                vect[i][j].v_y = r;
                r = rand()%128;
                vect[i][j].v_z = r;
            }else{
                r = -(rand()%129);
                vect[i][j].v_x = r;
                r = -(rand()%129);
                vect[i][j].v_y = r;
                r = -(rand()%129);
                vect[i][j].v_z = r;
            }

        }
    }

    // TODO
    // scalati vitezele tuturor particulelor cu 0.5
    //   -> folositi un cast la int8_t* pentru a parcurge vitezele fara
    //      a fi nevoie sa accesati individual componentele v_x, v_y, si v_z


    // compute max particle speed
    float max_speed = 0.0f;
    for(long i = 0; i < n * n; ++i)
    {
        printf("Segfault ??\n");
        float speed = sqrt(vect[i]->v_x * vect[i]->v_x +
                           vect[i]->v_y * vect[i]->v_y +
                           vect[i]->v_z * vect[i]->v_z);
        if(max_speed < speed) max_speed = speed;
    }

    // print result
    printf("viteza maxima este: %f\n", max_speed);

    free(vect);

    return 0;
}

