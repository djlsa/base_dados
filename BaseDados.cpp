#include "StdAfx.h"
#include "BaseDados.h"

map<Id, ListaItems> BaseDados::INDICE;
string BaseDados::NOME_FICHEIRO;

BaseDados::BaseDados(void)
{
}

BaseDados::~BaseDados(void)
{
}

// adiciona ao indice, indexando por hashcode da string em lowercase
void BaseDados::adicionarIndice(string str, ItemPesquisavel* item)
{
	Util::toLower(str);
	INDICE[Util::hashCode(str)].adicionar(item);
}

// adiciona item à base de dados, utilizando o campo vazio para definir o nome
ItemPesquisavel* BaseDados::adicionarItem(string tipo, string nome)
{
	ItemPesquisavel* item = ItemPesquisavel::getItemTipo(tipo);
	item->adicionarDados("", nome);
	ItemPesquisavel::getTodosItems().adicionar(item);
	BaseDados::escreverFicheiro();
	return item;
}

// remove um item da base de dados
void BaseDados::removerItem(Id id)
{
	if(ItemPesquisavel::getTodosItems().remover(id))
		BaseDados::escreverFicheiro();
}

// adiciona dados de detalhe a um item
bool BaseDados::adicionarDados(Id id, string nome, string texto)
{
	ItemPesquisavel* item = ItemPesquisavel::getTodosItems().get(id);
	if(item != 0)
	{
		item->adicionarDados(nome, texto);
		BaseDados::escreverFicheiro();
		return true;
	}
	return false;
}

// remove detalhes de um item
bool BaseDados::removerDados(Id id, string nome)
{
	ItemPesquisavel* item = ItemPesquisavel::getTodosItems().get(id);
	if(item != 0)
	{
		item->removerDados(nome);
		BaseDados::escreverFicheiro();
		return true;
	}
	return false;
}

// associa dois items
bool BaseDados::adicionarRelacao(Id id1, Id id2, int ordem)
{
	ItemPesquisavel* item1 = ItemPesquisavel::getTodosItems().get(id1);
	if(item1 != 0)
	{
		item1->adicionarItemRelacionado(id2, ordem);
		BaseDados::escreverFicheiro();
		return true;
	}
	return false;
}

// remove a associacao entre dois items (nao esta a funcionar por alguma razao...)
bool BaseDados::removerRelacao(Id id1, Id id2)
{
	ItemPesquisavel* item1 = ItemPesquisavel::getTodosItems().get(id1);
	bool sucesso1 = false;
	if(item1 != 0)
	{
		sucesso1 = item1->removerItemRelacionado(id2);
	}
	ItemPesquisavel* item2 = ItemPesquisavel::getTodosItems().get(id2);
	bool sucesso2 = false;
	if(item2 != 0)
	{
		sucesso2 = item2->removerItemRelacionado(id1);
	}
	if(sucesso1 || sucesso2)
		BaseDados::escreverFicheiro();
	return true;
}

// efetua uma pesquisa
ListaItems BaseDados::pesquisar(string str)
{
	Util::toLower(str);
	return INDICE[Util::hashCode(str)];
}

// grava a base de dados para ficheiro
void BaseDados::escreverFicheiro()
{
	ofstream ficheiro;
	ficheiro.open(NOME_FICHEIRO.c_str(), ios::binary | ios::trunc);
	if(!ficheiro)
	{
		cout << "Erro: Nao foi possivel ao abrir " << NOME_FICHEIRO << " para escrita" << endl;
		exit(EXIT_FAILURE);
	}
	CampoDados::getListaCampos().escreverNomesFicheiro(ficheiro);
	ItemPesquisavel::getTodosItems().escreverItemsFicheiro(ficheiro);
	ficheiro.close();
}

// carrega a base de dados a partir de ficheiro
void BaseDados::lerFicheiro(string nomeFicheiro)
{
	NOME_FICHEIRO = nomeFicheiro;
	ifstream ficheiro;
	ficheiro.open(nomeFicheiro.c_str(), ios::binary);
	if(!ficheiro)
	{
		cout << "A criar nova base de dados " << nomeFicheiro << endl;
		BaseDados::escreverFicheiro();
		return;
	}
	CampoDados::getListaCampos().lerNomesFicheiro(ficheiro);
	ItemPesquisavel::getTodosItems().lerItemsFicheiro(ficheiro);
	ficheiro.close();
}