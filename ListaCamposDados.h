#pragma once

class CampoDados;
class ItemPesquisavel;

// esta classe representa uma lista de campos de dados, tanto a lista global de todos os tipos de campo que guarda
// uma instancia de cada nome diferente (titulo, ano, artista, etc) assim como também representa a lista de campos
// de cada item pesquisavel.
class ListaCamposDados
{
private:
	map<Id, CampoDados*> campos;
	map<Id, string> nomes;
public:
	ListaCamposDados(void);
	~ListaCamposDados(void);

	map<Id, CampoDados*>& getCampos();

	void adicionar(CampoDados* campo);
	CampoDados* get(Id id);
	CampoDados* get(string nome);
	void remover(string nome);
	void adicionarNome(string nome);
	string getNome(CampoDados& campo);

	void escreverNomesFicheiro(ostream& stream);
	void escreverCamposFicheiro(ostream& stream);

	void escreverNomesFicheiro(ofstream& out);
	void lerNomesFicheiro(ifstream& in);
	void escreverCamposFicheiro(ofstream& out);
	void lerCamposFicheiro(ifstream& in);

	friend ostream& operator<<(ostream& out, ListaCamposDados& listaCampoDados);
};
