// src/routes/dashboard.routes.js
const express = require('express');
const { pool } = require('../config/db');

const UserRepository = require('../repositories/user.repository');
const TransactionRepository = require('../repositories/transaction.repository');
const DashboardService = require('../services/dashboard.service');
const DashboardController = require('../controllers/dashboard.controller');
const authenticateJWT = require('../middlewares/auth.middleware');

// Dependency Injection Wiring
const userRepository = new UserRepository(pool);
const transactionRepository = new TransactionRepository(pool);
const dashboardService = new DashboardService(userRepository, transactionRepository);
const dashboardController = new DashboardController(dashboardService);

const router = express.Router();

/**
 * @swagger
 * /api/dashboard:
 *   get:
 *     summary: Get User Dashboard
 *     description: Retrieves the current balance and transaction history for the authenticated user.
 *     tags: [Dashboard]
 *     security:
 *       - bearerAuth: []
 *     responses:
 *       '200':
 *         description: Dashboard data retrieved successfully.
 *       '401':
 *         description: Unauthorized. Invalid or missing token.
 */
// The authenticateJWT middleware blocks unauthorized access before it reaches the controller
router.get('/', authenticateJWT, dashboardController.getDashboard);

module.exports = router;