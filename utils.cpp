/**\file utils.cpp
 * \brief Ficheiro com a implementa�o do namespace utils.
 *
 * @version Projecto 2 - Programa�o - Domino - 10-11 - MIEIC
 * @author Carlos Miguel Correia da Costa - ei09097
 */



#include "utils.h"


/**
 * \brief Limpa as flags do cin e fica � espera de um input para prosseguir.
 * \details Serve para parar o programa e esperar que o utilizador fa�a algum input para prosseguir.
 *          Evita user system("pause"), que n�o � portable, n�o � standard e � muito mais "pesado" para o programa...
 */
void utils::getUserInput() {
	cout << "Press ENTER to continue..." << endl;

	//Limpa as flags do cin
	cin.clear();

	cin.sync(); // http://www.cplusplus.com/forum/beginner/1988/page5.html

	//String tempor�ria para guardar os dados lidos
	string temp;

	//Obten�o de uma linha de input para continuar
	getline(cin, temp);

	cin.clear();
	cin.sync();
}


/**
 * getline que faz o sync para n�o ler lixo do buffer.
 * @return Retorna a string com a linha lida do cin.
 */
string utils::getlineCin() {
	cin.clear();
	cin.sync();

	string input;
	getline(cin, input);

	cin.clear();
	cin.sync();

	return input;
}


/**
 * M�todo que limpa o ecr� da consola.
 * Evita usar system("cls"), que n�o � portable, n�o � standard e � muito mais "pesado" para o programa...
 */
void utils::clearConsoleScreen() {
	for (size_t i = 0; i < 80; ++i) {
		cout << "\n";
	}

	cout << endl;
}


/**
 * \brief Mostra a "message" ao utilizador e espera por um input de um n�mero entre min (default � 0) e size-1 (default � INT_MAX).
 * \details Enquanto n�o for introduzido um input v�lido vai pedindo ao utilizador para introduzir novamente os dados mostrando a errorMessage.
 */
int utils::getIntCin(const char* message, const char* errorMessage, int min, int size) {

	int numero;
	do {
		cout << message;

		/*
		 * Enquanto cin devolver um apontador NULL (e como tal no teste do if dar true (por estar negado)),
		 * devido ao facto de estar a ler dados que n�o s�o n�meros, (ex: caracteres...)
		 * informa ao utilizador que tem que introduzir um n� e pede para introduzir os dados novamente.
		 */
		if (!(cin >> numero)) {

			//Limpa as flags do cin
			cin.clear();

			//Extrai e ignora todos os caracteres que estavam no buffer de cin, visto que ao tentar-se introduzir
			//caracteres num int (por ex), o cin entra em estado de erro e n�o remove os caracteres que leu do buffer
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

			cout << errorMessage << endl;
		} else {

			//Testa se os n�meros introduzidos est�o dentro do intervalo pedido
			if (numero >= min && numero < size)
				break; //Se estiverem sai-se do loop
			else
				cout << errorMessage << endl; //Sen�o informasse o utilizador e pede-se novamente o n�
		}

	} while (true);

	cin.clear();
	cin.sync();

	return numero;
}


/**
 * M�todo que pede ao utilizador uma resposta booleana, esperando que este introduza 'S' ou 'N'
 * @param message mensagem a mostrar antes de pedir o input
 * @param message_erro mensagem a mostrar caso o usar introduza uma letra diferente de 'S' ou 'N'
 * @return true caso o user tenha introduzido S ou false caso tenha introduzido N
 */
bool utils::getYesNoCin(const char* message, const char* message_erro)
{
	bool continuar = false;

	//Pergunta ao utilizador se quer continuar (S/N)
	bool opcao_incorrecta;
	string opcao;

	do {
		cout << message;

		opcao = getlineCin();
		if ((opcao == "S") || (opcao == "s")) {
			continuar = true;
			opcao_incorrecta = false;
		}
		else if ((opcao == "N") || (opcao == "n")) {
			continuar = false;
			opcao_incorrecta = false;
		}
		else {
			cout << message_erro;
			opcao_incorrecta = true;
		}
	} while (opcao_incorrecta);

	cin.clear();
	cin.sync();

	return continuar;
}


/**
 * M�todo que converte uma string para mai�sculas
 * @param str string a converter
 * @return string convertida
 */
string utils::toUpper(string str) {
	for (string::size_type i = 0; i < str.size(); i++) {
		str[i] = toupper(str[i]);
	}

	return str;
}


/**
 * M�todo que converte uma string para min�sculas
 * @param str string a converter
 * @return string convertida
 */
string utils::toLower(string str) {
	for (string::size_type i = 0; i < str.size(); i++) {
		str[i] = tolower(str[i]);
	}

	return str;
}


/**
 * M�todo que divide uma string em substrings usando um coracter delimitador que � fornecido
 * @param in string a analisar
 * @param delim delimitador a usar na separa�o da string
 * @return vector <string> com as strings que antes estavam todas juntas em "in" separadas pelo delimitador
 */
vector<string> utils::split(string str, char delim) {
	vector<string> result;
	size_t i = 0, f;

	//Enquanto houver delimitadores em str, vai-se partindo str em substrings
	while (i < str.size() && ((f = str.find(delim, i)) != string::npos)) {
		result.push_back(str.substr(i, f - i));
		i = f + 1;
	}

	//Quando n�o houver mais delimitadores, adiciona-se o resto da str
	result.push_back(str.substr(i));
	return result;
}


/**
 * \brief M�todo que imprime uma tabela com os dados de um vector de colunas (vector de strings).
 *
 * @param title Titulo da tabela.
 * @param columns Vector de colunas.
 * @param colNames Vector com o nome das colunas.
 * @param reverse Caso seja true, inverte-se a ordem dos dados na coluna (mostra a tabela invetida - linhas 0 1 2 3 passam a 3 2 1 0).
 * @param hide_index Caso seja true, n�o imprime a numera�o das linhas.
 * @return Um booleano caso tenha sido feito algum output de dados.
 */
bool utils::printMenu(string & title, const vector<vector<string> > & columns, const vector<string> & colNames, bool menu, bool reverse, bool hide_index, bool hide_colnames) {
	//C�lculo do maior indice que existe em todas as colunas
	size_t indice_max(0);
	for (size_t indice = 0; indice < columns.size(); ++indice)
		indice_max = max(indice_max, columns[indice].size());

	//C�lculo do tamanho do indice m�ximo (em n� de caracteres)
	stringstream temp;
	temp << indice_max;
	size_t tamanho_indice_max = temp.str().size();

	//C�lculo do tamanho da string de maior comprimento em cada coluna
	vector<size_t> tamanho_max_strings;
	for (size_t indice_coluna = 0; indice_coluna < columns.size(); ++indice_coluna) {
		size_t string_max(0);
		for (size_t indice_string = 0; indice_string < columns[indice_coluna].size(); ++indice_string)
			string_max = (size_t)max(string_max, columns[indice_coluna][indice_string].size());
		tamanho_max_strings.push_back(string_max);
	}

	if(!hide_colnames)
	{
		//C�lculo do tamanho da string de maior comprimento em cada coluna para os nomes das colunas
		for (size_t indice_coluna = 0; indice_coluna < colNames.size(); ++indice_coluna) {
			if(tamanho_max_strings[indice_coluna] < colNames[indice_coluna].size() )
				tamanho_max_strings[indice_coluna] = colNames[indice_coluna].size();
		}
	}

	int n_linhas(0);	//Indica o numero de linhas j� mostradas no ecr�
	stringstream separador;
	stringstream nome_colunas;

	//Se o n�mero de colunas for n�o nulo e igual ao n� de nomes de colunas imprime-se a tabela
	if (columns.size() == colNames.size() && columns.size() > 0) {
		//>> Impress�o do t�tulo da tabela <<
		cout << title;
		++n_linhas;

		if(!hide_colnames)
		{
			cout << endl;
		}

		if(!hide_index)
		{
			//>> Cria o separador tendo um tra�o a mais do que o necess�rio no inicio e fim de cada coluna <<
			//Coluna dos n�meros dos �ndices
			separador << "+-";
			for (size_t coluna_num = 0; coluna_num < tamanho_indice_max; ++coluna_num)
				separador << "-";
			separador << "-"; //Tra�o extra no fim da coluna
		}

		//Restantes colunas
		for (size_t coluna = 0; coluna < tamanho_max_strings.size(); ++coluna) {
			separador << "+-"; //Inicio do separdor, com mais um tra�o no inicio
			for (size_t tracos = 0; tracos < tamanho_max_strings[coluna]; ++tracos) {
				separador << "-";
			}
			separador << "-"; //Tra�o extra no fim da coluna
		}
		separador << "+"; //Finaliza�o da �ltima coluna


		if(!hide_colnames)		// se imprimir indices
		{
			//>> 1� separador <<
			cout << separador.str() << endl;
			++n_linhas;
		}

		if(!hide_index && !hide_colnames)
		{
			//>> Cria�o da segunda linha com o nome das colunas <<
			nome_colunas << "| " << setw(tamanho_indice_max) << "#" << " |";
		}

		if(!hide_colnames)
		{
			if(hide_index)
				cout << "|";

			//Mostra os nomes das colunas
			for (size_t i = 0; i < colNames.size(); ++i) {
				nome_colunas << " " << setw(tamanho_max_strings[i]) << colNames[i] << " |";
			}
		}

		//>> 2� linha <<
		cout << nome_colunas.str() << endl;
		++n_linhas;

		//>> 2� separador <<
		cout << separador.str() << "\n";
		++n_linhas;


		//Se as colunas tiverem elementos, mostrasse os conte�dos
		for (size_t j = 0; j < indice_max; ++j) {
			//Se reverse for true, trocasse a ordem, invertendo a ordem dos dados na coluna
			//Em vez de ler do indice 0 a size-1, l�-se de size-1 a 0 do vector de colunas
			size_t indice = reverse ? indice_max - 1 - j : j;

			//Coluna do n� da linha
			if(!hide_index) {
				cout << "| " << setw(tamanho_indice_max);
				if (menu)
					cout << (indice + 1) << " |";
				else
					cout << indice << " |";
			}
			else
				cout << "|";

			//Restantes colunas
			for (size_t i = 0; i < columns.size(); ++i) {
				cout << " " << setw(tamanho_max_strings[i]);

				//Se o �ndice for v�lido, mostra-se o conte�do do vector nessa posi�o, sen�o fica em branco
				if (indice < columns[i].size())
					cout << columns[i][indice];
				else
					cout << "";

				cout << " |";
			}

			if(n_linhas >= 24) {		//Imprime por p�ginas de 24 linhas
				n_linhas = 0;			//Reinializa o contador
				getUserInput();
			}

			//Para evitar descontinuidades na tabela, porque o getch() pede um "\n" ao utilizador.
			//Sem este teste haveria duas mudan�as de linha na tabela quando s� deveria haver uma
			//(evitando assim uma linha em branco na tabela)
			if (n_linhas != 0)
				cout << endl;

			++n_linhas;
		}

		if(n_linhas!=1) {				//Caso j� tenha imprimido o �ltimo separador, n�o volta a imprimir
			//Separador inferior
			cout << separador.str();
		}

		return true;
	} else
		//Caso contr�rio n�o se mostra a tabela e returna-se falso, para evidenciar que n�o houve output de dados
		return false;
}
