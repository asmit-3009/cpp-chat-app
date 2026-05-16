# 💬 C++ Multi-Client TCP Chat Application

A real-time terminal-based chat application built in C++ using **TCP sockets** and **multithreading**. Multiple clients can connect to a server and exchange messages simultaneously.

---

## 🚀 Features

- Multi-client support (up to 10 simultaneous users)
- Real-time message broadcasting
- Unique username per client
- Join/leave notifications
- Colored terminal output
- Graceful disconnect with `/quit`

---

## 🛠️ Tech Stack

- **Language:** C++ (C++17)
- **Concepts:** TCP Sockets, Multithreading, Mutex, POSIX API
- **Platform:** macOS / Linux
- **Tools:** GCC, Git, VS Code

---

## 📁 Project Structure

cpp-chat-app/
├── src/
│   ├── server.cpp       
│   └── client.cpp       
├── include/
│   └── common.h         
└── README.md

---

## ⚙️ How to Compile

g++ -std=c++17 -pthread src/server.cpp -o server
g++ -std=c++17 -pthread src/client.cpp -o client

---

## ▶️ How to Run

Step 1 — Start the server:
./server

Step 2 — Connect clients (open new terminal for each):
./client

Step 3 — Enter your username and start chatting!
Type /quit to disconnect.

---

## 💡 Concepts Demonstrated

| Concept | Where Used |
|---|---|
| TCP Socket Programming | server.cpp, client.cpp |
| Multithreading | One thread per client |
| Mutex and Thread Safety | Shared client list |
| POSIX API | socket, bind, listen, accept |
| Modular Design | Separate server/client/header |

---

## 👨‍💻 Author

**Asmit Biswal**
GitHub: https://github.com/asmit-3009