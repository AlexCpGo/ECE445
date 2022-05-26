from __future__ import division, print_function
from concurrent.futures import thread
# coding=utf-8

# Flask utils
from flask import Flask, redirect, url_for, request, render_template
from flask_cors import CORS
from werkzeug.utils import secure_filename
from gevent.pywsgi import WSGIServer
import os
import shutil
# Keras utils
import pandas as pd
import numpy as np

# App utils
import logging
import warnings
warnings.filterwarnings('ignore')
import threading

# Define a flask app
import json
import base64
import requests
import time
from datetime import datetime
import uuid
import shutil

from utils import *

DATA_URI = "result"
DATABASE_URI = "app.db"
TTS_TOKEN_URI = './tts_token.txt'

app = Flask(__name__)
cors = CORS(app, resources={r"/api/*": {"origins": "*"}})


LOG_FORMAT = "%(asctime)s - %(levelname)s - %(message)s"
os.makedirs(DATA_URI, exist_ok=True)
os.makedirs('logs/', exist_ok=True)
# logging.basicConfig(filename='logs/run_{}.log'.format(time.strftime("%Y-%m-%d_%H-%M", time.localtime())), level=logging.INFO, format=LOG_FORMAT)


def infer():
    gps = []
    try:
        for i, path in enumerate(glob.glob('exp/acc*.npy')):
            path_arg = path.split('-')
            lat = path_arg[0].split('acc')[1]
            lng = path_arg[1]
            acc = np.load(path)
            t_n = 10
            N = 500
            T = t_n / N
            f_s = 1/T
            feature = DWT_series(acc, T, N, f_s)
            X = feature
            gps.append((lat, lng))

            shutil.move(path, 'exp/loaded/') # 将读过的数据放到loaded中

        clf = joblib.load('model/lr.model')
        result = clf.predict(X)
        for i in range(len(result)):
            json_dict = {}
            json_dict['label'] = int(result[i])
            json_dict['gps'] = gps[i]
            with open('result/result{}.json'.format(i), 'w') as file_obj:
                json.dump(json_dict, file_obj)
    except Exception as e:
        print('In Inference Stage:        ', e)

# 循环后台查看是否有log.txt进入
def look():
    now = datetime.now().strftime("%Y%m%d_%H%M%S")
    if os.path.exists('exp/log.txt'):
        is_infer = preprocess()
        print('running')
        shutil.move('exp/log.txt', f'exp/loaded/log_{now}.txt') # 将读过的数据放到loaded中
        if is_infer:
            infer()

@app.route('/', methods=['GET'])
def index():
    # Main page
    return render_template('index.html')

def ret_json(errcode, msg, data, total=None):
    if data is None:
        ret_data =  { 'errcode' : errcode, 'msg' : msg }
    elif total is not None:
        ret_data =  { 'errcode' : errcode, 'msg' : msg, 'data' : data , 'total': total }
    else:
        ret_data =  { 'errcode' : errcode, 'msg' : msg, 'data' : data }
    return json.dumps(ret_data), 200, [("Content-type", "application/json;charset=utf-8")]

def start_runner():
    def start_loop():
        while True:
            # look() # 执行传感器数据搜索....
            time.sleep(2)
    thread = threading.Thread(target = start_loop)
    thread.start()

@app.route('/api/result', methods=['POST','GET'])
def predict():
    # status, open_id = getopenid(request.headers.get("token"))
    # if status is False:
    #     return ret_json(10403, open_id, None)

    path = os.path.join(DATA_URI, 'result1.json')
    with open(path) as file:
        json_data = json.load(file)
    label = json_data['label']
    gps = json_data['gps']
    
    return ret_json(0, 'Predict Success', {'label': str(label), 'result': str(gps)})

if __name__ == '__main__':
    # Serve the app with gevent
    # http_server = WSGIServer(('', 5002), app)
    # app.run(host='0.0.0.0', port=5000, debug=False)
    # start_runner()
    http_server = WSGIServer(('', 5000), app)
    http_server.serve_forever()
