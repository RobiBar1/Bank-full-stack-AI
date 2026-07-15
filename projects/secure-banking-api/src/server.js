// src/server.js
require('dotenv').config(); // Must be the absolute first execution

const express = require('express');
const helmet = require('helmet');
const swaggerUi = require('swagger-ui-express');
const swaggerSpec = require('./config/swagger');
const healthRoutes = require('./routes/health.routes');
const errorHandler = require('./middlewares/error.middleware');

const app = express();
const PORT = process.env.PORT || 3000;

// --- 1. Pre-Routing Middlewares ---
// Secure HTTP headers
app.use(helmet());
// Parse incoming JSON payloads strictly
app.use(express.json());

// --- 2. Documentation Route ---
app.use('/api-docs', swaggerUi.serve, swaggerUi.setup(swaggerSpec));

// --- 3. Route Mounting ---
app.use('/api', healthRoutes);

// --- 4. Post-Routing Error Boundary ---
// The global error handler must be the final middleware injected into the pipeline.
app.use(errorHandler);

// --- 5. Server Initialization ---
const server = app.listen(PORT, () => {
    console.log(`[Server] Process initialized and listening on port ${PORT}`);
    console.log(`[Swagger] Documentation available at http://localhost:${PORT}/api-docs`);
});

// --- 6. Process Signal Handling ---
// Ensures standard file descriptors and database connections can close cleanly
process.on('SIGTERM', () => {
    console.log('[Server] SIGTERM received: initiating graceful shutdown.');
    server.close(() => {
        console.log('[Server] HTTP server closed cleanly.');
        process.exit(0);
    });
});

process.on('unhandledRejection', (reason, promise) => {
    console.error('[Server] Unhandled Promise Rejection:', reason);
    // In production, trigger a graceful shutdown here rather than continuing in an unknown state
    // server.close(() => process.exit(1)); 
});