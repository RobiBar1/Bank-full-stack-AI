// src/routes/health.routes.js
const express = require('express');
const { pool } = require('../config/db'); // Extracting pool here

const HealthRepository = require('../repositories/health.repository');
const HealthService = require('../services/health.service');
const HealthController = require('../controllers/health.controller');

// Manual Dependency Injection Wiring
const healthRepository = new HealthRepository(pool); // Passing pool here
const healthService = new HealthService(healthRepository);
const healthController = new HealthController(healthService);

const router = express.Router();

/**
 * @swagger
 * /api/health:
 *   get:
 *     summary: System health check
 *     description: Returns the operational status of the API and Database.
 *     tags: [System]
 *     security: []
 *     responses:
 *       200:
 *         description: Successful health check response.
 *       503:
 *         description: Service degraded (e.g., database disconnected).
 */
router.get('/health', healthController.checkHealth);

module.exports = router;