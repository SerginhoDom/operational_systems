from flask import Flask, render_template
import requests

app = Flask(__name__)
SERVER_URL = "http://localhost:8080"

@app.route("/")
def index():
    response = requests.get(f"{SERVER_URL}/temperature")
    temperature = response.json().get("temperature", 0)

    response = requests.get(f"{SERVER_URL}/statistics")
    statistics = response.json()

    return render_template("index.html", temperature=temperature, statistics=statistics)

if __name__ == "__main__":
    app.run(debug=True)