/* Programa: Hola mundo */
long fact_do(long n) {
    long result = 1;
    do {
        result *= n;
        n = n-1;
    } while (n > 1);
    return result;
}
int main (){
    long m = 5;
    long x = fact_do(m);
}
