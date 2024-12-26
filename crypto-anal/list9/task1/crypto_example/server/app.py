from flask import Flask, render_template

app = Flask(__name__)

cert = '../crypto_example_ext.crt'
key = '../crypto_example.key'
context = (cert, key)

@app.route("/", methods=['POST', 'GET'])
def mainpage():
	return render_template('main.html')

app.run(host='127.0.0.1', port='443', ssl_context=context)
