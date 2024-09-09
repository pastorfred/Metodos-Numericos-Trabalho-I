//Trabalho 1 da disciplina Métodos Numéricos desenvolvido pelo aluno Leonardo Chou da Rosa
#include <iostream>
#include <iomanip>
#include <math.h>
#include <string>
#include <sstream>
#include <cfenv>

using namespace std;

//Union para guardar todos os valores necessarios para a conversao de um numero para o padrao IEEE
union padraoIEEE {
	float num; //valor que vai ser convertido para o padrao IEEE
	struct
	{
		unsigned int mantissa : 23; //forçar o valor da mantissa a ser 23 bits (somente funciona em struct)
		unsigned int expoente : 8; //forçar o valor do expoente a ser 8 bits (somente funciona em struct)
		unsigned int sinal : 1; //forçar o valor do sinal a ser 1 bit (somente funciona em struct)
	} binario; 
};

//funcao que recebe um numero e a quantidade de bits do numero e retorna o numero em padrao ieee
string IEEE(int num, int bit) {
    stringstream ss;
        for (int j = bit - 1; j >= 0; j--) { //repete pela quantidade de bits que foi passada por parametro
            if (((num >> j) & 1) == 1) { //se a soma dos binarios do numero shiftado para a direita por j e 1 for igual a 1
                ss << 1; //insere 1 no stringstream
            } else {ss << 0;} //se for zero, insere 0 no stringstream
        }
    return ss.str(); //retorna o stringstream
}

//funcao que detecta se houve alguma excecao nas operacoes
string excecoes() {
    stringstream ss;
    int inv = 0, under = 0, over = 0, ine = 0, divzero = 0;
    if (fetestexcept(FE_DIVBYZERO)) {divzero++;} //se a flag FE_DIVBYZERO foi acionada, adiciona 1 ao contador da flag
    if (fetestexcept(FE_INEXACT)) {ine++;} //se a flag FE_INEXACT foi acionada, adiciona 1 ao contador da flag
    if (fetestexcept(FE_UNDERFLOW)) {under++;} //se a flag FE_UNDERFLOW foi acionada, adiciona 1 ao contador da flag
    if (fetestexcept(FE_OVERFLOW)) {over++;} //se a flag FE_OVERFLOW foi acionada, adiciona 1 ao contador da flag
    if (fetestexcept(FE_INVALID)) {inv++;} //se a flag FE_INVALID foi acionada, adiciona 1 ao contador da flag

    //insere tudo em uma stringstream
    ss << "Excecao FE_INEXACT: " << ine << endl << "Excecao FE_DIVBYZERO: " << divzero << endl << "Excecao FE_UNDERFLOW: " << under << endl << "Excecao FE_OVERFLOW: " << over << endl << "Excecao FE_INVALID: " << inv << endl;
    return ss.str(); //retorna a stringstream
}

void op(string num1, string num2, char op)
{
    //converte os numeros passados por parametro em float
    float num1_f = stof(num1);
    float num2_f = stof(num2);
    float res = 0;

    if ((num1[0] > 57) || (num1[0] < 48)) { //se a primeira caracter da string do num 1 tá fora dos numeros na tabela ASCII
        if ((num1[0] == '-') && ((num1[1] < 58) && (num1[1] > 47))) {num1_f = stof(num1);} //se a primeira caracter e um negativo e o resto e numero, o numero e considerado negativo
        else if ((num1[0] == '-') && ((num1[1] == 'I') || (num1[1] == 'i'))) //se a primeira caracter é - e a segunda caracter é "I"
        {
            num1_f = -INFINITY; //num 1 = -infinito
        }
        else if ((num1[0] == 'N' || num1[0] == 'n') || (num1[0] == '-' && (num1[1] == 'N' || num1[1] == 'n'))) //se a primeira caracter é n ou a primeira é - e a segunda n
        {
            num1_f = NAN; //num 1 = not a number
        }
        else
        {
            num1_f = INFINITY; //num 1 = infinito
        }
    }

    if ((num2[0] > 57) || (num2[0] < 48)) { //se a primeira caracter da string do num 2 tá fora dos numeros na tabela ASCII
        if ((num2[0] == '-') && ((num2[1] < 58) && (num2[1] > 47))) {num2_f = stof(num2);} //se a primeira caracter e um negativo e o resto e numero, o numero e considerado negativo
        else if ((num2[0] == '-') && ((num2[1] == 'I') || (num2[1] == 'i'))) //se a primeira caracter é - e a segunda caracter é "I"
        {
            num2_f = -INFINITY; //num 2 = -infinito
        }
        else if ((num2[0] == 'N' || num2[0] == 'n') || (num2[0] == '-' && (num2[1] == 'N' || num2[1] == 'n'))) //se a primeira caracter é n ou a primeira é - e a segunda n
        {
            num2_f = NAN; //num 2 = not a number
        }
        else
        {
            num2_f = INFINITY; //num 2 = infinito
        }
    }

    if (op == 42) {res = num1_f * num2_f;} //se o operador for multiplicação, multiplica os dois numeros
    else if (op == 43) {res = num1_f + num2_f;} //se o operador for soma, adiciona os dois numeros
    else if (op == 45) {res = num1_f - num2_f;} //se o operador for subtração, subtrai os dois numeros
    else if (op == 47) {res = num1_f / num2_f;} //se o operador for divisão, divide os dois numeros

    string exe = excecoes(); //executa a funcao que confere as flags baseado na operacao realizada anteriormente

    //cria as variaveis necessarias para a conversao IEEE dos tres numeros
    padraoIEEE num1_ieee; 
    padraoIEEE num2_ieee;
    padraoIEEE res_ieee;
    num1_ieee.num = num1_f;
    num2_ieee.num = num2_f;
    res_ieee.num = res;

    //guarda os resultados das conversoes em strings
    string num1_expoente = IEEE(num1_ieee.binario.expoente, 8);
    string num1_mantissa = IEEE(num1_ieee.binario.mantissa, 23);
    string num2_expoente = IEEE(num2_ieee.binario.expoente, 8);
    string num2_mantissa = IEEE(num2_ieee.binario.mantissa, 23);
    string resultado_expoente = IEEE(res_ieee.binario.expoente, 8);
    string resultado_mantissa = IEEE(res_ieee.binario.mantissa, 23);

    //imprime o resultado do programa
    cout << "Recebi " << fixed << setprecision(6) << num1_f << " " << op << " " << fixed << setprecision(6) << num2_f << " e resultado deu " << res << endl << endl;
    cout << "Num 1 = " << num1_ieee.binario.sinal << " " << num1_expoente << " " << num1_mantissa << " = " << num1_f << endl;
    cout << "Num 2 = " << num2_ieee.binario.sinal << " " << num2_expoente << " " << num2_mantissa << " = " << num2_f << endl;
    cout << "Resultado = " << res_ieee.binario.sinal << " " << resultado_expoente << " " << resultado_mantissa << " = " << res << endl << endl;
    cout << exe << endl;
}

int main()
{
    string num1, num2;
    char opps;
    cout << "Insira uma equacao numerica utilizando +, -, * ou / no formato num1 operacao num2: " << endl;
    cin >> num1 >> opps >> num2;
    feclearexcept(FE_ALL_EXCEPT); //zera os valores das flags antes da operacao
    op(num1, num2, opps);
}