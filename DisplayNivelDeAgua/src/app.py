# .venv\Scripts\activate 

# pip install virtualenv

# pip install pyserial
# pip install matplotlib

# Executar JVTech_Transmit_Leitura_3V3
# Executar JVTech_Receive_Valor_Tensao

import serial
import matplotlib.pyplot as plt
import time
import random

import matplotlib
matplotlib.use('Qt5Agg')


# Configuração da porta serial
SERIAL_PORT = "COM8"  # Substitua pelo nome correto da porta no seu sistema
BAUD_RATE = 9600  # Ajuste para o valor configurado no ESP32

def gerar_valor_tensao():
    tensao_aleatoria = random.uniform(1, 3)
    return tensao_aleatoria


def main():
    
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
    time.sleep(2)
    
    # Para o gráfico
    plt.ion()  # modo interativo
    fig, ax = plt.subplots()
    x_data, y_data = [], []
    line, = ax.plot(x_data, y_data, '-o')
    
    ax.set_xlim(0, 20)  # Limite do eixo X
    ax.set_ylim(0, 3.3)  # Voltagem máxima de 3.3V
    ax.set_xlabel("Tempo (s)")
    ax.set_ylabel("Tensão (V)")
    ax.set_title("Simulação de Sensor de Nível de Água")
    
    print("Aguardando ESP32 LoRa...")
    
    try:
        start_time = time.time()
        while True:
            # Leitura da serial
            if ser.in_waiting > 0:
                line_data = ser.readline().decode('utf-8').strip()
                try:
                    voltage = float(line_data)
                    elapsed_time = time.time() - start_time
                    print(f"Tensão recebida: {voltage}V")
                    
                    # Atualiza o gráfico
                    x_data.append(elapsed_time)
                    y_data.append(voltage)
                    line.set_xdata(x_data)
                    line.set_ydata(y_data)
                    ax.set_xlim(0, elapsed_time + 5)  # Ajusta o eixo X dinamicamente
                    ax.relim()
                    ax.autoscale_view()
                    plt.draw()
                    plt.pause(0.1)
                except ValueError:
                    print(f"Dado inválido recebido: {line_data}")
    except KeyboardInterrupt:
        print("\nEncerrando...")
    finally:
        ser.close()
        plt.ioff()
        plt.show()

if __name__ == "__main__":
    main()
