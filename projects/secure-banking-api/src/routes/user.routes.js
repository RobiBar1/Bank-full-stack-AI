// src/routes/user.routes.js
const express = require('express');
const requireAuth = require('../middlewares/auth.middleware');

const router = express.Router();

// GET /api/user/dashboard
// Protected by requireAuth middleware
router.get('/dashboard', requireAuth, (req, res) => {
    // This mock data exactly matches the TypeScript interface we built on the frontend
    res.status(200).json({
        balance: 5240.50,
        transactions: [
            {
                id: 1,
                type: 'deposit',
                amount: 1500.00,
                date: new Date().toISOString(),
                description: 'Salary Deposit'
            },
            {
                id: 2,
                type: 'withdrawal',
                amount: 45.20,
                date: new Date(Date.now() - 86400000).toISOString(), // 1 day ago
                description: 'Grocery Store'
            }
        ]
    });
});

module.exports = router;