#include "common.h"
#include "mat.h"

#define C1 0.2
#define C2 0.3

void slow_performance1(mat* x, mat* y, mat*z) {
    double t1, t2, t3;
    for ( int i = 0; i < z->n1; i++) {
        for (int j = 0; j < z->n2 - 1; j++) {
            switch((i + j) % 2) {
                case 0:
                    t1 = (C1 * mat_get(z,i,j) + mat_get(x,i,j)) / sqrt(2.0);
                    mat_set(z,i,j,t1);
                    break;
                case 1:
                    t2 = mat_get(z,i,j) / fmax(mat_get(y,0,i), mat_get(x,0,i));
                    mat_set(z,i,j,t2);
                    break;
            }
            t3 = mat_get(z,i,j+1) + C2 * mat_get(y,0,(5 * j) % 3);
            mat_set(z,i,j+1,t3);
        }
    }
}

void slow_performance2(mat* x, mat* y, mat*z) {
    // remove sqrt, mat_get, mat_set and fmax functions 
    // (Removing Procedure Calls & function inlining)
    double t1, t2, t3;
    double sqrt_2 = sqrt(2.0);
    for ( int i = 0; i < z->n1; i++) {
        for (int j = 0; j < z->n2 - 1; j++) {
            switch((i + j) % 2) {
                case 0:
                    z->data[i * z->n2 + j] = (C1 * z->data[i * z->n2 + j] + x->data[i * x->n2 + j]) / sqrt_2;
                    break;
                case 1:
                    z->data[i * z->n2 + j] = z->data[i * z->n2 + j] / (y->data[i] > x->data[i] ? y->data[i] : x->data[i]);
                    break;
            }
            z->data[i * z->n2 + j + 1] = z->data[i * z->n2 + j + 1] + C2 * y->data[(5 * j) % 3];
        }
    }
}

void slow_performance3(mat* x, mat* y, mat*z) {
    // change switch to ?:
    double sqrt_2 = sqrt(2.0);
    for ( int i = 0; i < z->n1; i++) {
        for (int j = 0; j < z->n2 - 1; j++) {
            z->data[i * z->n2 + j] = ((i + j) % 2) ?(z->data[i * z->n2 + j] / (y->data[i] > x->data[i] ? y->data[i] : x->data[i])):((C1 * z->data[i * z->n2 + j] + x->data[i * x->n2 + j]) / sqrt_2);
            z->data[i * z->n2 + j + 1] = z->data[i * z->n2 + j + 1] + C2 * y->data[(5 * j) % 3];
        }
    }
}

void slow_performance4(mat* x, mat* y, mat*z) {
    // unrolling 2
    double sqrt_2 = sqrt(2.0), y_x_max_i, y_x_max_i_1;
    for ( int i = 0; i + 1 < z->n1; i += 2) {
        y_x_max_i = y->data[i] > x->data[i] ? y->data[i] : x->data[i];
        y_x_max_i_1 = y->data[i + 1] > x->data[i + 1] ? y->data[i + 1] : x->data[i + 1];
        //z[i, 0]
        z->data[i * z->n2] =  (C1 * z->data[i * z->n2] + x->data[i * z->n2]) / sqrt_2;  
        //z[i+1, 0]
        z->data[(i + 1) * z->n2] = z->data[(i + 1) * z->n2] / y_x_max_i_1;
        int j;                  
        for (j = 1; j + 1 < z->n2 - 1; j += 2) {
            //z[i, j]       odd
            z->data[i * z->n2 + j ] = z->data[i * z->n2 + j] + C2 * y->data[(5 * j - 5) % 3];
            z->data[i * z->n2 + j] = z->data[i * z->n2 + j] / y_x_max_i;
            //z[i, j + 1]   even    
            z->data[i * z->n2 + (j + 1)] = z->data[i * z->n2 + (j + 1)] + C2 * y->data[(5 * j) % 3];              
            z->data[i * z->n2 + (j + 1)] = (C1 * z->data[i * z->n2 + (j + 1)] + x->data[i * z->n2 + (j + 1)]) / sqrt_2;
            //z[i + 1, j]           even
            z->data[(i + 1) * z->n2 + j ] = z->data[(i + 1) * z->n2 + j] + C2 * y->data[(5 * j - 5) % 3];
            z->data[(i + 1) * z->n2 + j] = (C1 * z->data[(i + 1) * z->n2 + j] + x->data[(i + 1) * z->n2 + j]) / sqrt_2;
            //z[(i + 1), j + 1]     odd    
            z->data[(i + 1) * z->n2 + (j + 1)] = z->data[(i + 1) * z->n2 + (j + 1)] + C2 * y->data[(5 * j) % 3];              
            z->data[(i + 1) * z->n2 + (j + 1)] = z->data[(i + 1) * z->n2 + (j + 1)] / y_x_max_i_1;
            
        }
                    //z[i, j]       odd
            z->data[i * z->n2 + j ] = z->data[i * z->n2 + j] + C2 * y->data[(5 * j - 5) % 3];
            z->data[i * z->n2 + j] = z->data[i * z->n2 + j] / y_x_max_i;
            //z[i, j + 1]   even    
            z->data[i * z->n2 + (j + 1)] = z->data[i * z->n2 + (j + 1)] + C2 * y->data[(5 * j) % 3];              
            //z[i + 1, j]           even
            z->data[(i + 1) * z->n2 + j ] = z->data[(i + 1) * z->n2 + j] + C2 * y->data[(5 * j - 5) % 3];
            z->data[(i + 1) * z->n2 + j] = (C1 * z->data[(i + 1) * z->n2 + j] + x->data[(i + 1) * z->n2 + j]) / sqrt_2;
            //z[(i + 1), j + 1]     odd    
            z->data[(i + 1) * z->n2 + (j + 1)] = z->data[(i + 1) * z->n2 + (j + 1)] + C2 * y->data[(5 * j) % 3];                        
    
    }
}

void slow_performance5(mat* x, mat* y, mat*z) {
    // unrolling 6
    double sqrt_2 = sqrt(2.0), y_x_max_i, y_x_max_i_1;
    for ( int i = 0; i + 1 < z->n1; i += 2) {
        y_x_max_i = y->data[i] > x->data[i] ? y->data[i] : x->data[i];
        y_x_max_i_1 = y->data[i + 1] > x->data[i + 1] ? y->data[i + 1] : x->data[i + 1];
        //z[i, 0]
        z->data[i * z->n2] =  (C1 * z->data[i * z->n2] + x->data[i * z->n2]) / sqrt_2;  
        //z[i+1, 0]
        z->data[(i + 1) * z->n2] = z->data[(i + 1) * z->n2] / y_x_max_i_1;
        int j;                  
        for (j = 1; j + 5 < z->n2 - 1; j += 6) {
            //z[i, j]       odd
            z->data[i * z->n2 + j ] = z->data[i * z->n2 + j] + C2 * y->data[0];
            z->data[i * z->n2 + j] = z->data[i * z->n2 + j] / y_x_max_i;
            //z[i, j + 1]   even    
            z->data[i * z->n2 + (j + 1)] = z->data[i * z->n2 + (j + 1)] + C2 * y->data[2];              
            z->data[i * z->n2 + (j + 1)] = (C1 * z->data[i * z->n2 + (j + 1)] + x->data[i * z->n2 + (j + 1)]) / sqrt_2;
            //z[i, j + 2]   odd                  
            z->data[i * z->n2 + (j + 2)] = z->data[i * z->n2 + (j + 2)] + C2 * y->data[1];              
            z->data[i * z->n2 + (j + 2)] = z->data[i * z->n2 + (j + 2)] / y_x_max_i;
            //z[i, j + 3]   even
            z->data[i * z->n2 + (j + 3)] = z->data[i * z->n2 + (j + 3)] + C2 * y->data[0];              
            z->data[i * z->n2 + (j + 3)] = (C1 * z->data[i * z->n2 + (j + 3)] + x->data[i * z->n2 + (j + 3)]) / sqrt_2;
            //z[i, j + 4]   odd    
            z->data[i * z->n2 + (j + 4)] = z->data[i * z->n2 + (j + 4)] + C2 * y->data[2];              
            z->data[i * z->n2 + (j + 4)] = z->data[i * z->n2 + (j + 4)] / y_x_max_i;
            //z[i, j + 5]   even                  
            z->data[i * z->n2 + (j + 5)] = z->data[i * z->n2 + (j + 5)] + C2 * y->data[1];              
            z->data[i * z->n2 + (j + 5)] = (C1 * z->data[i * z->n2 + (j + 5)] + x->data[i * z->n2 + (j + 5)]) / sqrt_2;
            //z[i + 1, j]           even
            z->data[(i + 1) * z->n2 + j ] = z->data[(i + 1) * z->n2 + j] + C2 * y->data[0];
            z->data[(i + 1) * z->n2 + j] = (C1 * z->data[(i + 1) * z->n2 + j] + x->data[(i + 1) * z->n2 + j]) / sqrt_2;
            //z[(i + 1), j + 1]     odd    
            z->data[(i + 1) * z->n2 + (j + 1)] = z->data[(i + 1) * z->n2 + (j + 1)] + C2 * y->data[2];              
            z->data[(i + 1) * z->n2 + (j + 1)] = z->data[(i + 1) * z->n2 + (j + 1)] / y_x_max_i_1;
            //z[(i + 1), j + 2]     even                  
            z->data[(i + 1) * z->n2 + (j + 2)] = z->data[(i + 1) * z->n2 + (j + 2)] + C2 * y->data[1];              
            z->data[(i + 1) * z->n2 + (j + 2)] = (C1 * z->data[(i + 1) * z->n2 + (j + 2)] + x->data[(i + 1) * z->n2 + (j + 2)]) / sqrt_2;
            //z[(i + 1), j + 3]     odd
            z->data[(i + 1) * z->n2 + (j + 3)] = z->data[(i + 1) * z->n2 + (j + 3)] + C2 * y->data[0];              
            z->data[(i + 1) * z->n2 + (j + 3)] = z->data[(i + 1) * z->n2 + (j + 3)] / y_x_max_i_1;
            //z[(i + 1), j + 4]     even    
            z->data[(i + 1) * z->n2 + (j + 4)] = z->data[(i + 1) * z->n2 + (j + 4)] + C2 * y->data[2];              
            z->data[(i + 1) * z->n2 + (j + 4)] = (C1 * z->data[(i + 1) * z->n2 + (j + 4)] + x->data[(i + 1) * z->n2 + (j + 4)]) / sqrt_2;
            //z[i + 1, j + 5]   odd                  
            z->data[(i + 1) * z->n2 + (j + 5)] = z->data[(i + 1) * z->n2 + (j + 5)] + C2 * y->data[1];              
            z->data[(i + 1) * z->n2 + (j + 5)] = z->data[(i + 1) * z->n2 + (j + 5)] / y_x_max_i_1;
            
        }
        //z[i, j]       odd
        z->data[i * z->n2 + j ] = z->data[i * z->n2 + j] + C2 * y->data[0];
        z->data[i * z->n2 + j] = z->data[i * z->n2 + j] / y_x_max_i;
        //z[i, j + 1]   even    
        z->data[i * z->n2 + (j + 1)] = z->data[i * z->n2 + (j + 1)] + C2 * y->data[2];              
        z->data[i * z->n2 + (j + 1)] = (C1 * z->data[i * z->n2 + (j + 1)] + x->data[i * z->n2 + (j + 1)]) / sqrt_2;
        //z[i, j + 2]   odd                  
        z->data[i * z->n2 + (j + 2)] = z->data[i * z->n2 + (j + 2)] + C2 * y->data[1];              
        z->data[i * z->n2 + (j + 2)] = z->data[i * z->n2 + (j + 2)] / y_x_max_i;
        //z[i, j + 3]   even
        z->data[i * z->n2 + (j + 3)] = z->data[i * z->n2 + (j + 3)] + C2 * y->data[0];              
        z->data[i * z->n2 + (j + 3)] = (C1 * z->data[i * z->n2 + (j + 3)] + x->data[i * z->n2 + (j + 3)]) / sqrt_2;
        //z[i, j + 4]   odd    
        z->data[i * z->n2 + (j + 4)] = z->data[i * z->n2 + (j + 4)] + C2 * y->data[2];              
        z->data[i * z->n2 + (j + 4)] = z->data[i * z->n2 + (j + 4)] / y_x_max_i;
        //z[i, j + 5]   even                  
        z->data[i * z->n2 + (j + 5)] = z->data[i * z->n2 + (j + 5)] + C2 * y->data[1];              
        //z[i + 1, j]           even
        z->data[(i + 1) * z->n2 + j ] = z->data[(i + 1) * z->n2 + j] + C2 * y->data[0];
        z->data[(i + 1) * z->n2 + j] = (C1 * z->data[(i + 1) * z->n2 + j] + x->data[(i + 1) * z->n2 + j]) / sqrt_2;
        //z[(i + 1), j + 1]     odd    
        z->data[(i + 1) * z->n2 + (j + 1)] = z->data[(i + 1) * z->n2 + (j + 1)] + C2 * y->data[2];              
        z->data[(i + 1) * z->n2 + (j + 1)] = z->data[(i + 1) * z->n2 + (j + 1)] / y_x_max_i_1;
        //z[(i + 1), j + 2]     even                  
        z->data[(i + 1) * z->n2 + (j + 2)] = z->data[(i + 1) * z->n2 + (j + 2)] + C2 * y->data[1];              
        z->data[(i + 1) * z->n2 + (j + 2)] = (C1 * z->data[(i + 1) * z->n2 + (j + 2)] + x->data[(i + 1) * z->n2 + (j + 2)]) / sqrt_2;
        //z[(i + 1), j + 3]     odd
        z->data[(i + 1) * z->n2 + (j + 3)] = z->data[(i + 1) * z->n2 + (j + 3)] + C2 * y->data[0];              
        z->data[(i + 1) * z->n2 + (j + 3)] = z->data[(i + 1) * z->n2 + (j + 3)] / y_x_max_i_1;
        //z[(i + 1), j + 4]     even    
        z->data[(i + 1) * z->n2 + (j + 4)] = z->data[(i + 1) * z->n2 + (j + 4)] + C2 * y->data[2];              
        z->data[(i + 1) * z->n2 + (j + 4)] = (C1 * z->data[(i + 1) * z->n2 + (j + 4)] + x->data[(i + 1) * z->n2 + (j + 4)]) / sqrt_2;
        //z[i + 1, j + 5]   odd                  
        z->data[(i + 1) * z->n2 + (j + 5)] = z->data[(i + 1) * z->n2 + (j + 5)] + C2 * y->data[1];                          

    }
}

void slow_performance6(mat* x, mat* y, mat*z) {
    // scalar replacement
    int index_ij, index_i1_0, z_n1 = z->n1, z_n2 = z->n2, two_z_n2 = 2*z->n2;
    double sqrt_2 = sqrt(2.0);
    double y_x_max_i, y_x_max_i_1;
    double c2y0 = C2 * y->data[0], c2y2 = C2 * y->data[2], c2y1 = C2 * y->data[1]; 
    double z_i_j, z_i1_j, z_i_j1, z_i1_j1, z_i_j2, z_i1_j2, z_i_j3, z_i1_j3, z_i_j4, z_i1_j4, z_i_j5, z_i1_j5;
    for ( int i = 0, index_i_0 = 0, index_i1_0 = z_n2; i + 1 < z_n1; i += 2, index_i_0 += two_z_n2, index_i1_0 += two_z_n2) {
        y_x_max_i = y->data[i] > x->data[i] ? y->data[i] : x->data[i];
        y_x_max_i_1 = y->data[i + 1] > x->data[i + 1] ? y->data[i + 1] : x->data[i + 1];

        //z[i, 0]
        z_i_j = z->data[index_i_0];
        z_i_j = C1 * z_i_j;
        z_i_j = z_i_j + x->data[index_i_0];
        z_i_j = z_i_j / sqrt_2;
        z->data[index_i_0] = z_i_j;

        //z[i+1, 0]
        z_i1_j = z->data[index_i1_0];
        z_i1_j = z_i1_j/y_x_max_i_1;
        z->data[index_i1_0] = z_i1_j;
        
        int j, index_i_j, index_i1_j;                  
        for (j = 1, index_i_j = index_i_0 + 1, index_i1_j = index_i1_0 + 1; j + 5 < z_n2 - 1; j += 6, index_i_j += 6, index_i1_j += 6) {
            z_i_j = z->data[index_i_j];
            z_i_j1 = z->data[index_i_j + 1];
            z_i_j2 = z->data[index_i_j + 2];
            z_i_j3 = z->data[index_i_j + 3];
            z_i_j4 = z->data[index_i_j + 4];
            z_i_j5 = z->data[index_i_j + 5];
            z_i1_j = z->data[index_i1_j];
            z_i1_j1 = z->data[index_i1_j + 1];
            z_i1_j2 = z->data[index_i1_j + 2];
            z_i1_j3 = z->data[index_i1_j + 3];
            z_i1_j4 = z->data[index_i1_j + 4];
            z_i1_j5 = z->data[index_i1_j + 5];
            
            z_i_j = z_i_j + c2y0;
            z_i_j = z_i_j / y_x_max_i;
            z_i_j1 = z_i_j1 + c2y2;
            z_i_j1 = C1 * z_i_j1;
            z_i_j1 = z_i_j1 + x->data[index_i_j + 1];
            z_i_j1 = z_i_j1/sqrt_2;    
            z_i_j2 = z_i_j2 + c2y1;
            z_i_j2 = z_i_j2 / y_x_max_i;
            z_i_j3 = z_i_j3 + c2y0;
            z_i_j3 = C1 * z_i_j3;
            z_i_j3 = z_i_j3 + x->data[index_i_j + 3];
            z_i_j3 = z_i_j3/sqrt_2;
            z_i_j4 = z_i_j4 + c2y2;
            z_i_j4 = z_i_j4 / y_x_max_i;
            z_i_j5 = z_i_j5 + c2y1;
            z_i_j5 = C1 * z_i_j5;
            z_i_j5 = z_i_j5 + x->data[index_i_j + 5];
            z_i_j5 = z_i_j5/sqrt_2;
            z_i1_j = z_i1_j + c2y0;
            z_i1_j = C1 * z_i1_j;
            z_i1_j = z_i1_j + x->data[index_i1_j];
            z_i1_j = z_i1_j/sqrt_2;
            z_i1_j1 = z_i1_j1 + c2y2;
            z_i1_j1 = z_i1_j1 / y_x_max_i_1;
            z_i1_j2 = z_i1_j2 + c2y1;
            z_i1_j2 = C1 * z_i1_j2;
            z_i1_j2 = z_i1_j2 + x->data[index_i1_j + 2];
            z_i1_j2 = z_i1_j2/sqrt_2;
            z_i1_j3 = z_i1_j3 + c2y0;
            z_i1_j3 = z_i1_j3 / y_x_max_i_1;
            z_i1_j4 = z_i1_j4 + c2y2;
            z_i1_j4 = C1 * z_i1_j4;
            z_i1_j4 = z_i1_j4 + x->data[index_i1_j + 4];
            z_i1_j4 = z_i1_j4/sqrt_2;
            z_i1_j5 = z_i1_j5 + c2y1;
            z_i1_j5 = z_i1_j5 / y_x_max_i_1;

            z->data[index_i_j] = z_i_j;
            z->data[index_i_j + 1] = z_i_j1;
            z->data[index_i_j + 2] = z_i_j2;
            z->data[index_i_j + 3] = z_i_j3;
            z->data[index_i_j + 4] = z_i_j4;
            z->data[index_i_j + 5] = z_i_j5;
            z->data[index_i1_j] = z_i1_j;
            z->data[index_i1_j + 1] = z_i1_j1;
            z->data[index_i1_j + 2] = z_i1_j2;
            z->data[index_i1_j + 3] = z_i1_j3;
            z->data[index_i1_j + 4] = z_i1_j4;
            z->data[index_i1_j + 5] = z_i1_j5;
        }
        //z[i, j]       odd
            z_i_j = z->data[index_i_j];
            z_i_j1 = z->data[index_i_j + 1];
            z_i_j2 = z->data[index_i_j + 2];
            z_i_j3 = z->data[index_i_j + 3];
            z_i_j4 = z->data[index_i_j + 4];
            z_i_j5 = z->data[index_i_j + 5];
            z_i1_j = z->data[index_i1_j];
            z_i1_j1 = z->data[index_i1_j + 1];
            z_i1_j2 = z->data[index_i1_j + 2];
            z_i1_j3 = z->data[index_i1_j + 3];
            z_i1_j4 = z->data[index_i1_j + 4];
            z_i1_j5 = z->data[index_i1_j + 5];
            
            z_i_j = z_i_j + c2y0;
            z_i_j = z_i_j / y_x_max_i;
            z_i_j1 = z_i_j1 + c2y2;
            z_i_j1 = C1 * z_i_j1;
            z_i_j1 = z_i_j1 + x->data[index_i_j + 1];
            z_i_j1 = z_i_j1/sqrt_2;
            z_i_j2 = z_i_j2 + c2y1;
            z_i_j2 = z_i_j2 / y_x_max_i;
            z_i_j3 = z_i_j3 + c2y0;
            z_i_j3 = C1 * z_i_j3;
            z_i_j3 = z_i_j3 + x->data[index_i_j + 3];
            z_i_j3 = z_i_j3/sqrt_2;
            z_i_j4 = z_i_j4 + c2y2;
            z_i_j4 = z_i_j4 / y_x_max_i;
            z_i_j5 = z_i_j5 + c2y1;
            z_i1_j = z_i1_j + c2y0;
            z_i1_j = C1 * z_i1_j;
            z_i1_j = z_i1_j + x->data[index_i1_j];
            z_i1_j = z_i1_j/sqrt_2;
            z_i1_j1 = z_i1_j1 + c2y2;
            z_i1_j1 = z_i1_j1 / y_x_max_i_1;
            z_i1_j2 = z_i1_j2 + c2y1;
            z_i1_j2 = C1 * z_i1_j2;
            z_i1_j2 = z_i1_j2 + x->data[index_i1_j + 2];
            z_i1_j2 = z_i1_j2/sqrt_2;
            z_i1_j3 = z_i1_j3 + c2y0;
            z_i1_j3 = z_i1_j3 / y_x_max_i_1;
            z_i1_j4 = z_i1_j4 + c2y2;
            z_i1_j4 = C1 * z_i1_j4;
            z_i1_j4 = z_i1_j4 + x->data[index_i1_j + 4];
            z_i1_j4 = z_i1_j4/sqrt_2;
            z_i1_j5 = z_i1_j5 + c2y1;
            
            z->data[index_i_j] = z_i_j;
            z->data[index_i_j + 1] = z_i_j1;
            z->data[index_i_j + 2] = z_i_j2;
            z->data[index_i_j + 3] = z_i_j3;
            z->data[index_i_j + 4] = z_i_j4;
            z->data[index_i_j + 5] = z_i_j5;
            z->data[index_i1_j] = z_i1_j;
            z->data[index_i1_j + 1] = z_i1_j1;
            z->data[index_i1_j + 2] = z_i1_j2;
            z->data[index_i1_j + 3] = z_i1_j3;
            z->data[index_i1_j + 4] = z_i1_j4;
            z->data[index_i1_j + 5] = z_i1_j5;   
            
    }
}

void slow_performance7(mat* x, mat* y, mat* z) {
  /* Add your final implementation here */
  // change multiplication to division, remove structure call
    int index_ij, index_i1_0, z_n1 = z->n1, z_n2 = z->n2, two_z_n2 = 2*z->n2;
    double* x_data = x->data;
    double* y_data = y->data;
    double* z_data = z->data;
    double inv_sqrt_2 = sqrt(0.5);
    double inv_y_x_max_i, inv_y_x_max_i_1;
    double c2y0 = C2 * y_data[0], c2y2 = C2 * y_data[2], c2y1 = C2 * y_data[1]; 
    double z_i_j, z_i1_j, z_i_j1, z_i1_j1, z_i_j2, z_i1_j2, z_i_j3, z_i1_j3, z_i_j4, z_i1_j4, z_i_j5, z_i1_j5;
    double xi, yi, xi1, yi1;

    
    for ( int i = 0, index_i_0 = 0, index_i1_0 = z_n2; i + 1 < z_n1; i += 2, index_i_0 += two_z_n2, index_i1_0 += two_z_n2) {
        xi = x_data[i], yi = y_data[i], xi1 = x_data[i+1], yi1 = y_data[i + 1];
        inv_y_x_max_i = 1.0/(yi > xi ? yi : xi);
        inv_y_x_max_i_1 = 1.0/(yi1 > xi1 ? yi1 : xi1);

        //z[i, 0]
        z_i_j = z_data[index_i_0];
        z_i1_j = z_data[index_i1_0];
        
        z_i_j = ((C1 * z_i_j) + x_data[index_i_0])* inv_sqrt_2;
        z_i1_j = z_i1_j*inv_y_x_max_i_1;
        
        z_data[index_i_0] = z_i_j;
        z_data[index_i1_0] = z_i1_j;
        
        
        int j, index_i_j, index_i1_j;                  
        for (j = 1, index_i_j = index_i_0 + 1, index_i1_j = index_i1_0 + 1; j + 5 < z_n2 - 1; j += 6, index_i_j += 6, index_i1_j += 6) {
            z_i_j = z_data[index_i_j];
            z_i_j1 = z_data[index_i_j + 1];
            z_i_j2 = z_data[index_i_j + 2];
            z_i_j3 = z_data[index_i_j + 3];
            z_i_j4 = z_data[index_i_j + 4];
            z_i_j5 = z_data[index_i_j + 5];
            z_i1_j = z_data[index_i1_j];
            z_i1_j1 = z_data[index_i1_j + 1];
            z_i1_j2 = z_data[index_i1_j + 2];
            z_i1_j3 = z_data[index_i1_j + 3];
            z_i1_j4 = z_data[index_i1_j + 4];
            z_i1_j5 = z_data[index_i1_j + 5];
            
            z_i_j = (z_i_j + c2y0) * inv_y_x_max_i;
            z_i_j1 = (C1 * (z_i_j1 + c2y2) + x_data[index_i_j + 1])*inv_sqrt_2;
            z_i_j2 = (z_i_j2 + c2y1) * inv_y_x_max_i;
            z_i_j3 = (C1 * (z_i_j3 + c2y0) + x_data[index_i_j + 3]) * inv_sqrt_2;
            z_i_j4 = (z_i_j4 + c2y2) * inv_y_x_max_i;
            z_i_j5 = (C1 * (z_i_j5 + c2y1) + x_data[index_i_j + 5]) * inv_sqrt_2;
            z_i1_j = (C1 * (z_i1_j + c2y0) + x_data[index_i1_j]) * inv_sqrt_2;
            z_i1_j1 = (z_i1_j1 + c2y2) * inv_y_x_max_i_1;
            z_i1_j2 = (C1 * (z_i1_j2 + c2y1) + x_data[index_i1_j + 2]) * inv_sqrt_2;
            z_i1_j3 = (z_i1_j3 + c2y0) * inv_y_x_max_i_1;
            z_i1_j4 = (C1 * (z_i1_j4 + c2y2) + x_data[index_i1_j + 4]) * inv_sqrt_2;
            z_i1_j5 = (z_i1_j5 + c2y1) * inv_y_x_max_i_1;

            z_data[index_i_j] = z_i_j;
            z_data[index_i_j + 1] = z_i_j1;
            z_data[index_i_j + 2] = z_i_j2;
            z_data[index_i_j + 3] = z_i_j3;
            z_data[index_i_j + 4] = z_i_j4;
            z_data[index_i_j + 5] = z_i_j5;
            z_data[index_i1_j] = z_i1_j;
            z_data[index_i1_j + 1] = z_i1_j1;
            z_data[index_i1_j + 2] = z_i1_j2;
            z_data[index_i1_j + 3] = z_i1_j3;
            z_data[index_i1_j + 4] = z_i1_j4;
            z_data[index_i1_j + 5] = z_i1_j5;
        }
        //z[i, j]       odd
            z_i_j = z_data[index_i_j];
            z_i_j1 = z_data[index_i_j + 1];
            z_i_j2 = z_data[index_i_j + 2];
            z_i_j3 = z_data[index_i_j + 3];
            z_i_j4 = z_data[index_i_j + 4];
            z_i_j5 = z_data[index_i_j + 5];
            z_i1_j = z_data[index_i1_j];
            z_i1_j1 = z_data[index_i1_j + 1];
            z_i1_j2 = z_data[index_i1_j + 2];
            z_i1_j3 = z_data[index_i1_j + 3];
            z_i1_j4 = z_data[index_i1_j + 4];
            z_i1_j5 = z_data[index_i1_j + 5];
            
            z_i_j = (z_i_j + c2y0) * inv_y_x_max_i;
            z_i_j1 = (C1 * (z_i_j1 + c2y2) + x_data[index_i_j + 1])*inv_sqrt_2;
            z_i_j2 = (z_i_j2 + c2y1) * inv_y_x_max_i;
            z_i_j3 = (C1 * (z_i_j3 + c2y0) + x_data[index_i_j + 3]) * inv_sqrt_2;
            z_i_j4 = (z_i_j4 + c2y2) * inv_y_x_max_i;
            z_i_j5 = z_i_j5 + c2y1;
            z_i1_j = (C1 * (z_i1_j + c2y0) + x_data[index_i1_j]) * inv_sqrt_2;
            z_i1_j1 = (z_i1_j1 + c2y2) * inv_y_x_max_i_1;
            z_i1_j2 = (C1 * (z_i1_j2 + c2y1) + x_data[index_i1_j + 2]) * inv_sqrt_2;
            z_i1_j3 = (z_i1_j3 + c2y0) * inv_y_x_max_i_1;
            z_i1_j4 = (C1 * (z_i1_j4 + c2y2) + x_data[index_i1_j + 4]) * inv_sqrt_2;
            z_i1_j5 = z_i1_j5 + c2y1;
            
            z_data[index_i_j] = z_i_j;
            z_data[index_i_j + 1] = z_i_j1;
            z_data[index_i_j + 2] = z_i_j2;
            z_data[index_i_j + 3] = z_i_j3;
            z_data[index_i_j + 4] = z_i_j4;
            z_data[index_i_j + 5] = z_i_j5;
            z_data[index_i1_j] = z_i1_j;
            z_data[index_i1_j + 1] = z_i1_j1;
            z_data[index_i1_j + 2] = z_i1_j2;
            z_data[index_i1_j + 3] = z_i1_j3;
            z_data[index_i1_j + 4] = z_i1_j4;
            z_data[index_i1_j + 5] = z_i1_j5;   
    }
}

void maxperformance(mat* x, mat* y, mat* z) {
  /* Add your final implementation here */
  // change multiplication to division, remove structure call
    int index_ij, index_i1_0, z_n1 = z->n1, z_n2 = z->n2, two_z_n2 = 2*z->n2;
    double* x_data = x->data;
    double* y_data = y->data;
    double* z_data = z->data;
    double inv_sqrt_2 = sqrt(0.5);
    double inv_y_x_max_i, inv_y_x_max_i_1;
    double c2y0 = C2 * y_data[0], c2y2 = C2 * y_data[2], c2y1 = C2 * y_data[1]; 
    double z_i_j, z_i1_j, z_i_j1, z_i1_j1, z_i_j2, z_i1_j2, z_i_j3, z_i1_j3, z_i_j4, z_i1_j4, z_i_j5, z_i1_j5, z_i_0, z_i1_0;
    double xi, yi, xi1, yi1;

    
    for ( int i = 0, index_i_0 = 0, index_i1_0 = z_n2; i + 1 < z_n1; i += 2, index_i_0 += two_z_n2, index_i1_0 += two_z_n2) {
        xi = x_data[i], yi = y_data[i], xi1 = x_data[i+1], yi1 = y_data[i + 1];
        inv_y_x_max_i = 1.0/(yi > xi ? yi : xi);
        inv_y_x_max_i_1 = 1.0/(yi1 > xi1 ? yi1 : xi1);

        //z[i, 0]
        z_i_0 = z_data[index_i_0];
        z_i1_0 = z_data[index_i1_0];
        
        z_i_0 = ((C1 * z_i_0) + x_data[index_i_0])* inv_sqrt_2;
        z_i1_0 = z_i1_0 * inv_y_x_max_i_1;

        int j, index_i_j, index_i1_j;                  
        for (j = 1, index_i_j = index_i_0 + 1, index_i1_j = index_i1_0 + 1; j + 5 < z_n2 - 1; j += 6, index_i_j += 6, index_i1_j += 6) {
            z_i_j = z_data[index_i_j];
            z_i_j1 = z_data[index_i_j + 1];
            z_i_j2 = z_data[index_i_j + 2];
            z_i_j3 = z_data[index_i_j + 3];
            z_i_j4 = z_data[index_i_j + 4];
            z_i_j5 = z_data[index_i_j + 5];
            z_i1_j = z_data[index_i1_j];
            z_i1_j1 = z_data[index_i1_j + 1];
            z_i1_j2 = z_data[index_i1_j + 2];
            z_i1_j3 = z_data[index_i1_j + 3];
            z_i1_j4 = z_data[index_i1_j + 4];
            z_i1_j5 = z_data[index_i1_j + 5];
            
            z_i_j = (z_i_j + c2y0) * inv_y_x_max_i;
            z_i_j1 = (C1 * (z_i_j1 + c2y2) + x_data[index_i_j + 1])*inv_sqrt_2;
            z_i_j2 = (z_i_j2 + c2y1) * inv_y_x_max_i;
            z_i_j3 = (C1 * (z_i_j3 + c2y0) + x_data[index_i_j + 3]) * inv_sqrt_2;
            z_i_j4 = (z_i_j4 + c2y2) * inv_y_x_max_i;
            z_i_j5 = (C1 * (z_i_j5 + c2y1) + x_data[index_i_j + 5]) * inv_sqrt_2;
            z_i1_j = (C1 * (z_i1_j + c2y0) + x_data[index_i1_j]) * inv_sqrt_2;
            z_i1_j1 = (z_i1_j1 + c2y2) * inv_y_x_max_i_1;
            z_i1_j2 = (C1 * (z_i1_j2 + c2y1) + x_data[index_i1_j + 2]) * inv_sqrt_2;
            z_i1_j3 = (z_i1_j3 + c2y0) * inv_y_x_max_i_1;
            z_i1_j4 = (C1 * (z_i1_j4 + c2y2) + x_data[index_i1_j + 4]) * inv_sqrt_2;
            z_i1_j5 = (z_i1_j5 + c2y1) * inv_y_x_max_i_1;

            z_data[index_i_j] = z_i_j;
            z_data[index_i_j + 1] = z_i_j1;
            z_data[index_i_j + 2] = z_i_j2;
            z_data[index_i_j + 3] = z_i_j3;
            z_data[index_i_j + 4] = z_i_j4;
            z_data[index_i_j + 5] = z_i_j5;
            z_data[index_i1_j] = z_i1_j;
            z_data[index_i1_j + 1] = z_i1_j1;
            z_data[index_i1_j + 2] = z_i1_j2;
            z_data[index_i1_j + 3] = z_i1_j3;
            z_data[index_i1_j + 4] = z_i1_j4;
            z_data[index_i1_j + 5] = z_i1_j5;
        }
        z_data[index_i_0] = z_i_0;
        z_data[index_i1_0] = z_i1_0;

        //z[i, j]       odd
        z_i_j = z_data[index_i_j];
        z_i_j1 = z_data[index_i_j + 1];
        z_i_j2 = z_data[index_i_j + 2];
        z_i_j3 = z_data[index_i_j + 3];
        z_i_j4 = z_data[index_i_j + 4];
        z_i_j5 = z_data[index_i_j + 5];
        z_i1_j = z_data[index_i1_j];
        z_i1_j1 = z_data[index_i1_j + 1];
        z_i1_j2 = z_data[index_i1_j + 2];
        z_i1_j3 = z_data[index_i1_j + 3];
        z_i1_j4 = z_data[index_i1_j + 4];
        z_i1_j5 = z_data[index_i1_j + 5];
        
        z_i_j = (z_i_j + c2y0) * inv_y_x_max_i;
        z_i_j1 = (C1 * (z_i_j1 + c2y2) + x_data[index_i_j + 1])*inv_sqrt_2;
        z_i_j2 = (z_i_j2 + c2y1) * inv_y_x_max_i;
        z_i_j3 = (C1 * (z_i_j3 + c2y0) + x_data[index_i_j + 3]) * inv_sqrt_2;
        z_i_j4 = (z_i_j4 + c2y2) * inv_y_x_max_i;
        z_i_j5 = z_i_j5 + c2y1;
        z_i1_j = (C1 * (z_i1_j + c2y0) + x_data[index_i1_j]) * inv_sqrt_2;
        z_i1_j1 = (z_i1_j1 + c2y2) * inv_y_x_max_i_1;
        z_i1_j2 = (C1 * (z_i1_j2 + c2y1) + x_data[index_i1_j + 2]) * inv_sqrt_2;
        z_i1_j3 = (z_i1_j3 + c2y0) * inv_y_x_max_i_1;
        z_i1_j4 = (C1 * (z_i1_j4 + c2y2) + x_data[index_i1_j + 4]) * inv_sqrt_2;
        z_i1_j5 = z_i1_j5 + c2y1;
        
        z_data[index_i_j] = z_i_j;
        z_data[index_i_j + 1] = z_i_j1;
        z_data[index_i_j + 2] = z_i_j2;
        z_data[index_i_j + 3] = z_i_j3;
        z_data[index_i_j + 4] = z_i_j4;
        z_data[index_i_j + 5] = z_i_j5;
        z_data[index_i1_j] = z_i1_j;
        z_data[index_i1_j + 1] = z_i1_j1;
        z_data[index_i1_j + 2] = z_i1_j2;
        z_data[index_i1_j + 3] = z_i1_j3;
        z_data[index_i1_j + 4] = z_i1_j4;
        z_data[index_i1_j + 5] = z_i1_j5;   
    }
}

/*
* Called by the driver to register your functions
* Use add_function(func, description) to add your own functions
*/
void register_functions() {
  add_function(&slow_performance1, "slow_performance1",1);
  add_function(&slow_performance2, "slow_performance2",1);
  add_function(&slow_performance3, "slow_performance3",1);
  add_function(&slow_performance4, "slow_performance4",1);
  add_function(&slow_performance5, "slow_performance5",1);
  add_function(&slow_performance6, "slow_performance6",1);
  add_function(&slow_performance7, "slow_performance7",1);
  add_function(&maxperformance, "maxperformance",1);
}