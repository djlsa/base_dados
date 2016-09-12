// BaseDadosMusica.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

// transforma um id hex em decimal
Id getId(string arg)
{
	Id id = 0;
	sscanf_s(arg.c_str(), "%x", &id); 
	return id;
}

// espera o input do utilizador para confirmar sim ou não
bool confirmar()
{
	cout << " S/N";
	int c = ' ';
	while(c != 's' && c != 'S' && c != 'n' && c != 'N')
		c = _getch();
	if(c == 's' || c == 'S')
		return true;
	return false;
}

int main(int argc, char* argv[])
{
	// coloca os argumentos num vector
	vector<string> args(argv, argv + argc);

	// ficheiro especificado
	if(argc > 1)
	{
		string nomeFicheiro = args[1];
		BaseDados::lerFicheiro(nomeFicheiro);

		// comando especificado
		if(argc > 2)
		{
			string comando = args[2];
			Util::toLower(comando);
			Id id = getId(comando);

			// adicionar item
			if(comando == "a" && argc >= 5)
			{
				string tipo = args[3];
				stringstream s;
				for(int i = 4; i < argc; i++)
				{
					if(i != 4)
						s << " ";
					s << args[i];
				}
				string nome = s.str();
				BaseDados::adicionarItem(tipo, nome)->escreverEcraLista(cout);
				cout << " adicionado com sucesso";
				exit(EXIT_SUCCESS);
			}
			// remover item
			else if(comando == "r" && argc >= 4)
			{
				Id idItem = getId(args[3]);
				if(idItem > 0)
				{
					ItemPesquisavel* item = ItemPesquisavel::getTodosItems().get(idItem);
					if(item != 0)
					{
						cout << "Apagar ";
						item->escreverEcraLista(cout);
						cout << " ?";
						if(confirmar())
							BaseDados::removerItem(idItem);
						exit(EXIT_SUCCESS);
					}
				}
			}
			// adicionar detalhe
			else if(comando == "ad" && argc >= 6)
			{
				Id idItem = getId(args[3]);
				if(idItem > 0)
				{
					ItemPesquisavel* item = ItemPesquisavel::getTodosItems().get(idItem);
					if(item != 0)
					{
						string nome = args[4];
						stringstream s;
						for(int i = 5; i < argc; i++)
						{
							if(i != 5)
								s << " ";
							s << args[i];
						}
						string texto = s.str();
						BaseDados::adicionarDados(idItem, nome, texto);
						cout << "Dados adicionados com sucesso" << endl;
						cout << *item;
						exit(EXIT_SUCCESS);
					}
				}
			}
			// remover detalhe
			else if(comando == "rd" && argc >= 5)
			{
				Id idItem = getId(args[3]);
				if(idItem > 0)
				{
					ItemPesquisavel* item = ItemPesquisavel::getTodosItems().get(idItem);
					if(item != 0)
					{
						string nome = args[4];
						cout << *item;
						cout << "Apagar dados " << nome << " ?";
						if(confirmar())
							BaseDados::removerDados(idItem, nome);
						cout << "Dados removidos com sucesso" << endl;
						cout << *item;
						exit(EXIT_SUCCESS);
					}
				}
			}
			// adicionar relacao
			else if(comando == "ar" && argc >= 5)
			{
				Id idItem1 = getId(args[3]);
				Id idItem2 = getId(args[4]);
				if(idItem1 > 0 && idItem2 > 0)
				{
					ItemPesquisavel* item = ItemPesquisavel::getTodosItems().get(idItem1);
					if(item != 0)
					{
						bool sucesso = false;
						if(argc == 5)
							sucesso = BaseDados::adicionarRelacao(idItem1, idItem2);
						else
						{
							stringstream s(args[5]);
							int ordem;
							s >> ordem;
							sucesso = BaseDados::adicionarRelacao(idItem1, idItem2, ordem);
						}
						if(sucesso)
						{
							cout << "Items relacionados com sucesso" << endl;
							cout << *item;
							exit(EXIT_SUCCESS);
						}
					}
				}
			}
			// remover relacao
			else if(comando == "rr" && argc >= 5)
			{
				Id idItem1 = getId(args[3]);
				Id idItem2 = getId(args[4]);
				if(idItem1 > 0 && idItem2 > 0)
				{
					ItemPesquisavel* item = ItemPesquisavel::getTodosItems().get(idItem1);
					if(item != 0)
					{
						if(BaseDados::adicionarRelacao(idItem1, idItem2))
						{
							cout << "Relacao removida com sucesso" << endl;
							cout << *item;
							exit(EXIT_SUCCESS);
						}
					}
				}
			}
			// ver detalhe, tambem é possivel executar sem o comando, especificando apenas o id
			else if(comando == "vd" || id > 0)
			{
				ItemPesquisavel* item = ItemPesquisavel::getTodosItems().get(id);
				if(item != 0)
					cout << *item;
				exit(EXIT_SUCCESS);
			}
			// fazer pesquisa, também é possivel omitir o comando "p" mas só caso as letras de pesquisa não
			// coincidam com um numero hexadecimal
			else if(comando == "p" || id == 0)
			{
				stringstream s;
				int inicio = (comando == "p") ? 3 : 2;
				for(int i = inicio; i < argc; i++)
				{
					if(i != inicio)
						s << " ";
					s << args[i];
				}
				cout << BaseDados::pesquisar(s.str());
				exit(EXIT_SUCCESS);
			}
		}
	}

	// em qualquer outro caso, exibir as instrucoes de utilização

	cout << "Instrucoes:" << endl << endl;
	cout << "BaseDadosMusica nome_ficheiro [p] termos_de_pesquisa" << endl << "Efetua uma pesquisa na base de dados" << endl << endl;
	cout << "BaseDadosMusica nome_ficheiro [vd] id_hexadecimal" << endl << "Ver detalhes do item com o id especificado" << endl << endl;
	cout << "BaseDadosMusica nome_ficheiro a palavra_tipo nome" << endl << "Adiciona um item com tipo e nome especificados" << endl;
	cout << "\tEx.: BaseDadosMusica musica.bd a Artista Nome do Artista" << endl << endl;
	cout << "BaseDadosMusica nome_ficheiro r id_hexadecimal" << endl << "Remove o item com o id especificado" << endl << endl;
	cout << "BaseDadosMusica nome_ficheiro ad id_hexadecimal nome texto" << endl << "Adiciona um detalhe a um item" << endl;
	cout << "\tEx.: BaseDadosMusica nome_ficheiro ff00cc Ano 2013" << endl << endl;
	cout << "BaseDadosMusica nome_ficheiro rd id_hexadecimal nome" << endl << "Remove o detalhe com o nome indicado do item especificado" << endl << endl;
	cout << "BaseDadosMusica nome_ficheiro ar id_hexadecimal_1 id_hexadecimal_2 numero_ordem" << endl << "Adiciona relacao entre os items com os ids especificados, indicando uma ordem (util para definir numero de ordem de musicas em albuns, colocar o artista de um determinado album no topo dos items relacionados (ordem 0), etc" << endl << endl;
	cout << "BaseDadosMusica nome_ficheiro rr id_hexadecimal_1 id_hexadecimal_2" << endl << "Remove a relacao entre os items com os ids especificados" << endl << endl;

	return 0;
}
