import requests
import sys
from time import sleep
from collections import defaultdict

# Funkcja do wykonywania zapytań do API Discogs
def make_request(url, params=None, retries=5, backoff_factor=1.5):
    for _ in range(retries):
        try:
            response = requests.get(url, params=params, headers={'User-Agent': 'DiscogsBandCheck/0.1'})
            response.raise_for_status()
            return response.json()
        except requests.exceptions.HTTPError as e:
            if response.status_code == 429:  # Przekroczenie limitu zapytań
                retry_after = int(response.headers.get("Retry-After", 60))
                sleep(retry_after)
            else:
                print(f"HTTP error occurred: {e}")
                sys.exit(1)
        except requests.exceptions.RequestException as e:
            print(f"Error during requests to Discogs API: {e}")
            sys.exit(1)
        sleep(backoff_factor)  # czekaj przed kolejną próbą
    print("Max retries exceeded")
    sys.exit(1)

# Funkcja do pobierania grup, w których grał muzyk
def get_artist_groups(artist_id):
    url = f"https://api.discogs.com/artists/{artist_id}"
    data = make_request(url)
    groups = []
    if "groups" in data:
        for group in data["groups"]:
            groups.append((group["id"], group["name"]))
    return groups

# Funkcja do analizowania wspólnych zespołów
def analyze_common_groups(artist_ids):
    all_groups = defaultdict(list)
    artist_names = {}

    # Pobieranie grup dla każdego artysty
    for artist_id in artist_ids:
        artist_data = make_request(f"https://api.discogs.com/artists/{artist_id}")
        artist_names[artist_id] = artist_data.get("name", "Unknown Artist")
        groups = get_artist_groups(artist_id)
        for group_id, group_name in groups:
            all_groups[group_id].append(artist_id)

    # Wyszukiwanie grup z więcej niż jednym artystą
    common_groups = {}
    for group_id, members in all_groups.items():
        if len(members) > 1:
            group_name = make_request(f"https://api.discogs.com/artists/{group_id}")["name"]
            common_groups[group_name] = sorted([(artist_names[member_id], member_id) for member_id in members])

    return common_groups

def main():
    if len(sys.argv) < 3:
        print("Please provide at least two artist IDs as arguments.")
        sys.exit(1)

    artist_ids = sys.argv[1:]
    common_groups = analyze_common_groups(artist_ids)

    # Wyświetlanie wyników
    for group_name in sorted(common_groups.keys()):
        print(f"Group: {group_name}")
        for name, id in common_groups[group_name]:
            print(f"  {name} (ID: {id})")

if __name__ == "__main__":
    main()
