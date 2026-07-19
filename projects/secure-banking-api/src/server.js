// src/server.js
require('dotenv').config();

const express = require('express');
const http = require('http'); // Added for Socket.IO integration
const cors = require('cors'); 
const helmet = require('helmet');
const swaggerUi = require('swagger-ui-express');
const swaggerSpec = require('./config/swagger');
const errorHandler = require('./middlewares/error.middleware');
const { initSocket } = require('./config/socket'); // Added to bind Socket.IO server config

// Route Imports
const healthRoutes = require('./routes/health.routes');
const dashboardRoutes = require('./routes/dashboard.routes');
const authRoutes = require('./routes/auth.routes');
const transactionRoutes = require('./routes/transaction.routes');

const { testConnection } = require('./config/db');
const { initializeDatabaseSchema } = require('./scripts/initTables');

const app = express();
const PORT = process.env.PORT || 3000;

app.use(cors()); 
app.use(express.json()); 
app.use(helmet());

// Mount routes
app.use('/api-docs', swaggerUi.serve, swaggerUi.setup(swaggerSpec));
app.use('/api', healthRoutes);
app.use('/api/auth', authRoutes);
app.use('/api/dashboard', dashboardRoutes);
app.use('/api/transactions', transactionRoutes);

// Global Error Handler
app.use(errorHandler);

// Create native HTTP server wrapper around the Express application instance
const server = http.createServer(app);

// Initialize the global Socket.IO instance linked to the HTTP server pipelines
initSocket(server);

const startServer = async () => {
    try {
        await testConnection();
        await initializeDatabaseSchema();

        // Start listening via the HTTP server instance instead of direct app.listen
        server.listen(PORT, () => {
            console.log(`[Server] Process initialized and listening on port ${PORT}`);
            console.log(`[Swagger] Documentation available at http://localhost:${PORT}/api-docs`);
        });

        process.on('SIGTERM', () => {
            server.close(() => {
                process.exit(0);
            });
        });
    } catch (error) {
        console.error('[Server] Fatal boot error:', error);
        process.exit(1);
    }
};

if (require.main === module) {
    startServer();
}

process.on('unhandledRejection', (reason, promise) => {
    console.error('[Server] Unhandled Promise Rejection:', reason);
});

module.exports = app;