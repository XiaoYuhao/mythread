
class A{
public:
    long a;
    long b;
    long c;
};

/*
int func(int &a, int b){
    int d;
    d = a + b;
    return d;
}
*/

int func(A &a){
    return 1;
}

int main(){
    int a ,b, c;
    a = 1;
    b = 2;
    A aa;
    aa.a = 3;
    aa.b = 4;
    aa.c = 5;
    c = func(aa);
    return 0;
}