#include <iostream>
using namespace std;
typedef struct pair_t {
	double a;
	double b;
	uint64_t u[3];
} test;

void printcnt(int i, int j, int cnt){
    cnt = cnt%5;
    switch(cnt){
        case 0:
        cout <<"A[" << i << "][" << j<< "].a ";
        break;
        case 1:
        cout <<"A[" << i << "][" << j<< "].b ";
        break;
        case 2:
        cout <<"A[" << i << "][" << j << "].u0 ";
        break;
        case 3:
        cout <<"A[" << i << "][" << j << "].u1 ";
        break;
        case 4:
        cout <<"A[" << i << "][" << j << "].u2 ";
        break;
    }
}

int main(){
    test A[3][3];
    // cout << sizeof(A) << endl;
    double t1, t2; 
    int cnt = 0;
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            for(int k = 0; k < 5; k++){
                if(cnt % 2==0) cout << "\n\nblock " << (cnt/2)%8 << endl;
                printcnt(i, j, cnt++);
            }
        }
    }
    cout << endl;

    // for (int i = 0; i < 2; i++) {
    //     for (int j = 0; j < 3; j++) {
    //         // t1 = A[i+1][j].a;
    //         // t2 = A[i][(j+1)%3].a;
    //         // A[i][j].b = t1 + t2; 
    //         cout << "A["<< i+1 << "][" << j << "].a" <<endl;
    //         cout << "A["<< i << "][" << (j + 1) % 3  << "].a" <<endl;
    //         cout << "A["<< i << "][" << j << "].b" <<endl;
    //     }
    // }
    // for (int i = 0; i < 2; i++) {
    //     for (int j = 0; j < 3; j++) { 
    //         // t1 = A[(j+1)%3][i].a;
    //         // t2 = A[j][i+1].a; 
    //         // A[j][i].b = t1 + t2;
    //         cout << "A["<< (j+1)%3 << "][" << i << "].a" <<endl;
    //         cout << "A["<< j << "][" << i + 1 << "].a" <<endl;
    //         cout << "A["<< j << "][" << i << "].b" <<endl;
            
    //     } 
    // }
    return 0;
}