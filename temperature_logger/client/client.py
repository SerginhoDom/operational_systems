from flask import Flask, render_template
import sqlite3
import matplotlib.pyplot as plt
import io
import base64

app = Flask(__name__)

# Подключение к базе данных
def get_db_connection():
    conn = sqlite3.connect('temperature_logs.db')
    conn.row_factory = sqlite3.Row
    return conn

# Главная страница
@app.route('/')
def index():
    conn = get_db_connection()
    logs = conn.execute('SELECT * FROM logs ORDER BY timestamp DESC LIMIT 10').fetchall()
    conn.close()
    return render_template('index.html', logs=logs)

# График температуры
@app.route('/plot')
def plot():
    conn = get_db_connection()
    logs = conn.execute('SELECT * FROM logs ORDER BY timestamp DESC LIMIT 100').fetchall()
    conn.close()

    timestamps = [log['timestamp'] for log in logs]
    temperatures = [log['temperature'] for log in logs]

    plt.figure(figsize=(10, 5))
    plt.plot(timestamps, temperatures, marker='o')
    plt.xlabel('Time')
    plt.ylabel('Temperature (°C)')
    plt.title('Temperature Over Time')
    plt.xticks(rotation=45)
    plt.tight_layout()

    # Сохранение графика в буфер
    buf = io.BytesIO()
    plt.savefig(buf, format='png')
    buf.seek(0)
    plt.close()

    # Кодирование графика в base64
    plot_url = base64.b64encode(buf.getvalue()).decode('utf8')
    return render_template('plot.html', plot_url=plot_url)

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5001)