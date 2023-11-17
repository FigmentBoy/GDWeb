from flask import Flask, Response, send_from_directory, request
import requests
import urllib.parse
import re

app = Flask(__name__)
app.config.from_object(__name__)

@app.route('/audio/<path:path>')
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
    print(downloadPath);
        
    headers = {
        "Host": "audio.ngfiles.com",
        "Range": request.headers.get("Range"),
    }
    
    res = requests.get(downloadPath, headers=headers)
    return res.content, res.status_code, res.headers.items()

@app.route('/', defaults={'path': 'index.html'})
@app.route('/<path:path>')
def index(path):
    res = send_from_directory('bin', path);
    res.headers['Cross-Origin-Embedder-Policy'] = 'require-corp'
    res.headers['Cross-Origin-Opener-Policy'] = 'same-origin'
    return res


if __name__ == '__main__':
    app.run(port=8000)
