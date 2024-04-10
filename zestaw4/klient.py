import socket

def send_data_to_server(data):
    # Ustalenie adresu IP serwera i portu
    server_address = ('127.0.0.1', 2020)
    
    # Tworzenie gniazda (socket) TCP/IP
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        # Nawiązanie połączenia z serwerem
        sock.connect(server_address)
        
        # Wysyłanie danych z zakończeniem \r\n
        sock.sendall(data.encode('utf-8'))
        
        # Oczekiwanie na odpowiedź i jej odbiór
        response = sock.recv(1024).decode('utf-8')
        
        print("Otrzymana odpowiedź od serwera:", response)

# Przykład użycia
data_to_send = "xyz\r\nucho oko\r\n"
send_data_to_server(data_to_send)

send_data_to_server("a")
send_data_to_server("b")
send_data_to_server("c\r\n")



