#include "StdAfx.h"
#include "ItemPesquisavel.h"

ListaItems ItemPesquisavel::TODOS;

ListaItems& ItemPesquisavel::getTodosItems()
{
	return TODOS;
}

ItemPesquisavel::ItemPesquisavel() : id(Util::gerarId())
{
}

ItemPesquisavel::~ItemPesquisavel(void)
{
}

Id ItemPesquisavel::getId()
{
	return id;
}

// retorna um item pesquisavel com um tipo pre-existente na BD
ItemPesquisavel* ItemPesquisavel::getItemTipo(Id id)
{
	ItemPesquisavel* item = new ItemPesquisavel;
	item->tipo = CampoDados::getListaCampos().get(id);
	return item;
}

// identico à funcao anterior mas por texto em vez de por id
ItemPesquisavel* ItemPesquisavel::getItemTipo(string tipo)
{
	ItemPesquisavel* item = new ItemPesquisavel;
	item->tipo = CampoDados::getListaCampos().get(tipo);
	return item;
}

// retorna a representacao textual do tipo
string ItemPesquisavel::getTipo()
{
	return CampoDados::getListaCampos().getNome(*tipo);
}

// adiciona um campo de dados de detalhe a este item
void ItemPesquisavel::adicionarDados(string nome, string texto)
{
	CampoDados* c = dados.get(nome);
	c->setTexto(texto);
	dados.adicionar(c);
}

// retorna o texto de um determinado campo de dados
string ItemPesquisavel::getDados(string nome)
{
	return dados.get(nome)->getTexto();
}

// remove um campo de dados por nome
void ItemPesquisavel::removerDados(string nome)
{
	dados.remover(nome);
}

// relaciona este item com outro e vice-versa
void ItemPesquisavel::adicionarItemRelacionado(Id id, int ordem)
{
	ItemPesquisavel* item = ItemPesquisavel::getTodosItems().get(id);
	if(item != 0)
	{
		itemsRelacionados.adicionar(item, ordem);
		item->itemsRelacionados.adicionar(this);
	}
}

// remove a relacao entre este item e outro
bool ItemPesquisavel::removerItemRelacionado(Id id)
{
	return itemsRelacionados.remover(id);
}

// gera a lista de palavras a usar para o indice de pesquisa
vector<string> ItemPesquisavel::getPalavrasIndice(bool relacionados)
{
	vector<string> palavras;
	map<Id, CampoDados*> campos = dados.getCampos();
	map<Id, CampoDados*>::iterator iter = campos.begin();
	stringstream s;
	// transforma o id em hexadecimal para ficar mais curto e mais facil de introduzir
	s << hex << id;
	palavras.push_back(s.str());
	// percorre todos os campos e usa o texto de cada um para indexar
	while(iter != campos.end())
	{
		string texto = campos[iter->first]->getTexto();

		// parte o texto em palavras
		size_t atual;
		size_t proximo = -1;
		do
		{
			atual = proximo + 1;
			proximo = texto.find_first_of( ' ', atual );
			string s = texto.substr( atual, proximo - atual);
			// só utilizar palavras com mais de 3 caracteres (ignorar 'the', 'of', etc)
			if(s.length() > 3)
			{
				palavras.push_back(s);
			}
			// utilizar também o texto parcial, da palavra atual até ao final
			string s2 = texto.substr(atual);
			if(s != s2)
				palavras.push_back(s2);
		}
		while (proximo != string::npos);

		// caso se esteja a indexar os items relacionados, parar após o primeiro campo
		// o primeiro campo tem sempre nome vazio e representa o nome/titulo do item
		if(relacionados)
			break;
		else
			iter++;
	}
	// caso seja o item a indexar, percorrer todos os items relacionados e indexar o titulo a este item
	// mas só no caso de o tipo ser diferente. assim ficam indexados os titulos de albums e musicas a
	// um artista mas não o nome de um artista relacionado, por ex.
	if(!relacionados)
	{
		map<Id, pair<ItemPesquisavel*, int>>& listaItems = itemsRelacionados.getListaItems();
		map<Id, pair<ItemPesquisavel*, int>>::iterator iter = listaItems.begin();
		while(iter != listaItems.end() )
		{
			ItemPesquisavel* item = listaItems[iter->first].first;
			if(this->getTipo() != item->getTipo())
			{
				vector<string> palavras2 = item->getPalavrasIndice(true);
				palavras.insert( palavras.end(), palavras2.begin(), palavras2.end() );
			}
			iter++;
		}
	}
	return palavras;
}

// método que faz a indexação em si
void ItemPesquisavel::indexar()
{
	// por cada palavra gerada, obtem-se os prefixos e sufixos e adiciona-se ao indice
	vector<string> palavras = getPalavrasIndice(false);
	for(vector<string>::iterator it = palavras.begin(); it != palavras.end(); ++it)
	{
		string palavra = *it;
		int tamanhoPalavra = palavra.length();
		for(int i = 1; i <= tamanhoPalavra; i++)
		{
			if(palavra[i] != ' ')
			{
				BaseDados::adicionarIndice(palavra.substr(0, i), this);
			}
			if(palavra[tamanhoPalavra - i] != ' ')
			{
				BaseDados::adicionarIndice(palavra.substr(tamanhoPalavra - i), this);
			}
		}
	}
}

void ItemPesquisavel::escreverFicheiro(ofstream& out)
{
	out.write((char*)&id, sizeof(id));
	out.write("\n", 1);
	Id idTipo = tipo->getId();
	out.write((char*)&idTipo, sizeof(idTipo));
	out.write("\n", 1);
	dados.escreverCamposFicheiro(out);
	itemsRelacionados.escreverIdsFicheiro(out);
}

ItemPesquisavel* ItemPesquisavel::lerFicheiro(ifstream& in)
{
	Id id;
	in.read((char*)&id, sizeof(id));
	in.ignore();
	Id idTipo;
	in.read((char*)&idTipo, sizeof(idTipo));
	in.ignore();
	ItemPesquisavel* item = ItemPesquisavel::getItemTipo(idTipo);
	if(item == 0)
		item = ItemPesquisavel::getItemTipo("");
	item->id = id;
	item->dados.lerCamposFicheiro(in);
	item->itemsRelacionados.lerIdsFicheiro(in, item);
	item->indexar();
	return item;
}

ostream& ItemPesquisavel::escreverEcraLista(ostream& out)
{
	out << hex << id << " [" << getTipo() << "] ";
	out << dados.get("")->getTexto() << endl;
	
	return out;
}

ostream& operator<<(ostream& out, ItemPesquisavel& itemPesquisavel)
{
	out << hex << itemPesquisavel.id << " [" << itemPesquisavel.getTipo() << "]";
	out << endl;
	out << itemPesquisavel.dados << endl;
	if(itemPesquisavel.itemsRelacionados.getListaItems().size() > 0)
	{
		out << "Relacionados:" << endl;
		out << itemPesquisavel.itemsRelacionados;
	}
	out << string(80, '-');
    return out;
}
