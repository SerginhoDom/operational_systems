from flask import Flask, jsonify, request
import sqlite3

app = Flask(__name__)

def get_db_connection():
    conn = sqlite3.connect('temperature_logs.db')
    conn.row_factory = sqlite3.Row
    return conn

@app.route('/current_temperature', methods=['GET'])
def get_current_temperature():
    conn = get_db_connection()
    temperature = conn.execute('SELECT temperature FROM logs ORDER BY timestamp DESC LIMIT 1').fetchone()
    conn.close()
    if temperature:
        return jsonify({'temperature': temperature['temperature']})
    return jsonify({'error': 'No data available'}), 404

@app.route('/statistics', methods=['GET'])
def get_statistics():
    start_time = request.args.get('start_time')
    end_time = request.args.get('end_time')
    if not start_time or not end_time:
        return jsonify({'error': 'Missing start_time or end_time'}), 400

    conn = get_db_connection()
    logs = conn.execute('SELECT * FROM logs WHERE timestamp BETWEEN ? AND ?', (start_time, end_time)).fetchall()
    conn.close()

    if logs:
        return jsonify([dict(log) for log in logs])
    return jsonify({'error': 'No data available for the specified period'}), 404

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)