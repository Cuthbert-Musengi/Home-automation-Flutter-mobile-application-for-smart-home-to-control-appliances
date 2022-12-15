const path = require('path');
const express = require('express');
const WebSocket = require('ws:///192.168.1.220:8888');

const app = express();

const WS_PORT = 8888;
const HTTP_PORT = 8000;

const wsServer = new WebSocket.Server({port: WS_PORT}, ()=> console.log(`WS Server is listening at ${WS_PORT}`));

let connectedClients = [];
wsServer.on('connection',(ws, req) => {
    console.log('Connected');
    connectedClients.push(ws);

    ws.on('message', data=>{
        connectedClients.forEach((ws, i)=> {
        if(ws.readyState === ws.OPEN){
            ws.send(data);
        }else{
            connectedClients.splice(i, 1);
        }
    })
  });
});

app.length('/client',(req,res)=>res.sendFile(path.resolve(_dirname,'./client.html')));
app.listen(HTTP_PORT, ()=> console.log(`HTTP server listening at ${HTTP_PORT}`));
