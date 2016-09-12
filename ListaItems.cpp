#include "StdAfx.h"
#include "ListaItems.h"

ListaItems::ListaItems(void)
{
}

ListaItems::~ListaItems(void)
{
}

map<Id, pair<ItemPesquisavel*, int>>& ListaItems::getListaItems()
{
	return listaItems;
}

ItemPesquisavel* ListaItems::get(Id id)
{
	map<Id, pair<ItemPesquisavel*, int>>::iterator it;
	it = listaItems.find(id);
	if(it != listaItems.end() )
		return it->second.first;
	return 0;
}

void ListaItems::adicionar(ItemPesquisavel* item, int ordem)
{
	listaItems[item->getId()] = make_pair(item, ordem);
}

bool ListaItems::remover(Id id)
{
	if(listaItems.find(id) != listaItems.end())
	{
		listaItems.erase(id);
		return true;
	}
	return false;
}

void ListaItems::escreverItemsFicheiro(ofstream& out)
{
	int size = listaItems.size();
	out.write((char*)&size, sizeof(size));
	out.write("\n", 1);
	map<Id, pair<ItemPesquisavel*, int>>::iterator iter = listaItems.begin();
	while(iter != listaItems.end() )
	{
		listaItems[iter->first].first->escreverFicheiro(out);
		//out.write("\n", 1);
		iter++;
	}
}

void ListaItems::lerItemsFicheiro(ifstream& in)
{
	int size = 0;
	in.read((char*)&size, sizeof(size));
	in.ignore();
	while(size-- > 0)
	{
		ItemPesquisavel* item = ItemPesquisavel::lerFicheiro(in);
		this->adicionar(item);
	}
}

void ListaItems::escreverIdsFicheiro(ofstream& out)
{
	int size = listaItems.size();
	out.write((char*)&size, sizeof(size));
	out.write("\n", 1);
	map<Id, pair<ItemPesquisavel*, int>>::iterator iter = listaItems.begin();
	while(iter != listaItems.end() )
	{
		out.write((char*)&iter->first, sizeof(iter->first));
		out.write("\n", 1);
		out.write((char*)&iter->second.second, sizeof(iter->second.second));
		out.write("\n", 1);
		iter++;
	}
}

void ListaItems::lerIdsFicheiro(ifstream& in, ItemPesquisavel* item)
{
	int size = 0;
	in.read((char*)&size, sizeof(size));
	in.ignore();
	while(size-- > 0)
	{
		Id id;
		in.read((char*)&id, sizeof(id));
		in.ignore();
		ItemPesquisavel* itemRelacionado = ItemPesquisavel::getTodosItems().get(id);
		int ordem;
		in.read((char*)&ordem, sizeof(ordem));
		in.ignore();
		if(itemRelacionado != 0)
		{
			item->adicionarItemRelacionado(itemRelacionado->getId(), ordem);
		}
	}
}

ostream& operator<<(ostream& out, ListaItems& listaItems)
{
	map<Id, pair<ItemPesquisavel*, int>>& lista = listaItems.getListaItems();
	map<Id, pair<ItemPesquisavel*, int>>::iterator iter = lista.begin();

	// não saem ordenados, não encontrei forma de ordenar a estrutura de dados
	// de acordo com a ordem especificada
	while(iter != lista.end() )
	{
		iter->second.first->escreverEcraLista(out);
		iter++;
	}
	return out;
}
