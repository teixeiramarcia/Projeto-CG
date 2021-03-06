#define _CRT_SECURE_NO_WARNINGS

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <string.h>

/**
* Fun��o respons�vel por calcular os pontos que descrevem os dois tri�ngulos
* que permitem desenhar o plano (de lado igual) no plano XZ e centrado na origem
* Esses pontos, depois de calculados, devem ser escritos no ficheiro
*/
void desenhaPlano(FILE* f, float tamLado) {
	fprintf(f, "6\n");   //escrita inicial do n�mero de pontos/v�rtices

	float h = 0.0f;     //todos os pontos ter�o y == 0

	//calculo e escrita dos pontos dos tri�ngulos:
	fprintf(f, "%f %f %f\n", tamLado / 2, h, tamLado / 2);
	fprintf(f, "%f %f %f\n", tamLado / 2, h, -tamLado / 2);
	fprintf(f, "%f %f %f\n", -tamLado / 2, h, tamLado / 2);

	fprintf(f, "%f %f %f\n", tamLado / 2, h, -tamLado / 2);
	fprintf(f, "%f %f %f\n", -tamLado / 2, h, -tamLado / 2);
	fprintf(f, "%f %f %f\n", -tamLado / 2, h, tamLado / 2);
}

/**
* Fun��o respons�vel por calcular os pontos que descrevem os 12 tri�ngulos
* que permitem desenhar a caixa centrada na origem
* Esses pontos, depois de calculados, devem ser escritos no ficheiro
*/
void desenhaCaixa(FILE* f, float tamX, float tamY, float tamZ) {
	fprintf(f, "36\n");   //escrita inicial do n�mero de pontos/v�rtices

	float hMax = tamY / 2;
	float x = tamX / 2;
	float z = tamZ / 2;

	//calculo e escrita dos pontos dos tri�ngulos da tampa (ordem m�o direita, y fixo):
	fprintf(f, "%f %f %f\n", x, hMax, z);
	fprintf(f, "%f %f %f\n", x, hMax, -z);
	fprintf(f, "%f %f %f\n", -x, hMax, z);

	fprintf(f, "%f %f %f\n", x, hMax, -z);
	fprintf(f, "%f %f %f\n", -x, hMax, -z);
	fprintf(f, "%f %f %f\n", -x, hMax, z);

	//calculo e escrita dos pontos dos tri�ngulos da base (ordem m�o esquerda):
	fprintf(f, "%f %f %f\n", x, -hMax, z);
	fprintf(f, "%f %f %f\n", -x, -hMax, z);
	fprintf(f, "%f %f %f\n", x, -hMax, -z);

	fprintf(f, "%f %f %f\n", x, -hMax, -z);
	fprintf(f, "%f %f %f\n", -x, -hMax, z);
	fprintf(f, "%f %f %f\n", -x, -hMax, -z);

	//calculo e escrita dos pontos dos tri�ngulos das faces laterais:
	//1� fase ("frente", z fixo == tamZ/2):
	fprintf(f, "%f %f %f\n", x, -hMax, z);
	fprintf(f, "%f %f %f\n", x, hMax, z);
	fprintf(f, "%f %f %f\n", -x, -hMax, z);

	fprintf(f, "%f %f %f\n", x, hMax, z);
	fprintf(f, "%f %f %f\n", -x, hMax, z);
	fprintf(f, "%f %f %f\n", -x, -hMax, z);

	//2� fase ("traseira", z fixo == -tamZ/2):
	fprintf(f, "%f %f %f\n", x, hMax, -z);
	fprintf(f, "%f %f %f\n", x, -hMax, -z);
	fprintf(f, "%f %f %f\n", -x, -hMax, -z);

	fprintf(f, "%f %f %f\n", x, hMax, -z);
	fprintf(f, "%f %f %f\n", -x, -hMax, -z);
	fprintf(f, "%f %f %f\n", -x, hMax, -z);

	//3� fase ("direita", x fixo == tamZ/2):
	fprintf(f, "%f %f %f\n", x, -hMax, -z);
	fprintf(f, "%f %f %f\n", x, hMax, -z);
	fprintf(f, "%f %f %f\n", x, -hMax, z);

	fprintf(f, "%f %f %f\n", x, hMax, -z);
	fprintf(f, "%f %f %f\n", x, hMax, z);
	fprintf(f, "%f %f %f\n", x, -hMax, z);

	//4� fase ("esquerda, x fixo == -tamZ/2):
	fprintf(f, "%f %f %f\n", -x, hMax, -z);
	fprintf(f, "%f %f %f\n", -x, -hMax, -z);
	fprintf(f, "%f %f %f\n", -x, -hMax, z);

	fprintf(f, "%f %f %f\n", -x, hMax, -z);
	fprintf(f, "%f %f %f\n", -x, -hMax, z);
	fprintf(f, "%f %f %f\n", -x, hMax, z);

}

/**
* Fun��o respons�vel por calcular os pontos que descrevem os v�rios tri�ngulos
* que permitem desenhar o cone (colocado no Y positivo e centrado em X == 0 e Z == 0)
* Esses pontos, depois de calculados, devem ser escritos no ficheiro
*/
void desenhaCone(FILE* f, float raioBase, float altura, int nSlices, int nStacks) {
	fprintf(f, "%d\n", nSlices * nStacks * 3);   //escrita inicial do n�mero de pontos/v�rtices

	float anguloSlice = 2 * M_PI / nSlices;  //angulo formado entre cada slice
	float razaoAlturaStacks = (float)altura / nStacks; //descreve a raz�o entre a altura total do cone e o n� de stacks

	float raioAtual = raioBase;  //vamos come�ar a desenhar pela base (onde o raio da stack � o maior)
	float alturaAtual = 0;       //vamos come�ar a desenhar pela base e queremos o cone "pousado" no plano XZ

	for (int st = 0; st < nStacks; st++) {

		for (int sl = 0; sl < nSlices; sl++) {
			float angulo = (float)anguloSlice * sl;

			//desenhar a base, segundo a regra da m�o esquerda, para poder ser vis�vel:
			if (st == 0) {
				fprintf(f, "%f %f %f\n", 0, 0.0f, 0); // escrita do ponto central (x == 0 e z == 0)
				fprintf(f, "%f %f %f\n", raioAtual * sin(anguloSlice + angulo), 0.0f, raioAtual * cos(anguloSlice + angulo));
				fprintf(f, "%f %f %f\n", raioAtual * sin(angulo), 0.0f, raioAtual * cos(angulo));
			}
			else {
				fprintf(f, "%f %f %f\n", 0, alturaAtual, 0); // escrita do ponto central (x == 0 e z == 0)
				fprintf(f, "%f %f %f\n", raioAtual * sin(angulo), alturaAtual, raioAtual * cos(angulo));
				fprintf(f, "%f %f %f\n", raioAtual * sin(angulo + anguloSlice), alturaAtual, raioAtual * cos(angulo + anguloSlice));
				//dois �litmos pontos, calculados com base em coordenadas polares
			}
		}

		raioAtual = (float)((nStacks - st + 1) * raioBase) / nStacks;
		alturaAtual = razaoAlturaStacks * st;
	}
}

/**
*Fun��o main, que tem como objetivo ler o input do utilizado, ler o ficheiro e chamar a fun��o correspondente
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
			f = fopen(argv[5], "w");
			desenhaCaixa(f, atof(argv[2]), atof(argv[3]), atof(argv[4]));
			fclose(f);
			break;
		case's':
			//escrita dos pontos da esfera:
			printf("Esfera\n");
			break;
		case'c':
			//escrita dos pontos do cone:
			printf("Cone\n");
			f = fopen(argv[6], "w");
			desenhaCone(f, atof(argv[2]), atof(argv[3]), atof(argv[4]), atof(argv[5]));
			fclose(f);
			break;
		default:
			printf("Erro, tente novamente!");
		}
	}
	return 0;
}