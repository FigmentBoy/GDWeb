from flask import Flask, Response, send_from_directory, request
import requests
import urllib.parse
import re

app = Flask(__name__)
app.config.from_object(__name__)

@app.route('/audio/<int:path>')
def send_audio(path):
    data = {
        "gameVersion": "21",
        "binaryVersion": "35",
        "secret": "Wmfd2893gb7",
        "songID": path
    }
    
    headers = {
        "User-Agent": "",
    }

    req = requests.post("http://www.boomlings.com/database/getGJSongInfo.php", data=data, headers=headers)

    downloadPath = re.compile(r"10~\|~(.*).mp3").search(req.text)
            
    if (downloadPath == None):
        return Response("Song not found", status=404)

    downloadPath = urllib.parse.unquote(downloadPath.group(1)) + ".mp3"
    host = urllib.parse.urlparse(downloadPath).netloc
    
    headers = {
        "Host": host,
        "Range": request.headers.get("Range"),
    }
    
    res = requests.get(downloadPath, headers=headers)
    return res.content, res.status_code, res.headers.items()

@app.route('/level/<int:id>')
def level(id):
    data = {
        "gameVersion": "21",
        "binaryVersion": "35",
        "secret": "Wmfd2893gb7",
        "levelID": id,
    }
    
    headers = {
        "User-Agent": "",
    }

    req = requests.post("http://www.boomlings.com/database/downloadGJLevel22.php", data=data, headers=headers)
    return req.text

@app.route('/search/', defaults={'string': ''})
@app.route('/search/<string:string>')
def search(string):
    data = {
        "gameVersion": "21",
        "binaryVersion": "35",
        "secret": "Wmfd2893gb7",
        "str": string,
        "type": "0",
    }
    
    headers = {
        "User-Agent": "",
    }

    req = requests.post("http://www.boomlings.com/database/getGJLevels21.php", data=data, headers=headers)
    return req.text

@app.route('/', defaults={'path': 'index.html'})
@app.route('/<path:path>')
def index(path):
    res = send_from_directory('./frontend/dist', path);
    res.headers['Cross-Origin-Embedder-Policy'] = 'require-corp'
    res.headers['Cross-Origin-Opener-Policy'] = 'same-origin'
    return res


if __name__ == '__main__':
    app.run(port=8000)
