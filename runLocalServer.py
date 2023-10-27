"""Use instead of `python3 -m http.server` when you need CORS"""

from http.server import HTTPServer, SimpleHTTPRequestHandler


class CORSRequestHandler(SimpleHTTPRequestHandler):
    def end_headers(self):
        self.send_header('Cross-Origin-Embedder-Policy', 'require-corp')
        self.send_header('Cross-Origin-Opener-Policy', 'same-origin')
        return super(CORSRequestHandler, self).end_headers()


httpd = HTTPServer(('0.0.0.0', 8000), CORSRequestHandler)
httpd.serve_forever()