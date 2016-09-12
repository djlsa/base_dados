#pragma once

class ItemPesquisavel;

// classe que representa uma lista de items, tanto a lista global de todos os items como também
// a lista de items relacionados de cada item pesquisavel e ainda a lista de items gerada por uma pesquisa
class ListaItems
{
private:
	// indexada pelo id do item, guarda o par item-ordem para se poder mostrar por ex. as faixas de um album na ordem correta
	map<Id, pair<ItemPesquisavel*, int>> listaItems;
public:
	ListaItems(void);
	~ListaItems(void);

	map<Id, pair<ItemPesquisavel*, int>>& getListaItems();

	ItemPesquisavel* get(Id id);
	void adicionar(ItemPesquisavel* item, int ordem = numeric_limits<int>::max());
	bool remover(Id id);

	void escreverItemsFicheiro(ofstream& out);
	void lerItemsFicheiro(ifstream& in);
	void escreverIdsFicheiro(ofstream& out);
	void lerIdsFicheiro(ifstream& in, ItemPesquisavel* item);

	friend bool comparador(const ItemPesquisavel* a, const ItemPesquisavel* b);
	friend ostream& operator<<(ostream& out, ListaItems& listaDeItems);
};