#include <iostream>
#include "stdlib.h"
using namespace std;
void gooo(int **a,int k,int n,int m){
    int i,j;
    int prom;int hi,hj;
    for(i=0;i<n;i++){
        for(j=0;j<m;j++)
        {
            prom=0;
            for(hi=0;hi<n;hi++)
                for(hj=0;hj<m;hj++)
                {
                    if(a[i][j]==a[hi][hj]&&i!=hi&&j!=hj){
                        prom==1;
                    }
                }
            if(prom==0){
                k++;
            }
        }
    }
    int* NEP=new int[k];
    int lol(0);
    for(i=0;i<n;i++)
        for(j=0;j<m;j++)
        {
            prom=0;
            for(int hi=0;hi<n;hi++)
                for(int hj=0;hj<m;hj++)
                {
                    if(a[i][j]==a[hi][hj]&&i!=hi&&j!=hj)
                        prom==1;
                }
            if(prom==0){
                NEP[lol]=a[i][j];
                lol++;
            }
        }
    int min=NEP[0];
    for(i=0;i<lol;i++){
        if(min>NEP[i]){
            min=NEP[i];
        }
    }
    cout<<"Минимальный из неповторющихся = "<<min<<endl;
}

int main()
{
    int k(0),i(0),j(0),n,m;
    cout<<"Введите n"<<endl;
    cin>>n;
    cout<<"Введите m"<<endl;
    cin>>m;
    int **a = new int * [n];
    for (i = 0; i<n; i++)
        a[i] = new int [m];
    cout<<"Введите массив"<<endl;
    for(i=0;i<n;i++)
        for(j=0;j<m;j++)
            cin>>a[i][j];
    cout << "\x1B[2J\x1B[H";
    gooo(a,k,n,m);
    return 0;
}

