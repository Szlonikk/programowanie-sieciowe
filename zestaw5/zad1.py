import sys
import requests

def get_header(url, scheme):
    full_url = f"{scheme}://{url}"
    try:
        response = requests.get(full_url)
        server_header = response.headers.get('Server', 'NIEZNANY')
        print(f"{scheme} port {('443' if scheme == 'https' else '80')}: {server_header}")
    except requests.exceptions.RequestException as e:
        print(f"{scheme} port {('443' if scheme == 'https' else '80')}: Error fetching")

def main():
    websites = sys.argv[1:]
    if not websites:
        print("Polecenie: python3 zad1.py <url1> <url2> ...")
        return

    for website in websites:
        get_header(website, 'http')
        get_header(website, 'https')

main()
