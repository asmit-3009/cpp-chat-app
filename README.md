# 💬 C++ Multi-Client TCP Chat Application

A real-time chat application built in C++ with a modern React web interface. 
Multiple clients can connect and exchange messages simultaneously through a browser.

---

## 🚀 Features

- Multi-client support (up to 10 simultaneous users)
- Real-time message broadcasting
- Modern React web interface with beautiful UI
- Node.js WebSocket bridge connecting browser to C++ backend
- Join/leave notifications
- Colored message bubbles (sent vs received)
- Graceful disconnect handling

---

## 🛠️ Tech Stack

| Layer | Technology |
|---|---|
| Backend | C++17, POSIX TCP Sockets, Multithreading, Mutex |
| Bridge | Node.js, Express, Socket.io |
| Frontend | React.js, CSS3 |
| Tools | GCC, Git, VS Code |

---

## 📁 Project Structure

cpp-chat-app/
├── src/
│   ├── server.cpp       # C++ TCP Server
│   └── client.cpp       # C++ Terminal Client
├── include/
│   └── common.h         # Shared constants
├── backend/
│   └── server.js        # Node.js WebSocket bridge
├── frontend/
│   └── src/
│       ├── App.js        # React chat UI
│       └── App.css       # Styling
└── README.md

---

## ⚙️ How to Run

### Step 1 — Compile C++ Server
g++ -std=c++17 -pthread src/server.cpp -o server

### Step 2 — Start C++ Server
./server

### Step 3 — Start Node.js Bridge
cd backend && node server.js

### Step 4 — Start React Frontend
cd frontend && npm start

### Step 5 — Open browser at http://localhost:3000

---

## 💡 Concepts Demonstrated

| Concept | Where Used |
|---|---|
| TCP Socket Programming | server.cpp, client.cpp |
| Multithreading | One thread per client |
| Mutex & Thread Safety | Shared client list |
| WebSocket Communication | Node.js bridge |
| React Hooks | useState, useEffect, useRef |
| Client-Server Architecture | Full 3-tier design |

---

## 👨‍💻 Author

**Asmit Biswal**
GitHub: https://github.com/asmit-3009
