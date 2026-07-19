// src/config/socket.js
const { Server } = require('socket.io');
const jwt = require('jsonwebtoken'); // Assumes standard jsonwebtoken library usage

let ioInstance = null;

function initSocket(server) {
    ioInstance = new Server(server, {
        cors: {
            origin: "http://localhost:5173",
            methods: ["GET", "POST"],
            credentials: true
        }
    });

    // Enforce JWT verification middleware on initial WebSocket handshake connection
    ioInstance.use((socket, next) => {
        const token = socket.handshake.auth?.token;

        if (!token) {
            return next(new Error('Authentication failed: Missing token token.'));
        }

        try {
            // Verify token signatures using your environment's secret key
            const decoded = jwt.verify(token, process.env.JWT_SECRET);
            
            // Explicitly bind the securely extracted email directly to the socket data instance
            socket.userEmail = decoded.email; 
            next();
        } catch (err) {
            return next(new Error('Authentication failed: Invalid or expired token signature.'));
        }
    });

    ioInstance.on('connection', (socket) => {
        // Safe to use now since the middleware verified the identity
        const authenticatedEmail = socket.userEmail;

        if (authenticatedEmail) {
            socket.join(authenticatedEmail);
            console.log(`[Socket.IO] Verified client connected. Encrypted room joined: ${authenticatedEmail}`);
        }

        socket.on('disconnect', () => {
            console.log(`[Socket.IO] Client disconnected from room: ${authenticatedEmail}`);
        });
    });

    return ioInstance;
}

function getIO() {
    if (!ioInstance) {
        throw new Error("Socket.io has not been initialized. Call initSocket(server) first.");
    }
    return ioInstance;
}

module.exports = {
    initSocket,
    getIO
};