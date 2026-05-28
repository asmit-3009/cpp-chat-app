import React, { useState, useEffect, useRef } from 'react';
import { io } from 'socket.io-client';
import './App.css';

function App() {
  const [username, setUsername] = useState('');
  const [usernameSet, setUsernameSet] = useState(false);
  const [message, setMessage] = useState('');
  const [messages, setMessages] = useState([]);
  const [connected, setConnected] = useState(false);
  const messagesEndRef = useRef(null);
  const socketRef = useRef(null);

  useEffect(() => {
    // Create socket ONCE
    socketRef.current = io('http://localhost:3001');
    const socket = socketRef.current;

    socket.on('ask_username', () => {
      setConnected(true);
    });

    socket.on('message', (msg) => {
      setMessages((prev) => [...prev, msg]);
    });

    socket.on('server_closed', () => {
      setMessages((prev) => [...prev, { type: 'info', text: '⚠️ Server closed the connection.' }]);
    });

    socket.on('error_msg', (err) => {
      setMessages((prev) => [...prev, { type: 'info', text: '❌ ' + err }]);
    });

    // Cleanup on unmount
    return () => {
      socket.disconnect();
    };
  }, []); // empty array = runs ONCE only

  useEffect(() => {
    messagesEndRef.current?.scrollIntoView({ behavior: 'smooth' });
  }, [messages]);

  const handleSetUsername = () => {
    if (username.trim() === '') return;
    socketRef.current.emit('set_username', username.trim());
    setUsernameSet(true);
  };

  const handleSendMessage = () => {
    if (message.trim() === '') return;
    socketRef.current.emit('send_message', message.trim());
    setMessages((prev) => [...prev, { type: 'own', text: message.trim() }]);
    setMessage('');
  };

  const handleKeyPress = (e) => {
    if (e.key === 'Enter') {
      usernameSet ? handleSendMessage() : handleSetUsername();
    }
  };

  return (
    <div className="app">
      <div className="chat-container">

        {/* Header */}
        <div className="chat-header">
          <div className="header-dot"></div>
          <h1>C++ Chat App</h1>
          <span className="status">{connected ? '🟢 Connected' : '🔴 Connecting...'}</span>
        </div>

        {/* Username Screen */}
        {!usernameSet ? (
          <div className="username-screen">
            <div className="username-box">
              <h2>Welcome 👋</h2>
              <p>Enter a username to join the chat</p>
              <input
                type="text"
                placeholder="Your username..."
                value={username}
                onChange={(e) => setUsername(e.target.value)}
                onKeyPress={handleKeyPress}
                autoFocus
              />
              <button onClick={handleSetUsername}>Join Chat →</button>
            </div>
          </div>
        ) : (
          <>
            {/* Messages */}
            <div className="messages-container">
              {messages.length === 0 && (
                <div className="empty-msg">No messages yet. Say hello! 👋</div>
              )}
              {messages.map((msg, index) => (
                <div
                  key={index}
                  className={`message ${
                    msg.type === 'own' ? 'own' :
                    msg.type === 'join' || msg.type === 'leave' || msg.type === 'info' ? 'system' :
                    'other'
                  }`}
                >
                  {msg.type === 'message' ? (
                    <><span className="msg-sender">{msg.sender}</span> {msg.text}</>
                  ) : (
                    msg.text
                  )}
                </div>
              ))}
              <div ref={messagesEndRef} />
            </div>

            {/* Input */}
            <div className="input-container">
              <input
                type="text"
                placeholder="Type a message..."
                value={message}
                onChange={(e) => setMessage(e.target.value)}
                onKeyPress={handleKeyPress}
                autoFocus
              />
              <button onClick={handleSendMessage}>Send ➤</button>
            </div>
          </>
        )}
      </div>
    </div>
  );
}

export default App;