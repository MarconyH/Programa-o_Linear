#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

int variaveis_num = 4;
int restricoes_num = 2;

float **forma_padrao(float **A);
float ***separar_matrizes(float **A, float *c);
float **excesso_folga_artificial(float **A, int *v, int count);
float **criar_matriz(int linhas, int colunas);
float *copy(float *matriz2, int tam);
float **copy(float **matriz2, int linha, int coluna);
bool otima(float *Cr, int *posicao);
void selecionar_entra_sai(float **R, float *b, int coluna, int *linha);
// void troca_coluna(float ***R, float ***B, float **Cr, float **Cb, int coluna, int linha);
float ***troca_coluna(float **R, float **B, float *Cr, float *Cb, int coluna, int linha);
void vetor_csv(float *vetor, int tamanho, string &caminhoArquivo);
void matriz_csv(float **matriz, int linhas, int colunas, string &caminhoArquivo);
float *ler_vetor_csv(int tamanho, const string &caminhoArquivo, bool pularLinha = true)
float **ler_matriz_csv(int linhas, int colunas, const string &caminhoArquivo, bool pularLinha = true)
void simplex(float **A, float *b, float *c);

int main()
{
	//{{5.0, 20.0, 1.0, 0, -1.0}, {10.0,15.0, 0, 1.0, -1.0}}
	// float A[2][4] = {{5.0, 10.0}, {20.0,15.0}, {1.0, 0}, {0,1.0}};
	float **A = criar_matriz(restricoes_num, variaveis_num + 1);
	A[0][0] = 5.0;
	A[0][1] = 20.0;
	A[0][2] = 1.0;
	A[0][3] = 0;
	A[0][4] = -1;
	A[1][0] = 10.0;
	A[1][1] = 15.0;
	A[1][2] = 0;
	A[1][3] = 1.0;
	A[1][4] = -1;
	float c[4] = {-45.0, -80.0, 0, 0};
	float b[2] = {400.0, 450.0};
	for (int i = 0; i < restricoes_num; i++)
	{
		for (int j = 0; j < variaveis_num + 1; j++)
		{
			cout << A[i][j] << " ";
		}
		cout << endl;
	}
	simplex(A, b, c);
	return 0;
}

float **criar_matriz(int linhas, int colunas)
{
	float **matriz = new float *[linhas];
	for (int i = 0; i < linhas; i++)
	{
		matriz[i] = new float[colunas];
	}
	return matriz;
}

float **excesso_folga_artificial(float **A, int *v, int count)
{
	// criando matriz dinamicamente
	float **A_new = criar_matriz(restricoes_num, variaveis_num + count);
	int temp = count;

	for (int i = 0; i < restricoes_num; i++)
	{
		for (int j = 0; j < variaveis_num + count; j++)
		{
			A_new[i][j] = 0;
		}
	}

	for (int i = 0; i < restricoes_num; i++)
	{
		for (int j = 0; j < variaveis_num + count; j++)
		{
			if (j < variaveis_num)
			{
				A_new[i][j] = A[i][j];
			}
			else
			{
				// A_new[i][j] = v[i];
				if (j < variaveis_num + count - 1)
				{
					continue;
				}
				else
				{
					if (v[i] == 1)
					{
						A_new[i][j] = -1;
						A_new[i][j - 1] = 1;
						count = count - 2;
					}
					else if (v[i] == -1)
					{
						A_new[i][j] = 1;
						count--;
					}
					else
					{
						A_new[i][j] = 0;
					}
				}
			}
		}
	}
	variaveis_num += temp;
	return A_new;
}

float **forma_padrao(float **A)
{
	// posição das variáveis de excesso ou folga
	int v[restricoes_num];
	// quantidade de variáveis de excesso ou folga
	int count = 0;
	for (int i = 0; i < restricoes_num; i++)
	{
		if (A[i][variaveis_num + 1] != 0)
		{
			// variavel de excesso
			if (A[i][variaveis_num + 1] == 1)
			{
				v[i] = -1;
				count++;
			}
			// variavel de folga e artificiais
			else
			{
				v[i] = 1;
				count = count + 2;
			}
		}
		else
		{
			v[i] = 0;
		}
	}
	return excesso_folga_artificial(A, v, count);
}

float ***separar_matrizes(float **A, float *c)
{
	float *Cb = new float[restricoes_num];
	float *Cr = new float[variaveis_num - restricoes_num];
	float **R = criar_matriz(restricoes_num, variaveis_num - restricoes_num);
	float **B = criar_matriz(restricoes_num, restricoes_num);
	int number_cb = 0;
	int number_cr = 0;
	bool coluna_cr = false;
	for (int j = 0; j < variaveis_num; j++)
	{
		for (int i = 0; i < restricoes_num; i++)
		{
			if (coluna_cr || c[j] != 0 || (A[i][j] != 0 && A[i][j] != 1))
			{
				if (!coluna_cr)
				{
					i = 0;
				}
				Cr[number_cr] = c[j];
				R[i][number_cr] = A[i][j];
				coluna_cr = true;
			}
			else
			{
				Cb[number_cb] = c[j];
				B[i][number_cb] = A[i][j];
			}
		}
		if (coluna_cr)
		{
			number_cr++;
			coluna_cr = false;
		}
		else
		{
			number_cb++;
		}
	}
	float ***vector = new float **[4];
	vector[0] = &Cb;
	vector[1] = &Cr;
	vector[2] = B;
	vector[3] = R;
	return vector;
}

float *copy(float *matriz, int tam)
{
	float *matriz1 = new float[tam];
	for (int i = 0; i < tam; i++)
	{
		matriz1[i] = matriz[i];
	}
	return matriz1;
}

float **copy(float **matriz, int linha, int coluna)
{
	float **matriz1 = new float *[linha];
	for (int i = 0; i < linha; i++)
	{
		matriz1[i] = new float[coluna];
	}
	for (int i = 0; i < linha; i++)
	{
		for (int j = 0; j < coluna; j++)
		{
			matriz1[i][j] = matriz[i][j];
		}
	}
	return matriz1;
}

bool otima(float *Cr, int *posicao)
{
	bool otimo = true;
	for (int i = 0; i < variaveis_num - restricoes_num; i++)
	{
		cout << Cr[i];
		if (Cr[i] < 0)
		{

			otimo = false;
			if (Cr[i] < Cr[*posicao])
			{
				*posicao = i;
			}
		}
	}
	return otimo;
}

void selecionar_entra_sai(float **R, float *b, int coluna, int *linha)
{
	for (int i = 0; i < restricoes_num; i++)
	{
		if (b[i] / R[i][coluna] < b[*linha] / R[*linha][coluna])
		{
			*linha = i;
		}
	}
}

// void troca_coluna(float ***R, float ***B, float **Cr, float **Cb, int coluna, int linha)
// {
// 	cout << endl << "B: " << *B[1][0] << endl;
// 	int colunab;
// 	for (int i = 0; i < restricoes_num; i++)
// 	{
// 		if (*(B[linha][i]) == 1)
// 		{
// 			colunab = i;
// 		}
// 	}
// 	float temp;
// 	for (int i = 0; i < restricoes_num; i++)
// 	{
// 		temp = *(R[i][coluna]);
// 		*(R[i][coluna]) = *(B[i][colunab]);
// 		*(B[i][colunab]) = temp;
// 	}
// 	temp = *(Cr[coluna]);
// 	*(Cr[coluna]) = *(Cb[colunab]);
// 	*(Cb[colunab]) = temp;
// }
float ***troca_coluna(float **R, float **B, float *Cr, float *Cb, int coluna, int linha)
{
	int colunab;
	for (int i = 0; i < restricoes_num; i++)
	{
		if (B[linha][i] == 1)
		{
			colunab = i;
		}
	}
	float temp;
	for (int i = 0; i < restricoes_num; i++)
	{
		temp = R[i][coluna];
		R[i][coluna] = B[i][colunab];
		B[i][colunab] = temp;
	}
	temp = Cr[coluna];
	Cr[coluna] = Cb[colunab];
	Cb[colunab] = temp;

	float ***vector = new float **[4];
	vector[0] = &Cb;
	vector[1] = &Cr;
	vector[2] = B;
	vector[3] = R;
	return vector;
}

void vetor_csv(float *vetor, int tamanho, const string &caminhoArquivo)
{
	ofstream arquivo(caminhoArquivo);

	if (arquivo.is_open())
	{
		for (int i = 0; i < tamanho; ++i)
		{
			arquivo << vetor[i];
			if (i < tamanho - 1)
			{
				arquivo << ",";
			}
		}
		arquivo << endl;
		arquivo.close();
	}
	else
	{
		cerr << "Erro ao abrir o arquivo: " << caminhoArquivo << endl;
	}
}

// Função para escrever uma matriz em um arquivo CSV
void matriz_csv(float **matriz, int linhas, int colunas, const string &caminhoArquivo)
{
	ofstream arquivo(caminhoArquivo);

	if (arquivo.is_open())
	{
		for (int i = 0; i < linhas; ++i)
		{
			for (int j = 0; j < colunas; ++j)
			{
				arquivo << matriz[i][j];
				if (j < colunas - 1)
				{
					arquivo << ",";
				}
			}
			arquivo << endl;
		}
		arquivo.close();
	}
	else
	{
		cerr << "Erro ao abrir o arquivo: " << caminhoArquivo << endl;
	}
}

float *ler_vetor_csv(int tamanho, const string &caminhoArquivo, bool pularLinha = true)
{
	ifstream arquivo(caminhoArquivo);
	string linha;

	if (arquivo.is_open())
	{
		// Pula a primeira linha se pularLinha for true
		if (pularLinha)
		{
			getline(arquivo, linha);
		}

		// Lê a linha que contém os valores do vetor
		getline(arquivo, linha);

		int posicao_inicial = 0;
		for (int i = 0; i < tamanho; ++i)
		{
			size_t posicao_virgula = linha.find(',', posicao_inicial);

			// Pega a substring entre as posições inicial e da vírgula
			string valor = linha.substr(posicao_inicial, posicao_virgula - posicao_inicial);

			// Converte o valor para float e armazena no vetor
			vetor[i] = stof(valor);

			// Atualiza a posição inicial para a próxima iteração
			posicao_inicial = posicao_virgula + 1;
		}

		arquivo.close();
	}
	else
	{
		cerr << "Erro ao abrir o arquivo: " << caminhoArquivo << endl;
	}
}

// Função para ler uma matriz de um arquivo CSV
float **ler_matriz_csv(, int linhas, int colunas, const string &caminhoArquivo, bool pularLinha = true)
{
	ifstream arquivo(caminhoArquivo);
	string linha;

	if (arquivo.is_open())
	{
		// Pula a primeira linha se pularLinha for true
		if (pularLinha)
		{
			getline(arquivo, linha);
		}

		for (int i = 0; i < linhas; ++i)
		{
			// Lê cada linha da matriz
			getline(arquivo, linha);

			int posicao_inicial = 0;
			for (int j = 0; j < colunas; ++j)
			{
				size_t posicao_virgula = linha.find(',', posicao_inicial);

				// Pega a substring entre as posições inicial e da vírgula
				string valor = linha.substr(posicao_inicial, posicao_virgula - posicao_inicial);

				// Converte o valor para float e armazena na matriz
				matriz[i][j] = stof(valor);

				// Atualiza a posição inicial para a próxima iteração
				posicao_inicial = posicao_virgula + 1;
			}
		}

		arquivo.close();
	}
	else
	{
		cerr << "Erro ao abrir o arquivo: " << caminhoArquivo << endl;
	}
}

void simplex(float **A, float *b, float *c)
{
	float ***matrizes = separar_matrizes(forma_padrao(A), c);
	float *Cb = *matrizes[0];
	float *Cr = *matrizes[1];
	float **B = matrizes[2];
	float **R = matrizes[3];
	// variavel que guarda a coluna do menor elemento do Cr
	int coluna = 0;
	int linha = 0;
	float *Cb_temp = copy(Cb, restricoes_num);
	float *Cr_temp = copy(Cr, variaveis_num - restricoes_num);
	float **B_temp = copy(B, restricoes_num, restricoes_num);
	float **R_temp = copy(R, restricoes_num, variaveis_num - restricoes_num);
	float *b_temp = copy(b, restricoes_num);

	while (!otima(Cr_temp, &coluna))
	{
		selecionar_entra_sai(R_temp, b_temp, coluna, &linha);
		cout << endl
			 << "B: " << B_temp[1][0] << endl;
		matrizes = troca_coluna(R_temp, B_temp, Cr_temp, Cb_temp, coluna, linha);
		Cb_temp = *matrizes[0];
		Cr_temp = *matrizes[1];
		B_temp = matrizes[2];
		R_temp = matrizes[3];

		matriz_csv(B_temp, restricoes_num, restricoes_num, "B.csv");
		matriz_csv(R_temp, restricoes_num, variaveis_num - restricoes_num, "R.csv");
		vetor_csv(Cb_temp, restricoes_num, "Cb.csv");
		vetor_csv(Cr_temp, variaveis_num - restricoes_num, "Cr.csv");
		vetor_csv(b_temp, restricoes_num, "b.csv");
		// cr
		system("Rscript atualizar_nao_basicas_fo.r");
		ler_vetor_csv()
		// b
		system("Rscript novo_valor_basicas.r");
		// fo
		system("Rscript calcular_fo.r");
		break;
		// if(!otima(Cr_temp, &coluna))
		// {
		// 	system("coeficientes_variaveis_nao_basicas");
		// }
	}
}
