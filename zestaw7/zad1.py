import requests
#eiphXJltZtwTWNBfXkjWfcSMvSprCQEY
#QFxsDizfMWWbZpNbQHis
def get_artist_albums(artist_id, user_token):
    url = f"https://api.discogs.com/artists/{artist_id}/releases"
    headers = {
        'User-Agent': 'YourAppName/1.0',
        'Authorization': f'Discogs token={user_token}'
    }
    response = requests.get(url, headers=headers)
    if response.status_code == 200:
        return response.json() 
    else:
        return response.status_code


artist_id = 359282  
user_token = 'QFxsDizfMWWbZpNbQHis'
albums = get_artist_albums(artist_id, user_token)

print(albums)
