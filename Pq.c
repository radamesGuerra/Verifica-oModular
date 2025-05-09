#include <stdio.h>
#include <stdlib.h>

//***********************************************************************//
//************************* Verificação Modular *************************//
/*
  Objetivo:

  Dados três de números inteiros positivos de entrada, H, G e n, calcular:

  base a;
  exponente x;
  módulo n1;
  a^x mod n1.
*/
//***********************************************************************//

// Função para calcular o MDC usando o algoritmo de Euclides
int mdc(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Função para verificar se um número é primo
int eh_primo(int num) {
    if (num <= 1) return 0;
    if (num == 2) return 1;
    if (num % 2 == 0) return 0;

    for (int i = 3; i * i <= num; i += 2) {
        if (num % i == 0) return 0;
    }
    return 1;
}

// Função para calcular o inverso modular usando o algoritmo estendido de Euclides
int inverso_modular(int a, int n) {
    int t0 = 0, t1 = 1;
    int r0 = n, r1 = a;

    while (r1 != 0) {
        int q = r0 / r1;

        int temp = r1;
        r1 = r0 - q * r1;
        r0 = temp;

        temp = t1;
        t1 = t0 - q * t1;
        t0 = temp;
    }

    if (r0 > 1) return -1; // Não existe inverso
    if (t0 < 0) t0 += n;

    return t0;
}

// Função para calcular a função totiente de Euler φ(n)
int totiente_euler(int n) {
    if (n == 1) return 1;
    if (eh_primo(n)) return n - 1;

    int result = n;

    // Verifica divisibilidade por 2
    if (n % 2 == 0) {
        result -= result / 2;
        while (n % 2 == 0) n /= 2;
    }

    // Verifica divisibilidade por números ímpares
    for (int i = 3; i * i <= n; i += 2) {
        if (n % i == 0) {
            result -= result / i;
            while (n % i == 0) n /= i;
        }
    }

    // Se n for um número primo maior que 2
    if (n > 1) {
        result -= result / n;
    }

    return result;
}

// Função para calcular a^b mod m usando exponenciação modular eficiente
int exp_modular(int a, int b, int m) {
    int resultado = 1;
    a = a % m;

    while (b > 0) {
        if (b % 2 == 1) {
            resultado = (resultado * a) % m;
        }
        a = (a * a) % m;
        b = b / 2;
    }

    return resultado;
}

int main() {
    int H, G, n, x, n1;

    printf("Digite o valor de H: ");
    scanf("%d", &H);

    printf("\nDigite o valor de G: ");
    scanf("%d", &G);

    printf("\nDigite o valor de n: ");
    scanf("%d", &n);

    printf("\nDigite o expoente x: ");
    scanf("%d", &x);

    printf("\nDigite o modulo n1: ");
    scanf("%d", &n1);

    // Etapa 1: Verificar se G e n são coprimos
    if (mdc(G, n) != 1) {
        printf("\nDivisao nao e possivel pois G e n nao sao coprimos (MDC(G, n) = %d)\n", mdc(G, n));
        return 0;
    }

    // Etapa 2: Calcular o inverso de G em Zn
    int G_inv = inverso_modular(G, n);
    if (G_inv == -1) {
        printf("\nInverso de G nao existe em Zn\n");
        return 0;
    }

    // Etapa 3: Calcular a = H * G_inv mod n
    int a = (H % n) * G_inv % n;
    if (a < 0) a += n; // Garantir que a seja positivo

    printf("\nBase a calculada: %d\n", a);

    // Etapa 4: Verificar se a e n1 são coprimos
    if (mdc(a, n1) != 1) {
        printf("\na e n1 nao sao coprimos (MDC(a, n1) = %d)\n", mdc(a, n1));
        // Continuamos mesmo assim, pois o problema não pede para parar aqui
    }

    // Etapa 5: Verificar se n1 é primo
    int n1_primo = eh_primo(n1);

    // Etapa 6 e 7: Definir x1
    int x1;
    if (n1_primo) {
        printf("\nn1 e primo, aplicando Pequeno Teorema de Fermat\n");
        x1 = n1 - 1;
    } else {
        printf("\nn1 nao e primo, aplicando Teorema de Euler\n");
        x1 = totiente_euler(n1);
    }
    printf("x1 calculado: %d\n", x1);

    // Etapa 8: Decompor x = x1 * q + r
    int q = x / x1;
    int r = x % x1;
    printf("\n***** Decomposicao *****\n");
    printf("\nx = %d * %d + %d\n", x1, q, r);

    // Etapa 10: Calcular valores intermediários
    int x2 = exp_modular(a, x1, n1);
    int x2_q = exp_modular(x2, q, n1);
    int a_r = exp_modular(a, r, n1);

    printf("\n***** Valores intermediarios *****\n");
    printf("\na^x1 mod n1 = %d\n", x2);
    printf("x2^q mod n1 = %d\n", x2_q);
    printf("a^r mod n1 = %d\n", a_r);

    // Etapa 11: Calcular o resultado final
    int resultado = (x2_q * a_r) % n1;
    if (resultado < 0) resultado += n1; // Garantir que seja positivo

    printf("\n***** Resultado final *****\n");
    printf("\n%d^%d mod %d e congruente a %d\n", a, x, n1, resultado);

    return 0;
}
