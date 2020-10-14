
extern "C"
{
    extern int start_context(int id);
}

int main(){
    int a;
    __asm__("movq %0, %%r10;"::"r"(&a):"%r10");
    start_context(a);
    return 0;
}