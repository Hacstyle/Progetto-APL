from flask import *
import sqlite3

app = Flask(__name__)

# Funzione per aggiungere un elemento al carrello
@app.route('/chart/add', methods=['POST'])
def riempi_carrello():

    data = request.get_json()
    
    nome = data.get('nome')

    connE = sqlite3.connect('ecommerce.db')
    cursorE = connE.cursor()

    # Esegue la query e salva i valori restituiti
    cursorE.execute("SELECT * FROM prodotti WHERE nome= '" + nome + "';")
    result = cursorE.fetchall()

    tipo = result[0][1]
    prezzo = result[0][2]

    connC = sqlite3.connect('carrello.db')
    cursorC = connC.cursor()

    # Esegue la query parametrizzata 
    cursorC.execute("INSERT INTO elementi (nome, tipo, prezzo) VALUES (?, ?, ?)", (nome, tipo, prezzo))

    # Rende definitive le modifiche al database
    connC.commit()

    cursorC.close()
    connC.close()

    # Risposta al client
    return jsonify('Elemento inserito nel carrello!')

# Funzione per eliminare un elemento dal carrello
@app.route('/chart/delete', methods=['DELETE'])
def svuota_carrello():
    data = request.get_json()
    
    nome = data.get('nome')

    connC = sqlite3.connect('carrello.db')
    cursorC = connC.cursor()

    cursorC.execute("DELETE FROM elementi WHERE nome='" + nome + "';")

    connC.commit()
    cursorC.close()
    connC.close()

    # Risposta al client
    return jsonify('Elemento eliminato dal carrello!') 

app.run(host='localhost', port=8081)