import serial.tools.list_ports
import os

BAUDRATE = 9600
TIMEOUT = 1
VALID_COMMANDS = {'1', '2', '3', '+', '-'}

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

    if not ports:
        print("No serial ports found. Please connect a device.")
        exit(1)

    print("Available ports:")
    for i, port in enumerate(ports, 1):
        print(f"{i}. {port.device} - {port.description}")
    choice = int(input("Select port number: ")) - 1

    if choice < 0 or choice >= len(ports):
        print("Invalid port selection.")
        exit(1)
    
    port = serial.Serial(
        port=ports[choice].device,
        baudrate=BAUDRATE,
        timeout=TIMEOUT
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
            elif mode in VALID_COMMANDS:
                port.write((mode + '\n').encode('utf-8'))
            else:
                port.write('0\n'.encode('utf-8'))
            clear()
            
        port.close()