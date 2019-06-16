#include "distribute.h"

uint32_t int_pow(uint32_t x, uint32_t y){
    // X^Y
    if( y == 0){
        return 1;
    }
    uint32_t ret = 0;
    for (int i = 0; i < y; i++) {
        ret *= x;
    }
    return ret;
}

void distribute(int k, int n){

    int secret_row, secret_col, share_row = 0, share_col = 0;

    //Open image

    //TODO: manage error
    FILE * fd = fopen("Secreto.bmp", "rb");
    BMPImage * secret = read_bmp(fd);

    // Create matrix for the algorithm

    matrix_t ** shares;
    shares = malloc(sizeof(*shares) * n);
    for (int i = 0; i < n; ++i) {
        shares[i] = NEW_MATRIX(440, 210);
        shares[i]->rows = 440;
        shares[i]->cols = 210;
    }


    matrix_t * S = NEW_MATRIX(n,n);
    S->cols = n;
    S->rows = n;

    matrix_t * A = NEW_MATRIX(n,k);
    A->cols = k;
    A->rows = n;

    matrix_t * W = NEW_MATRIX(n,n);
    W->rows = n;
    W->cols = n;

    matrix_t ** X = malloc(n * sizeof(matrix_t *));

    matrix_t ** V = malloc(n * sizeof(matrix_t *));

    matrix_t ** Sh = malloc(n * sizeof(matrix_t *));

    matrix_t ** G = malloc(n * sizeof(matrix_t *));

    for (secret_row = 0; secret_row < secret->header.height_px; secret_row += n) {
        for (secret_col = 0; secret_col < secret->header.width_px; secret_col += n) {

            // Armamos la matriz S
            for(int i=0; i < n; i++ ){
                for (int j = 0; j < n; j++) {
                    S->values[(i*S->cols) + j] = (uint32_t) secret->data[(secret_row+i*secret->header.width_px) + secret_col + j];
                }
            }

            // Armamos la matriz aleatoria A
            for(int i = 0; i < n; i++) {
                for(int j = 0; j < k; j++) {

                    unsigned char random = nextChar();

                    while(random == 255) {
                        random = nextChar();
                    }
                    A->values[i*A->cols + j] = (uint32_t) random;
                }
            }

            //Matriz dobleS
            matrix_t * Sd = projection(A);

            //Matriz R
            matrix_t * R = matrix_sub(S, Sd);


            // Calculamos vectores aleatorios Xj
            for(int z = 0; z < n; z ++) {
                X[z] = NEW_MATRIX(k,1);
                X[z]->rows = k;
                X[z]->cols = 1;
                for(int i = 0; i < k; i++) {
                    for(int j = 0; j < 1; j++) {
                        X[z]->values[i*X[z]->cols + j] = (uint32_t) nextChar();
                    }
                }
            }

            // Calculamos vectores aleatorios Vj
            for(int z = 0; z < n; z ++) {
                V[z] = matrix_mul(A, X[z]);
            }


            // Calculamos las matrices Gj
            int z = 0;
            for(int t = 0; t < n; t++) {

                G[t] = NEW_MATRIX(n,k);
                G[t]->rows = n;
                G[t]->cols = k;
                int sum = 0;
                for (int i = 0; i < R->rows; i++) {
                    for (int j = 0; j < R->cols; j += k) {
                        for (int l = 0; l < k; ++l) {
                            sum += (R->values[i * R->cols + j + l] * int_pow( (t+1), l) ) ;
                        }
                        G[t]->values[i * G[t]->cols + z] = sum % 251;
                        z++;
                    }
                    sum = 0;
                    z = 0;

                }

            }


            // HARDCODE WATERMARK
            // Esta imagen nos la tienen que dar enrealidad
            uint32_t watermark_data[] = {50,    100,    21,     14,
                                         22,    76,     200,    54,
                                         1,     91,     45,     7,
                                         24,    66,     96,     120};
            memcpy(W->values, watermark_data, W->cols*W->rows* sizeof(uint32_t));

            //Calculamos la matriz Rw
            matrix_t * Rw = matrix_sub(W, Sd);


            // Calculamos los shares
            for(int i = 0; i < n; i ++) {
                Sh[i] = merge(V[i], G[i]);
            }

            //Distribuimos los shares
            for (int l = 0; l < n; l++) {
                for(int i=0; i < n; i++){
                    for (int j = 0; j < 3; j++) {
                        shares[l]->values[((share_row + i)*shares[l]->cols) + share_col + j] = Sh[l]->values[(i*Sh[l]->cols) + j];
                    }
                }
            }
            share_col += 3;
        }
        share_col = 0;
        share_row += n;
    }

    print_matrix(shares[0]);

    //HARDCODE share file
    // Lo abro y leo la data
    fd = fopen("backtofutureshare.bmp", "rb");
    BMPImage * share = read_bmp(fd);

    // Esteganografiado
    for (int n = 0; n < shares[0]->rows*shares[0]->cols; n++) {
        lsb_2(shares[0]->values[n], share->data + n*4, 0);
    }

    // Escribo la data en el share
    fd = fopen("share1.bmp", "wb");
    write_bmp(share, fd);

    // ----------- 1 ------------------
    fd = fopen("backtofutureshare.bmp", "rb");
    BMPImage * share1 = read_bmp(fd);

    for (int n = 0; n < shares[0]->rows*shares[0]->cols; n++) {
        lsb_2(shares[0]->values[n], share->data + n*4, 0);
    }

    fd = fopen("share1.bmp", "wb");
    write_bmp(share1, fd);


    // ----------- 2 ------------------
    fd = fopen("backtofutureshare.bmp", "rb");
    BMPImage * share2 = read_bmp(fd);

    for (int n = 0; n < shares[0]->rows*shares[0]->cols; n++) {
        lsb_2(shares[0]->values[n], share->data + n*4, 0);
    }
    fd = fopen("share2.bmp", "wb");
    write_bmp(share2, fd);


    // ----------- 3 ------------------
    fd = fopen("backtofutureshare.bmp", "rb");
    BMPImage * share3 = read_bmp(fd);

    for (int n = 0; n < shares[0]->rows*shares[0]->cols; n++) {
        lsb_2(shares[0]->values[n], share->data + n*4, 0);
    }
    fd = fopen("share2.bmp", "wb");
    write_bmp(share3, fd);

    // ----------- 4 ------------------
    fd = fopen("backtofutureshare.bmp", "rb");
    BMPImage * share4 = read_bmp(fd);

    for (int n = 0; n < shares[0]->rows*shares[0]->cols; n++) {
        lsb_2(shares[0]->values[n], share->data + n*4, 0);
    }
    fd = fopen("share1.bmp", "wb");
    write_bmp(share4, fd);

    free(A);
    free(S);
    free(Sh);
    free(X);
    free(V);
    free(G);

}