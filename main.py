import serial.tools.list_ports
import os

def clear():
    if os.name == 'nt':
        os.system('cls')
    else:
        os.system('clear')

def list_serial_ports():
    ports = serial.tools.list_ports.comports()
    port_list = []
    for port in ports:
        if port.description == 'n/a':
            continue
        port_list.append(port)
    return port_list

def main():
    ports = list_serial_ports()
    
    port = serial.Serial(
        port=ports[0].device,
        baudrate=9600,
        timeout=1
    )
    
    return port

if __name__ == "__main__":
    port = main()

    if port.is_open:
        clear()

        while True:
            mode = str(input("Enter mode: "))
            if mode == 'q':
                break
            elif mode in ['1', '2', '3', '+', '-']:
                port.write((mode + '\n').encode('utf-8'))
            else:
                port.write('0\n'.encode('utf-8'))
            clear()
            
        port.close()