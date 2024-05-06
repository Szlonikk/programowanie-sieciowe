import requests
import sys

def check_website(url, expected_string):
    response = requests.get(url)

    if response.status_code != 200:
        print("Nieudane polaczenie: status", response.status_code)
        return False

    content_type = response.headers.get('Content-Type')
    if 'text/html' not in content_type:
        print("To nie strona HTML: ", content_type)
        return False

    if expected_string not in response.text:
        print("Brak zawartosci na stronie.")
        return False

    return True


def main():
    url = "http://th.if.uj.edu.pl/"
    expected_string = "Institute of Theoretical Physics"

    if check_website(url, expected_string):
        print("Strona dziala")
        sys.exit(0)
    else:
        print("Cos poszlo nie tak")
        sys.exit(1)

main()
