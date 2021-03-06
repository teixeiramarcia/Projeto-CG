#define _CRT_SECURE_NO_WARNINGS

#include <math.h>
#include <stdio.h>
#include <string.h>

/**
* Função responsável por calcular os pontos que descrevem os dois triângulos
* que permitem desenhar o plano (de lado igual) no plano XZ e centrado na origem
* Esses pontos, depois de calculados, devem ser escritos no ficheiro
*/
void desenhaPlano(FILE* f, float tamLado) {
	fprintf(f, "6\n");   //escrita inicial do número de pontos

	float h = 0.0f;     //todos os pontos terão y == 0

	//calculo e escrita dos pontos dos triângulos:
	fprintf(f, "%f %f %f\n", tamLado / 2, h, tamLado / 2);
	fprintf(f, "%f %f %f\n", tamLado / 2, h, -tamLado / 2);
	fprintf(f, "%f %f %f\n", -tamLado / 2, h, tamLado / 2);

	fprintf(f, "%f %f %f\n", tamLado / 2, h, -tamLado / 2);
	fprintf(f, "%f %f %f\n", -tamLado / 2, h, -tamLado / 2);
	fprintf(f, "%f %f %f\n", -tamLado / 2, h, tamLado / 2);
}

void desenhaCaixa(FILE* f, float tamX, float tamY, float tamZ) {

}

/**
*Função main, que tem como objetivo ler o input do utilizado, ler o ficheiro e chamar a função correspondente
*/
int main(int argc, const char* argv[]) {
	FILE* f;

	if (argc < 2) {
		printf("Numero insuficiente de parametros, por favor, tente novamente!");
	}
	else {
		switch (argv[1][0]) {
		case'p':
			//escrita dos pontos do plano:
			printf("Plano\n");
			f = fopen(argv[3], "w");
			desenhaPlano(f, atof(argv[2]));
			fclose(f);
			break;
		case'b':
			//escrita dos pontos da caixa:
			printf("Caixa\n");
			break;
		case's':
			//escrita dos pontos da esfera:
			printf("Esfera\n");
			break;
		case'c':
			//escrita dos pontos do cone:
			printf("Cone\n");
			break;
		default:
			printf("Erro, tente novamente!");
		}
	}
	return 0;
}