#pragma once

// classe que representa um campo de dados (par nome-texto)
class CampoDados
{
private:
	Id id;
	string texto;

	map<Id, string> nomes;
	// lista de todos os campos usados pela base de dados para os nomes serem
	// gravados em conjunto e poderem ser referenciados por id globalmente
	static ListaCamposDados LISTA_CAMPOS;
public:
	CampoDados(Id id);
	~CampoDados(void);

	Id getId();
	void setTexto(string texto);
	string getTexto();

	static ListaCamposDados& getListaCampos();

	void escreverFicheiro(ofstream& out);

	friend ostream& operator<<(ostream& out, CampoDados* campoDados);
};
