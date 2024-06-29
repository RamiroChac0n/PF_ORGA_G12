import serial
import tkinter as tk
from tkinter import scrolledtext
import time

# Función para convertir datos binarios a ASCII
def binary_to_ascii(binary_data):
    ascii_char = chr(int(binary_data, 2))  # Convertir binario a decimal y luego a carácter ASCII
    return ascii_char

# Función para leer datos del Arduino
def read_from_arduino():
    global arduino, text_area
    if arduino.in_waiting > 0:
        data = arduino.readline().decode('utf-8').strip()
        rows = data.split('\n')
        for row in rows:
            bits = row.split(',')
            if len(bits) == 7:
                binary_str = ''.join(bits)
                ascii_char = binary_to_ascii(binary_str)
                text_area.insert(tk.END, ascii_char + "\n")
    root.after(1000, read_from_arduino)

# Función para enviar comando de reset al Arduino
def send_reset_command():
    arduino.write(b'1')  # Enviar el comando '1' para resetear
    time.sleep(0.1)      # Esperar un breve periodo
    arduino.write(b'0')  # Enviar el comando '0' para terminar el pulso

# Configura la comunicación serial con Arduino
arduino = serial.Serial('COM1', 9600, timeout=1)

# Configura la interfaz gráfica
root = tk.Tk()
root.title("Proyecto Final - Grupo 12")

# Marco principal
frame = tk.Frame(root)
frame.pack(padx=10, pady=10)

# Texto de proyecto
project_label = tk.Label(frame, text="Proyecto Final - Grupo 12", font=("Arial", 16))
project_label.pack(pady=5)

# Texto de datos recibidos
data_label = tk.Label(frame, text="Datos recibidos del Arduino en ASCII:")
data_label.pack(pady=5)

# Área de texto para datos ASCII
text_area = scrolledtext.ScrolledText(frame, width=20, height=10, wrap=tk.WORD)
text_area.pack(pady=5)

# Botón para enviar comando de reset
reset_button = tk.Button(frame, text="Resetear Memoria", command=send_reset_command)
reset_button.pack(pady=5)

# Inicia la lectura de datos desde Arduino
root.after(1000, read_from_arduino)

root.mainloop()
