
<<<<<<< HEAD
int d = 100;

int sum(int a, int b){
    long c = 0;
    c = c + 4;
    c = d + c;
    return c;
}

int main(){
    int a;
    a = 101010;    
    return sum(a, a);
=======
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
>>>>>>> 2fbd3f96eabdcb7a359c5544e9757e9546bc908a
}