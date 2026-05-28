const express = require('express');
const http = require('http');
const { Server } = require('socket.io');
const net = require('net');
const cors = require('cors');

const app = express();
app.use(cors());

const server = http.createServer(app);
const io = new Server(server, {
  cors: {
    origin: "http://localhost:3000",
    methods: ["GET", "POST"]
  }
});

function stripColors(str) {
  return str.replace(/\x1B\[[0-9;]*m/g, '').trim();
}

function parseMessage(raw) {
  const clean = stripColors(raw);
  if (!clean || clean.length === 0) return null;
  if (clean.includes('Enter your username')) return null;

  if (clean.includes('has joined the chat!')) {
    return { type: 'join', text: clean };
  } else if (clean.includes('has left the chat.')) {
    return { type: 'leave', text: clean };
  } else if (clean.includes(':')) {
    const colonIndex = clean.indexOf(':');
    const sender = clean.substring(0, colonIndex).trim();
    const content = clean.substring(colonIndex + 1).trim();
    if (!content || content === '/quit') return null;
    return { type: 'message', sender, text: content };
  }
  return { type: 'info', text: clean };
}

io.on('connection', (socket) => {
  console.log('Browser client connected:', socket.id);

  let usernameSet = false;
  let tcpClient = null;
  let buffer = '';

  // Create TCP connection
  tcpClient = new net.Socket();
  tcpClient.connect(8080, '127.0.0.1', () => {
    console.log('TCP connected for:', socket.id);
  });

  tcpClient.on('data', (data) => {
    buffer += data.toString();
    const lines = buffer.split('\n');
    buffer = lines.pop(); // keep incomplete line in buffer

    lines.forEach((line) => {
      const trimmed = line.trim();
      if (!trimmed) return;

      if (trimmed.includes('Enter your username')) {
        socket.emit('ask_username');
      } else {
        const parsed = parseMessage(trimmed);
        if (parsed) socket.emit('message', parsed);
      }
    });
  });

  socket.on('set_username', (username) => {
    if (!usernameSet && tcpClient) {
      usernameSet = true;
      tcpClient.write(username + '\n');
    }
  });

  socket.on('send_message', (message) => {
    if (tcpClient && usernameSet) {
      tcpClient.write(message + '\n');
    }
  });

  socket.on('disconnect', () => {
    console.log('Browser disconnected:', socket.id);
    if (tcpClient) {
      tcpClient.destroy();
      tcpClient = null;
    }
  });

  tcpClient.on('error', (err) => {
    console.error('TCP error:', err.message);
    socket.emit('error_msg', 'Could not connect to chat server');
  });

  tcpClient.on('close', () => {
    console.log('TCP closed for:', socket.id);
  });
});

server.listen(3001, () => {
  console.log('Node.js bridge running on port 3001');
});