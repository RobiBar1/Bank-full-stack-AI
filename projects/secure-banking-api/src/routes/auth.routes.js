// src/routes/auth.routes.js
const express = require('express');
const { pool } = require('../config/db');

const UserRepository = require('../repositories/user.repository');
const EmailService = require('../utils/email.util'); // <-- Updated import
const AuthService = require('../services/auth.service');
const AuthController = require('../controllers/auth.controller');
const validate = require('../middlewares/validate.middleware');
const { registerSchema } = require('../validations/user.schema');

// Dependency Injection Wiring
const userRepository = new UserRepository(pool);
const emailService = new EmailService(); // <-- Updated instantiation
const authService = new AuthService(userRepository, emailService); // <-- Passed to service
const authController = new AuthController(authService);

const router = express.Router();

router.post('/register', validate(registerSchema), authController.register);

router.post('/verify', authController.verify);

router.post('/login', authController.login);

module.exports = router;