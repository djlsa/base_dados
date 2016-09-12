#pragma once

class ListaItems;

// classe que representa um item pesquisavel qualquer. utiliza um campodados para definir o seu tipo
class ItemPesquisavel
{

private:
	Id id;
	CampoDados* tipo;
	ListaCamposDados dados;
	ListaItems itemsRelacionados;
	// lista de todos os items pesquisaveis usados pela BD para poderem ser referenciados por id
	static ListaItems TODOS;
	vector<string> getPalavrasIndice(bool relacionados);
public:
	static ListaItems& getTodosItems();

	ItemPesquisavel(void);
	~ItemPesquisavel(void);

	Id getId();
	string getTipo();
	static ItemPesquisavel* getItemTipo(Id id);
	static ItemPesquisavel* getItemTipo(string tipo);

	void adicionarDados(string nome, string texto);
	string getDados(string nome);
	void removerDados(string nome);
	void adicionarItemRelacionado(Id id, int ordem = numeric_limits<int>::max());
	bool removerItemRelacionado(Id id);

	void indexar();

	void escreverFicheiro(ofstream& out);
	static ItemPesquisavel* lerFicheiro(ifstream& in);

	ostream& escreverEcraLista(ostream& out);
	friend ostream& operator<<(ostream& out, ItemPesquisavel& itemPesquisavel);
};
