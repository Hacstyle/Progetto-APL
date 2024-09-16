#include <iostream>
#include <sqlite3.h>
#include "crow.h"
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

// Funzione per la connessione al database
sqlite3* db;
void connect_db() {
    int rc = sqlite3_open("ecommerce.db", &db);
    if(rc != SQLITE_OK) {
        cerr << "Errore nell'apertura del database: " << sqlite3_errmsg(db) << endl;
        exit(1);
    }
}

// Funzione di callback dopo la select
int callback(void *data, int argc, char **argv, char **azColName) {
    for (int i = 0; i < argc; i++) {
        cout << azColName[i] << ": " << (argv[i] ? argv[i] : "NULL") << endl;
    }
    cout << endl;
    return 0;
}

// Funzione per eseguire la query dopo DELETE
void execute_sql(const string& sql) {
    char *zErrMsg = 0;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, 0, &zErrMsg);
    if( rc!=SQLITE_OK ){
        cerr << "SQL error: " << zErrMsg << endl;
        sqlite3_free(zErrMsg);
    }
}

// Funzione per eseguire una query SQL dopo GET
void execute_sql_call(const string& sql) {
    char *zErrMsg = 0;
    int rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
    if( rc!=SQLITE_OK ){
        cerr << "SQL error: " << zErrMsg << endl;
        sqlite3_free(zErrMsg);
    }
}

int main() {
    connect_db();

    crow::SimpleApp app;

    // Gestisce la richiesta GET per elencare tutti i prodotti
    CROW_ROUTE(app, "/list").methods("GET"_method)([]() {
        string sql = "SELECT * FROM prodotti;";
        execute_sql_call(sql);

        return "Elenco dei prodotti restituito";
    });

    // Gestisce la richiesta GET per elencare i mobili
    CROW_ROUTE(app, "/list/mobili").methods("GET"_method)([]() {
        string sql = "SELECT * FROM prodotti WHERE tipo='mobile';";
        execute_sql_call(sql);

        return "Elenco dei mobili restituito!";
    });

    // Gestisce la richiesta GET per elencare i gioielli
    CROW_ROUTE(app, "/list/gioielli").methods("GET"_method)([]() {
        string sql = "SELECT * FROM prodotti WHERE tipo='gioiello';";
        execute_sql_call(sql);

        return "Elenco dei gioielli restituito!";
    });

    // Gestisce la richiesta GET per elencare gli immobili
    CROW_ROUTE(app, "/list/immobili").methods("GET"_method)([]() {
        string sql = "SELECT * FROM prodotti WHERE tipo='immobile';";
        execute_sql_call(sql);

        return "Elenco degli immobili restituito!";
    });

    // Gestisce la richiesta POST per inserire nuovi prodotti
    CROW_ROUTE(app, "/insert").methods("POST"_method)([](const crow::request& req) {
        auto json_data = json::parse(req.body);
        string nome = json_data["nome"];
        string tipo = json_data["tipo"];
        int prezzo = json_data["prezzo"];

        if (tipo != "mobile" && tipo!="gioiello" && tipo!="immobile")
        {
            return "Elemento non valido. Tipi validi: mobile, gioiello, immobile";
        }

        string sql = "INSERT INTO prodotti (nome, tipo, prezzo) VALUES (?, ?, ?);";

        sqlite3_stmt* stmt;

        // Prepara l'sql parametrizzata: -1 calcola automaticamente la lughezza della stringa
        int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            cerr << "Errore nella preparazione della query: " << sqlite3_errmsg(db) << endl;
        }

        // Lega i parametri alle variabili: -1 calcola automaticamente la lunghezza della stringa, sqlite_transient libera automaticamente la memoria associata alla stringa
        sqlite3_bind_text(stmt, 1, nome.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, tipo.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 3, prezzo);

        // Esegue la query
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            cerr << "Errore nell'esecuzione della query: " << sqlite3_errmsg(db) << endl;
        }

        // Libera la memoria allocata per stmt (tutta la query preparata)
        sqlite3_finalize(stmt);

        return "Prodotto inserito!";
    });

    // Gestisce la richiesta DELETE per eliminare un prodotto
    CROW_ROUTE(app, "/delete").methods("DELETE"_method)([](const crow::request& req) {
        auto json_data = json::parse(req.body);
        string nome = json_data["nome"];


        string sql = "DELETE FROM prodotti WHERE nome='" + nome + "';";

        execute_sql(sql);

        return "Prodotto eliminato!";
    });

    app.port(8080).run();
}