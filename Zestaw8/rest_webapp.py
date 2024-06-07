#!/usr/bin/python3
# -*- coding: UTF-8 -*-

'''
Aplikacja WSGI implementująca najważniejsze części opisywanej na wykładzie
usługi REST dającej dostęp do bazy z danymi osób.

Uwaga: kod dydaktyczny bez pełnej obsługi błędów i sytuacji nadzwyczajnych.

Aplikacja nie potrafi sama stworzyć swojej bazy danych, trzeba to zrobić
przed jej uruchomieniem. Skrypt rest_webapp.sh pokazuje jak.
'''

plik_bazy = './osoby.sqlite'

import re, sqlite3

class OsobyApp:
    def __init__(self, environment, start_response):
        '''
Konstruktor wywoływany przez serwer WSGI. Jak każdy konstruktor tworzy nowy
obiekt, następnie zapamiętuje w jego polach przekazane przez serwer argumenty
i inicjuje pola na odpowiedź.
'''
        self.env = environment
        self.start_response = start_response
        self.status = '200 OK'
        self.headers = [ ('Content-Type', 'text/html; charset=UTF-8') ]
        self.content = b''

    def __iter__(self):
        '''
Metoda obsługująca proces iterowania po stworzonym obiekcie. Serwer WSGI
wymaga aby w środku była co najmniej jedna instrukcja "yield" zwracająca
ciąg bajtów do odesłania klientowi HTTP.
'''
        try:
            self.route()
        except sqlite3.Error as e:
            s = 'SQLite error: ' + str(e)
            self.failure('500 Internal Server Error', s)
        n = len(self.content)
        self.headers.append( ('Content-Length', str(n)) )
        self.start_response(self.status, self.headers)
        yield self.content

    def failure(self, status, detail = None):
        '''
Metoda wstawiająca do pól obiektu status błędu oraz dokument HTML
z komunikatem o jego wystąpieniu.
'''
        self.status = status
        s = '<html>\n<head>\n<title>' + status + '</title>\n</head>\n'
        s += '<body>\n<h1>' + status + '</h1>\n'
        if detail is not None:
            s += '<p>' + detail + '</p>\n'
        s += '</body>\n</html>\n'
        self.content = s.encode('UTF-8')
        self.headers = [ ('Content-Type', 'text/html; charset=UTF-8') ]

    def route(self):
        path_info = self.env['PATH_INFO']
        if path_info == '/osoby':
            self.handle_osoby()
        elif path_info.startswith('/osoby/search'):
            self.handle_osoby_search()
        elif path_info.startswith('/osoby/'):
            self.handle_osoba(int(path_info.split('/')[2]))
        elif path_info == '/psy':
            self.handle_psy()
        elif path_info.startswith('/psy/'):
            self.handle_pies(int(path_info.split('/')[2]))
        else:
            self.failure('404 Not Found')

    def handle_osoby(self):
        if self.env['REQUEST_METHOD'] == 'GET':
            colnames, rows = self.sql_select()
            self.send_rows(colnames, rows)
        elif self.env['REQUEST_METHOD'] == 'POST':
            colnames, vals = self.read_tsv()
            q = 'INSERT INTO osoby (' + ','.join(colnames) + ') VALUES (' + ','.join(['?']*len(vals)) + ')'
            self.sql_modify(q, vals)
        else:
            self.failure('501 Not Implemented')

    def handle_osoby_search(self):
        params = self.env['QUERY_STRING']
        query = 'SELECT * FROM osoby WHERE '
        conditions = []
        values = []
        if 'imie' in params:
            conditions.append('imie = ?')
            values.append(params['imie'])
        if 'nazwisko' in params:
            conditions.append('nazwisko = ?')
            values.append(params['nazwisko'])
        query += ' AND '.join(conditions)
        colnames, rows = self.sql_select_query(query, values)
        self.send_rows(colnames, rows)

    def handle_osoba(self, id):
        if self.env['REQUEST_METHOD'] == 'GET':
            colnames, rows = self.sql_select(id)
            self.send_rows(colnames, rows)
        elif self.env['REQUEST_METHOD'] == 'DELETE':
            self.delete_person(id)
        else:
            self.failure('501 Not Implemented')

    def handle_psy(self):
        if self.env['REQUEST_METHOD'] == 'GET':
            colnames, rows = self.sql_select_pies()
            self.send_rows(colnames, rows)
        elif self.env['REQUEST_METHOD'] == 'POST':
            colnames, vals = self.read_tsv()
            q = 'INSERT INTO psy (' + ','.join(colnames) + ') VALUES (' + ','.join(['?']*len(vals)) + ')'
            self.sql_modify(q, vals)
        else:
            self.failure('501 Not Implemented')

    def handle_pies(self, id):
        if self.env['REQUEST_METHOD'] == 'GET':
            colnames, rows = self.sql_select_pies(id)
            self.send_rows(colnames, rows)
        elif self.env['REQUEST_METHOD'] == 'DELETE':
            q = 'DELETE FROM psy WHERE id = ' + str(id)
            self.sql_modify(q)
        else:
            self.failure('501 Not Implemented')

    def read_tsv(self):
        f = self.env['wsgi.input']
        n = int(self.env['CONTENT_LENGTH'])
        raw_bytes = f.read(n)
        lines = raw_bytes.decode('UTF-8').splitlines()
        colnames = lines[0].split('\t')
        vals = lines[1].split('\t')
        return colnames, vals

    def send_rows(self, colnames, rows):
        s = '\t'.join(colnames) + '\n'
        for row in rows:
            s += '\t'.join([str(val) for val in row]) + '\n'
        self.content = s.encode('UTF-8')
        self.headers = [ ('Content-Type', 'text/tab-separated-values; charset=UTF-8') ]

    def sql_select(self, id = None):
        conn = sqlite3.connect(plik_bazy)
        crsr = conn.cursor()
        query = 'SELECT * FROM osoby'
        if id is not None:
            query += ' WHERE id = ' + str(id)
        crsr.execute(query)
        colnames = [ d[0] for d in crsr.description ]
        rows = crsr.fetchall()
        crsr.close()
        conn.close()
        return colnames, rows

    def sql_select_query(self, query, params):
        conn = sqlite3.connect(plik_bazy)
        crsr = conn.cursor()
        crsr.execute(query, params)
        colnames = [ d[0] for d in crsr.description ]
        rows = crsr.fetchall()
        crsr.close()
        conn.close()
        return colnames, rows

    def sql_select_pies(self, id = None):
        conn = sqlite3.connect(plik_bazy)
        crsr = conn.cursor()
        query = 'SELECT * FROM psy'
        if id is not None:
            query += ' WHERE id = ' + str(id)
        crsr.execute(query)
        colnames = [ d[0] for d in crsr.description ]
        rows = crsr.fetchall()
        crsr.close()
        conn.close()
        return colnames, rows

    def sql_modify(self, query, params = None):
        conn = sqlite3.connect(plik_bazy)
        crsr = conn.cursor()
        if params is None:
            crsr.execute(query)
        else:
            crsr.execute(query, params)
        rowid = crsr.lastrowid   # id wiersza wstawionego przez INSERT
        crsr.close()
        conn.commit()
        conn.close()
        return rowid

    def delete_person(self, id):
        conn = sqlite3.connect(plik_bazy)
        crsr = conn.cursor()
        crsr.execute('SELECT id FROM psy WHERE owner_id = ?', (id,))
        if crsr.fetchone():
            self.failure('400 Bad Request', 'Person is an owner of a dog and cannot be deleted')
            crsr.close()
            conn.close()
            return

        
        q = 'DELETE FROM osoby WHERE id = ?'
        self.sql_modify(q, (id,))
        crsr.close()
        conn.close()

if __name__ == '__main__':
    from wsgiref.simple_server import make_server
    port = 8000
    httpd = make_server('', port, OsobyApp)
    print('Listening on port %i, press ^C to stop.' % port)
    httpd.serve_forever()
