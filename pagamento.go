package main

import (
	"database/sql"
	"fmt"
	"net/http"

	_ "github.com/mattn/go-sqlite3"
)

// Funzione per gestire il pagamento
func payment_process(w http.ResponseWriter, r *http.Request) {
	if r.Method != "POST" {
		http.Error(w, "Metodo non consentito", http.StatusMethodNotAllowed)
		return
	}

	err := r.ParseForm()
	if err != nil {
		http.Error(w, "Errore nell'analisi del form", http.StatusBadRequest)
		return
	}

	numero := r.Form.Get("numero")
	scadenza := r.Form.Get("scadenza")
	cvc := r.Form.Get("cvc")

	transazione(numero, scadenza, cvc)

	// Apre la connessione al database
	dbC, err := sql.Open("sqlite3", "carrello.db")
	if err != nil {
		panic(err)
	}
	defer dbC.Close()

	// Inserisce in sum i valori estratti dal risultato della query (una riga)
	var sum float64
	err = dbC.QueryRow("SELECT SUM(prezzo) FROM elementi").Scan(&sum)
	if err != nil {
		panic(err.Error())
	}

	fmt.Println("Hai pagato ", sum, "€! \n")

	dbE, err := sql.Open("sqlite3", "ecommerce.db")
	if err != nil {
		panic(err)
	}
	defer dbE.Close()

	// Inserisce in rows i risultati della query (più righe)
	rows, err := dbE.Query("SELECT nome FROM prodotti")
	if err != nil {
		panic(err.Error())
	}
	defer rows.Close()

	// Slice per memorizzare i nomi nel carrello
	var nomi []string

	// Itera sui risultati e appendiamo i valori allo slice
	for rows.Next() {
		var elemento string
		err := rows.Scan(&elemento)
		if err != nil {
			panic(err.Error())
		}
		nomi = append(nomi, elemento)
	}

	// Elimina dal database ecommerce i prodotti acquistati
	for _, elemento := range nomi {
		_, err = dbE.Exec("DELETE FROM prodotti WHERE nome= '" + elemento + "';")
		if err != nil {
			panic(err.Error())
		}
	}

	// Svuota il carrello
	_, err = dbC.Exec("DELETE FROM elementi")
	if err != nil {
		panic(err.Error())
	}

	fmt.Println("Il carrello ora è vuoto. \n")

}

func transazione(numero string, scadenza string, cvc string) {

	fmt.Print("Completamento transazione... \n")
}

func main() {

	http.HandleFunc("/payment", payment_process)
	fmt.Println("Server avviato sulla porta 8082")
	http.ListenAndServe(":8082", nil)
}
