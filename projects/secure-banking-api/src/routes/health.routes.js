// src/routes/health.routes.js
const express = require('express');
const { checkHealth } = require('../controllers/health.controller');

const router = express.Router();

/**
 * @swagger
 * /api/health:
 *   get:
 *     summary: System health check
 *     description: Returns the operational status and uptime of the banking API.
 *     tags: [System]
 *     security: []
 *     responses:
 *       200:
 *         description: Successful health check response.
 *         content:
 *           application/json:
 *             schema:
 *               type: object
 *               properties:
 *                 status:
 *                   type: string
 *                   example: OK
 *                 timestamp:
 *                   type: string
 *                   format: date-time
 *                   example: "2026-07-14T11:03:36.000Z"
 *                 uptime_seconds:
 *                   type: number
 *                   example: 345.12
 */
router.get('/health', checkHealth);

module.exports = router;