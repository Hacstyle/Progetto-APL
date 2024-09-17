# Progetto-APL

In questo repository sono presenti i file per l'esecuzione di alcune delle operazioni previste in una piattaforma di e-commerce che vende prodotti unici storici, di tipo: gioiello, mobile, immobile. In particolare: creazione, ricerca e rimozione dei prodotti; aggiunta e rimozione dei prodotti nel carrello; acquisto dei prodotti nel carrello.

## Requisiti:

- Un compilatore C++
- Python
- Go

### Librerie esterne C++:

- sqlite3
- crow
- nlohmann/json

### Librerie esterne Python:

- flask
- sqlite3

### Librerie esterne GO:

- mattn/go-sqlite3

## Istruzioni

Per avviare l'applicazione, basta scaricare il repository, spostarsi nella sua directory ed avviare i seguenti comandi in terminali diversi:

```bash
./crd.out
python3 chart.py
go run pagamento.go
```
Sarà possibile verificare i risultati delle operazioni nello standard output.

-Per avere la lista dei prodotti disponibili, visitare con un browser l'url: localhost:8080/list
-Per avere la lista dei gioielli disponibili, visitare con un browser l'url: localhost:8080/gioielli
-Per avere la lista dei mobili disponibili, visitare con un browser l'url: localhost:8080/mobili
-Per avere la lista degli immobili disponibili, visitare con un browser l'url: localhost:8080/immobili

Per aggiungere un prodotto nel database, eseguire una richiesta POST del tipo:

```bash
curl -X POST -H "Content-Type: application/json" -d '{"nome":"corona del re", "tipo":"gioiello", "prezzo":400}' http://localhost:8080/insert
```

Per eliminare un prodotto dal database, eseguire una richiesta DELETE del tipo:

```bash
curl -X DELETE -H "Content-Type: application/json" -d '{"nome":"corona del re"}' http://localhost:8080/delete
```

Per aggiungere un prodotto al carrello, eseguire una richiesta post del tipo:

```bash
curl -X POST -H "Content-Type: application/json" -d '{"nome":"corona del re"}' http://localhost:8081/chart/add
```

Per eliminare un prodotto dal carrello, eseguire una richiesta DELETE del tipo:

```bash
curl -X DELETE -H "Content-Type: application/json" -d '{"nome":"corona del re"}' http://localhost:8081/chart/delete
```

Per acquistare i prodotti inseriti nel carrello, eseguire una richiesta POST del tipo:

```bash
curl -X POST -H "Content-Type: application/json" -d '{"numero":"555555", "scadenza":"5-05", "cvc:"555"}' http://localhost:8082/payment
```


