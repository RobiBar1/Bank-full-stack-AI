// src/server.js
require('dotenv').config();

const express = require('express');
const helmet = require('helmet');
const swaggerUi = require('swagger-ui-express');
const swaggerSpec = require('./config/swagger');
const healthRoutes = require('./routes/health.routes');
const errorHandler = require('./middlewares/error.middleware');

const app = express();
const PORT = process.env.PORT || 3000;

app.use(helmet());
app.use(express.json());

app.use('/api-docs', swaggerUi.serve, swaggerUi.setup(swaggerSpec));

// Mount routes
app.use('/api', healthRoutes);

// Global Error Handler
app.use(errorHandler);

const server = app.listen(PORT, () => {
    console.log(`[Server] Process initialized and listening on port ${PORT}`);
    console.log(`[Swagger] Documentation available at http://localhost:${PORT}/api-docs`);
});

process.on('SIGTERM', () => {
    console.log('[Server] SIGTERM received: initiating graceful shutdown.');
    server.close(() => {
        console.log('[Server] HTTP server closed cleanly.');
        process.exit(0);
    });
});

process.on('unhandledRejection', (reason, promise) => {
    console.error('[Server] Unhandled Promise Rejection:', reason);
});