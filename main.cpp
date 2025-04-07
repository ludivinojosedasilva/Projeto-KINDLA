#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

struct Ebook {
    string titulo;
    string caminho;
    string conteudo;
};

// Verifica se arquivo existe
bool arquivoExiste(const string& nome) {
    ifstream test(nome);
    return test.good();
}

// Cria arquivo com conteúdo caso ele não exista
void criarArquivoSeNaoExistir(const string& nome, const string& conteudo) {
    if (!arquivoExiste(nome)) {
        ofstream out(nome);
        out << conteudo;
        out.close();
    }
}

void inicializarArquivos() {
    criarArquivoSeNaoExistir("amoreninha.txt", "Capítulo 1 - A Moreninha\nEra uma vez uma linda história de amor...\n");
    criarArquivoSeNaoExistir("domcasmurro.txt", "Capítulo 1 - Dom Casmurro\nBentinho e Capitu, lembranças do tempo de seminário...\n");
    criarArquivoSeNaoExistir("ebooks.txt", "A Moreninha;amoreninha.txt\nDom Casmurro;domcasmurro.txt\n");
}

vector<Ebook> carregarEbooks(const string& nomeArquivo) {
    vector<Ebook> ebooks;
    ifstream in(nomeArquivo);
    if (!in) {
        cerr << "Erro ao abrir o arquivo: " << nomeArquivo << endl;
        return ebooks;
    }

    string linha;
    while (getline(in, linha)) {
        stringstream ss(linha);
        string titulo, caminho;
        if (getline(ss, titulo, ';') && getline(ss, caminho)) {
            ifstream ebookFile(caminho);
            stringstream buffer;
            buffer << ebookFile.rdbuf();
            ebooks.push_back({titulo, caminho, buffer.str()});
        }
    }
    return ebooks;
}

void mostrarEbooks(const vector<Ebook>& ebooks) {
    cout << "\n== eBooks Disponíveis ==" << endl;
    for (size_t i = 0; i < ebooks.size(); ++i) {
        cout << i << ". " << ebooks[i].titulo
             << " (" << ebooks[i].conteudo.size() << " caracteres)\n";
    }
}

void lerEbook(const vector<Ebook>& ebooks) {
    cout << "Digite o índice do eBook: ";
    int idx;
    cin >> idx;
    cin.ignore();
    if (idx >= 0 && idx < (int)ebooks.size()) {
        cout << "\n== Conteúdo de \"" << ebooks[idx].titulo << "\" ==\n";
        cout << ebooks[idx].conteudo << endl;
    } else {
        cout << "Índice inválido!\n";
    }
}

void pesquisarSubstring(const vector<Ebook>& ebooks) {
    cout << "Digite a substring a ser pesquisada: ";
    string sub;
    getline(cin, sub);
    cout << "\nSubstring pesquisada: " << sub << endl;
    for (size_t i = 0; i < ebooks.size(); ++i) {
        cout << "\nOcorrências encontradas em " << ebooks[i].titulo << ":\n";
        size_t pos = ebooks[i].conteudo.find(sub);
        bool encontrou = false;
        while (pos != string::npos) {
            cout << "Caractere " << pos << endl;
            encontrou = true;
            pos = ebooks[i].conteudo.find(sub, pos + 1);
        }
        if (!encontrou) cout << "Nenhuma ocorrência.\n";
    }
}

void alterarTexto(vector<Ebook>& ebooks) {
    cout << "Digite o índice do eBook a ser alterado: ";
    int idx;
    cin >> idx;
    cin.ignore();
    if (idx < 0 || idx >= (int)ebooks.size()) {
        cout << "Índice inválido!\n";
        return;
    }

    cout << "\nConteúdo atual:\n" << ebooks[idx].conteudo << endl;
    cout << "\nDigite o novo conteúdo (fim com linha vazia):\n";

    string linha, novoTexto;
    while (getline(cin, linha)) {
        if (linha.empty()) break;
        novoTexto += linha + "\n";
    }

    ebooks[idx].conteudo = novoTexto;

    ofstream out(ebooks[idx].caminho);
    out << novoTexto;
    out.close();

    cout << "Conteúdo atualizado com sucesso!\n";
}

int main() {
    inicializarArquivos();
    vector<Ebook> ebooks = carregarEbooks("ebooks.txt");

    if (ebooks.empty()) {
        cout << "Nenhum eBook carregado. Verifique o arquivo ebooks.txt\n";
        return 1;
    }

    int opcao;
    do {
        cout << "\nKINDLA - Sistema de leitura de eBook\n";
        cout << "Selecione uma opção abaixo:\n";
        cout << "1. Mostrar eBooks Disponíveis\n";
        cout << "2. Ler um eBook (selecionar por índice)\n";
        cout << "3. Pesquisar\n";
        cout << "4. Alterar Texto\n";
        cout << "5. Sair do Sistema\n";
        cout << "Opção: ";
        cin >> opcao;
        cin.ignore();

        switch (opcao) {
            case 1: mostrarEbooks(ebooks); break;
            case 2: lerEbook(ebooks); break;
            case 3: pesquisarSubstring(ebooks); break;
            case 4: alterarTexto(ebooks); break;
            case 5: cout << "Saindo do sistema...\n"; break;
            default: cout << "Opção inválida!\n";
        }

    } while (opcao != 5);

    return 0;
}
