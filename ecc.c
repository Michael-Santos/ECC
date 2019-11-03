/*
 * @Author Michael dos Santos
 * @RA 726572
 * 
 * @Author Victor Watanabe
 * @RA 726591
*/

#include<stdlib.h>
#include<stdlib.h>

#define NUMBER_NUM_BITS 64

typedef long long int NUMBER;

/*
 * Modelo de um ponto (x, y)
*/
typedef struct point
{
    NUMBER x;
    NUMBER y;
} Point;

/*
 * Obtém o valor de um bit específico
 * 
 * @params value - Número em que se deseja encontrar o bit
 * @params index - Índice do bit (bit zero é o bit mais à esquerda)
 * @returns Value of the bit
*/
int get_bit_by_index(NUMBER value, int index) {
    NUMBER left_shift = value << index;
    return left_shift >> NUMBER_NUM_BITS - 1;
}

/*
 * Encontra o bit mais a esquerda que possui valor 1
 * 
 * @params value - Número para encontrar o bit mais à esquerda com valor 1 
 * @returns Índice do bit
*/
int find_leftmost_one_bit(NUMBER value) {
    int bit = 0;
    
    for(int i = 0; i < NUMBER_NUM_BITS; i++) {
        bit = get_bit_by_index(value, i);
        // Workaround, pois apenas recebo -1
        if (bit == -1) return i;
    }   
    return(-1);
}

/*
 * Verifica se é um ponto de origem
 * 
 * @params p - Ponto a ser verficado
 * @returns Retornar 1 se o ponto é de origin e 0 caso contrário
*/
int is_origin(Point p) {
    return p.x == 0 && p.y == 0;
}

/*
 * Verifica se o primeiro ponto é inverso do segundo
 * 
 * @params Q - Primeiro ponto
 * @params G - Segundo ponto
 * @params p - Módulo
 * @retuns Retornar 1 se são inversos e 0 caso contrário
*/
int is_inverse(Point Q, Point G, NUMBER module) {
    return G.x == Q.x && G.y == (module - Q.y) % module;
}   

/*
 * Verifica se dois pontos são iguais
 * 
 * @params Q - Primeiro ponto
 * @params G - Segundo ponto
 * @params modulo - Módulo
 * @returns Retornar 1 se são iguais e 0 caso contrário
*/
int is_equal(Point G, Point Q) {
    return G.x == Q.x && G.y == Q.y;
}

/*
 * Calcula o inverso multiplicativo de um número
 * 
 * @params number - Número a calcular o inverso multiplicativo
 * @params module - Módulo
 * @returns Retorna o inverso multiplicativo do número
*/
NUMBER calc_multiplicative_inverse(NUMBER number, NUMBER module) {
    NUMBER aux, multiplicative_inverse;
    
    if(number < 0) number += module;
    number = number % module;

    multiplicative_inverse = 1;
    do {
        aux = number;
        multiplicative_inverse++;
        aux = number * multiplicative_inverse;
    } while((aux - module) % module != 1);

    return multiplicative_inverse;
}

/*
 * Calcula a soma entre dois pontos
 * 
 * @params a - Parâmetro da curva
 * @params p - Módulo (precisa ser um número primo)
 * @params Q - Primeiro ponto
 * @params G - Segundo ponto
 * @return Soma entre os dois pontos
*/
Point calc_sum(NUMBER a, NUMBER p, Point Q, Point G) {
    NUMBER lambda, xr, yr;
    Point result;

    if(is_origin(G)) return Q;
    
    if(is_origin(Q)) return G;
    
    if(is_inverse(Q, G, p)) {
        result.x = 0;
        result.y = 0;
        return result;
    }

    if(is_equal(Q, G))
        lambda = ((3 * Q.x * Q.x + a) * calc_multiplicative_inverse(2 * Q.y, p)) % p;
    else
        lambda = ((G.y - Q.y) * calc_multiplicative_inverse(G.x - Q.x, p)) % p;     

    result.x = (lambda * lambda - Q.x - G.x) % p;
    result.y = (lambda * (Q.x - result.x) - Q.y) % p;

    if(result.x < 0) result.x = p + result.x;
    if(result.y < 0) result.y = p + result.y; 

    return result;
}

/*
 * Calcula a operação de multiplicação entre o escalar e um ponto (nG)
 * 
 * @params n - Escalar da multiplicação
 * @params a - Parâmetro da curva
 * @params p - Módulo (precisa ser um número primo)
 * @params G - Ponto da multiplicação
 * @returns Resultado da multiplicação entre o escalar e o ponto
*/
Point calc_double_n_sum(NUMBER n, NUMBER a, NUMBER p, Point G) {
    int index, bit;
    Point result = G;

    index = find_leftmost_one_bit(n);

    for(int i = ++index; i < NUMBER_NUM_BITS; i++) {
        bit = get_bit_by_index(n, i);
        result = calc_sum(a, p, result, result);

        // Workaround, pois apenas recebo -1 ou 0
        if(bit == -1) result = calc_sum(a, p, result, G);
    }
    return result;
}

int main() {
    Point G, Q, result;
    NUMBER n, a, p;
    
    do {
        scanf("%llu", &n);
        if(n == 0) break;
        
        scanf("%lld %lld %lld %lld", &a, &p, &G.x, &G.y);

        result = calc_double_n_sum(n, a, p, G);
        printf("%lld %lld\n", result.x, result.y);
    } while(n != 0);

    return (0);       
}