import requests
from bs4 import BeautifulSoup
import json
import sys

def fetch_usd_exchange_rate(url):
    try:
        response = requests.get(url)
        response.raise_for_status()  
    except requests.RequestException as e:
        print(f"Request failed: {e}", file=sys.stderr)
        sys.exit(1)
    
    try:
        soup = BeautifulSoup(response.content, 'html.parser')
        exchange_rate_div = soup.find('div', class_='profilLast')
        if not exchange_rate_div:
            raise ValueError("Could not find the exchange rate on the page")
        exchange_rate = exchange_rate_div.get_text(strip=True)
    except Exception as e:
        print(f"Failed to parse the page: {e}", file=sys.stderr)
        sys.exit(1)
    
    return exchange_rate

def main():
    url = "https://www.bankier.pl/waluty/kursy-walut/nbp/USD"
    exchange_rate = fetch_usd_exchange_rate(url)
    
    result = {
        "USD_exchange_rate": exchange_rate
    }
    with open('exchange_rate.json', 'w') as file:
        json.dump(result, file, indent=4)
    
    print(json.dumps(result, indent=4))

main()
