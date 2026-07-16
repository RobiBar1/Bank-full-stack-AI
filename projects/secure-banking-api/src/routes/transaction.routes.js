// src/routes/transaction.routes.js
const express = require('express');
const { pool } = require('../config/db');

const UserRepository = require('../repositories/user.repository');
const TransactionRepository = require('../repositories/transaction.repository');
const TransactionService = require('../services/transaction.service');
const TransactionController = require('../controllers/transaction.controller');

const validate = require('../middlewares/validate.middleware');
const { transferSchema } = require('../validations/transaction.schema');
const authenticateJWT = require('../middlewares/auth.middleware');

// Dependency Injection Wiring
const userRepository = new UserRepository(pool);
const transactionRepository = new TransactionRepository(pool);
const transactionService = new TransactionService(pool, userRepository, transactionRepository);
const transactionController = new TransactionController(transactionService);

const router = express.Router();

router.post('/transfer', authenticateJWT, validate(transferSchema), transactionController.processTransfer);

module.exports = router;